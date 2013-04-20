define( function (require) {

	"use_strict"
	require('init')

	var App = require("App")
	  , Backbone = require("backbone")
	  , UserModel = require ("share/models/UserModel")
	  , Form = require("share/utils/form")


	var LoginView = Backbone.View.extend({

			events: {
				'submit form[name="login-form"]': 'onLoginSubmit'
			},

			initialize: function() {
				App.on('user:logout', this.onUserLogout, this)
				this.form = Form( $('form[name="login-form"]', this.$el) )
			},

			onLoginSubmit: function() {
				var form =
				App.login( this.form.toJSON() )
				return false
			},

			onUserLogout: function() {
				this.form.el.reset()
			},

			focus: function() {
				$('input[name="login"]', this.$el).focus()
			},
	})

	return LoginView;

});
