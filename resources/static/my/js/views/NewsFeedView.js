define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , ActivitiesView = require('views/ActivitiesView')
	  , template = require('tpl/newsfeed')


	var NewsFeedView = Backbone.View.extend({

			events: {

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
				this.$el.html( template() )
				return this
			},
	})

	return NewsFeedView;

});
