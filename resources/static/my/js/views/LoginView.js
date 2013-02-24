define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , SessionModel  = require("share/models/SessionModel")
	  , UserModel = require ("share/models/UserModel")

	  , loginTemplate = require ("tpl/login")
	  , userNavbarTemplate = require ("tpl/user-navbar")

	  , Form = require("share/utils/form")



	var LoginView = Backbone.View.extend({

			events: {
				'click form[name="login-form"] button[name="submit"]': 'login'
			},

			initialize: function() {
				var session = new SessionModel()
				var self = this

				session.fetch({
					success: function() {
						if (session.has('user_id')) {
							session.id = ''	// TODO: remove this epic hack
							App.session = session
							self.loadUser(session.get("user_id"))
						} else
							self.updateNavbar()
					},
					error: function() {
						self.updateNavbar()
					}
				})
			},

			render: function() {
				this.$el.html( loginTemplate() )
				return this
			},


			show: function(message) {
				this.render()
			},


			login: function() {
				var form = Form( $('form[name="login-form"]', this.$el) )
				var data = form.toJSON()

				var session = new SessionModel(data)
				var self = this
				session.save({},{
					wait: true,
					success: function() {
						App.showMessage("Login successful", "alert-success" )
						session.id = ''	// TODO: remove this epic hack
						App.session = session
						session.unset("password")

						self.loadUser(session.get("user_id"))
					},
					error: function() {
						App.showMessage("Login failed")
					},

				})
			},


			logout: function() {
				var self = this
				if ( App.session ) {
					App.session.destroy({
						success: function() {
							console.log( 'xxxxxx' )
							delete App.session
							delete App.user
							self.updateNavbar()
							App.router.navigate("#/login", {trigger: true})
							App.showMessage("Now you are logged out")
						}
					})
				}
			},


			loadUser: function(id) {
				var user = new UserModel()
				user.id = id

				var self = this
				user.fetch({
					success: function() {
						App.user = user
						self.updateNavbar()
					}
				})
			},


			updateNavbar: function() {
				var self = this
				var $el = $('ul#user-navbar').html( userNavbarTemplate({ user:  App.user ? App.user.toJSON() : false }))
				$.query('a[href="?"]',['#/logout'],$el).on('click',function() {
					self.logout()
					return false
				})
			}

	})

	return LoginView;

});