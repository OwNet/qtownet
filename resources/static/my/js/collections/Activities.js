define( function (require) {

	var Backbone = require("backbone")

	var Activities = Backbone.Collection.extend({

		url: '/api/activities',

		_params: null,

		setFilter: function(obj /*{user,group,type}*/ ) {
			this._params = obj
		},

		fetch: function(opts, ) {
			if (opts)
				opts.data = opts.data ?  _.extend(opts.data, this._params) : this._params
			else
				opts = { data: this._params }


			return Activities.__super__.fetch.call(this, opts)
		},


		fetchPageCount: function(opts) {
			return $.ajax({
				type: 'GET',
				url: this.url + '/pagesCount',
				data: this._params,
				success: function(count) {
					this.pageCount = count
					if (opts && opts.success)
						opts.success(count)
				},
				error: opts ? opts.error : null,
			})
		}

	})

})
