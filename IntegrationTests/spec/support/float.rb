##
#
# Extensions to float.
#
class Float
	def period
		self * 5
	end
	alias :periods :period

	def second
		self
	end
	alias :seconds :second

	def minute
		self * 60
	end
	alias :minutes :minute

	def thousand
		self * 1000
	end
	alias :thousands :thousand

	def milion
		self * 1000000
	end
	alias :milions :milion
end