define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , UserModel = require ("share/models/UserModel")

	  , loginTemplate = require ("tpl/login")
	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")



	var LoginView = Backbone.View.extend({

			events: {
				'click form[name="login-form"] button[name="submit"]': 'login'
			},

			initialize: function() {

			},

			render: function() {
				this.$el.html( loginTemplate() )
				return this
			},

			show: function() {
				if (this.isShown)
					return

				if ( App.isUserLogged() ) {
					App.router.navigate('#/')
					return
				}
				App.router.navigate('#/login')
				this.render()
			},

			hide: function() {
				this.$el.html('')
				this.isShown = false
			},

			login: function() {
				var form = Form( $('form[name="login-form"]', this.$el) )
				var fields = form.toJSON()

				App.login({
					login: fields.login,
					password: fields.password,
					success: function() {
						App.router.navigate( '' , {trigger: true})
					},
					error: function(){ App.showMessage("Login failed") },
				})
			},
	})

	return LoginView;

});
