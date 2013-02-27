require "spec_helper.rb"

feature "Inject" do

		let(:proxy) { Ownet::Proxy.start }

		before { set_browser_proxy_to proxy }

		pending "Showing inject" do
			# fake http://ownet.fiit.stuba.sk
			# fake images, stylesheets and scripts

			# visit 'http://ownet.fiit.stuba.sk'

			# inject should be visible
		end

		pending "Hiding inject" do
			# fake http://ownet.fiit.stuba.sk
			# fake images, stylesheets and scripts

			# visit 'http://ownet.fiit.stuba.sk'

			# hide inject

			# inject should not be visible
		end

		pending "Setting page caching preferences" do
			# fake http://ownet.fiit.stuba.sk
			# fake images, stylesheets and scripts

			# visit 'http://ownet.fiit.stuba.sk'

			# set page caching preferences

			# check db
		end

		after { proxy.stop }
end