define( function (require) {

	"use_strict"

	var App = require("App")
	  , Backbone = require("backbone")
	  , Files = require('collections/Files')
	  , JIframe = require("jiframe")
	  
	  , filesTemplate = require ("tpl/files")
	  , fileTableTemplate = require ("tpl/filestable")
	  , groupFormTemplate = require ("tpl/groupform")
	  , pagerTemplate = require ("tpl/filespager")

	  , FileModel = require ("share/models/FileModel")
	  , Form = require("share/utils/form")


	var FileView = Backbone.View.extend({

			events: {

				'submit form[name="file-upload-form"]': 'uploadFile',
				'click a[id="pagerBtn"]' : 'onPageClick',
				'click a[name="delete-file"]': "deleteFile",
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
			
			showFiles: function(filter, page) {
				if (filter == "my"){
					$("#all").parent().removeClass("active")
					$("#my").parent().addClass("active")	
				}
				
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
			},

			uploadFile: function(e) {
				e.preventDefault()
				
				var $input = $('#upload-box');
				var $form = $('#file-upload-form');
				var data = $form.serializeArray();
				    			
    			$.ajax('/api/files',{
    				files: $input,
    				data: data,
    				type:'POST',
    				iframe: true,
    				dataType:'json',
    				processData: false,
    				beforeSend: function(e){
    					$('#myModal').modal('show')
    				},

    				success: function(e){
    					$('#myModal').modal('hide')
    					$("#upload_file").slideUp('slow', function(){
    						App.router.navigate("#/files")
    					})
    				}
    				
    			})
		
			},

			hide: function() {
				this.$el.html('')
				this.isShown = false
			},

			deleteFile: function(e){
				e.preventDefault();

				var id = $(e.currentTarget).data("id");
				var file = new FileModel()

				file.set('uid', id)

				var self = this

				file.destroy({
					success: function() {
						App.router.navigate("#/files")
						App.showMessage("File deleted")
						self.showFiles("all", 1)
					},
					error: function() {
						App.showMessage("Cannot delete file")
					},
				})
			},			
	})

	return FileView;

});
