define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var SessionModel = Backbone.Model.extend({

			urlRoot: '/api/session',

			url: function() {
				return this.urlRoot
			},

			isNew: function() {
				return this.get('user_id') == null
			}

	})

	return SessionModel

})