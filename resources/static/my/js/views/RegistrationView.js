define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , UserModel = require ("share/models/UserModel")
	  , registraitonTemplate = require ("tpl/registration")

	  , Form = require("share/utils/form")


	var RegistrationView = Backbone.View.extend({

			events: {
				'click form[name="registration-form"] button[name="submit"]': 'registration'
			},

			initialize: function() {

			},


			render: function() {
				this.$el.html( registraitonTemplate() )
				return this
			},


			show: function() {
				this.render()
			},

			hide: function() {
				this.$el.html('')
			},

			registration: function() {
				var form = Form( $('form[name="registration-form"]', this.$el) )
				var data = form.toJSON()
				var user = new UserModel( data )

				user.save({},{
					wait: true,
					success: function() {
						App.router.navigate('login', {trigger: true})
						App.showMessage("Registration was successful. Now You can log in.", "alert-success")
					},
					error: function() {
						App.showMessage("Registration failed")
					},
				})
			},

	})

	return RegistrationView;

});
