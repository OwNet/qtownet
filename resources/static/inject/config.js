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
	},

	shim: {
			bootstrap: ['jquery'],

			backbone: {
				deps: ['underscore', 'jquery'],
				exports: 'Backbone',
			},

			jade: {
				exports: 'jade',
			},

	}

});


require(['App', 'router'], function(App, Router) {
	App.once('ready', function() {
		App.router = new Router()
		App.router.start()
		App.sendMessage('ready')
	})

	App.initialize()
});
