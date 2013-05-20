define( function (require) {

	var Backbone = require("backbone")
	  , FileModel = require('share/models/FileModel')

	var Files = Backbone.Collection.extend({

		model: FileModel,

		url: function(pageCount) {
			if (pageCount)
				return '/api/files/'+ this._filter + 'PagesCount'
			else
				return '/api/files/'+ (this._filter !='all' ? this._filter : '')
		},

		initialize: function(models, opts) {
			this._filter = opts.filter
		},

		fetch: function(opts) {
			opts || (opts={})
			opts.data = { page: opts.page || 1 }
			opts.reset = true

			return Files.__super__.fetch.call(this, opts)
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

	Files.Model = FileModel

	return Files
})

