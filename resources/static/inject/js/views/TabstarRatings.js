define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require("backbone")


	var TabstarRatings = Backbone.View.extend({

			events: {
				'click li>a': 'onStarClick',
				'mouseover li>a': 'onStarMouseOver',
				'mouseout': 'onMouseOut',
			},

			initialize: function(opts) {

			},

			setValue: function(val) {
				this.$el.attr('data-rating',val)
			},

			value: function() {
				return this.$el.attr('data-rating')
			},

			onStarClick: function(e) {
				this.setValue( $(e.target).attr('data-rate') )
			},

			onStarMouseOver: function(e) {
				this.$el.attr('data-hover-rating', $(e.target).attr('data-rate') )
			},

			onMouseOut: function() {
				this.$el.removeAttr('data-hover-rating')
			},

	})

	return TabstarRatings;

});