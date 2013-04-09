define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , recommendationsTemplate = require ("tpl/recommendations")
	  , profileTemplate = require ("tpl/profile")
	  , recommendationsTableTamplate = require ("tpl/recommendationstable")
	  , recommendationsPagerTemplate = require ("tpl/recommendationspager")
	  , ratingsPagerTemplate = require ("tpl/ratingspager")
	  , ratingsTemplate = require ("tpl/ratings")
	  , ratingsTableTamplate = require ("tpl/ratingstable")
	  , UserModel = require ("share/models/UserModel")
	  , ActivityModel = require ("share/models/ActivityModel")

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")


	var RecommendationsView = Backbone.View.extend({

			events: {
				'click a[name="recommpager"]' : "showRecommPage",
				'click a[name="ratingpager"]' : "showRatingPage",
				'click a[name="myRecommendations"]' : "showMyRecommendations",
				'click a[name="myRatings"]' : "showMyRatings",
				'click a[name="allRecommendations"]' : "showAllRecommendations",
				'click a[name="allRatings"]' : "showAllRatings",
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
						$('div#pager').html( recommendationsPagerTemplate({action :action.toJSON(), filter: filter}))
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

			showAllRecommendations: function(){
				this.showRecommendations("all", 1);
			},

			showRecommPage: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var filter = $(e.currentTarget).data("filter");
				this.showRecommendations(filter,id)
			},

			showRatingPage: function(e){
				e.preventDefault();
				var id = $(e.currentTarget).data("id");
				var filter = $(e.currentTarget).data("filter");
				this.showRatings(filter,id)
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
						$('div#pager').html( ratingsPagerTemplate({action :action.toJSON(), filter: filter}))
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

			showAllRatings: function(){
				this.showRatings("all", 1);
			},
			


			
	})

	return RecommendationsView;

});