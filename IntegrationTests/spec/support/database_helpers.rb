module DatabaseHelpers
	##
	#
	# Number of rows in table *entity*.
	#
	def count_of(entity)
		execute_with_retry("SELECT COUNT(*) FROM #{entity};").first[0]
	end

	##
	#
	# Select all rows from *table*. Options are *where*, *order* and *limit*. Values are inserted directly into query.
	#
	def select_from(table, options = {})
		where = if options[:where]
			conditions = options[:where].map{ |column, value| 
				"#{column} = #{value}" 
			}.join(', ')
			"WHERE #{conditions}"
		end
		order = if options[:order]
			"ORDER BY #{options[:order]}"
		end
		limit = if options[:limit]
			"LIMIT #{options[:limit]}"
		end
		execute_with_retry "SELECT * FROM #{table} #{where} #{order} #{limit};"
	end

	##
	#
	# Execute select_from with limit: 1 option.
	#
	def first_row_from(table, options = {})
		select_from(table, { limit: 1 }.merge(options)).first
	end

	##
	#
	# Insert into *data* into *table*. *data* is hash :column_name => 'value'.
	#
	def insert_into(table, data)
		keys = "(#{data.keys.join(', ')})"
		values = "(#{data.values.map{ |value| "'#{value}'" }.join(', ')})"
		execute_with_retry "INSERT INTO #{table} #{keys} VALUES #{values}; "
	end

	private

		def execute_with_retry(query)
			tries = 0
			begin
				tries += 1
				proxy.database.execute query
			rescue SQLite3::BusyException
				sleep 0.1
				if tries <= 30 # retry for 3s
					retry
				else
					raise e 
				end
			end
		end
end