require 'sqlite3'

module Ownet
	module Includes
		module Database
			##
			#
			# Proxy sqlite3 database connection
			#
			attr_reader :database

			private

				def initialize_database
					if options[:premigrate_database]
						FileUtils.cp Ownet::Includes::Database::default_database, 
							data_directory + "/ownet.sqlite"
					end
				end

				def start_database
					@database = SQLite3::Database.open data_directory + "/ownet.sqlite"
				end

				def self.included(base)
				    base.extend ClassMethods 
				end

				def self.default_database
					test_root + '/tmp/ownet.default.sqlite'
				end

				module ClassMethods

					def initialize_defaults_for_database
						if defaults[:premigrate_database]
							database = SQLite3::Database.open(
								Ownet::Includes::Database::default_database
							)
							database.execute("CREATE TABLE IF NOT EXISTS migrations (name TEXT PRIMARY KEY); ")
							Dir[test_root + '/' + Ownet::Proxy::defaults[:migrations_path] + '/*'].sort.each { |file|
								File.open(file) { |f|
									sql = f.read
									database.transaction
									begin
										database.execute_batch(sql)
										database.execute("INSERT INTO migrations (name) VALUES (?);", 
											File.basename(file))
										database.commit
									rescue => e
										puts "Premigrate database: #{file}"
										puts e
										database.rollback
									end
								}
							}
							database.close
						end
					end

					def finalize_defaults_for_database
					end
				end
		end
	end
end