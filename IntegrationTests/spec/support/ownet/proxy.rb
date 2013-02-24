require 'yaml'

module Ownet
	class Proxy
		##
		#
		# Access proxy server options
		#
		attr_reader :options

		include Includes::Requests
		include Includes::Faker
		include Includes::Multicast
		include Includes::Database
		include Includes::Filesystem
		include Includes::Process
		include Includes::Utils

		##
		#
		# Clean test_root/tmp directory, clean log/ownet_proxy.log, premigrate database, 
		# start default faker and multicast inspector
		#
		def self.initialize_defaults
			@@defaults = {
				binary_path: "<set binary_path in config.yml>", 
				static_path: "<set static_path in config.yml>",
				migrations_path: "<set migrations_path in config.yml>", 
				listen_port: 5081,
				faker_port: 7081, 
				multicast_address: '229.229.229.1', 
				multicast_port: 5081,
				premigrate_database: true
			}
			@@defaults.merge!(
				symlink_static: true, 
				symlink_migrations: true
			) unless windows?
			@@defaults.merge!(config_options)			

			%w{filesystem process database faker multicast}.each { |m|
				send "initialize_defaults_for_#{m}"
			}
		end

		def self.finalize_defaults
			%w{multicast faker database process filesystem}.each { |m|
				send "finalize_defaults_for_#{m}"
			}
		end

		def self.defaults
			@@defaults
		end

		##
		#
		# Initialize filesystem, copy premigrated database, clean faker 
		# and multicast, etc.
		#
		def initialize(instance_options = {})
			@options = @@defaults.merge(instance_options)

			%w{filesystem database requests faker multicast}.each { |m|
				send "initialize_#{m}"
			}
		end

		##
		#
		# Start proxy server process
		#
		def start
			%w{process database}.each { |m|
				send "start_#{m}"
			}
			nil
		end

		##
		#
		# Stop proxy server process
		#
		def stop
			%w{process}.each { |m|
				send "stop_#{m}"
			}
			nil
		end

		##
		#
		# Restop proxy server process and yield given block between
		#
		def restart
			stop
			yield if block_given?
			start
		end

		##
		#
		# Convenient class method to start proxy
		#
		def self.start(instance_options = {})
			proxy = self.new(instance_options)
			proxy.start
			proxy
		end

		private

			def self.config_options
				@@config_options ||= YAML.load_file(File.dirname(__FILE__) + '/../config.yml')\
										['ownet_proxy']\
										.inject({}){|memo,(k,v)| memo[k.to_sym] = v; memo}
			end
	end
end