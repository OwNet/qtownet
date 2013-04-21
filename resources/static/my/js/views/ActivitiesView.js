define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , Activities = require('collections/Activities')

	require('Backbone.CollectionBinder')



	var MessagesView = Backbone.View.extend({

			events: {

			},

			defaultOptions: {
				refresh: true,
				interval: 5000,
				filter: {},
			}

			initialize: function(opts) {
				opts || (opts = {})
				this.options = _.extend({}, this.defaultOptions, opts.options)

				this.activities = new Activities()
				this.activities.setFilter(opts.filter)

				var elManagerFactory = new Backbone.CollectionBinder.ViewManagerFactory(this._viewFactory)
				this.collectionBinder = new Backbone.CollectionBinder(elManagerFactory)
			},


			render: function() {
				this.$el.html( '' )
				this.collectionBinder.bind(this.activities, $('...'))

				if (this.options.refresh) {

				}

				return this
			},

			remove: function() {
				this.collectionBinder.unbind()
				delete this.collectionBinder

				this.activities.off()
				delete this.activities

				if (this.refresh.timer) {
					clearTimeout(this.refresh.timer)
					delete this.refresh.timer
				}

				this.undelegateEvents()
				MessagesView.__super__.remove.call(this)
			},

			refresh: function() {
				this.activities.fetch()
			},

			_refreshSetTimeout: function() {
				var self = this
				this.refresh.timer = setTimeout( function() {
					self.refresh()
					self._refreshSetTimeout()
				},this.options.interval)
			},

			_viewFactory: function(model) {
				return new Backbone.View()
			}


	})

	return MessagesView;

});
