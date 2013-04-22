define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , ActivitiesView = require('views/ActivitiesView')

	  , profileTemplate = require ("tpl/profile")
	  , profileTableTemplate = require ("tpl/profiletable")
	  , profileFormTemplate = require ("tpl/profileform")
	  , profileStatsTemplate = require ("tpl/profile_stats")
	  , downloadOrdersTemplate = require("tpl/downloadorders")
	  , profilePagerTemplate = require ("tpl/profilepager")
	  , pagerDownloadTemplate = require ("tpl/downloadorderspager")

	  , UserModel = require ("share/models/UserModel")
	  , DownloadOrdersModel = require ("share/models/DownloadOrdersModel")

	  , Form = require("share/utils/form")


	var ProfileView = Backbone.View.extend({

			events: {
				'click form[name="profile-form"] button[name="update"]': "saveProfile",
				"click input[type=radio]": "onRadioClick",
				'click a[data-id]' : 'showOtherProfile',
				'click a[name="showDownloadOrders"]': "showDownloadOrders",
				'click img[name="deleteDO"]': "deleteDO",
			},

			render: function() {
				this.trigger('render')
				return this
			},

			show: function(id){
				var user = new UserModel({ id: id })
				  , current = (App.user.id == id)

				self = this
				user.fetch({
					success: function(){
						self.$el.html( profileTemplate({user :user.toJSON(), current: current}) )
						$('div#profile-info').html( profileTableTemplate({user :user.toJSON(), current: current}))
						$('div#stats').html( profileStatsTemplate({user :user.toJSON(), current: current}))
						self.showActivities(id)
					}
				})

				return this
			},

			edit: function(){
				var user = new UserModel({ id: App.user.id})
				  , self = this

				user.fetch({
					success: function() {
						self.$el.html( profileTemplate({user :user.toJSON(), current: true}) )
						$('div#activities').html( profileFormTemplate({user :user.toJSON()}))
						$('div#pager').hide();
					}
				})
			},


			hide: function() {
				if (this.activitiesView) {
					this.activitiesView.remove()
					delete this.activitiesView
				}

				this.$el.html('')
			},


			showActivities: function(id) {
				var options = { params: { user_id: id }}
				this.activitiesView = new ActivitiesView({ el: $('#newsfeed_list'), options: options }).render()
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

				downloadorders.fetch({page:page,
					success: function() {
						$('div#activities').html( downloadOrdersTemplate({downloadorders: downloadorders.toJSON()}))
					},
					error: function(){
						App.showMessage("Error reading downloadorders")
					},
				})

				var action = new Action()


				action.fetch({
					success: function() {
						$('div#pager').html( pagerDownloadTemplate({action :action.toJSON(), current :page}))
					},
					error: function() {

					},
				})

				this.$el.html( profileTemplate({user: App.user.toJSON()}) )
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

	})

	return ProfileView;

});
