define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , MessageModel = require('share/models/MessageModel')
	  , ActivitiesView = require('views/ActivitiesView')

	  , template = require('tpl/newsfeed')
 	  ,	Form = require("share/utils/form")

	var NewsFeedView = Backbone.View.extend({

			events: {
				'click a[data-filter]' : 'onFilterClick',
				'click form[name="create-message-form"] button[name="submit-message"]': 'sendMessage',
			},

			initialize: function(opts) {

			},

			show: function() {
				if (this.isShown)
					return

				this.render()

				var opts = { params: { /*group_id: 0*/ }}

				this.activitiesView = new ActivitiesView({ el: $('#newsfeed_list'), options: opts }).render()
				this.isShown = true
			},

			hide: function() {
				this.activitiesView.remove()
				delete this.activitiesView
				this.$el.html('')
				this.isShown = false
			},

			render: function() {
				var data = {
					group_id: 0,
				}

				this.$el.html( template(data) )
				return this
			},

			onFilterClick: function(e) {
				var typeName = $(e.target).closest('a').attr('data-filter')
				if (typeName=='all'){
					this.activitiesView.setParams({page: 1})
					this.activitiesView.deleteParam("user_id")
					this.activitiesView.deleteParam("type")
					}
				else if (typeName == "my"){
					this.activitiesView.setParams({user_id: App.user.id, page: 1})
					this.activitiesView.deleteParam("type")
				}
				else {
					this.activitiesView.deleteParam("user_id")
				 	typeId = ['recommendations','ratings','messages'].indexOf(typeName)
				 	this.activitiesView.setParams({type: typeId, page: 1})
				}
				$('.messages-left > .links > .link').removeClass('active')
				$(e.target).closest('a').parent().addClass('active')
				this.activitiesView.render()

				return false
			},

			sendMessage: function() {
				var $text = $('textarea[name="message"]', this.$el)
				var content = $text.val()

				if (content==="")
					return
				$('textarea[name="message"]', this.$el).val("")
				var data = {
					message: content,
					group_id: 0,
					parent_id: 0,
				}

				var message = new MessageModel(data)
				var self = this

				message.save(null, {
					success: function() {
						self.activitiesView.render()
						$text.val('')
					},
					error: function() {	App.showMessage("Message send failed!")	},
				})
			},
	})

	return NewsFeedView;

});
