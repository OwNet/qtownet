require "spec_helper.rb"

describe "Local network synchronization" do
	let(:first) { 
		Ownet::Proxy.start listen_port: 5082, faker: nil
	}
	let(:second) { 
		Ownet::Proxy.start listen_port: 5083, faker: nil
	}

	it "synchronizes database" do
		first # lazy
		first.post('http://test.ownet/api/users', {
			login: 'login',
			password: 'password',
		  	last_name: 'last_name',
		  	first_name: 'first_name'
		}.to_json)

		sleep 1.second

		second # lazy
		sleep second.period + 1.second

		response = first.get 'http://test.ownet/api/sync/now'
		puts response.body

		response = second.get 'http://test.ownet/api/sync/now'
		puts response.body

		sleep 10

		response = second.post('http://test.ownet/api/sync/get_updates', {
	      	client_id: 999,
	      	sync_all_groups: true,
	      	client_record_numbers: {}
	    }.to_json)

		response.status.should == 200
		response.headers['Content-Type'].should == 'application/json'
		puts response.body
    	objects = response.body.from_json
    	objects.count.should == 1
	end

	after { first.stop }
	after { second.stop }
end