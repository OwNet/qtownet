define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , MessageModel = require('share/models/MessageModel')
	  , template = require('tpl/message')
	  , commentsTemplate = require('tpl/comments')


	var MessageView = Backbone.View.extend({

			events: {
				'click img[name="delete-activity"]' : "onDeleteClick",
				'click a[name="loadComments"]' : "loadComments",
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
				var model = new MessageModel()
				model.set('id', this.model.get('object_id'))

				var self = this
				model.fetch({
					success: function() { 
						$(e.currentTarget).parent().next()( commentsTemplate({ message:model }) )
					}
				})
			},

	})

	return MessageView;

});
