require "rubygems"
require "bundler/setup"

require 'json'
require 'capybara/rspec'
require 'selenium-webdriver'

require 'rbconfig'

def windows?
	(RbConfig::CONFIG['host_os'] =~ /mswin|mingw|cygwin/)
end

def test_root
	File.dirname(__FILE__) + "/.."
end

# load spec/support helpers
Dir[File.dirname(__FILE__) + "/support/**/*.rb"].each {|f| require f}

# configure rspec
RSpec.configure	do |config|
	config.include(FixtureHelpers)
	config.include(DatabaseHelpers)
	config.include(LoginHelper)
	config.include(ApiHelpers)
	config.include(BrowserHelpers)
	
	config.include(Ownet)
end

# configure capybara
Capybara.configure do |config|
	Capybara.register_driver :firefox do |app|
		profile = Selenium::WebDriver::Firefox::Profile.new
		profile.proxy = Selenium::WebDriver::Proxy.new( :http => "127.0.0.1:5081")
		Capybara::Selenium::Driver.new app, :profile => profile
	end

  	config.default_driver = :firefox
  	config.run_server = false 			# no application server, we will use proxy
  	config.default_wait_time = 3 			# retry for 3s on async events
  	config.ignore_hidden_elements = true	# have_selector should fail for hidden selectors
end

# initialize and finalize
Ownet::Proxy::initialize_defaults
at_exit { Ownet::Proxy::finalize_defaults }
