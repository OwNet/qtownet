define( function (require) {

	"use_strict"
	require('init')

	var Backbone = require('backbone')
	  , SessionModel  = require('share/models/SessionModel')
	  , UserModel = require ('share/models/UserModel')
	  , Groups = require('collections/groups')

	var App = {

		initialize: function() {

			window.addEventListener("message", this.receiveMessage, false)

			App.on('OwNet:page',function(page) { App.page = page })

			App.user = new UserModel()
			App.groups = new Groups()
			App.session = new SessionModel()

			App.session.fetch({
				success: App.loadUser
			})

			App.trigger('ready')
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

		login: function(opts /*{login,password,success,error}*/ ) {
			var session = new SessionModel({login:opts.login, password:opts.password})

			session.save({},{
				success: function() {
					session.unset('password')
					App.session = session
					App.loadUser( session.get('user_id'))

					if (opts.success)
						opts.success()
				},
				error: opts.error
			})
		},

		logout: function(alreadyLogouted) {
			var success = function (){
				App.session.clear
				App.user.clear
				App.groups.clear
				App._isUserLogged = false
				App.trigger('user:logout')
			}

			if (alreadyLogouted)
				success()
			else
				this.session.destroy({
					success: success,
					error: function (){ App.fatalError('Logout failed') }
				})
		},

		loadUser: function() {
			App.user.clear()
			App.user.set('id', App.session.get('user_id'))

			$.when( App.user.fetch(), App.groups.fetch() )
				.done( function(){
					App._isUserLogged = true
					App.trigger('user:loaded')
				})
				.fail( function(){ App.fatalError('User cannot be loaded!') })
		},

		isUserLogged: function (){
			return App._isUserLogged
		},

		fatalError: function(msg) {
			App.error(msg)
			location.reload()
		},

		error: function(msg) {
			alert('Error: '+msg)
		}
	}

	_.extend(App, Backbone.Events)

	return App

})
