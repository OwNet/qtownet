define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var PageRatingModel = Backbone.Model.extend({

			urlRoot: '/api/ratings',

	})

	PageRatingModel.getPageStats = function(uri, opts) {
		opts || (opts={})
		return $.ajax({
			type: 'GET',
			url: '/api/ratings/page_stats',
			data: { uri: uri },
			success: opts.success,
			error: opts.error,
		})
	}



	return PageRatingModel

})
