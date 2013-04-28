define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , JIframe = require("jiframe")
	  , MessageModel = require('share/models/MessageModel')
	  , ActivitiesView = require('views/ActivitiesView')

	  , template = require('tpl/files')
 	  ,	Form = require("share/utils/form")

	var FilesView = Backbone.View.extend({

			events: {
				'click a[data-filter]' : 'onFilterClick',
				// 'click form[name="file-upload-form"] button[name="upload-file"]': 'startUploading',
				'change input[name="selectfile"]' : 'uploadMe',
			},

			initialize: function(opts) {

			},

			show: function() {
				if (this.isShown)
					return

				this.render()
				$("#upload_file").hide()
				var opts = { params: { /*group_id: 0*/ }}

				this.activitiesView = new ActivitiesView({ el: $('#newsfeed_list'), options: opts }).render()
				this.isShown = true
			},

			upload: function() {
				$("#upload_file").show()
				$("#newsfeed_list").hide()
				// return this
			},

			uploadMe: function(e) {
				var $input = $('#upload-box');
				console.log($input)
    			$.ajax('/api/files',{
    				files: $input,
    				type:'POST',
    				iframe: true,
    				dataType:'json',
    				
    			}).error(function(data) {
		            console.log(data);
        		});
			},

			fileSelected: function() {
				var oFile = document.getElementById('upload-box').files[0];
				var oReader = new FileReader();
				oReader.readAsDataURL(oFile);
			},

			startUploading: function() {
				var vFD = new FormData(document.getElementById('file-upload-form'));
				var oXHR = new XMLHttpRequest();
				oXHR.open('POST', 'http://my.ownet/api/files');
    			oXHR.send(vFD);


			},

			hide: function() {
				this.activitiesView.remove()
				delete this.activitiesView
				this.$el.html('')
				this.isShown = false
			},

			render: function() {
				var data = {
					group_id: 0,
				}

				this.$el.html( template(data) )
				return this
			},

			onFilterClick: function(e) {
				var typeName = $(e.target).closest('a').attr('data-filter')
				if (typeName=='all'){
					this.activitiesView.deleteParam("user_id")
					this.activitiesView.deleteParam("type")
					}
				else if (typeName == "my"){
					this.activitiesView.setParams({user_id: App.user.id, page: 1})
					this.activitiesView.deleteParam("type")
				}
				else {
					this.activitiesView.deleteParam("user_id")
				 	typeId = ['recommendations','ratings','messages'].indexOf(typeName)
				 	this.activitiesView.setParams({type: typeId, page: 1})
				}
				$('.messages-left > .links > .link').removeClass('active')
				$(e.target).closest('a').parent().addClass('active')
				this.activitiesView.refresh()

				return false
			},

			sendMessage: function() {
				var $text = $('textarea[name="message"]', this.$el)
				var content = $text.val()

				if (content==="")
					return

				var data = {
					message: content,
					group_id: 0,
					parent_id: 0,
				}

				var message = new MessageModel(data)
				var self = this

				message.save(null, {
					success: function() {
						self.activitiesView.showPage(1, true)
						$text.val('')
					},
					error: function() {	App.showMessage("Message send failed!")	},
				})
			},
	})

	return FilesView;

});
