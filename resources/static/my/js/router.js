define( function (require) {

	"use_strict"

	var Backbone = require( 'backbone' )
	  , NavbarView = require( 'views/NavbarView')
	  , LoginView = require( 'views/LoginView' )
	  , RegistrationView = require( 'views/RegistrationView' )
	  , GroupsView = require( 'views/GroupsView' )
	  , ProfileView = require( 'views/ProfileView' )



	var Router  = Backbone.Router.extend({

		initialize: function(options){

			this.views = {
				navbar: new NavbarView({ el:$('#appmenu') }),
				login : new LoginView({ el:$("#content") }),
				registration: new RegistrationView({ el:$("#content") }),
				groups: new GroupsView({ el:$("#content") }),
				profile: new ProfileView({ el:$("#content") }),
			}

		},

		start: function(){
			Backbone.history.start()
		},

		routes: {
			""    : "home",
			login : "login",
			registration: "registration",
			groups : "groups",
			creategroups : "creategroups",
			showgroup: "showgroup",
			editgroup: "editgroup",
			profile: "profile",
			showprofile: "showprofile",
			editprofile: "editprofile",

		},

		activate: function(href) {
			$("#appmenu .active").removeClass("active")
			$.query('#user-navbar a[href="?"]', href).parent().addClass('active')
		},

		home: function() {

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
			this.views.groups.show()
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

		profile: function() {
			this.views.profile.show()
			this.activate("#/profile")
		},

		showprofile: function() {
			this.activate("#/showprofile")
		},

		editprofile: function() {
			this.activate("#/editprofile")
		},

	});

	return Router;

});