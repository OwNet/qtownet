require "spec_helper.rb"

describe "Static file serving" do
	let(:proxy) { Ownet::Proxy.start }

	it "sends HTML file" do
		response = proxy.get('http://my.ownet')
		response.status.should == 200
		response.headers['Content-Type'].should == 'text/html'
	end

	it "sends JS file" do
		response = proxy.get('http://share.ownet/lib/require.js')
		response.status.should == 200
		response.headers['Content-Type'].should == 'application/javascript'
		response.body.should == data_from_fixture('static/require.js')
	end

	it "sends multiple files" do
		proxy.get('http://my.ownet')

		response = proxy.get('http://share.ownet/lib/require.js')
		response.status.should == 200
		response.headers['Content-Type'].should == 'application/javascript'
		response.body.should == data_from_fixture('static/require.js')

		response = proxy.get('http://share.ownet/lib/require.js')
		response.status.should == 200
		response.headers['Content-Type'].should == 'application/javascript'
		response.body.should == data_from_fixture('static/require.js')
	end

	pending "sends nice error pages" do
		#Faraday::Error::ConnectionFailed => end of file reached

		response = proxy.get('http://non-existent.ownet/non-existent.html')
		response.status.should == 404
		response.headers['Content-Type'].should == '...'
		response.body.should == '...'
	end

	after { proxy.stop }
end