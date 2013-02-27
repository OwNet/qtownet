require "spec_helper.rb"

describe "Faker helper" do

	let(:fake) { 
		fake = Ownet::Proxy::Faker.new(10001) 
		fake.start
		fake
	}

	let(:faraday) {
		Faraday.new proxy: "http://127.0.0.1:#{fake.port}"
	}

	it "fakes one request" do
		fake.url 'http://ownet.fiit.stuba.sk/',
			with_body: data_from_fixture('web/ownet-index.html'), 
		    with_headers: { 'Content-Type' => 'text/html' }

		response = faraday.get 'http://ownet.fiit.stuba.sk/'
		response.status.should == 200
		response.headers['Content-Type'].should == 'text/html'
		response.body.force_encoding('UTF-8').should == data_from_fixture('web/ownet-index.html')

		fake.requests.count.should == 1
		fake.requests.last.url.should == 'http://ownet.fiit.stuba.sk/'
	end

  	it "fakes one request with long url" do
  		long_url = 'http://www.google-analytics.com/__utm.gif?utmwv=5.3.8&utms=4&utmn=870411303&utmhn=ownet.fiit.stuba.sk&utmcs=UTF-8&utmsr=1280x800&utmvp=994x462&utmsc=24-bit&utmul=sk&utmje=1&utmfl=-&utmdt=OwNet%20%7C%20about%20project&utmhid=704863773&utmr=-&utmp=%2F&utmac=UA-1816665-12&utmcc=__utma%3D148641961.1583670603.1354874383.1354874383.1354874383.1%3B%2B__utmz%3D148641961.1354874383.1.1.utmcsr%3D(direct)%7Cutmccn%3D(direct)%7Cutmcmd%3D(none)%3B&utmu=q~'
		fake.url long_url,
			with_body: 'body',
      		with_headers: { 'Content-Type' => 'text/html' }

		response = faraday.get long_url
		response.body.should == 'body'
		response.status.should == 200
		response.headers['Content-Type'].should == 'text/html'

		fake.requests.count.should == 1
		fake.requests.last.url.should == long_url
  	end

	it "fakes multiple requests" do
		fake.url 'http://ownet.fiit.stuba.sk/',
			with_body: data_from_fixture('web/ownet-index.html'), 
		    with_headers: { 'Content-Type' => 'text/html' }
		fake.url 'http://example.com/',
			with_status: 201, 
			with_body: 'body', 
		    with_headers: { 'Content-Type' => 'text/plain' }

		response = faraday.get 'http://ownet.fiit.stuba.sk/'
		response.status.should == 200
		response.headers['Content-Type'].should == 'text/html'
		response.body.force_encoding('UTF-8').should == data_from_fixture('web/ownet-index.html')

		response = faraday.get 'http://example.com/'
		response.status.should == 201
		response.headers['Content-Type'].should == 'text/plain'
		response.body.force_encoding('UTF-8').should == 'body'

		fake.requests.count.should == 2
		fake.requests.first.url.should == 'http://ownet.fiit.stuba.sk/'
		fake.requests.last.url.should == 'http://example.com/'
	end

	it "returns error when non-faked request is accessed" do
		response = faraday.get 'http://ownet.fiit.stuba.sk/'
		response.status.should == 555
		response.headers['Content-Type'].should == 'text/plain'
		response.headers['X-Ownet-Integration-Tests-Faker-Exception'].should == 'TRUE'
		response.headers['X-Ownet-Integration-Tests-Faker-Exception-Type'].should == 'Ownet::Includes::Faker::Faker::FakerError'
		response.headers['X-Ownet-Integration-Tests-Faker-Exception-Message'].should == 'Non-faked attempt to access http://ownet.fiit.stuba.sk/'
		response.body.from_yaml.should be_an Array
	end

	after { fake.stop }
end