require "spec_helper.rb"

describe "Migrations" do

	let(:proxy) { 
		Ownet::Proxy.new symlink_migrations: false, premigrate_database: false 
	}

	it "should be run only once"  do
		# all migrations
		proxy.start

		number_of_migration_files = Dir[proxy.migrations_directory + '/*'].count { |file| File.file?(file) }
		count_of(:migrations).should == number_of_migration_files

		last_migration_file = File.basename(Dir[proxy.migrations_directory + '/*'].sort.last)
		first_row_from(:migrations, order: 'name DESC').first.should == last_migration_file

		# new migration
		proxy.restart {
			File.open(proxy.migrations_directory + '/30121221-last-migration.sql', 'w') { |file|
				file.write "CREATE TABLE last_migration_test (id INTEGER PRIMARY KEY, name TEXT);"
			}
			File.open(proxy.migrations_directory + '/30121222-last-migration.sql', 'w') { |file|
				file.write "INSERT INTO last_migration_test (name) VALUES ('first');"
			}
		}

		count_of(:migrations).should == number_of_migration_files + 2
		first_row_from(:migrations, order: 'name DESC').first.should == '30121222-last-migration.sql'

		# run migration only once and do not lose any data
		insert_into :last_migration_test, name: 'second'
		proxy.restart {
			insert_into :last_migration_test, name: 'third'
		}

		count_of(:migrations).should == number_of_migration_files + 2
		first_row_from(:migrations, order: 'name DESC').first.should == '30121222-last-migration.sql'

		count_of(:last_migration_test).should == 3
		first_row_from(:last_migration_test, order: 'name DESC').last.should == 'third'
	end

	it "should not be run after error" do
		number_of_migration_files = Dir[proxy.migrations_directory + '/*'].count { |file| File.file?(file) }
		last_migration_file = File.basename(Dir[proxy.migrations_directory + '/*'].sort.last)

		File.open(proxy.migrations_directory + '/30121221-last-migration.sql', 'w') { |file|
			file.write "CREE TLE last_migration_test (name TEXT PRARY KEY);"
		}
		File.open(proxy.migrations_directory + '/30121222-last-migration.sql', 'w') { |file|
			file.write "CREATE TABLE last_migration_test (name TEXT PRIMARY KEY);"
			file.write "INSERT INTO last_migration_test (name) VALUES ('first');"
		}

		proxy.start

		count_of(:migrations).should == number_of_migration_files
		first_row_from(:migrations, order: 'name DESC').first.should == last_migration_file
		lambda {
			first_row_from(:last_migration_test).first 
		}.should raise_error
	end

	after { proxy.stop }
end