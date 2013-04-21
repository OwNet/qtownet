define( function (require) {

	"use_strict"
	require('init')

	var App = require('App')
	  , Backbone = require('backbone')
	  , TabstarRatings = require('views/TabstarRatings')
	  , PageRatingModel = require('share/models/PageRatingModel')


	var PageRatingView = Backbone.View.extend({

		initialize: function() {
			this.tabstar = new TabstarRatings({ el: $('#star-ratings') })
			this.tabstar.on('change', this.onUserRate, this)

			App.on('OwNet:page', this.loadPageStats, this)
			App.on('user:loaded', this.loadPageStats, this)
		},

		loadPageStats: function() {

			if ( !App.isUserLogged() || !App.page )
				return

			var self = this
			PageRatingModel.getPageStats(App.page.uri,{
				success: function(stats) {
					self.stats = stats
					self.updateStats(stats)
					self.updateUserRate(stats)
				},
				error: function() { App.error('Loading page stats failed') },
			})
		},

		updateStats: function(stats) {
			var avg = Number(stats.average).toFixed(1).replace(/\.0+$/,'')
			$('#average-rating').text( avg )
			$('#average-count').text( stats.count+' votes' )
		},

		updateUserRate: function(stats) {
			if ( stats.val ) {
				this.tabstar.setValue( stats.val )
			} else
				this.tabstar.setValue( 0 )
		},

		onUserRate: function(val) {
			var model = new PageRatingModel()
			  , rate = { val: val }

			if (this.stats.id)
				rate.id = this.stats.id
			else
				rate.absolute_uri = App.page.uri

			var self = this

			model.save(rate, {
				success: function() {
					self.loadPageStats()
				},
				error: function(model, resp) {
					if (resp.status===401) { // Unauthorized
						alert('You have been logged out!')
						App.logout(true)
					}
					else
						App.error(resp.statusText)
				},
			})
		},

	})

	return PageRatingView;

});
