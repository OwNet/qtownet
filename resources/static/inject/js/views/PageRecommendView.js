
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
			this.$title = $('input[name="title"]', this.$el)
			this.$ok = $('#recommend-ok')

			App.groups.on('reset', this.updateGroups, this)
			App.on('OwNet:page', this.updateForm, this)

			this.updateGroups()
		},

		updateGroups: function() {
			var $groups = $('select[name="group_id"]', this.$el)

			$groups.html('')
			App.groups.where({ member: "1" }).forEach( function(grp) {
				$groups.append( $('<option>', { value:grp.get('id'), text: grp.get('name')  } ) )
			})
		},

		updateForm: function() {
			this.$title.val(App.page.title)
		},

		onSubmit: function() {

			try {
				var attrs = {
					title: $('input[name="title"]', this.$el).val(),
					description: $('textarea[name="description"]', this.$el).val(),
					group_id: $('select[name="group_id"] > option:selected', this.$el).val(),
					absolute_uri: App.page.uri,
				}

				var model = new RecommendModel()
				  , self = this

				model.save(attrs, {
					success: function() { self.showOkStatusAndClose() },
					error: function (model, resp){
						if (resp.status===409) { // Conflict
							var grp = App.groups.get(attrs.group_id).get('name')
							if (confirm('You have already recommend this site in group '+grp+'. Do you want edit your recommendation?')) {
								var error = JSON.parse(resp.responseText)
								model.set('id', error.uid)
								model.save(null, {
									success: function() { self.showOkStatusAndClose() },
									error: function (model, resp){ App.error(resp.statusText) },
								})
							}
						}
						else if (resp.status===401) { // Unauthorized
							alert('You have been logged out!')
							App.logout(true)
						} else
							App.error(resp.statusText)
					},
				})
			}
			catch(e) {
				App.fatalError('Unexpected Error')
				console.log( e )
			}

			return false
		},

		showOkStatusAndClose: function() {
			this.$ok.show()
			var self = this
			setTimeout(function() {
				self.$ok.fadeOut()
				App.router.close()
			}, 1500)
		}




	})

	return PageRecommendView;

});
