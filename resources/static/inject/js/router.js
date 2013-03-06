define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require( 'backbone' )
	  , TabstarRatings = require('views/TabstarRatings')
	  , App = require('App')


	var Router  = Backbone.Router.extend({

		initialize: function(options){

			this.views = {
				tabstar: new TabstarRatings({ el: $('#star-ratings') })
			}

			App.on('OwNet:tabselect',this.onTabSelect,this)

		},

		start: function(){
			Backbone.history.start()
		},

		onTabSelect: function(tab) {
			this.navigate('#/'+tab,{trigger:true})
		},

		routes: {
			'page_rating' : 'page_rating'
		},

		page_rating: function() {
			this.views.tabstar.render()
		}



	});

	return Router;

});