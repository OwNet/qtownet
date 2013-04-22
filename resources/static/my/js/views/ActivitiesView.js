define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , Activities = require('collections/Activities')
	  , RecommendationView = require('views/activities/RecommendationView')
	  , RatingView = require('views/activities/RatingView')
	  , MessageView = require('views/activities/MessageView')
	  , pagerTemplate = require('tpl/pager')


	require('Backbone.CollectionBinder')

	var activitiesTypesView = {
		0: RecommendationView,
		1: RatingView,
		2: MessageView,
	}


	var ActivitiesView = Backbone.View.extend({

			events: {
				'click a[data-page]' : 'onPageClick',
			},

			defaultOptions: {
				//refresh: true,
				refresh: false,
				interval: 10000,
				params: { page:1 },
			},

			initialize: function(opts) {
				opts || (opts = {})
				this.options =  $.extend(true, {}, this.defaultOptions, opts.options)

				this.activities = new Activities()
				this.activities.setParams(this.options.params)

				var viewManagerFactory = new Backbone.CollectionBinder.ViewManagerFactory(this._viewFactory)
				this.collectionBinder = new Backbone.CollectionBinder(viewManagerFactory)

				this.page = 1
			},

			render: function() {
				this.activities.reset([])
				this.$el.html( '' )

				this.$list = $('<div>')
				this.$pager = $('<div>')
				this.$el.append(this.$list).append(this.$pager)

				this.collectionBinder.bind(this.activities, this.$list)

				this.options.refresh ? this._refreshSetTimeout(0) : this.refresh()

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
				ActivitiesView.__super__.remove.call(this)
			},

			refresh: function() {
				if (this.refreshXhr) {
					this.refreshXhr.abort()
					this.refreshPagesCountXhr.abort()
				}

				var self = this
				this.refreshXhr = this.activities.fetch()
				this.refreshPagesCountXhr = this.activities.fetchPageCount()

				return $.when( this.refreshXhr, this.refreshPagesCountXhr).then( function() {
					delete self.refreshXhr
					delete self.refreshPagesCountXhr
					self.$pager.html( pagerTemplate({ pages: self.activities.pages, current: self.options.params.page  }) )
				})
			},

			showPage: function(page, force) {
				if (page != this.options.params.page || force) {
					this.options.params.page = page
					this.refresh().then( function() {
						window.scrollTo(0,0)
					})
				}
			},

			setParams: function(params) {
				this.options.params = _.extend(this.options.params, params)
			},

			deleteParam: function(key) {
				delete this.options.params[key]
			},

			onPageClick: function(e) {
				var page = $(e.target).attr('data-page')
				this.showPage(page)
				return false
			},

			_refreshSetTimeout: function(timeout) {
				if (this._refreshTimer) {
					clearTimeout(this._refreshTimer)
				}

				var self = this
				this._refreshTimer = setTimeout( function() {
					self.refresh()
					this._refreshTimer = null
					self._refreshSetTimeout()
				}, timeout !== undefined ? timeout : this.options.interval)
			},

			_viewFactory: function(model) {
				var View = activitiesTypesView[model.get('type')] || Backbone.View
				return new View({model: model})
			}


	})

	return ActivitiesView;

});
