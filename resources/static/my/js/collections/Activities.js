define( function (require) {

	var Backbone = require("backbone")

	var Activities = Backbone.Collection.extend({

		url: '/api/activities',

		_filter: {},

		setFilter: function(obj /*{user,group,type}*/ ) {
			this._filter = obj
		},

		fetch: function(opts, ) {
			if (opts)
				opts.data = opts.data ?  _.extend(opts.data, this._filter) : this._filter
			else
				opts = { data: this._filter }


			Activities.__super__.fetch.call(this, opts)
		},


	})

})
