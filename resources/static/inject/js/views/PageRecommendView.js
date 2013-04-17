
define( function (require) {

	"use_strict"
	require('init')

	var App = require('App')
	  , Backbone = require('backbone')
	  , RecommendModel = require('share/models/PageRecommendModel')


	var PageRecommendView = Backbone.View.extend({

		events: {
			'submit form[name="recommend-form"]' : 'onSubmit'
		},

		initialize: function() {
			this.updateGroups()
		},

		updateGroups: function() {
			var $groups = $('select[name="group_id"]', this.$el)

			$groups.html('')
			$groups.append( $('<option value="", disabled> -- select group -- </option>') )

			App.groups.where({ member: "1" }).forEach( function(grp) {
				$groups.append( $('<option>', { value:grp.get('id'), text: grp.get('name')  } ) )
			})
		},

		onSubmit: function() {

			try {
				var attrs = {
					title: $('input[name="title"]', this.$el).val(),
					description: $('textarea[name="description"]', this.$el).val(),
					group_id: $('select[name="group_id"] > option:selected', this.$el).val(),
					absolute_uri: App.uri,
				}

				var model = new RecommendModel()
				model.save(attrs)
			}
			catch(e) {
				console.log( e )
				// TODO
			}

			return false
		}




	})

	return PageRecommendView;

});
