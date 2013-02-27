require 'webrick'
require 'rack'
require 'uri'

module Ownet
	module Includes
		module Faker

			##
			#
			# Access faker, alternative names for nice DSL
			#
			def faker
				options[:faker]
			end
			alias :fake :faker
			alias :fakes :faker

			##
			#
			# Proxy that fakes requests
			#
			class Faker
				##
				#
				# Access received requests
				#
				attr_reader :requests

				attr_reader :port

				##
				#
				# Fake url with status, data and headers
				#
				def url(url, options = {})
					normalized_url = normalize_url(url)
					@fakes[normalized_url] = {
						original_url: url
					}
					if options[:with_response]
						@fakes[normalized_url][:response] = options[:with_response]
					else
						@fakes[normalized_url][:response] = [
							options[:with_status] || 200, 
							options[:with_headers] || {}, 
							[options[:with_body] || ''], 
						]
					end
					nil
				end

				##
				#
				# Load saved requests in marshaled data
				#
				def urls_saved_in(requests_data)
					requests = requests_data.from_marshal
					requests.each { |r|
						url r[:url], with_response: r[:response]
					}
				end
				
				##
				#
				# Clean faked urls
				#
				def clean_all
					@fakes = {}
					@requests = []
				end

				##
				#
				# Create faker for given listen port
				#
				def initialize(port)
					@port = port
					clean_all
				end
				
				##
				#
				# Start webrick
				#
				def start
					@server = WEBrick::HTTPServer.new(
						:BindAddress => '127.0.0.1', 
						:Port => @port, 
						:AccessLog => [], 
						:Logger => WEBrick::Log.new(nil, 0),
			  			:AccessLog => [nil, nil],)
					@server.mount('/', Rack::Handler::WEBrick, self)
					
					Thread.new { @server.start }
				end

				def stop
					@server.shutdown
				end

				def call(env)
					request = Rack::Request.new(env)
					normalized_url = normalize_url(request.url)
					if @fakes[normalized_url]
						@requests << FakerRequest.new(
							request, 
							@fakes[normalized_url][:original_url]
						)
						return @fakes[normalized_url][:response]
					else
						@requests << FakerRequest.new(request)
						raise FakerError.new("Non-faked attempt to access #{request.url}")
					end
				rescue => exception
					return [
						555, 
						{ 
							'Content-Type' => 'text/plain', 
							'X-Ownet-Integration-Tests-Faker-Exception' => 'TRUE', 
							'X-Ownet-Integration-Tests-Faker-Exception-Type' => exception.class.name, 
							'X-Ownet-Integration-Tests-Faker-Exception-Message' => exception.to_s
						}, 
						[exception.backtrace.to_yaml]
						#["#{exception.class.name}: #{exception.to_s}\n\n#{exception.backtrace.join("\n")}"]
					]
				end

				private

					def normalize_url(url)
						url = URI.decode(url)
						url = url.gsub('+', ' ') # webrick and URI.decode handle this differently
						url = url.gsub(/\/\z/, '') # trailing /
						url
					end

					##
					#
					# Requests in faker.requests
					#
					class FakerRequest
						attr_reader :url
						#attr_reader :request_method, :body

						def initialize(request, original_url = nil)
							@url = original_url || request.url
							#@request_method = request.request_method
							#@body = request.body
						end
					end

					class FakerError < StandardError
					end
			end

			private

				def initialize_faker
					faker.clean_all if faker
				end

				def self.included(base)
				    base.extend ClassMethods 
				end

				module ClassMethods
					def initialize_defaults_for_faker
						defaults[:faker] = Faker.new(
							defaults.delete(:faker_port)
						)
						defaults[:faker].start
					end

					def finalize_defaults_for_faker
						defaults[:faker].stop
					end
				end
		end
	end
end