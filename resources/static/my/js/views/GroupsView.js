define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , groupsTemplate = require ("tpl/groups")
	  , createGroupsTemplate = require ("tpl/creategroups")
	  , groupsTableTemplate = require ("tpl/groupstable")
	  , showGroupTemplate = require ("tpl/showgroup")
	  , groupFormTemplate = require ("tpl/groupform")
	  , UserModel = require ("share/models/UserModel")
	  , GroupsModel = require ("share/models/GroupsModel")

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")

	  , Action = Backbone.Model.extend({
	  		urlRoot: '/api/groups',
			defaults: {	}

		})


	var GroupsView = Backbone.View.extend({

			events: {
				'click form[name="create-group-form"] button[name="submit"]': 'saveGroup',
				'click a[name="show"]': "showGroup", 
				'click a[name="delete"]': "deleteGroup", 
				'click a[name="edit"]': "editGroup", 
				'click a[name="join"]': "joinGroup", 
				'click a[name="all"]' : "showWithFilter",
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
				$.post('/api/groups/joinGroup', { group_id: id }, function(data) {
    				App.router.navigate("#/groups", {trigger: true})
    				App.showMessage("Group joined")
					this.show("all")
    			}, 'json').error(
    				App.showMessage("Cannot join")
    			);
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

				groups.fetch({
					success: function() {
						$('div#groups_list').html( groupsTableTemplate({groups :groups.toJSON(), filter: filter}))

					},
					error: function() {
						App.showMessage("No groups founded")
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
				if(data.has_approvement != "1"){
					data.has_approvement = "0"
					if(data.has_password != "1") {
						data.has_password = "0"
					}
				}else{
					data.has_approvement = "0"
					data.has_password = "0"
				}

				var group = new GroupsModel(data)


				group.save({group_type: "1", user_id: App.user ? App.user.id : "0"},{
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
						$('div#groups_list').html( showGroupTemplate({group :group.toJSON()}))
						
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