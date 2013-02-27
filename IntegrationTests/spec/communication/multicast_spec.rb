require "spec_helper.rb"

describe "Local network communication" do
	let(:proxy) { Ownet::Proxy.start }

	it "responds to no external heartbeat" do
		proxy.sleep 1 * proxy.period + 1.second

		proxy.multicast.messages.count.should == 1

		message = proxy.multicast.messages.last
		message['id'].should > 0
		message['initialized'].should > 0
		message['port'].should == 5081
		message['score'].should == 1
		message['status'].should == 'server'

		proxy.sleep 1 * proxy.period

		proxy.multicast.messages.count.should == 2

		message = proxy.multicast.messages.last
		message['status'].should == 'server'
	end

	it "responds to lower score external heartbeat" do
		# does not work yet

		proxy.multicast.send(id: 1, score: 0, status: 'client')
		proxy.multicast.send(id: 2, score: 0, status: 'client')
		proxy.multicast.send(id: 3, score: 0, status: 'server')

		proxy.sleep 1 * proxy.period + 1.second

		proxy.multicast.messages.count.should == 4
		proxy.multicast.messages.last['status'].should == 'server'

		proxy.multicast.send(id: 'client-4', score: 5, status: 'server', initialized: 1_000_000)

		proxy.sleep 1 * proxy.period

		proxy.multicast.messages.count.should == 6
		proxy.multicast.messages.last['status'].should == 'client'
	end

	it "responds to higher score external heartbeat" do
		# does not work yet
		
		proxy.multicast.send(id: 1, score: 0, status: 'client')
		proxy.multicast.send(id: 2, score: 0, status: 'client')
		proxy.multicast.send(id: 3, score: 5, status: 'server', initialized: 1_000_000)

		proxy.sleep 1 * proxy.period + 1.second

		proxy.multicast.messages.count.should == 4
		proxy.multicast.messages.last['status'].should == 'client'

		proxy.sleep 1 * proxy.period

		proxy.multicast.send(id: 1, score: 0, status: 'client')
		proxy.multicast.send(id: 2, score: 0, status: 'client')

		proxy.sleep 2 * proxy.period

		proxy.multicast.messages.last['status'].should == 'server'
	end

	describe "with other proxy" do
		let(:other) { 
			Ownet::Proxy.start listen_port: 5083, faker: nil
		}

		it "chooses roles" do
			proxy # lazy
			sleep 1.second

			other # lazy
			sleep 1 * other.period + 1.second

			proxy.multicast.messages.count.should == 2

			message = proxy.multicast.messages[0]
			message['id'].should > 0
			message['initialized'].should > 0
			message['port'].should == 5081
			message['score'].should == 1
			message['status'].should == 'server'

			message = proxy.multicast.messages[1]
			message['id'].should > 0
			message['initialized'].should > 0
			message['port'].should == 5083
			message['score'].should == 1
			message['status'].should == 'client'
		end

		after { other.stop }
	end

	after { proxy.stop }
end