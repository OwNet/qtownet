define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , profileTemplate = require ("tpl/profile")
	  , profileTableTemplate = require ("tpl/profiletable")
	  , profileFormTemplate = require ("tpl/profileform")
	  , menuTemplate = require ("tpl/menu")
	  , downloadOrdersTemplate = require("tpl/downloadorders")
	  , pagerRecommTemplate = require ("tpl/profilepager")
	  , pagerDownloadTemplate = require ("tpl/downloadorderspager")
	  , showactivitiesTemplate = require ("tpl/showactivities")
	  , UserModel = require ("share/models/UserModel")
	  , ActivityModel = require ("share/models/ActivityModel")
	  , DownloadOrdersModel = require ("share/models/DownloadOrdersModel")

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")

	  , Action = Backbone.Model.extend({
	  		urlRoot: '/api/users',
			defaults: {	}

		})


	var ProfileView = Backbone.View.extend({

			events: {
				'click form[name="profile-form"] button[name="update"]': "saveProfile", 
				'click a[name="editprofile"]': "editProfile",
				'click a[name="RecommendationPager"]' : "showPage",
				'click a[name="showDownloadOrders"]': "showDownloadOrders",
				'click a[name="deleteDO"]': "deleteDO",
			},

			initialize: function() {
				this.updateNavbar()
			},

			render: function() {
				this.$el.html( profileTemplate({}) )

				return this
			}, 
		
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
						$('div#menu').html( menuTemplate({user :App.user.toJSON()}))
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

			showMyActivities: function(page) {
				var Action;
				var ActivitiesCollection;
				ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities/my',
					model: ActivityModel
				})

				Action = Backbone.Model.extend({
					urlRoot: '/api/activities/myPagesCount',
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
        	
        				$('div#activities').html( profileFormTemplate({user :user.toJSON()}))
        				$('div#pager').hide();

        				
					}
        		})
        						
			},

			showDownloadOrders: function(page) {
				var Action;
				var downloadOrdersCollection;
				downloadOrdersCollection = Backbone.Collection.extend({
					url: '/api/orders',
					model: DownloadOrdersModel
				})

				Action = Backbone.Model.extend({
					urlRoot: '/api/orders/allPagesCount',
					defaults: {	}
				})

				var downloadorders = new downloadOrdersCollection()

				downloadorders.fetch({data: {page: page},
					success: function() {
						$('div#activities').html( downloadOrdersTemplate({downloadorders: downloadorders.toJSON()}))
						$('div#menu').html( menuTemplate({user :App.user.toJSON()}))
					},
					error: function(){
						App.showMessage("Error reading downloadorders")
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

			deleteRating: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");
        		
        		var data = {object_id : id}

        		var DeleteRating = Backbone.Model.extend({
			  		urlRoot: '/api/ratings',
					defaults: {	}
				})
        		var rating = new DeleteRating(data)
        		rating.id = id
        		var self = this

        		rating.destroy({
        			wait: true,
        			success: function() {
        				App.router.navigate("#/ratings", {trigger: true})
        				App.showMessage("Rating deleted")
						self.showRatings("all", 1)
					},
					error: function() {
						App.showMessage("Cannot delete rating")
					},
        		})

			},
			deleteDO: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");
        		
        		var data = {id : id}

        		var DeleteDO = Backbone.Model.extend({
			  		urlRoot: '/api/orders',
					defaults: {	}
				})

        		var downloadorder = new DeleteDO(data)
        		downloadorder.id = id
        		var self = this

        		downloadorder.destroy({
        			wait: true,
        			success: function() {
        				App.router.navigate("#/showdownloadorders", {trigger: true})
        				App.showMessage("Download Order deleted")
						self.showDownloadOrders(1)
					},
					error: function() {
						App.showMessage("Cannot delete")
					},
        		})

			},

			updateNavbar: function() {
				$('#navbar').html( userNavbarTemplate({ user:  App.user ? App.user.toJSON() : false }))
			},

			
	})

	return ProfileView;

});