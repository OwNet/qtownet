define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , MessageModel = require('share/models/MessageModel')
	  , template = require('tpl/message')


	var MessageView = Backbone.View.extend({

			events: {
				'click img[name="delete-activity"]' : "onDeleteClick",
				'click a[name="loadComments"]' : "loadComments",
				'click button[name="submit-comment"]' : 'submitComment',

				'click a[name="delete-comment"]' : "deleteComment",
			},

			initialize: function(opts) {
				this.model = opts.model
			},

			render: function() {
				var data = {
					message: this.model.attributes,
					user: App.user.attributes,
				}

				this.$el.html( template(data) )
				return this
			},

			close: function() {
				this.undelegateEvents()
				this.off()
				this.remove()
			},

			onDeleteClick: function() {
				if (confirm('Are you sure ?')){
					var model = new MessageModel()
					model.set('id', this.model.get('object_id'))

					var self = this
					model.destroy({
						success: function() { self.model.collection.remove(self.model.id) }
					})
				}
			},

			loadComments: function(e) {
				if (e)
					e.preventDefault()

				if (!this.message)
					this.message = new MessageModel({uid: this.model.get('object_id')})

				console.log( this.message.id )

				var self = this
				this.message.fetch().done( function() {
					self.model.set('comments', self.message.get('comments'))
					self.render()
				})
			},

			submitComment: function(e) {
				e.preventDefault()

				var self = this
				  , text = $('input[name="comment"]', this.$el).val()

				this.message.postComment(text).done( function() {
					self.loadComments()
				})
			},

			deleteComment: function(e) {
				var id = $(e.currentTarget).data('id')
				  , self = this

				MessageModel.deleteMessage(id).done( function() {
					self.loadComments()
				})
			},
	})

	return MessageView;

});
