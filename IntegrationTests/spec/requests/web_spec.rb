require "spec_helper.rb"

describe "Uncached web file serving" do
	let(:proxy) { Ownet::Proxy.start }

	pending "sends HTML file" do
		# deal with inject
		proxy.fake.url 'http://ownet.fiit.stuba.sk/', 
			with_body: data_from_fixture('web/ownet-index.html'), 
			with_headers: { 'Content-Type' => 'text/html' }

		response = proxy.get('http://ownet.fiit.stuba.sk')

		# proxy last request example
		proxy.faker.requests.last.url.should == 'http://ownet.fiit.stuba.sk/'

		response.status.should == 200
		response.headers['Content-Type'].should == 'text/html'
		response.body.force_encoding('UTF-8').should == data_from_fixture('web/ownet-index.html')
	end

	pending "sends JS file" do
	end

	it "sends multiple files" do
    request_urls = [
      'http://ownet.fiit.stuba.sk/',
      'http://ownet.fiit.stuba.sk/js/jquery.min.js',
      'http://ownet.fiit.stuba.sk/js/jquery.tools.min.js',
      'http://ownet.fiit.stuba.sk/style.css',
      'http://www.google-analytics.com/ga.js',
      'http://www.google-analytics.com/__utm.gif?utmwv=5.3.8&utms=4&utmn=870411303&utmhn=ownet.fiit.stuba.sk&utmcs=UTF-8&utmsr=1280x800&utmvp=994x462&utmsc=24-bit&utmul=sk&utmje=1&utmfl=-&utmdt=OwNet%20%7C%20about%20project&utmhid=704863773&utmr=-&utmp=%2F&utmac=UA-1816665-12&utmcc=__utma%3D148641961.1583670603.1354874383.1354874383.1354874383.1%3B%2B__utmz%3D148641961.1354874383.1.1.utmcsr%3D(direct)%7Cutmccn%3D(direct)%7Cutmcmd%3D(none)%3B&utmu=q~',
      'http://ownet.fiit.stuba.sk/images/ownet1.png',
      'http://ownet.fiit.stuba.sk/images/slogan.jpg',
      'http://ownet.fiit.stuba.sk/images/sk.png',
      'http://ownet.fiit.stuba.sk/images/uk.png',
      'http://ownet.fiit.stuba.sk/images/fb.png',
      'http://ownet.fiit.stuba.sk/images/menu_about.jpg',
      'http://ownet.fiit.stuba.sk/images/menu_use.jpg',
      'http://ownet.fiit.stuba.sk/images/menu_faq.jpg',
      'http://ownet.fiit.stuba.sk/images/menu_contact.jpg',
      'http://ownet.fiit.stuba.sk/images/screens.jpg',
      'http://ownet.fiit.stuba.sk/images/world.png',
      'http://ownet.fiit.stuba.sk/images/gear.jpg',
      'http://ownet.fiit.stuba.sk/images/who.jpg',
      'http://ownet.fiit.stuba.sk/images/features.jpg',
      'http://ownet.fiit.stuba.sk/images/header.jpg',
      'http://ownet.fiit.stuba.sk/images/cont_bg.jpg',
      'http://ownet.fiit.stuba.sk/images/footer_bg.jpg'
    ]

    request_urls.each do |url|
      proxy.fake.url url,
        with_body: url,
        with_headers: { 'Content-Type' => 'text/html' }
      proxy.get url
    end

    request_urls.count.should == proxy.faker.requests.count
	end

	pending "sends nice error pages" do
		#TODO: Fake non-existent domain.

		response = proxy.get('http://non-existent-domain.non-existent-tld')
		response.status.should == 404
		response.headers['Content-Type'].should == '...'
		response.body.should == '...'
	end

	after { proxy.stop }
end