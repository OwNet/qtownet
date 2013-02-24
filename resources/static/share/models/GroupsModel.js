define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var GroupsModel = Backbone.Model.extend({

			urlRoot: '/api/groups',

			defaults: {	}

	})

	return GroupsModel

})
