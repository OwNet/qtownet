define( function (require) {

	"use_strict"

	var Backbone = require('backbone')
	  , SessionModel  = require('share/models/SessionModel')
	  , UserModel = require ('share/models/UserModel')

	var App = {

		initialize: function() {

			var session = new SessionModel()

			session.fetch({
				success: function() {
					var user = new UserModel({ id: session.get('user_id') })
					user.fetch({
						success: function(){
							App.session = session
							App.user = user
							App.trigger('ready')
						},
						error:   function(){ App._userLoadError() },
					})
				},
				error: function() {
					App.trigger('ready')
					App.router.navigate("#/login", {trigger: true}
				)} // user is not logged in
			})
		},

		login: function(opts /*{login,password,succes,error}*/ ) {

			var session = new SessionModel({login:opts.login, password:opts.password})

			session.save({},{
				success: function() {
					session.unset('password')
					var user = new UserModel({ id: session.get('user_id') })
					user.fetch({
						success: function() {
							if ( App.isUserLogged() ) {
								App.session.off()
								App.user.off()
							}
							App.session = session
							App.user = user
							opts.success(user)
							App.trigger('login',App.user)
						},
						error: function (){ App.fatalError('Error: User cannot be loaded!') }
					})
				},
				error: opts.error
			})
		},

		logout: function(opts) {
			this.session.destroy({
				success: function (){
					App.session.off()
					delete App.session
					App.user.off()
					delete App.user
					opts.success()
					App.trigger('logout')
				},
				error: function (){ App.fatalError('Error: Logout failed') }
			})
		},

		isUserLogged: function (){
			return App.user != null
		},

		showMessage: function(text, msgclass, clear /* = true*/) {
			var $slot = $('#messages')
			var $msg = $('<div>',{ text:text }).addClass('alert').addClass(msgclass)

			if ( clear !==false )
				$slot.html( $msg )
			else
				$slot.append( $msg )

			setTimeout( this.clearMessages, 3000)
		},

		clearMessages: function() {
			$('#messages').hide( 'fast', function (){ $(this).html('').show() })
		},

		fatalError: function(msg) {
			alert(msg)
			location.reload()
		},



	}

	_.extend(App, Backbone.Events)

	return App

})
