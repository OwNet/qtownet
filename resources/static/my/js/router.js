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
				login : new LoginView({ el:$("#content") }),
				registration: new RegistrationView({ el:$("#content") }),
				groups: new GroupsView({ el:$("#content") }),
				newsfeed: new NewsFeedView({el: $("#content")}),
				profile: new ProfileView({ el:$("#content") }),
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

		activate: function(href) {
			$("#appmenu .active").removeClass("active")
			if (href)
				$.query('.navbar a[href="?"]', href).parent().addClass('active')
		},

		newsfeed: function() {
			this.activate(null),
			this.views.newsfeed.show()
		},

		login: function() {
			this.views.login.show()
			this.activate("#/login")
		},

		registration: function() {
			this.views.registration.show()
			this.activate("#/registration")
		},

		groups: function() {
			this.views.groups.show("all", 1)
			this.activate("#/groups")
		},

		creategroups: function() {
			this.views.groups.createGroups()
			this.activate("#/creategroups")
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
			this.views.profile.show(App.user ? App.user.id : "0")
			this.activate("#/profile")
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
