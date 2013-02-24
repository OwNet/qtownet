require "spec_helper.rb"

describe "Session service" do
	let(:proxy) { Ownet::Proxy.start }
	
	it "sends success on create action with correct credentials" do
		insert_into :users, {
			first_name: 'First', 
			last_name: 'Last', 
			login: 'login', 
			password: 'password',
			role_id: 1
		}
		response = proxy.post('http://whatever.ownet/api/session', {
			login: 'login', 
			password: 'password'
		}.to_json)

		response.status.should be 201
		hash = response.body.from_json
		hash['user_id'].should_not be_nil
	end

	it "sends error on create action with wrong password" do
		insert_into :users, {
			first_name: 'First', 
			last_name: 'Last', 
			login: 'login', 
			password: 'password',
			role_id: 1
		}
		response = proxy.post('http://whatever.ownet/api/session', {
			login: 'login', 
			password: 'wrong-password'
		}.to_json)

		response.status.should be 400
		hash = response.body.from_json
		hash.should == {}
	end

	it "sends error on create action with wrong login" do
		insert_into :users, {
			first_name: 'First', 
			last_name: 'Last', 
			login: 'login', 
			password: 'password',
			role_id: 1
		}
		response = proxy.post('http://whatever.ownet/api/session', {
			login: 'wrong-login', 
			password: 'password'
		}.to_json)

		response.status.should be 400
		hash = response.body.from_json
		hash.should == {}
	end
	
	pending "sends error on create action when logged in" do
		# nemala by byt odpoved api konzistentna s tou hore?
		insert_into :users, {
			first_name: 'First', 
			last_name: 'Last', 
			login: 'login', 
			password: 'password',
			role_id: 1
		}
		proxy.post('http://whatever.ownet/api/session', {
			login: 'login', 
			password: 'password'
		}.to_json)
		response = proxy.post('http://whatever.ownet/api/session', {
			login: 'login', 
			password: 'password'
		}.to_json)

		response.status.should be 400
		response.body.should == 'null' 
		#hash = response.body.from_json
		#hash.should == {}
	end

	pending "sends success on del action" do
		# body je prazdne kvoli 204, pouzit iny status kod alebo neocakavat body
		create_user_and_log_in
		
		response = proxy.delete('http://whatever.ownet/api/session')
		response.status.should be 204
		hash = response.body.from_json
		hash['success'].should_not be_nil
	end

	pending "sends error on del action when not logged in" do
		# vrati 204, podla kodu by malo 400 (to iste cez curl)
		response = proxy.delete('http://whatever.ownet/api/session')
		response.status.should be 400
		hash = response.body.from_json
		hash.should be_nil
	end

	after { proxy.stop }
end