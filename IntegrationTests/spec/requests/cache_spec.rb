require "spec_helper.rb"

describe "Cached web file serving" do
	let(:proxy) { Ownet::Proxy.start }

	before do
		proxy.fake.url 'http://ownet.fiit.stuba.sk/', 
			with_body: data_from_fixture('web/ownet-index.html'), 
			with_headers: { 'Content-Type' => 'text/html' }
	end	

	pending "sends HTML file" do
		# deal with inject
		proxy.get('http://ownet.fiit.stuba.sk')

		# file will have to be retrieved from cache
		proxy.fakes.clean_all

		response = proxy.get('http://ownet.fiit.stuba.sk')
		response.status.should == 200
		response.headers['Content-Type'].should == 'text/html'
		response.body.force_encoding('UTF-8').should == data_from_fixture('web/ownet-index.html')
	end

	pending "sends JS file" do
	end

	pending "sends multiple files" do
	end

	pending "sends nice error pages" do
		#TODO: Fake non-existent domain.

		proxy.get('http://non-existent-domain.non-existent-tld')

		response = proxy.get('http://non-existent-domain.non-existent-tld')
		response.status.should == 404
		response.headers['Content-Type'].should == '...'
		response.body.should == '...'
	end

	after { proxy.stop }
end