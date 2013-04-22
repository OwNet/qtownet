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

			approveUser: function(user_id) {
				return GroupModel.call('approveUser', {group_id: this.id, user_id: user_id})
			},

			declineUser: function(user_id) {
				return GroupModel.call('declineUser', {group_id: this.id, user_id: user_id})
			},

			deleteUser: function(user_id) {
				return GroupModel.call('deleteUser', {group_id: this.id, user_id: user_id})
			},

			addAdmin: function(user_id) {
				return GroupModel.call('addAdmin', {group_id: this.id, user_id: user_id})
			},
	})


	GroupModel.call = function(path, data) {

		return $.ajax({
			type: 'POST',
			url: '/api/groups/'+path,
			data: JSON.stringify(data),
			contentType: 'json',
		})
	}

	GroupModel.joinGroup = function (group_id){
		return GroupModel.call('joinGroup',{group_id: group_id})
	}

	return GroupModel

})
