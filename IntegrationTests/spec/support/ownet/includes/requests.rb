require 'faraday'

module Ownet
	module Includes
		module Requests

			def initialize_requests
				# connection for proxy requests
				@faraday = Faraday.new proxy: "http://127.0.0.1:#{@options[:listen_port]}"
			end

			##
			#
			# HTTP GET request using proxy
			#
			def get(url, &block)
				request :get, url, block
			end

			##
			#
			# HTTP POST request using proxy
			#
			def post(url, body, &block)
				request :post, url, body, block
			end

			##
			#
			# HTTP PUT request using proxy
			#
			def put(url, body, &block)
				request :put, url, body, block
			end

			##
			#
			# HTTP DELETE request using proxy
			#
			def delete(url, &block)
				request :delete, url, block
			end

			##
			#
			# HTTP HEAD request using proxy
			#
			def head(url, &block)
				request :head, url, block
			end

			private

				def request(type, *options, &block)
					response = @faraday.send(type, *options, &block)
					
					# raise error when error response from faker found
					if response.headers['X-Ownet-Integration-Tests-Faker-Exception']
						type = response.headers['X-Ownet-Integration-Tests-Faker-Exception-Type']
						message = response.headers['X-Ownet-Integration-Tests-Faker-Exception-Message']
						exception = Kernel.const_get(type).new(message)
						exception.set_backtrace(YAML.load(response.body))
						raise exception
					end
					
					# windows compatibility
					response.body.gsub!("\r\n", "\n") if response.body if windows?
					
					response
				end
		end
	end
end