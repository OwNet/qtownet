define( function (require) {

	var Backbone = require("backbone")
	  , GroupModel = require('share/models/GroupModel')

	var Groups = Backbone.Collection.extend({

		model: GroupModel,

		url: function(pageCount) {
			if (pageCount)
				return '/api/groups/'+ this._filter + 'PagesCount'
			else
				return '/api/groups/'+ (this._filter !='all' ? this._filter : '')
		},

		initialize: function(models, opts) {
			this._filter = opts.filter
		},

		fetch: function(opts) {
			opts || (opts={})
			opts.data = { page: opts.page || 1 }
			opts.reset = true

			return Groups.__super__.fetch.call(this, opts)
		},

		fetchPageCount: function() {
			var self = this

			return $.ajax({
				type: 'GET',
				url: this.url(true),
				success: function(data) { self.pages = data.pages },
			})
		},

	})

	Groups.Model = GroupModel

	return Groups
})
