define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , template = require('tpl/rating')


	var RatingView = Backbone.View.extend({

			events: {

			},

			initialize: function(opts) {
				this.model = opts.model
			},

			render: function() {
				var data = {
					activity: this.model.attributes,
					user: App.user.attributes,
				}

				this.$el.html( template(data) )
				return this
			},

			close: function() {
				this.remove()
			},

	})

	return RatingView;

});
