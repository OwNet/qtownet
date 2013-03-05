define( function (require) {

	"use_strict"

	var Backbone = require( 'backbone' )
	  , LoginView = require( 'views/LoginView' )
	  , RegistrationView = require( 'views/RegistrationView' )
	  , GroupsView = require( 'views/GroupsView' )	  



	var Router  = Backbone.Router.extend({

		initialize: function(options){

			options.App.router = this

			this.views = {
				login : new LoginView({ el:$("#content") }),
				registration: new RegistrationView({ el:$("#content") }),
				groups: new GroupsView({ el:$("#content") }),
			}

			Backbone.history.start()
		},


		routes: {
			""    : "home",
			login : "login",
			logout: "logout",
			registration: "registration",
			groups : "groups",
			creategroups : "creategroups",
			showgroup: "showgroup",
			editgroup: "editgroup"

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
			this.views.groups.show("all")
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

	});

	return Router;

});