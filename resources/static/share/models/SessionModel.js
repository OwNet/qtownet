define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var SessionModel = Backbone.Model.extend({

			urlRoot: '/api/session',

			defaults: {
				id: null
			}

	})

	return SessionModel

})