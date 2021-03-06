define( function (require) {

	"use_strict"


	var $ = require('jquery')

	var Form = function ($form) {
		if ( ! (this instanceof Form) )
			return new Form($form)

		this.$el = $form
		this.el = $form.get(0)
	}


	Form.prototype.toJSON = function() {
		var ar = this.$el.serializeArray()
		var data = {}

		for (var i=0; i<ar.length; i++)
			data[ ar[i].name ] = ar[i].value

		return data
	}



	return Form
})
