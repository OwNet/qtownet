define(function (require) {

	"use_strict"
	require('init')

	var Backbone = require( 'backbone' )
	  , HeaderView = require( 'views/HeaderView' )
	  , LoginView = require( 'views/LoginView' )
	  , PageRatingView = require( 'views/PageRatingView' )
	  , PageRecommendView = require( 'views/PageRecommendView' )
	  , App = require('App')
	  , trigger = {trigger: true}

	var Router  = Backbone.Router.extend({

		initialize: function(options){

			this.views = {
				loginView : new LoginView({ el: $('#login') }),
				pageRatingView : new PageRatingView({ el:$('#page-ratings') }),
				pageRecommendView : new PageRecommendView({ el:$('#page-recommend') }),
				headerView : new HeaderView({ el: $('#header') })
			}

			this.$tabs = $('#content > .tab-content > .tab-pane')
			// this.$body = $(document.body)
			this.$body = $('#body-wrap')

			App.on('OwNet:show', this.show, this)
			App.on('OwNet:close', this.close, this)
			App.on('user:logout', function (){ this.navigate('login', trigger) }, this)
			App.on('user:loaded', function (){ this.navigate(this.requestedTab, trigger) }, this)
			App.on('OwNet:prefetch', this.prefetch, this)
			App.on('OwNet:cached', this.cachedLinks, this)
			App.on('OwNet:caching:check', this.checkCaching, this)
			App.on('OwNet:caching:change', this.changeCaching, this)
		},

		start: function() {
			Backbone.history.start()
		},

		tabSelect: function(tab) {
			if (tab == this.activeTab)
				return

			this.$tabs.filter('.active').removeClass('active')
			this.$tabs.filter('[data-tab="'+tab+'"]').addClass('active')
			this.activeTab = tab
		},

		show: function(tab) {
			this.requestedTab = tab

			if ( App.isUserLogged() )
				this.navigate(tab, trigger)
			else
				this.navigate('login', trigger)

			var size = this.size[tab]
			App.sendMessage('iframe:resize', size)

			this.$body.css('left', -size.width)
			this.$body.animate({left: 0}, 300)

		},


		close: function() {
			var size = this.size[this.activeTab]

			this.$body.animate({left: -size.width}, 300, null, function() {
				App.sendMessage('iframe:close')
			})

			this.activeTab = null
			this.navigate(null)
		},

		prefetch: function(data) {
		    $.ajax({
		        type: "POST",
		        url: "http://inject.ownet/api/prefetch/create/",
		        data: JSON.stringify(data),
		    });

		},

		cachedLinks: function (links) {
		   
		    $.ajax({
		        type : "POST",
		        url: "http://inject.ownet/api/prefetch/list/",
		        data: JSON.stringify(links),
		        success: function (data) {
		            if (data && data.links) {
		                if (Array.isArray(data.links)) data.links = data.links.toString();
		                App.sendMessage("highlight", data.links);
		            }
		        },
		      
		    });
		},

		checkCaching: function (data) {
		    if (data && data.url) {
		        $.ajax({
		            type: "POST",
		            data: JSON.stringify({ url : data.url }),
		            url: "http://inject.ownet/api/cache_exceptions/check",
		            success: function (data) {
		                App.sendMessage("caching:checked", data);
		            }
		        });
		    }
		},

		changeCaching: function (data) {
		    if (data && data.url) {
		        var service = null;
		        if (data.settings) {
		            service = "http://inject.ownet/api/cache_exceptions/add";
		        }
		        else {
		            service = "http://inject.ownet/api/cache_exceptions/remove";
		        }
		        $.ajax({
		            type: "POST",
		            data: JSON.stringify({ url : data.url }),
		            url: service,
		            success: function () {
		                App.sendMessage("caching:check", data);
		            }
		        });
		    }
		},

		routes: {
			'login' : 'login',
			'page_actions' : 'page_actions'
		},

		login: function() {
			this.tabSelect('login')
			this.views.loginView.focus()
		},

		page_actions: function() {
			this.tabSelect('page_actions')
		},

		size: {
			login: { width : 420, height : 311 },
			page_actions: { width : 420, height : 505 }
		}

	});

	return Router;

});
