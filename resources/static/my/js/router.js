define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require( 'backbone' )
	  , App = require("App")
	  , NavbarView = require( 'views/NavbarView')
	  , LoginView = require( 'views/LoginView' )
	  , RegistrationView = require( 'views/RegistrationView' )
	  , GroupsView = require( 'views/GroupsView' )
	  , GroupView = require('views/GroupView')
	  , ProfileView = require( 'views/ProfileView' )
	  , NewsFeedView = require( 'views/NewsFeedView' )
	  , FilesView = require( 'views/FilesView')
	  , OfflineView = require( 'views/OfflineView')


	var Router  = Backbone.Router.extend({

		initialize: function(options){

			this.views = {
				navbar: new NavbarView({ el:$('#appmenu') }),
				login : new LoginView({ el:$("#login") }),
				registration: new RegistrationView({ el:$("#registration") }),
				groups: new GroupsView({ el:$("#groups") }),
				group: new GroupView({ el:$("#group") }),
				newsfeed: new NewsFeedView({el: $("#newsfeed")}),
				profile: new ProfileView({ el:$("#profile") }),
				files: new FilesView({ el:$("#files") }),
				offline: new OfflineView({ el:$("#offline")}),
			}

			this.$content = $('#content')
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
			'profile/:id/show': "profile",

			'groups' : "groups",
			'groups/create' : "creategroups",
			'groups/filter/:filter' : 'filtergroups',

			'group/:id/show': "showgroup",
			'group/:id/edit' : "editgroup",
			'group/:id/members' : "groupmembers",

			'files' : "files",
			'files/filter/:filter' : 'filterfiles',
			'files/upload' : "upload",

			'offline/:url'	: "offline" 
		},

		activate: function(href, view, fn) {

			if ( !App.user && href!='#/login' && href!='#/registration' ) {
				this.navigate('#/login', {trigger: true})
				return
			}

			$("#appmenu .active").removeClass("active")
			if (href)
				$.query('.navbar a[href="?"]', href).parent().addClass('active')

			if (this.activeView) {
				var h = this.$content.height()
				this.$content.css('min-height',h+'px')
				this.activeView.hide()
			}

			if (fn)
				fn.call(this, view)

			if (view)
				view.once('render', function() {
					this.$content.css('min-height','0')
				}, this)
			else
				this.$content.css('min-height','0')

			this.activeView = view
		},


		newsfeed: function() {
			this.activate(null, this.views.newsfeed, function() {
				this.views.newsfeed.show()
			})
		},



		login: function() {
			this.activate("#/login", this.views.login, function() {
				this.views.login.show()
			})
		},

		logout: function() {
			this.activate("#/logout", null, function() {
				this.views.navbar.onLogoutClick()
			})
		},

		registration: function() {
			this.activate("#/registration", this.views.registration, function() {
				this.views.registration.show()
			})
		},



		profile: function(id) {
			var tab

			if (!id) {
				id = App.user.id
				tab = "#/profile"
			}

			this.activate(tab, this.views.profile, function() {
				this.views.profile.show(id)
			})
		},

		editprofile: function() {
			this.activate("#/profile", this.views.profile, function() {
				this.views.profile.edit()
			})
		},

		showdownloadorders: function() {
			this.activate("#/profile", this.views.profile, function() {
				this.views.profile.showDownloadOrders(1)
			})
		},



		groups: function() {
			this.activate("#/groups", this.views.groups, function() {
				this.views.groups.show()
				this.views.groups.showGroups('all',1)
			})
		},

		creategroups: function() {
			this.activate("#/groups", this.views.groups, function() {
				this.views.groups.show()
				this.views.groups.createGroups()
			})
		},

		filtergroups: function(filter) {
			this.activate("#/groups", this.views.groups, function() {
				this.views.groups.show()
				this.views.groups.showGroups(filter,1)
			})
		},



		showgroup: function(id) {
			this.activate("#/groups", this.views.group, function() {
				this.views.group.show(id)
			})
		},

		editgroup: function(id) {
			this.activate("#/groups", this.views.group, function() {
				this.views.group.edit(id)
			})
		},

		groupmembers: function(id) {
			this.activate("#/groups", this.views.group, function() {
				this.views.group.members(id)
			})
		},

		files: function() {
			this.activate("#/files", this.views.files, function() {
				this.views.files.show()
				this.views.files.showFiles('all',1)
			})
		},

		filterfiles: function(filter) {
			this.activate("#/files", this.views.groups, function() {
				$("#all").parent().removeClass("active")
				$("#my").parent().addClass("active")	
				this.views.files.show()
				this.views.files.showFiles(filter,1)
			})
		},

		upload: function() {
			this.activate("#/files", this.views.files, function() {
				this.views.files.show()
				this.views.files.showFiles('all',1)
				this.views.files.upload()
			})
		},

		offline: function(url) {
			this.activate("#/offline", this.views.offline, function() {
				this.views.offline.show(url)
			})
		}

	});

	return Router;

});
