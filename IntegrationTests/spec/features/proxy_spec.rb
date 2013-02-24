require "spec_helper.rb"

feature "Proxy" do

		let(:proxy) { Ownet::Proxy.start }

		before { set_browser_proxy_to proxy }

		pending "Visiting uncached web page" do
			# fake test page with a few dependency files

			# visit page

			# test css

			# test js

			# test downloaded files
		end

		pending "Visiting cached web page" do
			# fake test page with a few dependency files

			# visit page

			proxy.faker.clean_all

			# visit page

			# test css

			# test js

			# test downloaded files
		end

		pending "Visiting ownet.fiit.stuba.sk" do
			# not working on selenium

			proxy.fake.urls_saved_in(
				data_from_fixture('web/ownet-fiit-stuba-sk.marshal')
			)

			visit 'http://ownet.fiit.stuba.sk'
			proxy.faker.requests.count.should == 23

			proxy.faker.clean_all

			# browser already caches static files
			visit 'http://ownet.fiit.stuba.sk'

			page.driver.console_messages.should == []
			page.driver.error_messages.should == []

			page.should have_content 'About the project'
			page.should have_content 'Despite of the advancements in information and telecommunication technologies, slow and intermittent Internet connection is still a serious issue in many places of the World.'
			
			# test css, selector should be hidden
			page.should have_no_selector '.about .toggle', visible: true

			# this does not trigger javascript event
			#find('.about h2').click

			# test javascript by executing script instead
			page.execute_script('$(".about .toggle:first").slideToggle(100);');			

			page.driver.console_messages.should == []
			page.driver.error_messages.should == []

			# selector should be now visible
			page.should have_selector '.about .toggle', visible: true

			# except google analytics
			proxy.faker.requests.select { |request|
				not request.url.start_with? 'http://www.google-analytics.com/'
			}.count.should == 0
		end

		pending "Prefetching unavailable web page" do
			# fake http://ownet.fiit.stuba.sk
			# fake images, stylesheets and scripts

			# visit 'http://ownet.fiit.stuba.sk'

			proxy.fakes.clean_all

			# visit 'http://ownet.fiit.stuba.sk'
			# test page

			# click_link 'Link to unavailable page'
			# current_path.should == '...'
			# test unavailable error page

			# click_link 'Add to prefetch list'
			# current_path.should == '...'
			# test original page
		end

		after { proxy.stop }
end