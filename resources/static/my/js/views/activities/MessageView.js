define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , template = require('tpl/message')


	var MessageView = Backbone.View.extend({

			events: {

			},

			initialize: function(opts) {
				this.model = opts.model
			},

			render: function() {
				var data = {
					message: this.model.attributes,
					user: App.user.attributes,
				}

				this.$el.html( template(data) )
				return this
			}

	})

	return MessageView;

});
