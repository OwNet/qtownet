define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , profileTemplate = require ("tpl/profile")
	  , profileTableTemplate = require ("tpl/profiletable")
	  , showProfileTemplate = require ("tpl/showprofile")
	  , profileFormTemplate = require ("tpl/profileform")
	  , UserModel = require ("share/models/UserModel")

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")

	  , Action = Backbone.Model.extend({
	  		urlRoot: '/api/users',
			defaults: {	}

		})


	var ProfileView = Backbone.View.extend({

			events: {
				'click form[name="profile-form"] button[name="update"]': 'saveProfile',
				'click a[name="showprofile"]': "showProfile",  
				'click a[name="editprofile"]': "editProfile", 
			},

			initialize: function() {
				this.updateNavbar()
			},


			render: function() {
				this.$el.html( profileTemplate({}) )
				return this
			}, 

			showProfile1: function(){
				this.$el.html( profileTemplate({}) )
				return this
			},

			
			show: function(message) {
				var UsersCollection = Backbone.Collection.extend({
		  			url: '/api/users/'+App.user.id,
		  			model: UserModel
				})
				
				var users = new UsersCollection()

				users.fetch({
					success: function() {
						$('div#user_profile').html( profileTableTemplate({users :users.toJSON()}))

					},
					error: function() {
						App.showMessage("No user found")
					},
				})

				this.$el.html( profileTemplate({ }) )
				return this
			},

			saveProfile: function() {
				var form = Form( $('form[name="profile-form"]', this.$el) )
				var data = form.toJSON()
				var self = this
				
				var user = new UserModel(data)


				user.save({id: App.user ? App.user.id : "0"},{
					wait: true,
					success: function() {
						App.router.navigate('profile', {trigger: true})
						initialize()
						App.showMessage("Profile created", "alert-success")
					},
					error: function() {
						App.showMessage("Profile creation failed")
					},
				})
			},

			showProfile: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");
        		
        		var user = new UserModel()
        		user.id = id
        		var self = this
        		user.fetch({
        			success: function() {
        				App.router.navigate("#/showprofile", {trigger: true})
						$('div#user_profile').html( showProfileTemplate({user :user.toJSON()}))
						
					}
        		})
			},

			editProfile: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");
        		
        		var user = new UserModel()
        		user.id = id

        		user.fetch({
        			success: function() {
        				App.router.navigate("#/editprofile", {trigger: true})

        				$('div#user_profile').html( profileFormTemplate({user: user.toJSON()}))
        				
					}
        		})
        						
			},

			updateNavbar: function() {
				$('ul#user-navbar').html( userNavbarTemplate({ user:  App.user ? App.user.toJSON() : false }))
			},

			
	})

	return ProfileView;

});