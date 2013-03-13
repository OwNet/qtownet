define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require( 'backbone' )
	  , NavbarView = require( 'views/NavbarView')
	  , LoginView = require( 'views/LoginView' )
	  , RegistrationView = require( 'views/RegistrationView' )
	  , GroupsView = require( 'views/GroupsView' )
	  , ProfileView = require( 'views/ProfileView' )
	  , MessagesView = require( 'views/MessagesView' )
	  , RecommendationsRatingView = require( 'views/RecommendationsRatingView')



	var Router  = Backbone.Router.extend({

		initialize: function(options){

			this.views = {
				navbar: new NavbarView({ el:$('#appmenu') }),
				login : new LoginView({ el:$("#content") }),
				registration: new RegistrationView({ el:$("#content") }),
				groups: new GroupsView({ el:$("#content") }),
				messages: new MessagesView({el: $("#content")}),
				profile: new ProfileView({ el:$("#content") }),
				recommendations: new RecommendationsRatingView({ el:$("#content")}),
				ratings: new RecommendationsRatingView({el:$("#content")}),
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
			editprofile: "editprofile",
			listmembers: "listmembers",
			recommendations: "recommendations",
			ratings: "ratings",

		},

		activate: function(href) {
			$("#appmenu .active").removeClass("active")
			$.query('.navbar a[href="?"]', href).parent().addClass('active')
		},

		home: function() {
			this.views.messages.showHome(1, 1)
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

		listmembers: function() {
			this.activate("#/listmembers")
		},

		profile: function() {
			this.views.profile.show()
			this.activate("#/profile")
		},

		editprofile: function() {
			this.activate("#/editprofile")
		},

		recommendations: function() {
			this.views.recommendations.showRecommendations()
			this.activate("#/recommendations")
		},

		ratings: function() {
			this.views.recommendations.showRatings()
			this.activate("#/ratings")
		},

	});

	return Router;

});