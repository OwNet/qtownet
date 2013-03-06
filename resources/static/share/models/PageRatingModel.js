define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var PageRatingModel = Backbone.Model.extend({

			urlRoot: '/api/ratings',

			defaults: {	}

	})

	PageRatingModel.fetchByURL = function(uri, opts) {
		opts || (opts = {})
		$.ajax({
			url: '/api/ratings/page_stats',
			type: 'GET',
			data: { uri : uri },
			success: function(stats) {
				var model = new PageRatingModel(stats)
				opts.success(model)
			},
			error: opts.error,
		})
	}

	return PageRatingModel

})
