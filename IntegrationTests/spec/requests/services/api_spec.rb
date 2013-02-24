require "spec_helper.rb"

describe "API request dispatching" do
	let(:proxy) { Ownet::Proxy.start }
	
	it "sends API response" do
		response = proxy.get('http://whatever.ownet/api/sample')
		response.status.should == 200
		response.headers['Content-Type'].should == 'application/json'
		response.body.from_json['module'].should == 'sample'
	end
	
	it "sends multiple API responses" do
		proxy.get('http://whatever.ownet/api/sample')
		
		response = proxy.get('http://whatever.ownet/api/sample')
		response.status.should == 200
		response.headers['Content-Type'].should == 'application/json'
		response.body.from_json['module'].should == 'sample'
	end

	it "sends nice error pages" do
		response = proxy.get('http://whatever.ownet/api/non-existent-api-module')
		response.status.should == 404
		#response.headers['Content-Type'].should == 'application/json'
		#response.body.from_json['Status'].should == 'FAILED'
	end

	after { proxy.stop }
end