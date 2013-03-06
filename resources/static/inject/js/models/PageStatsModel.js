define( function (require) {

	"use_strict"


	var Backbone = require('backbone')
	  , PageRatingModel = require('share/models/PageRatingModel')


	var PageStatsModel = Backbone.Model.extend({

			urlRoot: '/api/ratings/page_stats',
			url: function (){ return this.urlRoot },
			save: null, // disable saving,

			initialize: function() {
				this._userRating = new PageRatingModel()
			},

			hasUserRate: function() {
				return this._userRating.has('val')
			},

			getUserRating: function() {
				return this._userRating
			},

			fetch: function(uri,opts) {
				opts || (opts = {})
				opts.data || (opts.data = {})

				if (!uri) throw new error('uri not specified')

				opts.data.uri = uri

				var self = this

				var success = opts.success
				opts.success = function(stats) {
					if (stats.has('val'))
						self._userRating.set({ id: stats.get('id'), val: stats.get('val') })
					else
						self._userRating.clear()

					if (success)
						success.apply(this,arguments)
				}


				return PageStatsModel.__super__.fetch.call(this,opts)
			},
	})

	return PageStatsModel

})
