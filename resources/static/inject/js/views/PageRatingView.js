define( function (require) {

	"use_strict"
	require('init')

	var App = require('App')
	  , Backbone = require('backbone')
	  , TabstarRatings = require('views/TabstarRatings')
	  , PageStatsModel = require('models/PageStatsModel')


	var PageRatingView = Backbone.View.extend({

		initialize: function() {
			this.tabstar = new TabstarRatings({ el: $('#star-ratings') })
			this.tabstar.on('change', this.onUserRate, this)

			this._pageStats = new PageStatsModel()
			this._pageStats.on('change', this.updateStats, this)
			this._pageStats.getUserRating().on('change', this.updateUserRate, this)


			App.on('OwNet:URI',this.loadPageStats,this)
		},

		loadPageStats: function(url, opts) {
			if (url)
				this.pageUrl = url

			this._pageStats.fetch( this.pageUrl, opts )
		},

		updateStats: function() {
			var avg = Number(this._pageStats.get('average')).toFixed(1)
			var count = this._pageStats.get('count')

			avg = avg.replace(/\.0+$/,'')

			$('#average-rating').text( avg )
			$('#average-count').text( count+' votes' )
		},

		updateUserRate: function() {
			if ( this._pageStats.hasUserRate() ) {
				var rate = this._pageStats.getUserRating()
				this.tabstar.setValue( rate.get('val') )
			} else
				this.tabstar.setValue( 0 )
		},

		onUserRate: function(val) {
			var self = this
			var rate = this._pageStats.getUserRating()

			if ( !App.isUserLogged() ) {
				this.tabstar.setValue( 0 )
				App.router.tabSelect('login')
				App.once('login', function (){
					App.router.tabSelect('page_rating')
					self.loadPageStats( self.pageUrl, {
						success: function() {
							rate.save({val:val})

						}
					} )

				})
				return
			}

			var self = this

			rate.set('val',val)
			rate.set('absolute_uri',self.pageUrl)
			rate.save({},{
				success: function() {
					self.loadPageStats()
				}
			})
		},

	})

	return PageRatingView;

});