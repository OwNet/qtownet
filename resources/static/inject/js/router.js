define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require( 'backbone' )
	  , LoginView = require( 'views/LoginView' )
	  , PageRatingView = require( 'views/PageRatingView' )
	  , PageRecommendView = require( 'views/PageRecommendView' )
	  , App = require('App')


	var Router  = Backbone.Router.extend({

		initialize: function(options){

			this.views = {
				login : new LoginView({ el: $('#login') }),
				pageRatingView : new PageRatingView({ el:$('#page-ratings') }),
				pageRecommendView : new PageRecommendView({ el:$('#page-recommend') }),
			}

			this.$tabs = $('#page > .tab-content > .tab-pane')

			App.on('OwNet:tabselect',this.tabSelect,this)

		},

		start: function(){
			Backbone.history.start()
		},

		tabSelect: function(tab) {
			this.$tabs.filter('.active').removeClass('active')
			this.$tabs.filter('[data-tab="'+tab+'"]').addClass('active')
			this.navigate('#/'+tab,{trigger:true})
		},

		routes: {
			'login'       : 'login',
			'page_actions' : 'page_actions',
		},

		login: function() {

		},

		page_actions: function() {
			// this.views.page_rating
		},

	});

	return Router;

});
