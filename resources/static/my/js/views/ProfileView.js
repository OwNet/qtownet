define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , profileTemplate = require ("tpl/profile")
	  , profileTableTemplate = require ("tpl/profiletable")
	  , profileFormTemplate = require ("tpl/profileform")
	  , pagerRecommTemplate = require ("tpl/profilepager")
	  , showactivitiesTemplate = require ("tpl/showactivities")
	  , UserModel = require ("share/models/UserModel")
	  , ActivityModel = require ("share/models/ActivityModel")

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")

	  , Action = Backbone.Model.extend({
	  		urlRoot: '/api/users',
			defaults: {	}

		})


	var ProfileView = Backbone.View.extend({

			events: {
				'click form[name="profile-form"] button[name="update"]': 'saveProfile', 
				'click a[name="editprofile"]': "editProfile",
				'click a[name="RecommendationPager"]' : "showPage",
			},

			initialize: function() {
				this.updateNavbar()
			},


			render: function() {
				this.$el.html( profileTemplate({}) )

				return this
			}, 
		
			/*showAll: function(message) {
				var UsersCollection = Backbone.Collection.extend({
		  			url: '/api/users/',
		  			model: UserModel
				})
				
				var users = new UsersCollection()

				users.fetch({
					success: function() {
						$('div#all_profiles').html( showAllTemplate({users :users.toJSON()}))

					},
					error: function() {
						App.showMessage("No user found")
					},
				})

				this.$el.html( profileTemplate({ }) )
				return this
			},*/

			showActivities: function(page) {
				var Action;
				var ActivitiesCollection;
				ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities',
					model: ActivityModel
				})

				Action = Backbone.Model.extend({
					urlRoot: '/api/activities/allPagesCount',
					defaults: {	}
				})

				var activities = new ActivitiesCollection()

				activities.fetch({data: {page: page},
					success: function() {
						$('div#activities').html( showactivitiesTemplate({activities: activities.toJSON()}))
					},
					error: function(){
						App.showMessage("Error reading activities")
					},
				})

				var action = new Action()


				action.fetch({
					success: function() {
						$('div#user_profile').html( profileTableTemplate({user :App.user.toJSON()}))
						$('div#pager').html( pagerRecommTemplate({action :action.toJSON()}))
					},
					error: function() {
						
					},
				})

				this.$el.html( profileTemplate({ }) )
				return this

			},

			/*showActivities: function() {
				var ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities',
					model: ActivityModel
				})

				var activities = new ActivitiesCollection()

				activities.fetch({
					success: function() {
						$('div#activities').html( showactivitiesTemplate({activities: activities.toJSON()}))
					},
					error: function(){
						App.showMessage("Error reading activities")
					},
				})
				this.$el.html( profileTemplate({ }) )
				return this

			},*/

			show: function(){
				        		     		
        		App.user.fetch({
        			success: function() {
						$('div#user_profile').html( profileTableTemplate({user :App.user.toJSON()}))
						
					}
        		})

        		this.showActivities(1)
        		

				this.$el.html( profileTemplate({ }) )
				return this
			},

			showPage: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				this.showActivities(id)
			},
			saveProfile: function() {
				var form = Form( $('form[name="profile-form"]', this.$el) )
				var data = form.toJSON()
				
				App.user.save(data, {
					wait: true,
					success: function() {
						App.router.navigate('profile', {trigger: true})
					
						App.showMessage("Profile updated", "alert-success")
					},
					error: function() {
						App.showMessage("Profile update failed")
					},
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
        	
        				$('div#edit_profile').html( profileFormTemplate({user :user.toJSON()}))

        				
					}
        		})
        						
			},

			updateNavbar: function() {
				$('ul#user-navbar').html( userNavbarTemplate({ user:  App.user ? App.user.toJSON() : false }))
			},

			
	})

	return ProfileView;

});