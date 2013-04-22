define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , Groups = require('collections/Groups')
	  , ActivitiesView = require('views/ActivitiesView')

	  , groupsTemplate = require ("tpl/groups")
	  , groupsTableTemplate = require ("tpl/groupstable")
	  , groupFormTemplate = require ("tpl/groupform")
	  , pagerTemplate = require ("tpl/groups_pager")

	  , UserModel = require ("share/models/UserModel")
	  , Form = require("share/utils/form")


	var GroupsView = Backbone.View.extend({

			events: {
				'click form[name="create-group-form"] button[name="submit"]': 'saveGroup',
				'click a[name="delete-group"]': "deleteGroup",
				'click img[name="join"]': "joinGroup",
			},

			render: function() {
				this.hide()
				this.$el.html( groupsTemplate() )
				this.trigger('render')
				return this
			},

			show: function() {
				this.hide()
				this.render()
				this.isShown = true
			},

			showGroups: function(filter, page) {
				var groups = new Groups([],{ filter: filter})

				$.when(
					groups.fetch({ page: page }),
					groups.fetchPageCount()
				).done( function() {
					$('#groups_list').html( groupsTableTemplate({groups :groups.toJSON(), filter: filter}))
					$('#pager').html( pagerTemplate({ pages: groups.pages, filter: filter, act_page: page}))
				})

				return this
			},

			hide: function() {
				this.$el.html('')
			},

			createGroups: function() {
				var group = new Groups.Model({name: "", description:"", password: "", has_password: "1", has_approvement: "1"})

				$('#groups_list').html( groupFormTemplate({group: group.toJSON(), isNew: true}))
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

				var group = new Groups.Model(data)
				group.save({group_type: 0, has_password: 0}, {
					success: function() {
						App.router.navigate('groups', {trigger: true})
						App.showMessage("Created", "alert-success")
					},
					error: function() {
						App.showMessage("Creation failed")
					},
				})
			},

			deleteGroup: function(e){
				e.preventDefault();

				var id = $(e.currentTarget).data("id");

				var group = new Groups.Model()
				group.set('id', id)

				var self = this
				group.destroy({
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

			//////////////////////////////////////////////////////////

			joinGroup: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id")
				  , self = this

				Groups.Model.joinGroup(id).done( function() {
					self.showGroups('all',1)
					App.showMessage("Joined", "alert-success")
				}).fail( function() {
					App.showMessage("Joining failed")
				})
			},

	})

	return GroupsView;

});
