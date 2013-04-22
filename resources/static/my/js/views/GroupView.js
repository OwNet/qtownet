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
	  , Form = require("share/utils/form")


	var GroupsView = Backbone.View.extend({

			events: {
				'click form[name="create-group-form"] button[name="submit"]': 'saveGroup',
				'click a[name="delete-group"]': "deleteGroup",
				/*
				'click img[name="join"]': "joinGroup",
				'click img[name="leave"]' : "leaveGroup",
				'click a[name="leave"]' : "leaveGroup",
				'click a[name="listMembers"]' : "listMembers",
				'click a[name="groups-page"]' : "showPage",
				'click img[name="add-admin"]' : "addAdmin",
				'click img[name="delete-member"]' : "deleteUser",
				'click img[name="approve-member"]' : "approveUser",
				'click img[name="reject-member"]' : "rejectUser",*/
			},

			joinGroup: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");

				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/joinGroup',
					defaults: {	}
				})

				var action = new Action()
				var self = this

				action.save({group_id: id },{
					wait: true,
					success: function() {
						App.router.navigate('groups', {trigger: true})
						App.showMessage("Joined", "alert-success")
						self.show("all",1)
					},
					error: function() {
						App.showMessage("Joining failed")
					},
				})
			},

			addAdmin: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var group_id = $(e.currentTarget).data("group");

				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/addAdmin',
					defaults: {	}
				})

				var action = new Action()
				var self = this

				action.save({group_id: group_id, user_id: id},{
					wait: true,
					success: function() {
						App.showMessage("Added", "alert-success")
						self.showMembers(group_id)
					},
					error: function() {
						App.showMessage("Failed")
					},
				})
			},

			deleteUser: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var group_id = $(e.currentTarget).data("group");

				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/deleteUser',
					defaults: {	}
				})

				var action = new Action()
				var self = this

				action.save({group_id: group_id, user_id: id},{
					wait: true,
					success: function() {
						App.showMessage("Deleted", "alert-success")
						self.showMembers(group_id)
					},
					error: function() {
						App.showMessage("Failed")
					},
				})
			},

			approveUser: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var group_id = $(e.currentTarget).data("group");

				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/approveUser',
					defaults: {	}
				})

				var action = new Action()
				var self = this

				action.save({group_id: group_id, user_id: id},{
					wait: true,
					success: function() {
						App.showMessage("Approved", "alert-success")
						self.showMembers(group_id)
					},
					error: function() {
						App.showMessage("Failed")
					},
				})
			},

			rejectUser: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var group_id = $(e.currentTarget).data("group");

				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/declineUser',
					defaults: {	}
				})

				var action = new Action()
				var self = this

				action.save({group_id: group_id, user_id: id},{
					wait: true,
					success: function() {
						App.showMessage("Rejected", "alert-success")
						self.showMembers(group_id)
					},
					error: function() {
						App.showMessage("Failed")
					},
				})
			},

			leaveGroup: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");

				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/deleteUser',
					defaults: {	}
				})

				var action = new Action()
				var self = this

				action.save({group_id: id, user_id: App.user ? App.user.id : "0"},{
					wait: true,
					success: function() {
						App.router.navigate('groups', {trigger: true})
						App.showMessage("Leaved", "alert-success")
						self.show("all",1)
					},
					error: function() {
						App.showMessage("Leaving failed")
					},
				})
			},

			showPage: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var filter = $(e.currentTarget).data("filter");
				this.show(filter,id)
			},

			deleteGroup: function(e){
				e.preventDefault();

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
				this._show(group_id).done( function() {

					var opts = {
						el: $('#activities'),
						options: { params: { group_id: group_id }}
					}

					this.activitiesView = new ActivitiesView(opts).render()
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

			/*showGroup: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");

        		var group = new GroupsModel()
        		group.id = id
        		var self = this
        		group.fetch({
        			success: function() {
        				App.router.navigate("#/showgroup", {trigger: true})
						self.$el.html( showGroupTemplate({group :group.toJSON()}) )
						$('div#group_detail').html( groupDetailTemplate({group :group.toJSON()}))

						var messageView = new MessagesView({el:$("#group_messages")})
						messageView.showHome(1, group.id)
					}
        		})
			},

			showMembers: function(group_id) {
				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/getGroupUsers',
					defaults: {	}
				})

				var action = new Action()

        		var group = new GroupsModel()
        		group.id = group_id
        		var self = this
        		group.fetch({
        			success: function() {

						action.fetch({data: {group_id: group.id},
							wait: true,
							success: function() {
								App.router.navigate('listMembers', {trigger: true})
								$('div.groups-right').html( listMembersGroupTemplate({group :group.toJSON(), action : action.toJSON(),  user:  App.user ? App.user.toJSON() : false}))
							},
							error: function() {
								App.showMessage("Error")
							},
						})
					}
        		})
			},

			listMembers: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");
        		this.showMembers(id)
			},
			*/


	})

	return GroupsView;

});
