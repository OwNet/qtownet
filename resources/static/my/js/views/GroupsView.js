define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , groupsTemplate = require ("tpl/groups")
	  , createGroupsTemplate = require ("tpl/creategroups")
	  , groupsTableTemplate = require ("tpl/groupstable")
	  , showGroupTemplate = require ("tpl/showgroup")
	  , groupFormTemplate = require ("tpl/groupform")
	  , groupDetailTemplate = require ("tpl/showgroupdetail")
	  , listMembersGroupTemplate = require ("tpl/listmembersgroup")
	  , pagerTemplate = require ("tpl/groups_pager")
	  , UserModel = require ("share/models/UserModel")
	  , GroupsModel = require ("share/models/GroupsModel")

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")


	var GroupsView = Backbone.View.extend({

			events: {
				'click form[name="create-group-form"] button[name="submit"]': 'saveGroup',
				'click a[name="show"]': "showGroup", 
				'click a[name="delete"]': "deleteGroup", 
				'click a[name="edit"]': "editGroup", 
				'click a[name="join"]': "joinGroup", 
				'click a[name="filter"]' : "showWithFilter",
				'click a[name="leave"]' : "deleteUser",
				'click a[name="listMembers"]' : "listMembers",
			},

			initialize: function() {
				this.updateNavbar()
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


				action.save({group_id: id },{
					wait: true,
					success: function() {
						App.router.navigate('groups', {trigger: true})
						App.showMessage("Joined", "alert-success")
						this.show("all")
					},
					error: function() {
						App.showMessage("Joining failed")
					},
				})
			},

			deleteUser: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");

				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/deleteUser',
					defaults: {	}
				})

				var action = new Action()


				action.save({group_id: id, user_id: App.user ? App.user.id : "0"},{
					wait: true,
					success: function() {
						App.router.navigate('groups', {trigger: true})
						App.showMessage("Leaved", "alert-success")
						this.show("all")
					},
					error: function() {
						App.showMessage("Leaving failed")
					},
				})
			},

			showWithFilter: function(e){
				e.preventDefault();
				var filter = $(e.currentTarget).data("filter");
				this.show(filter)
			},

			deleteGroup: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");
        		
        		var data = {group_id : id}

        		var group = new GroupsModel(data)
        		group.id = id

        		group.destroy({
        			wait: true,
        			success: function() {
        				App.router.navigate("#/groups", {trigger: true})
        				App.showMessage("Group deleted")
						this.show("all")
					},
					error: function() {
						App.showMessage("Cannot delete")
					},
        		})

			},

			show: function(filter) {
				var GroupsCollection = Backbone.Collection.extend({
		  			url: '/api/groups',
		  			model: GroupsModel
				})
				
				var groups = new GroupsCollection()

				groups.fetch({data: {page: "1"}, 
					success: function() {
						$('div#groups_list').html( groupsTableTemplate({groups :groups.toJSON(), filter: filter}))
					},
					error: function() {
						App.showMessage("No groups founded")
					},
				})
				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/allPagesCount',
					defaults: {	}
				})

				var action = new Action()


				action.fetch({
					success: function() {
						$('div#pager').html( pagerTemplate({pages :action.pages.toJSON()}))
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

				
				this.$el.html( createGroupsTemplate() )
				$('div#form-create').html( groupFormTemplate({group: groups.toJSON()}))
				return this
			},

			saveGroup: function() {
				var form = Form( $('form[name="create-group-form"]', this.$el) )
				var data = form.toJSON()
				if(data.has_approvement != 1){
					data.has_approvement = 0
					if(data.has_password != 1) {
						data.has_password = 0
					}
				}

				var group = new GroupsModel(data)


				group.save({group_type: 0},{
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
					}
        		})
			},

			listMembers: function(e){
				e.preventDefault();
        		var id = $(e.currentTarget).data("id");

        		var Action = Backbone.Model.extend({
			  		urlRoot: '/api/groups/getGroupUsers',
					defaults: {	}
				})

				var action = new Action()
        		
        		var group = new GroupsModel()
        		group.id = id
        		var self = this
        		group.fetch({
        			success: function() {

						action.save({group_id: id },{
							wait: true,
							success: function() {
								App.router.navigate('listMembers', {trigger: true})
								$('div#group_detail').html( listMembersGroupTemplate({group :group.toJSON(), action : action.toJSON(),  user:  App.user ? App.user.toJSON() : false}))
							},
							error: function() {
								App.showMessage("Joining failed")
							},
						})

        				/*App.router.navigate("#/showgroup", {trigger: true})
						self.$el.html( showGroupTemplate({group :group.toJSON()}) )
						$('div#group_detail').html( groupDetailTemplate({group :group.toJSON()}))*/
					}
        		})
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
        				
					}
        		})
        						
			},

			updateNavbar: function() {
				$('ul#user-navbar').html( userNavbarTemplate({ user:  App.user ? App.user.toJSON() : false }))
			},

			
	})

	return GroupsView;

});