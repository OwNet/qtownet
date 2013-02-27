require "spec_helper.rb"

describe "Sync new items in journal" do
	let(:proxy) { Ownet::Proxy.start }

	before do
		proxy.fake.url 'http://ownet.fiit.stuba.sk/',
			with_body: data_from_fixture('web/ownet-index.html'),
			with_headers: { 'Content-Type' => 'text/html' }
	end

	it "syncs cache" do
		# deal with inject
		proxy.get('http://ownet.fiit.stuba.sk')
    sleep 1

    response = proxy.post('http://test.ownet/api/sync/get_updates', {
      client_id: 999,
      sync_all_groups: true,
      client_record_numbers: {}
    }.to_json)

		response.status.should == 200
		response.headers['Content-Type'].should == 'application/json'
    body = response.body.from_json
    body.count.should == 1
    content = body.first['content'].from_json
    content.count.should == 1
    content.first['columns']['cache_id'].should == 2749733511
	end

  it "syncs new users" do
		proxy.post('http://test.ownet/api/users', {
			login: 'login',
			password: 'password',
      last_name: 'last_name',
      first_name: 'first_name'
		}.to_json)
    sleep 1

    response = proxy.post('http://test.ownet/api/sync/get_updates', {
      client_id: 999,
      sync_all_groups: true,
      client_record_numbers: {}
    }.to_json)

		response.status.should == 200
		response.headers['Content-Type'].should == 'application/json'
    body = response.body.from_json
    body.count.should == 1
    content = body.first['content'].from_json
    content.count.should == 1
    content.first['columns']['login'].should == 'login'
    content.first['columns']['last_name'].should == 'last_name'
  end

  it "syncs sends new items only" do
		proxy.get('http://ownet.fiit.stuba.sk')
		proxy.post('http://test.ownet/api/users', {
			login: 'login',
			password: 'password',
      last_name: 'last_name',
      first_name: 'first_name'
		}.to_json)
    sleep 1

    response = proxy.post('http://test.ownet/api/sync/get_updates', {
      client_id: 999,
      sync_all_groups: true,
      client_record_numbers: {}
    }.to_json)

		response.status.should == 200
		response.headers['Content-Type'].should == 'application/json'
    body = response.body.from_json
    body.count.should == 2
    client_id = body.first['client_id']

    response = proxy.post('http://test.ownet/api/sync/get_updates', {
      client_id: 999,
      sync_all_groups: true,
      client_record_numbers: {
        "" => {
          client_id => 0
        }
      }
    }.to_json)

		response.status.should == 200
		response.headers['Content-Type'].should == 'application/json'
    body = response.body.from_json
    body.count.should == 1
  end

	after { proxy.stop }
end