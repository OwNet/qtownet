require "spec_helper.rb"

feature "Registration" do

		let(:proxy) { Ownet::Proxy.start }

		before { set_browser_proxy_to proxy }

		scenario "Registering new user" do
			visit 'http://my.ownet'

			click_link 'Register'
			
			current_url.should end_with '/registration'

			fill_in 'login', with: 'login'
			fill_in 'password', with: 'password'
			# fill_in 'password_confirmation', with: 'password'
			fill_in 'first_name', with: 'First'
			fill_in 'last_name', with: 'Last'
			fill_in 'email', with: 'login@example.com'
			fill_in 'phone', with: '+ 123 456 789'
			click_button 'Sign Up'

			page.should have_content 'Registration was successful.'
			
			count_of(:users).should == 1

			row = first_row_from(:users, where: { login: 'login' })
			# check stored attributes
		end

		scenario "Registering new user with incorrect inputs" do
			# hanging on selenium/linux, failing on selenium/windows

			visit 'http://my.ownet'
			click_link 'Register'

			#
			# no inputs
			#

			click_button 'Sign Up'
			
			delay
			page.should have_no_content 'Registration was successful.'

			# page.should have_content 'Please fill in login.'
			# page.should have_content 'Please fill in password.'
			# ...

			#
			# no login, invalid e-mail
			#

			fill_in 'password', with: 'password'
			fill_in 'first_name', with: 'First'
			fill_in 'last_name', with: 'Last'
			fill_in 'email', with: 'loginexamplecom'
			fill_in 'phone', with: '+ 123 456 789'
			click_button 'Sign Up'

			# page.should have_content 'Please fill in login.'
			# page.should have_content 'Please fill in a valid e-mail address.'

			#
			# finally all ok
			#

			fill_in 'login', with: 'login'
			fill_in 'password', with: 'password'
			# fill_in 'password_confirmation', with: 'mistyped_password'
			fill_in 'first_name', with: 'First'
			fill_in 'last_name', with: 'Last'
			fill_in 'email', with: 'login@example.com'
			fill_in 'phone', with: '+ 123 456 789'
			click_button 'Sign Up'

			# page.should have_content 'Registration successful...'

			# count_of(:users).should == 1

			row = first_row_from(:users, where: { login: 'login' })
			# check stored attributes
		end

		scenario "Registering existing user" do
			insert_into :users, {
				first_name: 'First', 
				last_name: 'Last', 
				login: 'login', 
				role_id: 1, 
				password: '<hash?>'
			}

			visit 'http://my.ownet'
			click_link 'Register'

			fill_in 'login', with: 'login'
			fill_in 'password', with: 'password'
			# fill_in 'password_confirmation', with: 'password'
			fill_in 'first_name', with: 'First'
			fill_in 'last_name', with: 'Last'
			fill_in 'email', with: 'login@example.com'
			fill_in 'phone', with: '+ 123 456 789'
			click_button 'Sign Up'

			# page.should have_content 'User with login login already exists.'

			count_of(:users).should == 1
		end

		after { proxy.stop }
end