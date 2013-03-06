define( function (require) {

	"use_strict"
	require('init')

	var App = require('App')
	  , Backbone = require('backbone')
	  , TabstarRatings = require('views/TabstarRatings')
	  , PageRatingModel = require('share/models/PageRatingModel')

	  console.log( PageRatingModel )

	var PageRatingView = Backbone.View.extend({

		initialize: function() {
			this.tabstar = new TabstarRatings({ el: $('#star-ratings') })

			App.on('OwNet:URI',this.loadPageRatings,this)
		},

		loadPageRatings: function(url) {
			console.log( 'url:',url )
			PageRatingModel.fetchByURL(url,{
				success: function(rating) {
					console.log( rating.toJSON() )
					this.rating = rating
				}
			})
		}


	})

	return PageRatingView;

});