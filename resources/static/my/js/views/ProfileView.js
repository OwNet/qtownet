define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , profileTemplate = require ("tpl/profile")
	  , profileTableTemplate = require ("tpl/profiletable")
	  , otherProfileTableTemplate = require ("tpl/profiletable")
	  , profileFormTemplate = require ("tpl/profileform")
	  , menuTemplate = require ("tpl/menu")
	  , otherMenuTemplate = require ("tpl/menuother")
	  , downloadOrdersTemplate = require("tpl/downloadorders")
	  , profilePagerTemplate = require ("tpl/profilepager")
	  , pagerOtherProfileTemplate = require ("tpl/otherprofilepager")
	  , pagerDownloadTemplate = require ("tpl/downloadorderspager")
	  , showactivitiesTemplate = require ("tpl/showactivities")
	  , UserModel = require ("share/models/UserModel")
	  , MessageModel = require ("share/models/MessageModel")
	  , ActivityModel = require ("share/models/ActivityModel")
	  , DownloadOrdersModel = require ("share/models/DownloadOrdersModel")

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")


	var ProfileView = Backbone.View.extend({

			events: {
				'click form[name="profile-form"] button[name="update"]': "saveProfile", 
				'click a[name="editprofile"]': "editProfile",
				'click a[name="ProfilePager"]' : "showMyPage",
				'click a[name="showDownloadOrders"]': "showDownloadOrders",
				'click a[name="deleteDO"]': "deleteDO",
				'click a[name="deleteActivityRecommendation"]': "deleteActivityRecommendation",
				'click a[name="deleteActivityRating"]': "deleteActivityRating",
				'click a[name="deletemessage"]': "deleteMessage",
				"click input[type=radio]": "onRadioClick",
				'click a[name="showOtherUser"]' : "otherShow",
			},

			initialize: function() {
				this.updateNavbar()
			},

			render: function() {
				this.$el.html( profileTemplate({}) )

				return this
			}, 

			show: function(filter, id){
				        		 
				if (filter == "my"){        		     		
        		App.user.fetch({
        			success: function() {
						$('div#user_profile').html( profileTableTemplate({user :App.user.toJSON()}))
						$('div#menu').html( menuTemplate({user :App.user.toJSON()}))
						$('a[name="showDownloadOrders"]')
					}
        		})

        		this.showActivities("my", id, 1)
        		}

        		if (filter == "other"){
        		var user = new UserModel()
        		user.id = id
        		console.log(id)
        		user.fetch({
        			success: function(){
        				$('div#user_profile').hide()
        				$('div#other_user_profile').html( otherProfileTableTemplate({user :user.toJSON()}))
        				$('div#menu').html( otherMenuTemplate({user :user.toJSON()}))
        				$('a[name="editprofile"]').hide()
        				$('a[name="showDownloadOrders"]').hide()
        			}
        		})

        		this.showActivities("all", id, 1)
        		}        		

				this.$el.html( profileTemplate({ }) )
				return this
			},

			otherShow: function(e) {
				e.preventDefault()
				var id = $(e.currentTarget).data("id");
      			this.show("other", id)
    		},

		
			showActivities: function(filter, id, page) {
				var Action;
				var ActivitiesCollection;

				if (filter == "my") {
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
						$('div#activities').html( showactivitiesTemplate({activities: activities.toJSON(), user: App.user.toJSON()}))	
						$('a[name="showOtherUser"]').hide()				
					},
					error: function(){
						App.showMessage("Error reading activities")
					},
				})

				var action = new Action()


				action.fetch({
					success: function() {
						$('div#pager').html( profilePagerTemplate({action :action.toJSON()}))
					},
					error: function() {
						
					},
				})
				}

				if (filter == "all"){
				ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities',
					model: ActivityModel,
				})

				Action = Backbone.Model.extend({
					urlRoot: '/api/activities/allPagesCount',
					defaults: {	}
				})

				var activities = new ActivitiesCollection()
				var otheruser = new UserModel()
				otheruser.id = id

				activities.fetch({data: {page: page},
					success: function() {
						otheruser.fetch({
							success: function() {
								$('div#activities').html( showactivitiesTemplate({activities: activities.toJSON(), user: otheruser.toJSON()}))
								$('a[name="deleteActivityRating"]').hide()
								$('a[name="deletemessage"]').hide()
								$('a[name="deleteActivityRecommendation"]').hide()

							},
							error: function(){
								App.showMessage("Error reading activities")
							},	
						})
												
					},
					error: function(){
						App.showMessage("Error reading activities")
					},
				})

				var action = new Action()


				action.fetch({
					success: function() {
						$('div#pager').html( pagerOtherProfileTemplate({action :action.toJSON()}))
					},
					error: function() {
						
					},
				})
				}

				

				this.$el.html( profileTemplate({ }) )
				return this

			},
					
			showMyPage: function(e){
				e.preventDefault();
				var pid = $(e.currentTarget).data("id");
				var id = null
				App.user.fetch({
        			success: function() {
						$('div#user_profile').html( profileTableTemplate({user :App.user.toJSON()}))
						$('div#menu').html( menuTemplate({user :App.user.toJSON()}))
						$('a[name="showDownloadOrders"]')
					}
        		})
				this.showActivities("my", id, pid)
			},
			showOtherPage: function(e){
				e.preventDefault();
				var pid = $(e.currentTarget).data("id");
				var user = new UserModel()
        		user.id = id
        		user.fetch({
        			success: function(){
        				$('div#user_profile').hide()
        				$('div#other_user_profile').html( profileTableTemplate({user :user.toJSON()}))
        				$('div#menu').html( otherMenuTemplate({user :user.toJSON()}))
        				$('a[name="editprofile"]').hide()
        				$('a[name="showDownloadOrders"]').hide()
        			}
        		})

        		this.showActivities("all", id, 2)
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
        				$('a[name="showDownloadOrders"]').show()
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
						$('div#pager').html( profilePagerTemplate({action :action.toJSON()}))
					},
					error: function() {
						
					},
				})

				this.$el.html( profileTemplate({ }) )
				return this

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

			onRadioClick: function (e) {
            e.stopPropagation();
            var user = new UserModel()
            user.set({ gender: $(e.currentTarget).val() }, {silent:true});
        	},

        	deleteActivityRecommendation: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");
        		
        		var data = {object_id : id}

        		var DeleteRecommendation = Backbone.Model.extend({
			  		urlRoot: '/api/recommendations',
					defaults: {	}
				})
        		var recommendation = new DeleteRecommendation(data)
        		recommendation.id = id
        		var self = this

        		recommendation.destroy({
        			wait: true,
        			success: function() {
        				App.router.navigate("#/profile", {trigger: true})
        				App.showMessage("Recommendation deleted")
						self.showactivities("all", 1)
					},
					error: function() {
						App.showMessage("Cannot delete recommendation")
					},
        		})

			},

			deleteActivityRating: function(e){
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
        				App.router.navigate("#/profile", {trigger: true})
        				App.showMessage("Rating deleted")
						self.showactivities("all", 1)
					},
					error: function() {
						App.showMessage("Cannot delete rating")
					},
        		})

			},

			deleteMessage: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");
        		var group_id = $(e.currentTarget).data("group_id");
        		var data = {group_id : group_id}

        		var message = new MessageModel(data)
        		message.id = id
        		message.group_id = group_id
        		var self = this

        		message.destroy({
        			success: function() {
        				App.router.navigate("#/profile", {trigger: true})
        				App.showMessage("Message deleted")
					},
					error: function() {
						App.showMessage("Cannot delete")
					},
        		})

			},

			
	})

	return ProfileView;

});