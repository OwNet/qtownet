define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , ActivitiesView = require('views/ActivitiesView')

	  , template = require('tpl/showgroup')
	  , groupFormTemplate = require ("tpl/groupform")
	  , crateMessageTemplate = require('tpl/create_message')
	  , listMembersGroupTemplate = require ("tpl/listmembersgroup")
	  // , pagerTemplate = require ("tpl/groups_pager")

	  , UserModel = require ("share/models/UserModel")
	  , GroupModel = require('share/models/GroupModel')
	  , MessageModel = require('share/models/MessageModel')
	  , Form = require("share/utils/form")


	var GroupsView = Backbone.View.extend({

			events: {
				'click button[name="submit-message"]' : 'sendMessage',
				'click form[name="create-group-form"] button[name="submit"]': 'saveGroup',
				'click a[name="delete-group"]': "deleteGroup",
				'click a[name="leave-group"]' : "leaveGroup",

				'click img[name="approve-member"]' : "approveUser",
				'click img[name="reject-member"]' : "rejectUser",
				'click img[name="delete-member"]' : "deleteUser",
				'click img[name="add-admin"]' : "addAdmin",
				'click img[name="leave"]' : "leaveGroup",
			},

			showPage: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var filter = $(e.currentTarget).data("filter");
				this.show(filter,id)
			},

			deleteGroup: function(e){
				e.preventDefault();

				if (!this._confirm())
					return

        		var self = this
        		this.group.destroy({
        			success: function() {
        				App.router.navigate("#/groups", {trigger: true})
        				App.showMessage("Group deleted")
					},
					error: function() { App.showMessage("Cannot delete") },
        		})
			},

			render: function() {
				this.$el.html( template({ group: this.group.toJSON() }) )
				this.trigger('render')
				return this
			},

			_show:function(group_id) {
				this.group = new GroupModel({id: group_id})

				var self = this
				return this.group.fetch().done( function() {
					self.render()
				})
			},

			show: function(group_id) {
				var self = this
				this._show(group_id).done( function() {

					var opts = {
						el: $('#activities'),
						options: { params: { group_id: group_id }}
					}

					self.activitiesView = new ActivitiesView(opts).render()
					$('#create_message').html( crateMessageTemplate() )
				})
			},

			edit: function(group_id) {
				var self = this

				this._show(group_id).done( function() {

					self.group.fetch().done( function() {
						$('#view_content').html( groupFormTemplate({group: self.group.toJSON(), isNew: false}))
					})

				})
			},

			members: function(group_id) {
				var self = this

				this._show(group_id).then( function() {	return self.group.fetchMembers() }).then( function() {

					var data = {
						group: self.group.toJSON(),
						members: self.group.members
					}

					$('#view_content').html( listMembersGroupTemplate(data))
				})
			},

			hide: function() {
				if ( this.activitiesView ) {
					this.activitiesView.remove()
					delete this.activitiesView
				}

				this.$el.html('')
				this.isShown = false
			},


			/////////////////////////////////////////////////////////

			saveGroup: function() {
				var form = Form( $('form[name="create-group-form"]', this.$el) )
				var data = form.toJSON()
				data.has_approvement = (data.has_approvement === "on" ? 1:0)

				var group = new GroupModel(data)
				group.save({group_type: 0, has_password: 0}, {
					success: function() {
						// App.router.navigate('groups', {trigger: true})
						App.showMessage("Saved", "alert-success")
					},
					error: function() {
						App.showMessage("Save failed")
					},
				})
			},

			sendMessage: function(e) {
				var $text = $('textarea[name="message"]', this.$el)
				var content = $text.val()

				if (content=="")
					return
				$('textarea[name="message"]', this.$el).val("")
				var data = {
					message: content,
					group_id: this.group.id,
					parent_id: 0,
				}

				var message = new MessageModel(data)
				var self = this

				message.save(null, {
					success: function() {
						self.activitiesView.showPage(1, true)
						$text.val('')
					},
					error: function() {	App.showMessage("Message send failed!")	},
				})
			},


			addAdmin: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id")
				  , self = this

				if (!this._confirm())
					return

				this.group.addAdmin(id).done( function() {
					App.showMessage("Added", "alert-success")
					self.members(self.group.id)
				}).fail( function() {
					App.showMessage("Failed")
				})
			},

			deleteUser: function(e){
				e.preventDefault();

				if (!this._confirm())
					return

				var id = $(e.currentTarget).data("id")
				  , self = this

				this.group.deleteUser(id).done( function() {
					App.showMessage("Deleted", "alert-success")
					self.members(self.group.id)
				}).fail( function() {
					App.showMessage("Failed")
				})
			},

			approveUser: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id")
				  , self = this

				this.group.approveUser(id).done( function() {
					App.showMessage("Approved", "alert-success")
					self.members(self.group.id)
				}).fail( function() {
					App.showMessage("Failed")
				})
			},

			rejectUser: function(e){
				e.preventDefault();

				if (!this._confirm())
					return

				var id = $(e.currentTarget).data("id")
				  , self = this

				this.group.declineUser(id).done( function() {
					App.showMessage("Rejected", "alert-success")
					self.members( self.group.id )
				}).fail( function() {
					App.showMessage("Failed")
				})
			},

			leaveGroup: function(e){
				e.preventDefault();

				if (!this._confirm())
					return

				this.group.deleteUser(App.user.id).done( function() {
					App.router.navigate('groups', {trigger: true})
					App.showMessage("Leaved", "alert-success")
				}).fail( function() {
					App.showMessage("Failed")
				})
			},

			_confirm: function(msg) {
				return confirm(msg || 'Are you sure?')
			}

	})

	return GroupsView;

});
