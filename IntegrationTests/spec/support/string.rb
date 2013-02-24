##
#
# Extensions to String class. 
#
class String
	##
	#
	# Parse JSON.
	#
	def from_json
		JSON.parse(self)
	end

	##
	#
	# Parse YAML.
	#
	def from_yaml
		YAML.load(self)
	end

	##
	#
	# Parse Marshal.
	#
	def from_marshal
		Marshal.load(self)
	end
end