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
			''    : "newsfeed",
			'newsfeed' : "newsfeed",

			'login' : "login",
			'logout': "logout",
			'registration': "registration",

			'profile': "profile",
			'profile/edit': "editprofile",
			'profile/showdownloadorders' : "showdownloadorders",

			'groups' : "groups",
			'groups/create' : "creategroups",
			'groups/show': "showgroup",
			'groups/edit': "editgroup",
		},

		activate: function(href, view) {

			if ( !App.user && href!='#/login' && href!='#/registration') {
				this.navigate('#/login', {trigger: true})
				return false
			}

			$("#appmenu .active").removeClass("active")
			if (href)
				$.query('.navbar a[href="?"]', href).parent().addClass('active')

			if (this.activeView && this.activeView!=view)
				this.activeView.hide()

			this.activeView = view
			return true
		},


		newsfeed: function() {
			if (this.activate(null, this.views.newsfeed))
				this.views.newsfeed.show()
		},



		login: function() {
			if (this.activate("#/login", this.views.login))
				this.views.login.show()
		},

		logout: function() {
			this.views.navbar.onLogoutClick()
			this.activate("#/logout")
		},

		registration: function() {
			if (this.activate("#/registration", this.views.registration))
				this.views.registration.show()
		},



		profile: function() {
			if (this.activate("#/profile", this.views.profile))
				this.views.profile.show(App.user ? App.user.id : "0")
				

		},

		editprofile: function() {

		},

		showdownloadorders: function() {
			this.activate("#/profile/showdownloadorders", this.views.profile)
			this.views.profile.showDownloadOrders(1)
		},




		groups: function() {
			if (this.activate("#/groups", this.views.groups))
				this.views.groups.show("all", 1)
		},

		creategroups: function() {
			if (this.activate("#/creategroups", this.views.groups))
				this.views.groups.createGroups()
		},

		showgroup: function() {
			this.activate("#/showgroups")
		},

		editgroup: function() {
			this.activate("#/editgroups")
		},


	});

	return Router;

});
