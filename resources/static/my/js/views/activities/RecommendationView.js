define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , PageRecommendModel = require('share/models/PageRecommendModel')
	  , template = require('tpl/recommendation')


	var RecommentationView = Backbone.View.extend({

			events: {
				'click img[name="delete-activity"]' : "onDeleteClick",
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
				this.undelegateEvents()
				this.off()
				this.remove()
			},

			onDeleteClick: function() {
				if (confirm('Are you sure ?')){
					var model = new PageRecommendModel()
					model.set('id', this.model.get('object_id'))

					var self = this
					model.destroy({
						success: function() { self.model.collection.remove(self.model.id) }
					})
				}
			},
	})

	return RecommentationView;

});
