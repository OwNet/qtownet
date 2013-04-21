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
				params: { page:1 },
			}

			initialize: function(opts) {
				opts || (opts = {})
				this.options = _.extend({}, this.defaultOptions, opts.options)

				this.activities = new Activities()
				this.activities.setParam(this.options.params)

				var viewManagerFactory = new Backbone.CollectionBinder.ViewManagerFactory(this._viewFactory)
				this.collectionBinder = new Backbone.CollectionBinder(viewManagerFactory)

				this.page = 1
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

				if (this._refreshTimer) {
					clearTimeout(this._refreshTimer)
					delete this._refreshTimer
				}

				this.undelegateEvents()
				MessagesView.__super__.remove.call(this)
			},

			refresh: function() {
				if (this.refreshXhr)
					this.refreshXhr.abort()

				var self = this
				this.refreshXhr = this.activities.fetch({
					success: function() {

					},
					error: function() {

					},
					complete: function() {
						delete self.refreshXhr
					}
				})
			},

			setPage: function(page) {
				if (page != this.options.params.page) {
					this.options.params.page = page
					this.refresh()
				}
			}

			_refreshSetTimeout: function() {
				var self = this
				this._refreshTimer = setTimeout( function() {
					self.refresh()
					self._refreshSetTimeout()
				}, this.options.interval)
			},

			_viewFactory: function(model) {
				return new Backbone.View()
			}


	})

	return MessagesView;

});
