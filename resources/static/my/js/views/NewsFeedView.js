define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , ActivitiesView = require('views/ActivitiesView')
	  , template = require('tpl/newsfeed')


	var NewsFeedView = Backbone.View.extend({

			events: {
				'click a[data-filter]' : 'onFilterClick',
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
				this.undelegateEvents()
				this.activitiesView.remove()
				delete this.activitiesView

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

				if (typeName=='all')
					this.activitiesView.deleteParam('type')
				else {
				  typeId = ['recommendations','ratings','messages'].indexOf(typeName)
				  this.activitiesView.setParams({type: typeId})
				}


				return false
			}
	})

	return NewsFeedView;

});
