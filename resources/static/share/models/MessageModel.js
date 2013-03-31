define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var MessageModel = Backbone.Model.extend({

			urlRoot: '/api/messages',

			defaults: {	}

	})

	return MessageModel

})
