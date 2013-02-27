module BrowserHelpers

	##
	#
	# Use this helper to switch browser between proxies (you can have multiple proxies). 
	#
	def set_browser_proxy_to(proxy_object)
		#page.driver.browser.set_proxy host: "127.0.0.1", 
		#	port: proxy_object.options[:listen_port]
	end

	##
	#
	# Sleep for 1 second time.
	#
	def delay
		sleep 1
	end

	##
	#
	# Save current page screenshot to test_root/tmp directory
	#
	def take_screenshot
		page.driver.render test_root + "/tmp/#{(Time.now.to_f * 1.milion).round}.png"
	end
end