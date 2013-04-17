define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var PageRatingModel = Backbone.Model.extend({

			urlRoot: '/api/recommendations',

	})



	return PageRatingModel

})
