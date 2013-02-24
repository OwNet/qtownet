module ApiHelpers

	##
	#
	# Helper to create user and log in. You can override user info with *user_data*.
	#
	def create_user_and_log_in(user_data = {})
		user_data = {
						first_name: 'First', 
						last_name: 'Last', 
						login: 'login', 
						role_id: 1, 
						password: 'password'
					}.merge(user_data)
		insert_into :users, user_data

		proxy.post('http://whatever.ownet/api/session', {
			login: user_data[:login], 
			password: user_data[:password]
		}.to_json)
	end
end