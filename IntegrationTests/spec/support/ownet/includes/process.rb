module Ownet
	module Includes
		module Process

			##
			#
			# Process PID file
			#
			def pid_file
 				data_directory + "/ownet.pid"
			end

			private

				def start_process
					# configuration options
					ENV['OWNET_TEST_ENVIRONMENT'] = root_directory

					# run proxy
					log_file_path = test_root + "/log/ownet_proxy.log"
					@ownet_process = ::Process.spawn(test_root + '/' + options[:binary_path], 
						[:out, :err] => [log_file_path, "a"])
					::Process.detach(@ownet_process) 

					# continue when proxy is initialized
					sleep 0.15
					loop {
						break if File.exists?(pid_file)
						sleep 0.05
					}
				end

				def stop_process
					begin
						if windows?
							# doesn't know TERM
							::Process.kill('KILL', @ownet_process)
						else
							::Process.kill('TERM', @ownet_process)
						end
					rescue Errno::ESRCH
					end

					# delete PID file
					File.unlink pid_file
				end

				def self.included(base)
				    base.extend ClassMethods 
				end

				module ClassMethods
					def initialize_defaults_for_process
						File.unlink test_root + '/log/ownet_proxy.log'
					rescue Errno::ENOENT
					end

					def finalize_defaults_for_process
					end
				end
		end
	end
end