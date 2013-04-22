define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require( 'backbone' )
	  , App = require("App")
	  , NavbarView = require( 'views/NavbarView')
	  , LoginView = require( 'views/LoginView' )
	  , RegistrationView = require( 'views/RegistrationView' )
	  , GroupsView = require( 'views/GroupsView' )
	  , ProfileView = require( 'views/ProfileView' )
	  , NewsFeedView = require( 'views/NewsFeedView' )



	var Router  = Backbone.Router.extend({

		initialize: function(options){

			this.views = {
				navbar: new NavbarView({ el:$('#appmenu') }),
				login : new LoginView({ el:$("#login") }),
				registration: new RegistrationView({ el:$("#registration") }),
				groups: new GroupsView({ el:$("#groups") }),
				newsfeed: new NewsFeedView({el: $("#newsfeed")}),
				profile: new ProfileView({ el:$("#profile") }),
			}

		},

		start: function(){
			Backbone.history.start()
		},

		routes: {
			""    : "newsfeed",
			newsfeed : "newsfeed",
			login : "login",
			registration: "registration",
			groups : "groups",
			creategroups : "creategroups",
			showgroup: "showgroup",
			editgroup: "editgroup",
			profile: "profile",
			editprofile: "editprofile",
			showdownloadorders : "showdownloadorders",
			listmembers: "listmembers",
			ratings: "ratings",
			logout: "logout",
		},

		activate: function(href, view) {
			$("#appmenu .active").removeClass("active")
			if (href)
				$.query('.navbar a[href="?"]', href).parent().addClass('active')

			if (this.activeView && this.activeView!=view)
				this.activeView.hide()

			this.activeView = view
		},

		newsfeed: function() {
			this.activate(null, this.views.newsfeed),
			this.views.newsfeed.show()
		},

		login: function() {
			this.activate("#/login", this.views.login)
			this.views.login.show()
		},

		registration: function() {
			this.activate("#/registration", this.views.registration)
			this.views.registration.show()
		},

		groups: function() {
			this.activate("#/groups", this.views.groups)
			this.views.groups.show("all", 1)
		},

		creategroups: function() {
			this.activate("#/creategroups", this.views.groups)
			this.views.groups.createGroups()
		},

		showgroup: function() {
			this.activate("#/showgroups")
		},

		editgroup: function() {
			this.activate("#/editgroups")
		},
		editprofile: function() {
			this.activate("#/editprofile")
		},

		listmembers: function() {
			this.activate("#/listmembers")
		},

		profile: function() {
			this.activate("#/profile", this.views.profile)
			this.views.profile.show(App.user ? App.user.id : "0")

		},

		showdownloadorders: function() {
			this.activate("#/showdownloadorders")
			this.views.profile.showDownloadOrders(1)
		},

		logout: function() {
			this.views.navbar.onLogoutClick()
			this.activate("#/logout")
		}

	});

	return Router;

});
