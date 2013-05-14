define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var MessageModel = Backbone.Model.extend({

			urlRoot: '/api/messages',
			idAttribute: 'uid',

			defaults: {	},

			postComment: function(text) {

				var data = {
					parent_id: this.get('uid'),
					group_id: this.get('group_id'),
					message: text
				}

				var message = new MessageModel(data)
				return message.save()
			},

	})

	MessageModel.deleteMessage = function(uid) {
		var tmp = new MessageModel({uid: uid})
		return tmp.destroy()
	}

	return MessageModel

})
