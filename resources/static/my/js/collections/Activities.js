define( function (require) {

	var Backbone = require("backbone")

	var Activities = Backbone.Collection.extend({

		url: '/api/activities',

		_params: null,

		setParams: function(obj /*{user,group,type}*/ ) {
			this._params = obj
		},

		fetch: function(opts) {
			opts || (opts={})
			opts.data = opts.data ?  _.extend(opts.data, this._params) : this._params
			opts.reset = true

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

	return Activities
})
