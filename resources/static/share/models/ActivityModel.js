define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var ActivityModel = Backbone.Model.extend({

			urlRoot: '/api/activities',

	})



	return ActivityModel

})
