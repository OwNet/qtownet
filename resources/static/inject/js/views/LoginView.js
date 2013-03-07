define( function (require) {

	"use_strict"
	require('init')

	var App = require("App")
	  , Backbone = require("backbone")
	  , UserModel = require ("share/models/UserModel")
	  , Form = require("share/utils/form")


	var LoginView = Backbone.View.extend({

			events: {
				'click form[name="login-form"] button[name="submit"]': 'login'
			},

			login: function() {
				var form = Form( $('form[name="login-form"]', this.$el) )
				var fields = form.toJSON()

				App.login({
					login: fields.login,
					password: fields.password,
					success: function() {

					},
				})
			},
	})

	return LoginView;

});