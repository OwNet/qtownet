define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , recommendationsTemplate = require ("tpl/recommendations")
	  , profileTemplate = require ("tpl/profile")
	  , recommendationsTableTamplate = require ("tpl/recommendationstable")
	  , pagerTemplate = require ("tpl/recommendationspager")
	  , ratingsTemplate = require ("tpl/ratings")
	  , ratingsTableTamplate = require ("tpl/ratingstable")
	  , UserModel = require ("share/models/UserModel")
	  , ActivityModel = require ("share/models/ActivityModel")

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")


	var RecommendationsView = Backbone.View.extend({

			events: {
				'click a[name="pager"]' : "showPage",
				'click a[name="myRecommendations"]' : "showMyRecommendations",
				'click a[name="myRatings"]' : "showMyRatings",
			},


			initialize: function() {
				
			},


			render: function() {
				this.$el.html( recommendationsTemplate({}) )

				return this
			}, 
		

			showRecommendations: function(filter, page) {
				var RecommendationsCollection;
				var Action;
				
				if (filter == "all") {
					ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities',
					model: ActivityModel
				})

				Action = Backbone.Model.extend({
				  		urlRoot: '/api/activities/allPagesCount?type=0',
						defaults: {	}
					})
				} else if (filter == "my") {
					ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities/my',
					model: ActivityModel
				})

				Action = Backbone.Model.extend({
				  		urlRoot: '/api/activities/myPagesCount?type=0',
						defaults: {	}
					})
				}

				var activities = new ActivitiesCollection()

				activities.fetch({data: {page: page, type: '0'},
					success: function() {
						$('div#recommendations').html( recommendationsTableTamplate({activities: activities.toJSON(), filter: filter}))
					},
					error: function(){
						App.showMessage("Error reading Recommendations")
					},
				})

				var action = new Action()


				action.fetch({
					success: function() {
						$('div#pager').html( pagerTemplate({action :action.toJSON(), filter: filter}))
					},
					error: function() {
						
					},
				})

				this.$el.html( recommendationsTemplate({ }) )
				return this

			},
			
			showMyRecommendations: function(){
				this.showRecommendations("my", 1);
			},

			showPage: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var filter = $(e.currentTarget).data("filter");
				this.showRecommendations(filter,id)
			},

			showRatings: function(filter, page) {
				var RatingsCollection;
				var Action;
				
				if (filter == "all") {
					ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities',
					model: ActivityModel
				})

				Action = Backbone.Model.extend({
				  		urlRoot: '/api/activities/allPagesCount?type=1',
						defaults: {	}
					})
				} else if (filter == "my") {
					ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities/my',
					model: ActivityModel
				})

				Action = Backbone.Model.extend({
				  		urlRoot: '/api/activities/myPagesCount?type=1',
						defaults: {	}
					})
				}

				var activities = new ActivitiesCollection()

				activities.fetch({data: {page: page, type: '1'},
					success: function() {
						$('div#ratings').html( ratingsTableTamplate({activities: activities.toJSON(), filter: filter}))
					},
					error: function(){
						App.showMessage("Error reading Ratings")
					},
				})

				var action = new Action()


				action.fetch({
					success: function() {
						$('div#pager').html( pagerTemplate({action :action.toJSON(), filter: filter}))
					},
					error: function() {
						
					},
				})

				this.$el.html( ratingsTemplate({ }) )
				return this

			},

			showMyRatings: function(){
				this.showRatings("my", 1);
			},
			


			
	})

	return RecommendationsView;

});