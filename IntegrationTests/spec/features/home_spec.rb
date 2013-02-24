require "spec_helper.rb"

feature "Home" do

		let(:proxy) { Ownet::Proxy.start }

		before { set_browser_proxy_to proxy }

		pending "Showing welcome message" do
			visit 'http://my.ownet'

			#page.should have_content '<welcome message>'
		end

		after { proxy.stop }
end