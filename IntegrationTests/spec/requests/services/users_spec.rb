require "spec_helper.rb"
require "support/database_helpers.rb"

# NOTE: users row => [id, first_name, last_name, login, role, password, mail,...]

describe "Users service" do
	let(:proxy) { Ownet::Proxy.start }

	it "sends success on create user" do

		response = proxy.post('http://my.ownet/api/users', {
			id: 1,
			login: 'login',
			password: 'password',
			first_name: 'First',
			last_name: 'Last',
			email: 'mail@mail.ownet',
		}.to_json)

		response.status.should be 201

		user = first_row_from 'users'
		user[1].should == 'First'
		user[3].should == 'login'
	end

	it "sends success on delete user" do

		insert_into :users, {
			id: 1 ,
			first_name: 'First',
			last_name: 'Last',
			login: 'login',
			password: 'password',
			role_id: 1
		}

		proxy.post( 'http://my.ownet/api/session',{
			login: 'login',
			password: 'password'
		}.to_json)

		response = proxy.delete( 'http://my.ownet/api/users/1')

		response.status.should be 204

		duser = first_row_from 'users'
		duser.should be nil

	end

	it "sends success on edit user" do

		user = {
			id: 1,
			login: 'login',
			password: 'password',
			first_name: 'First',
			last_name: 'Last',
			email: 'mail@mail.ownet',
			role_id: 1
		}

		insert_into :users, user

		response = proxy.post( 'http://my.ownet/api/session',{
			login: 'login',
			password: 'password'
		}.to_json)

		user[:first_name] = 'edited'

		response = proxy.put( 'http://my.ownet/api/users/1', user.to_json)

		response.status.should be 200

		row = first_row_from('users')

		row[0].should be user[:id]
		row[1].should eq user[:first_name]

	end


	after { proxy.stop }
end
