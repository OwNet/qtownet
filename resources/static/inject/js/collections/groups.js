define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require('backbone')


	var Groups = Backbone.Collection.extend({

		url: '/api/groups'

	})

	return Groups;

});
