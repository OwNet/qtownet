define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require( 'backbone' )
	  , LoginView = require( 'views/LoginView' )
	  , PageRatingView = require( 'views/PageRatingView' )
	  , App = require('App')


	var Router  = Backbone.Router.extend({

		initialize: function(options){

			this.views = {
				login : new LoginView({ el: $('#login') }),
				pageRatingView : new PageRatingView({ el:$('#page-ratings') }),
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
			'page_rating' : 'page_rating',
		},

		login: function() {

		},

		page_rating: function() {
			this.views.page_rating
		},

	});

	return Router;

});