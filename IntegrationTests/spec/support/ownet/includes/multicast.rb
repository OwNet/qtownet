require 'socket'
require 'ipaddr'

module Ownet
	module Includes
		module Multicast

			##
			# Access multicast inspector.
			#
			def multicast
				options[:multicast_inspector]
			end

			##
			# Communication period in seconds.
			#
			def period
				5
			end

			##
			# Sleep <time> seconds
			#
			def sleep(time)
				Kernel.sleep(time)
			end

			##
			#
			# Local network multicast communication
			#
			class MulticastInspector
				attr_reader :multicast_address, :multicast_port
				
				##
				#
				# Read received messages
				#
				attr_reader :messages

				##
				#
				# Clean all received messages
				#
				def clean_all
					@messages = []
				end

				##
				#
				# Send message to multicast group
				#
				def send(message)
				  	socket = UDPSocket.open
				  	socket.setsockopt(Socket::IPPROTO_IP, Socket::IP_TTL, [1].pack('i'))
				  	socket.send(message.to_json, 0, @multicast_address, @multicast_port)
				ensure
				  	socket.close 
				end

				##
				#
				# Create multicast inspector for given multicast address and port.
				#
				def initialize(multicast_address, multicast_port)
					@multicast_address = multicast_address
					@multicast_port = multicast_port
					@messages = []
				end

				##
				#
				# Start multicast inspector.
				#
				def start
					ip = IPAddr.new(@multicast_address).hton + IPAddr.new("0.0.0.0").hton
					@socket = UDPSocket.new
					@socket.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, 1)
					@socket.bind(Socket::INADDR_ANY, @multicast_port)
					@socket.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_MEMBERSHIP, ip)
					@thread = Thread.new { 
						loop do
							begin
							  	msg, info = @socket.recvfrom(1024)
							  	#puts "Received message #{msg} from #{info[2]} (#{info[3]})/#{info[1]} len #{msg.size}" 

							  	@messages << JSON.parse(msg)
							rescue => error 
								puts "Communication error: #{error}"
							end
						end
					}
				end
			
				def stop
					@thread.kill
					@socket.close
				end
			end

			private

				def initialize_multicast
					multicast.clean_all
				end

				def self.included(base)
				    base.extend ClassMethods 
				end

				module ClassMethods
					def initialize_defaults_for_multicast
						defaults[:multicast_inspector] = MulticastInspector.new(
							defaults.delete(:multicast_address), 
							defaults.delete(:multicast_port)
						)
						defaults[:multicast_inspector].start
					end

					def finalize_defaults_for_multicast
						defaults[:multicast_inspector].stop
					end
				end
		end	
	end
end