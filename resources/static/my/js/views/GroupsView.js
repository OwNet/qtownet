define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , groupsTemplate = require ("tpl/groups")
	  // , createGroupsTemplate = require ("tpl/creategroups")
	  , groupsTableTemplate = require ("tpl/groupstable")
	  , showGroupTemplate = require ("tpl/showgroup")
	  , groupFormTemplate = require ("tpl/groupform")
	  , groupDetailTemplate = require ("tpl/showgroupdetail")
	  , listMembersGroupTemplate = require ("tpl/listmembersgroup")
	  , pagerTemplate = require ("tpl/groups_pager")
	  , UserModel = require ("share/models/UserModel")
	  , GroupsModel = require ("share/models/GroupsModel")

	  // , MessagesView = require( 'views/MessagesView' )

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")


	var GroupsView = Backbone.View.extend({

			events: {
				'click form[name="create-group-form"] button[name="submit"]': 'saveGroup',
				'click a[name="show"]': "showGroup",
				'click img[name="delete"]': "deleteGroup",
				'click a[name="delete"]': "deleteGroup",
				'click img[name="edit"]': "editGroup",
				'click a[name="edit"]': "editGroup",
				'click img[name="join"]': "joinGroup",
				'click a[name="filter"]' : "showWithFilter",
				'click img[name="leave"]' : "leaveGroup",
				'click a[name="leave"]' : "leaveGroup",
				'click a[name="listMembers"]' : "listMembers",
				'click a[name="groups-page"]' : "showPage",
				'click img[name="add-admin"]' : "addAdmin",
				'click img[name="delete-member"]' : "deleteUser",
				'click img[name="approve-member"]' : "approveUser",
				'click img[name="reject-member"]' : "rejectUser",
				'click a[name="showFront"]' : "showFront",
				'click a[name="showGroup"]' : "showGroup",
			},

			initialize: function() {
				this.updateNavbar()
			},

			showFront: function() {
				var messageView = new MessagesView({el:$("#content") })
				messageView.showFront(1, 0)
			},

			render: function() {
				this.$el.html( groupsTemplate({}) )
				return this
			},

			showGroup1: function(){
				this.$el.html( groupsTemplate({}) )
				return this
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

			showWithFilter: function(e){
				e.preventDefault();
				var filter = $(e.currentTarget).data("filter");
				App.router.navigate('groups', {trigger: true})
				this.show(filter,1)
			},

			showPage: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var filter = $(e.currentTarget).data("filter");
				this.show(filter,id)
			},

			deleteGroup: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");

        		var data = {group_id : id}

        		var group = new GroupsModel(data)
        		group.id = id
        		var self = this

        		group.destroy({
        			wait: true,
        			success: function() {
        				App.router.navigate("#/groups", {trigger: true})
        				App.showMessage("Group deleted")
						self.show("all", 1)
					},
					error: function() {
						App.showMessage("Cannot delete")
					},
        		})

			},

			show: function(filter, page) {

				var GroupsCollection ;
				var Action;
				if (filter == "all") {
					GroupsCollection= Backbone.Collection.extend({
			  			url: '/api/groups',
			  			model: GroupsModel
					})

					Action = Backbone.Model.extend({
				  		urlRoot: '/api/groups/allPagesCount',
						defaults: {	}
					})
				} else if (filter == "admin") {
					GroupsCollection= Backbone.Collection.extend({
			  			url: '/api/groups/admin',
			  			model: GroupsModel
					})

					Action = Backbone.Model.extend({
				  		urlRoot: '/api/groups/adminPagesCount',
						defaults: {	}
					})
				} else if (filter == "my") {
					GroupsCollection= Backbone.Collection.extend({
			  			url: '/api/groups/my',
			  			model: GroupsModel
					})

					Action = Backbone.Model.extend({
				  		urlRoot: '/api/groups/myPagesCount',
						defaults: {	}
					})
				} else if (filter == "wait") {
					GroupsCollection= Backbone.Collection.extend({
			  			url: '/api/groups/awaiting',
			  			model: GroupsModel
					})

					Action = Backbone.Model.extend({
				  		urlRoot: '/api/groups/awaitingPagesCount',
						defaults: {	}
					})
				} else if (filter == "notMy") {
					GroupsCollection= Backbone.Collection.extend({
			  			url: '/api/groups/notMy',
			  			model: GroupsModel
					})

					Action = Backbone.Model.extend({
				  		urlRoot: '/api/groups/notMyPagesCount',
						defaults: {	}
					})
				}


				var groups = new GroupsCollection()

				groups.fetch({data: {page: page},
					success: function() {
						$('div#groups_list').html( groupsTableTemplate({groups :groups.toJSON(), filter: filter}))
					},
					error: function() {
						App.showMessage("No groups founded")
					},
				})


				var action = new Action()


				action.fetch({
					success: function() {
						$('div#pager').html( pagerTemplate({action :action.toJSON(), filter: filter, act_page: page}))
					},
					error: function() {

					},
				})

				this.$el.html( groupsTemplate({ }) )
				return this
			},

			createGroups: function() {
				var group = new GroupsModel({name: "", description:"", password: "", has_password: "1", has_approvement: "1"})
				var GroupsCollection = Backbone.Collection.extend({
		  			url: '/api/groups',
		  			model: GroupsModel
				})

				var groups = new GroupsCollection(group)


				$('div#groups_list').html( groupFormTemplate({group: groups.toJSON()}))
				return this
			},

			saveGroup: function() {
				var form = Form( $('form[name="create-group-form"]', this.$el) )
				var data = form.toJSON()
				if(data.has_approvement == "on"){
					data.has_approvement = "1"
				} else {
					data.has_approvement = "0"
				}

				var group = new GroupsModel(data)
				group.save({group_type: 0, has_password: 0},{
					wait: true,
					success: function() {
						App.router.navigate('groups', {trigger: true})
						App.showMessage("Created", "alert-success")
					},
					error: function() {
						App.showMessage("Creation failed")
					},
				})
			},

			showGroup: function(e){
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

        				/*App.router.navigate("#/showgroup", {trigger: true})
						self.$el.html( showGroupTemplate({group :group.toJSON()}) )
						$('div#group_detail').html( groupDetailTemplate({group :group.toJSON()}))*/
					}
        		})
			},

			listMembers: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");
        		this.showMembers(id)
			},

			editGroup: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");

        		var group = new GroupsModel()
        		group.id = id

        		group.fetch({
        			success: function() {
        				App.router.navigate("#/editgroup", {trigger: true})
        				$('div#groups_list').html( groupFormTemplate({group: group.toJSON()}))
        				$('div.groups-right').html( groupFormTemplate({group: group.toJSON()}))
					}
        		})

			},

			updateNavbar: function() {
				$('#navbar').html( userNavbarTemplate({ user:  App.user ? App.user.toJSON() : false }))
			},


	})

	return GroupsView;

});
