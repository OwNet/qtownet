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


	var MessagesView = Backbone.View.extend({

			events: {
				
			},

			initialize: function() {
				this.updateNavbar()
			},


			render: function() {
				this.$el.html( groupsTemplate({}) )
				return this
			}, 

			show: function() {

				var MessagesCollection ;
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
						$('div#pager').html( pagerTemplate({action :action.toJSON(), filter: filter}))
					},
					error: function() {
						
					},
				})

				

				this.$el.html( groupsTemplate({ }) )
				return this
			},

			updateNavbar: function() {
				$('#navbar').html( userNavbarTemplate({ user:  App.user ? App.user.toJSON() : false }))
			},

			
	})

	return MessagesView;

});