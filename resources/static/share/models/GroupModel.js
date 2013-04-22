define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var GroupModel = Backbone.Model.extend({

			urlRoot: '/api/groups',

			defaults: {	},

			fetchMembers: function() {
				var self = this
				return $.ajax({
					type: 'GET',
					url: this.urlRoot + '/getGroupUsers',
					data: { group_id: this.id },
					success: function (data){ self.members = data },
				})
			},

	})

	return GroupModel

})
