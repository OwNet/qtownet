#encoding: UTF-8

require "rubygems"
require "bundler/setup"

require 'capybara/rspec'
require 'capybara-webkit'
require 'faraday'
require 'fileutils'

require './scripts/lib/local_web_server.rb'

# arguments
if ARGV.count < 2
	puts "Usage: ruby download_url_with_dependencies.rb <url> <target_file>"
end
url = ARGV[0]
target_file = ARGV[1]

requests = []

# local proxy
proxy = LocalWebServer.new(9001) { |env|
	begin
		request = Rack::Request.new(env)

		puts "Browser requested URL #{request.url}"

		# run real request
		method = request.request_method.downcase.to_sym
		request_url = request.url.to_s
	    body = request.body.read
	    headers = Hash[
	    	env.select { |k,v| 
	    		k.start_with? 'HTTP_' and not %w{HTTP_VERSION HTTP_CONNECTION HTTP_ACCEPT_ENCODING}.include?(k)
	    	}.map { |pair| 
	    		[
	    			pair[0].sub(/^HTTP_/, '').downcase.gsub('_', '-').gsub(/^(\w)|-(\w)/) { |a| 
	    				a.upcase
	    			}, 
	    			pair[1]
	    		]
	    	}
	    ]

		response = Faraday.run_request(method, request_url, body, headers)
		response_headers = Hash[
			response.headers.to_hash.map { |pair| 
	    		[
	    			pair[0].gsub(/^(\w)|-(\w)/) { |a| 
	    				a.upcase
	    			}, 
	    			pair[1]
	    		]
	    	}
	    ]

		# store and return response
		triplet = [response.status, response_headers, [response.body]]
		requests << {
			method: method,
			url: request_url, 
			body: body, 
			headers: headers, 
			response: triplet
		}
		triplet
	rescue => e
		puts e
	end
}
proxy.start

# webdriver
session = Capybara::Session.new(:webkit)
session.driver.browser.set_proxy host: "127.0.0.1", port: proxy.port

puts "Visiting url #{url}"
session.visit url

puts "Saving requests in #{target_file}"
File.open(target_file, 'w') { |file|
	Marshal.dump(requests, file)
}