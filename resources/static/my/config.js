require.config({

	baseUrl: 'js',

	paths: {
			async       :  'share/lib/async',
			jquery      :  'share/lib/jquery',
			underscore  :  'share/lib/lodash',
			bootstrap   :  'share/lib/bootstrap',
			backbone    :  'share/lib/backbone',
			jade        :  'share/lib/jade.runtime.min',

			'utils' : 'share/utils/utils',
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

			utils: ['jquery.escape']
	}

});


require(['App', 'router', 'utils'], function(App, Router) {
	App.initialize()
	new Router({ App:App })
});