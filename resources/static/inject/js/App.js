define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require('backbone')
	  , SessionModel  = require('share/models/SessionModel')
	  , UserModel = require ('share/models/UserModel')

	var App = {

		initialize: function() {

			window.addEventListener("message", this.receiveMessage, false)

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
				error: function() { App.trigger('ready') } // user is not logged in
			})

		},

		receiveMessage: function(event) {
			var msg = event.data
			if (!msg.OwNet)
				return
			App.trigger('OwNet:'+msg.name, msg.data)
		},

		sendMessage: function(name, data) {
			parent.postMessage({name:name, data:data}, '*')
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

		fatalError: function(msg) {
			alert(msg)
			location.reload()
		},
	}

	_.extend(App, Backbone.Events)

	return App

})
