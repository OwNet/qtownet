module LoginHelper
	require "support/database_helpers.rb"
	##
	#
	# Number of rows in table *entity*.
	#
	def create_user_and_login 
		insert_into :users, {
			id: 1 ,
			first_name: 'First',
			last_name: 'Last',
			login: 'login',
			password: 'password',
			role_id: 1,
			uid: "a"
		}

		proxy.post( 'http://my.ownet/api/session',{
			login: 'login',
			password: 'password'
		}.to_json)
	end

end