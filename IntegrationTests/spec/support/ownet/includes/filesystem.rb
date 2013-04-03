module Ownet
	module Includes
		module Filesystem
			attr_reader :root_directory

			##
			# App data directory
			#
			def data_directory
				root_directory + '/data'
			end

			##
			# App resources directory
			#
			def resources_directory
				root_directory + '/resources'
			end

			##
			# resources_directory/migrations
			#
			def migrations_directory
				resources_directory + '/migrations'
			end

			##
			# resources_directory/static
			#
			def static_directory
				resources_directory + '/static'
			end

			##
			#
			# Clean filesystem
			#
			def clean
				# clean root directory
				FileUtils.rm_rf root_directory

				nil
			end

			private

				def initialize_filesystem
					# temporary root directory
					begin
						name = ('a'..'z').to_a.shuffle.take(10).join('')
						@root_directory = test_root + "/tmp/" + name
					end while Dir.exists?(@root_directory)
					FileUtils.mkdir_p @root_directory

					# create data directory
					FileUtils.mkdir_p data_directory

					# create resources directory
					FileUtils.mkdir_p resources_directory

					# symlink or copy migrations
					if options[:symlink_migrations]
						FileUtils.ln_s test_root + '/' + options[:migrations_path], 
						migrations_directory
					else
						FileUtils.cp_r test_root + '/' + options[:migrations_path], 
							migrations_directory
					end

					# symlink or copy static
					if options[:symlink_static]
						FileUtils.ln_s test_root + '/' + options[:static_path], 
							static_directory
					else
						FileUtils.cp_r test_root + '/' + options[:static_path], 
							static_directory
					end

					# settings
					FileUtils.mkdir_p root_directory + '/The Reconnected'
					File.open(root_directory + "/The Reconnected/OwNet Client.ini", 'w') { |file|
						file.write "[application]\n"
						file.write "listen_port=#{options[:listen_port]}\n"
						if options[:faker]
							file.write "proxy_host_name=127.0.0.1\n"
							file.write "proxy_port=#{options[:faker].port}\n"
						end
						file.write "data_folder_path=#{data_directory}\n"
						file.write "resources_folder_path=#{resources_directory}\n"
						file.write "multicast_group_address=#{options[:multicast_inspector].multicast_address}\n"
						file.write "multicast_port=#{options[:multicast_inspector].multicast_port}\n"

						file.write "[current_workspace]"
						file.write "id={aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa}"
						file.write "name=OwNet Network"

						file.write "[workspaces]"
						file.write "%7Baaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa%7D=OwNet Network"
					}
				end

				def path(p)
					if windows?
						p
					else
						p.gsub("/", "\\")
					end
				end

				def self.included(base)
				    base.extend ClassMethods 
				end

				module ClassMethods
					def initialize_defaults_for_filesystem
						# remove root directories that were not cleaned
						FileUtils.rm_rf Dir[test_root + '/tmp/*']
					end

					def finalize_defaults_for_filesystem
					end
				end
		end
	end
end