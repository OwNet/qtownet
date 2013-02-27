define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var UserModel = Backbone.Model.extend({

			urlRoot: '/api/users',

			defaults: {	}

	})

	return UserModel

})
