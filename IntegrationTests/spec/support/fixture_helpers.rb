module FixtureHelpers
	##
	#
	# Load data from file *fixture_path* in test_root/spec/fixtures/
	#
	def data_from_fixture(fixture_path)
		data = File.read(test_root + '/spec/fixtures/' + fixture_path)
		data.gsub!("\r\n", "\n") if windows?
		data
	end
end