define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var UserModel = Backbone.Model.extend({

			urlRoot: '/api/users',

			defaults: {	},

			name: function() {
				var attrs = this.attributes
				return attrs.first_name + ' ' + attrs.last_name
			}

	})

	return UserModel

})
