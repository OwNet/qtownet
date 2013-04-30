define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , Files = require('collections/Files')
	  , JIframe = require("jiframe")
	  // , JForm = require("jform")

	  , filesTemplate = require ("tpl/files")
	  , fileTableTemplate = require ("tpl/filestable")
	  , groupFormTemplate = require ("tpl/groupform")
	  , pagerTemplate = require ("tpl/filespager")

	  , FileModel = require ("share/models/FileModel")
	  , Form = require("share/utils/form")


	var FileView = Backbone.View.extend({

			events: {
				// 'click a[data-filter]' : 'onFilterClick',
				'submit form[name="file-upload-form"]': 'uploadMe',
				'change input[name="selectfile"]': 'showFile',
				'click a[id="pagerBtn"]' : 'onPageClick',
				'click a[name="delete-file"]': "deleteF",
			},

			initialize: function(opts) {
				this.model = opts.model
			},

			render: function() {
				this.hide()
				this.$el.html( filesTemplate() )
				this.trigger('render')
				return this
			},

			show: function() {
				this.hide()
				this.render()
				$("#upload_file").hide()
				this.isShown = true
			},

			onPageClick: function(e) {
				var page = $(e.target).attr('data-id')
				var filter = $(e.target).attr('data-filter')
				this.showFiles(filter, page)
				return false
			},

			// showPage: function(page, force) {
			// 	if (page != this.options.params.page || force) {
			// 		this.options.params.page = page
			// 		this.refresh().then( function() {
			// 			window.scrollTo(0,0)
			// 		})
			// 	}
			// },

			showFiles: function(filter, page) {
				var files = new Files([],{ filter: filter})

				$.when(
					files.fetch({ page: page }),
					files.fetchPageCount()
				).done( function() {
					$('#newsfeed_list').html( fileTableTemplate({files :files.toJSON(), filter: filter, curusr: App.user}))
					$('#pager').html( pagerTemplate({ pages: files.pages, filter: filter, current: page}))
				})

				return this
			},

			upload: function() {
				$("#upload_file").slideDown('slow')
				// $(":file").filestyle()
				// $("#newsfeed_list").show()
				// $("#submitbutton").hide()
				// return this
			},

			showFile: function(){
				$("#subfile").val($("#upload-box").val())
				$(":text").prop("disabled", false)
				$("#inputDescription1").prop("disabled", false)
				$("#submitbutton").show()
				$("#uploadButton").hide()

			},

			uploadMe: function(e) {
				e.preventDefault()
				var $input = $('#upload-box');
				var $form = $('#file-upload-form');
				var data = $form.serializeArray();
				console.log(data)
				console.log($input)
    			$.ajax('/api/files',{
    				files: $input,
    				data: data,
    				type:'POST',
    				iframe: true,
    				dataType:'json',
    				processData: false,
    				success: function(e){
    				// $input.clearInputs();
    					$(":text").val('');
    					$(":input").val('');
    					$("#upload_file").slideUp('slow', function(){
    						App.router.navigate("#/files")
    					})
    					// setTimeout(function(){App.router.navigate("#/files")}(),20000)
    					
    				// $('input[type=password]').val('');
    					// $(":text").prop("disabled", true)
    					// $("#uploadButton").show()
    					// $("#submitbutton").hide();
    				// $("#upload_file").slideUp('slow')
    			}
    				
    			})
		
			},

			navi: function() {
    			App.router.navigate("#/files")
    		},

			hide: function() {
				this.$el.html('')
			},

			deleteF: function(e){
				e.preventDefault();

				var id = $(e.currentTarget).data("id");

				var file = new FileModel()
				console.log(id)
				file.set('uid', id)
				console.log(file.get('id'))

				var self = this
				file.destroy({
					success: function() {
						console.log("Deleted")
						// App.router.navigate("#/files", {trigger: true})
						// App.showMessage("Group deleted")
						// self.show("all", 1)
					},
					error: function() {
						console.log("Cannot delete")
						// App.showMessage("Cannot delete")
					},
				})
			},

			// deleteDO: function(e){
			// 	e.preventDefault();
			// 	var id = $(e.currentTarget).data("id");

			// 	var data = {uid : id}

			// 	var DeleteDO = Backbone.Model.extend({
			// 		urlRoot: '/api/files',
			// 		defaults: {	}
			// 	})

			// 	var downloadorder = new DeleteDO(data)
			// 	downloadorder.uid = id
			// 	var self = this

			// 	downloadorder.destroy({
			// 		wait: true,
			// 		success: function() {
			// 			App.router.navigate("#/showdownloadorders", {trigger: true})
			// 			App.showMessage("Download Order deleted")
			// 			self.showDownloadOrders(1)
			// 		},
			// 		error: function() {
			// 			App.showMessage("Cannot delete")
			// 		},
			// 	})

			// },

			deleteFile: function(e){
				e.preventDefault()
				var id = $(e.currentTarget).data("id");
				console.log(id)
				$.ajax({
					url: '/api/files/'+id,
    				type:'DELETE',
    				success: function(){
    				// $input.clearInputs();
    					App.showMessage("File deleted")
    				// $('input[type=password]').val('');
    					// $(":text").prop("disabled", true)
    					// $("#uploadButton").show()
    					// $("#submitbutton").hide();
    				// $("#upload_file").slideUp('slow')
    				}
				})
			},	
			
	})

	return FileView;

});
