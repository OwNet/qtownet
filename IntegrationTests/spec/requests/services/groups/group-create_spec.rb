require "spec_helper.rb"

describe "group_service" do
	let(:proxy) { Ownet::Proxy.start }
	
	it "sends success on create action without approvement and without has_password" do
		
		create_user_and_log_in
		
		insert_into :group_types, {
			name: 'general'			
		}
		
		response = proxy.post('http://my.ownet/api/groups', {
			name: 'skupina', 
			description: 'desc',
			has_approvement: 0,
			has_password: 0,
			group_type: 1
		}.to_json)
		
		response.status.should be 201
		
	end
	
	it "sends success on create action with has_approvement and without has_password" do
		
		create_user_and_log_in
		
		insert_into :group_types, {
			name: 'general'			
		}
		
		response = proxy.post('http://my.ownet/api/groups', {
			name: 'skupina', 
			description: 'desc',
			has_approvement: 1,
			has_password: 0,
			group_type: 1
		}.to_json)
		
		response.status.should be 201
		
	end
	
	it "sends error on create action with has_approvement and with Has_password without password stated" do
		
		create_user_and_log_in
		
		insert_into :group_types, {
			name: 'general'			
		}
		
		response = proxy.post('http://my.ownet/api/groups', {
			name: 'skupina', 
			description: 'desc',
			has_approvement: 1,
			has_password: 1,
			group_type: 1
		}.to_json)
		
		response.status.should be 400
		hash = response.body.from_json
		hash['password'].should_not be 'required'
		
	end
	
	it "sends error on create action with has_approvement and with Has_password with password stated" do
		
		create_user_and_log_in
		
		insert_into :group_types, {
			name: 'general'			
		}
		
		pass = 'pass'
		
		response = proxy.post('http://my.ownet/api/groups', {
			name: 'skupina', 
			description: 'desc',
			has_approvement: 1,
			has_password: 1,
			group_type: 1,
			password: 'pass'
		}.to_json)
		
		
		first_row_from(:groups, order: 'password DESC').at(3).should_not == 'pass'
		first_row_from(:groups, order: 'password DESC').at(9).should_not == ''
		response.status.should be 201
	
	end
	
	it "sends error on missing arguments" do
		
		create_user_and_log_in
		
		insert_into :group_types, {
			name: 'general'			
		}
		
		response = proxy.post('http://my.ownet/api/groups', {
			
		}.to_json)
		
		response.status.should be 400
		hash = response.body.from_json
		hash['has_approvement'].should_not be 'required'
		hash['name'].should_not be 'required'
		hash['has_password'].should_not be 'required'
		hash['description'].should_not be 'required'
		hash['group_type'].should_not be 'required'
		
	end
	
	it "sends error on creating of group without beeing logged_in" do
		
		
		insert_into :group_types, {
			name: 'general'			
		}
		
		response = proxy.post('http://my.ownet/api/groups', {
			name: 'skupina', 
			description: 'desc',
			has_approvement: 1,
			has_password: 0,
			group_type: 1
		}.to_json)
		
		response.status.should be 403
		
	end
	
	after { proxy.stop }
end