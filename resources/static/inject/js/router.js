define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require( 'backbone' )
	  , TabstarRatings = require('views/TabstarRatings')


	var Router  = Backbone.Router.extend({

		initialize: function(options){

			options.App.router = this

			this.views = {
				tabstar: new TabstarRatings({ el: $('#star-ratings') })
			}

		},


		routes: {

		},



	});

	return Router;

});