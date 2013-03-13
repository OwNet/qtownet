define( function (require) {
	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , userNavbarTemplate = require ("tpl/user-navbar")


	var NavbarView = Backbone.View.extend({

			events: {
				'click #navbar a[href="#/logout"]': 'onLogoutClick',
			},

			initialize: function() {
				this.render()

				this._bindAppUser()

				App.on('login logout', function() {
					this._bindAppUser()
					this.render()
				}, this)

			},

			render: function() {
				var $el = $('#navbar').html( userNavbarTemplate({ user:  App.user ? App.user.toJSON() : false }))
			},

			onLogoutClick: function() {
				App.logout({
					success: function() {
						App.router.navigate("#/login", {trigger: true})
						App.showMessage("Now you are logged out")
					}
				})
				return false
			},

			_bindAppUser: function() {
				if (App.user)
					App.user.on('change:first_name change:last_name', this.render, this)
			},
	})

	return NavbarView;

});