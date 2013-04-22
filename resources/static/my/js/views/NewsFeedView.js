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

				this.activitiesView = new ActivitiesView({ el: $('#newsfeed_list') }).render()
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
					this.activitiesView.deleteParam("user_id")
					this.activitiesView.deleteParam("type")
					}
				else if (typeName == "my")
					this.activitiesView.setParams({user_id: App.user.id, page: 1})
				else {
					this.activitiesView.deleteParam("user_id")
				 	typeId = ['recommendations','ratings','messages'].indexOf(typeName)
				 	this.activitiesView.setParams({type: typeId, page: 1})
				}
				$('.messages-left > .links > .link').removeClass('active')
				$(e.target).closest('a').parent().addClass('active')
				this.activitiesView.refresh()

				return false
			},

			sendMessage: function() {
				var form = Form( $('form[name="create-message-form"]', this.$el) )
				var data = form.toJSON()

				var message = new MessageModel(data)
				var self = this

				message.save({},{
						wait: true,
						success: function() { self.activitiesView.showPage(1, true) },
						error: function() {	App.showMessage("Message send failed!")	},
				})
			},
	})

	return NewsFeedView;

});
