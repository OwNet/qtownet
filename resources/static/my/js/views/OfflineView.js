define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , ActivitiesView = require('views/ActivitiesView')

	  , offlineTemplate = require ("tpl/offline")

	  , Form = require("share/utils/form")


	var OfflineView = Backbone.View.extend({

			events: {
				'click form[name="offline-form"] button[name="submit"]': 'saveOrder',
			},

			render: function() {
				this.hide()
				this.$el.html( '<div id="offline-form"></div>' )
				this.trigger('render')

				return this
			},

			show: function(url) {
				this.render()
				$('#offline-form').html(offlineTemplate({ url: url}) )
				
			},

			hide: function() {
				this.$el.html('')
				this.isShown = false
			},

			saveOrder: function() {
				var form = Form( $('form[name="offline-form"]', this.$el) )
				
				return $.ajax('http://my.ownet/api/orders/',{
				    data: JSON.stringify(form.toJSON()), 
				    type: 'POST',
				    processData: false,
				    contentType: 'application/json',
				    success: function(data) { 
						App.router.navigate('profile/showdownloadorders', {trigger: true})
					},
				})
			},

	})

	return OfflineView;

});
