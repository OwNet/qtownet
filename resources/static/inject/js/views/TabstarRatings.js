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
				console.log( 'set',val )
				this.$el.attr('data-rating',val)
			},

			value: function() {
				return this.$el.attr('data-rating')
			},

			onStarClick: function(e) {
				var val = $(e.target).attr('data-rate')
				this.setValue(val)
				this.trigger('change',val)
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