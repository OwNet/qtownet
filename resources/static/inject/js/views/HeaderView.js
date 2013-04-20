define( function (require) {

	"use_strict"
	require('init')

	var App = require("App")
	  , Backbone = require("backbone")


	var HeaderView = Backbone.View.extend({

			events: {
				'click #header-close' : 'onCloseClick',
				'click #header-user-logout'  : 'onLogoutClick',
			},

			initialize: function() {
				this.$header = $('#header-user')
				this.$username = $('#header-user-name')
				this.$logout = $('#header-user-logout')

				App.on('user:loaded', this.onUserLoaded, this)
				App.on('user:logout', this.onUserLogout, this)
			},

			onUserLoaded: function() {
				this.$username.text( App.user.name() )
				this.$header.show()
			},

			onUserLogout: function() {
				this.$header.hide()
				this.$username.text('')
			},

			onCloseClick: function() {
				App.router.close()
			},

			onLogoutClick: function() {
				App.logout()
			}
	})

	return HeaderView;

});
