define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , recommendationsTemplate = require ("tpl/recommendations")
	  , profileTemplate = require ("tpl/profile")
	  , recommendationsTableTamplate = require ("tpl/recommendationstable")
	  , ratingsTableTamplate = require ("tpl/ratingstable")
	  , UserModel = require ("share/models/UserModel")
	  , ActivityModel = require ("share/models/ActivityModel")

	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")


	var RecommendationsView = Backbone.View.extend({


			initialize: function() {
				
			},


			render: function() {
				this.$el.html( recommendationsTemplate({}) )

				return this
			}, 
		

			showRecommendations: function() {
				var ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities',
					model: ActivityModel
				})

				var activities = new ActivitiesCollection()

				activities.fetch({
					success: function() {
						$('div#recommendations').html( recommendationsTableTamplate({activities: activities.toJSON()}))
					},
					error: function(){
						App.showMessage("Error reading activities")
					},
				})
				this.$el.html( recommendationsTemplate({ }) )
				return this

			},

			showRatings: function() {
				var ActivitiesCollection = Backbone.Collection.extend({
					url: '/api/activities',
					model: ActivityModel
				})

				var activities = new ActivitiesCollection()

				activities.fetch({
					success: function() {
						$('div#recommendations').html( ratingsTableTamplate({activities: activities.toJSON()}))
					},
					error: function(){
						App.showMessage("Error reading activities")
					},
				})
				this.$el.html( recommendationsTemplate({ }) )
				return this

			},


			
	})

	return RecommendationsView;

});