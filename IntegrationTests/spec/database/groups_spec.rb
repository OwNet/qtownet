require "spec_helper.rb"

describe "Groups" do

	let(:proxy) { Ownet::Proxy.start }

	pending "should delete users when group is deleted" do
		# insert into users, groups, ...
		# delete user
		# check if group is deleted
	end

	after { proxy.stop }
end