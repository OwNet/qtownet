define( function (require) {

	"use_strict"


	var Backbone = require('backbone')


	var DownloadOrdersModel = Backbone.Model.extend({

			urlRoot: '/api/prefetch/show',

	})



	return DownloadOrdersModel

})