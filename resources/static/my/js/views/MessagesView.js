define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , messagesTemplate = require ("tpl/messages")
	  , messagesListTemplate = require ("tpl/messages_list")
	  , createMessageTemplate = require ("tpl/create_message")
	  , UserModel = require ("share/models/UserModel")
	  , MessageModel = require ("share/models/MessageModel")
	  , pagerTemplate = require ("tpl/messages_pager")
	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")


	var MessagesView = Backbone.View.extend({

			events: {
				'click form[name="create-message-form"] button[name="submit-message"]': 'save',
				'click form[name="comment-message-form"] button[name="submit-message"]': 'comment',
				'click a[name="pager-message"]' : "showPage",
				'click a[name="delete-message"]' : "delete",
				'click a[name="messages"]' : "showFront",
			},

			initialize: function() {
				this.updateNavbar()
			},


			render: function() {
				this.$el.html( messagesTemplate({}) )
				return this
			}, 

			showFront: function() {
				this.show(1, 0)
			},

			showPage: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var group_id = $(e.currentTarget).data("group_id");
				this.show(id, group_id)
			},

			showHome: function(page, group_id) {
				this.$el.html( messagesTemplate() )
				this.show(page, group_id)
			},

			show: function(page, group_id) {

				var MessagesCollection = Backbone.Collection.extend({
		  			url: '/api/messages',
		  			model: MessageModel
				})
				
				var Action = Backbone.Model.extend({
			  		urlRoot: '/api/messages/allPagesCount',
					defaults: {	}
				})

				var messages = new MessagesCollection()
				var self = this
				messages.fetch({data: {page: page, group_id: group_id}, 
					success: function() {
						$('div#messages_list').html( messagesListTemplate({messages: messages.toJSON(), user: App.user ? App.user.toJSON() : false, is_admin: false, group_id: group_id, page:page}))
					},
					error: function() {
						App.showMessage("No groups founded")
					},
				})
				
				var action = new Action()
				action.fetch({data: {group_id: group_id}, 
					success: function() {
						$('div#pager').html( pagerTemplate({action :action.toJSON(), group_id: group_id}))
					},
					error: function() {
						
					},
				})
				$('div#create_message').html( createMessageTemplate({group_id: group_id}))
				
				return this
			},

			save: function() {
				var form = Form( $('form[name="create-message-form"]', this.$el) )
				var data = form.toJSON()
				
				var message = new MessageModel(data)
				var self = this

				message.save({},{
					wait: true,
					success: function() {
						App.showMessage("Created", "alert-success")
						self.show(1, data.group_id)
					},
					error: function() {
						App.showMessage("Creation failed")
					},
				})
			},

			comment: function(e) {
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				
				var m = this.$('#comment-'+id+' > .control-group > .controls > .comment-message').val()
				var group_id = this.$('#comment-'+id+' > .control-group > .controls > .group_id').val()
				var parent_id = this.$('#comment-'+id+' > .control-group > .controls > .parent_id').val()
				var page = this.$('#comment-'+id+' > .control-group > .controls > .page').val()

				//Form( $('.comment-1', this.$el) )
				console.log(m)
				
				var data = {message: m, group_id :group_id, parent_id :parent_id}
				var message = new MessageModel(data)
				var self = this

				message.save({},{
					wait: true,
					success: function() {
						App.showMessage("Created", "alert-success")
						self.show(page, data.group_id)
					},
					error: function() {
						App.showMessage("Creation failed")
					},
				})
			},

			delete: function(e){
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
        				App.showMessage("Message deleted")
						self.show(1, group_id)
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

	return MessagesView;

});