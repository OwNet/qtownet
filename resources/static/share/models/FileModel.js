define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var FileModel = Backbone.Model.extend({

			urlRoot: '/api/files',
			idAttribute: 'uid',

			defaults: {	},

	})


	FileModel.call = function(path, data) {

		return $.ajax({
			type: 'POST',
			url: '/api/files/'+path,
			data: JSON.stringify(data),
			contentType: 'json',
		})
	}

	
	return FileModel

})
