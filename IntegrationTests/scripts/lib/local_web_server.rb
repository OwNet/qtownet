require 'webrick'
require 'rack'

class LocalWebServer
	attr_reader :port
	
	def initialize(port, &block)
		@port = port
		@block = block
	end

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
		@block.call(env)
	end
end