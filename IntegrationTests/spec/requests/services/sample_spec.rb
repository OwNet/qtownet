require "spec_helper.rb"

describe "Sample service" do
	let(:proxy) { Ownet::Proxy.start }
	
	it "sends response on index action" do
		response = proxy.get('http://whatever.ownet/api/sample')
		hash = response.body.from_json
		hash['module'].should == 'sample'
	end

	after { proxy.stop }
end