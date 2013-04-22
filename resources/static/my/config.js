require.config({

	baseUrl: 'js',

	paths: {
			init        :  'share/lib/init',
			async       :  'share/lib/async',
			jquery      :  'share/lib/jquery',
			underscore  :  'share/lib/lodash',
			bootstrap   :  'share/lib/bootstrap',
			backbone    :  'share/lib/backbone',
			jade        :  'share/lib/jade.runtime.min',

			'jquery.escape' : 'share/utils/jquery.escape',

			'Backbone.CollectionBinder' : 'share/lib/Backbone.CollectionBinder',
			'Backbone.ModelBinder'      : 'share/lib/Backbone.ModelBinder'
	},

	shim: {
			bootstrap: ['jquery'],

			backbone: {
				deps: ['underscore', 'jquery'],
				exports: 'Backbone',
			},

			'Backbone.ModelBinder' : {
				deps: ['backbone']
			},

			'Backbone.CollectionBinder' : {
				deps: ['backbone', 'Backbone.ModelBinder']
			},

			jade: {
				exports: 'jade',
			},

			utils: ['jquery.escape']
	}

});


require(['App', 'router'], function(App, Router) {
	App.once('ready', function() {
		App.router = new Router()
		App.router.start()
	})

	App.initialize()
});
