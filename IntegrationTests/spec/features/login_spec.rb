require "spec_helper.rb"

feature "Login" do

		let(:proxy) { Ownet::Proxy.start }

		before { set_browser_proxy_to proxy }

		pending "Signing in with correct credentials" do
			# hashed password?

			insert_into :users, {
				first_name: 'First', 
				last_name: 'Last', 
				login: 'login', 
				role_id: 1, 
				password: 'password'
			}

			visit 'http://my.ownet'
			click_link 'Login'

			fill_in 'login', with: 'login'
			fill_in 'password', with: 'password'
			click_button 'Log In'

			page.should have_content 'Login successful'

			# within("#user_name") { 
			 	page.should have_content 'First Last'
			# }
		 	page.should have_content 'Logout'

			#
			# reload to test session
			#

			#visit 'http://my.ownet'

			# within("#user_name") { 
			# 	page.should have_content 'First Last'
			# }
		 	#page.should have_content 'Logout'
		end

		scenario "Signing in with incorrect credentials" do
			insert_into :users, {
				first_name: 'First', 
				last_name: 'Last', 
				login: 'login', 
				role_id: 1, 
				password: 'password'
			}

			visit 'http://my.ownet'
			click_link 'Login'

			#
			# incorrect password
			#

			fill_in 'login', with: 'login'
			fill_in 'password', with: 'bad_password'
			click_button 'Log In'

			page.should have_content 'Login failed'

			# within("#user_name") { 
			 	page.should_not have_content 'First Last'
			# }

			#
			# incorrect login
			#

			fill_in 'login', with: 'bad_login'
			fill_in 'password', with: 'password'
			click_button 'Log In'

			# page.should have_content 'Login or password was incorrect.'

			# within("#user_name") { 
			 	page.should_not have_content 'First Last'
			# }

			#
			# finally correct credentials
			#

			#fill_in 'login', with: 'login'
			#fill_in 'password', with: 'password'
			#click_button 'Log In'

			# page.should have_content 'You have been logged in.'

			# within("#user_name") { 
			# 	page.should have_content 'First Last'
			# }

			#
			# reload to test session
			#

			#visit 'http://my.ownet'

			# within("#user_name") { 
			# 	page.should have_content 'First Last'
			# }
		end

		pending "Signing out" do
			# fails, user name and logout link on reload issue?
			create_user_and_log_in

			visit 'http://my.ownet'

			click_link 'Logout'

			page.should have_content 'Login'

			# within("#user_name") { 
			 	page.should_not have_content 'First Last'
			# }
		end

		after { proxy.stop }
end