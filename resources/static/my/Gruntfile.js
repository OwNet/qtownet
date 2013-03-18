module.exports = function(grunt) {

	grunt.initConfig({

		pkg: grunt.file.readJSON('package.json'),


		jade: {
			index: {
				options: {
					data: {
						debug: true
					},
					compileDebug: true,
					pretty: true,
				},
				files: {
					"index.html": "templates/index.jade"
				}
			},

			client: {
				options: {
					data: {
						debug: true
					},
					client: true,
					amd: true,
					namespace: false,
					compileDebug: true,
					pretty: true,
				},
				files: [{
					expand: true,
					cwd: 'templates-client',
					src: ["**/*.jade"],
					dest: 'js/tpl/',
					ext: '.js',
				}],
			}

		},

		less: {
			style: {
				options: {
					dumpLineNumbers: 'comment',
				},
				files: {
					"css/style.css": "style/style.less"
				}
			},
		},

		symlink: {
			share: {
				dest: 'js/share',
				relativeSrc: '../../share',
				options: {type: 'dir'}
			}
		},

		clean: ["css/style.css", "js/tpl"],

		watch: {

			jade: {
				files: ['templates/**/*.jade'],
				tasks: ['jade:index'],
			},

			jade_client: {
				files: ['templates-client/**/*.jade'],
				tasks: ['jade:client'],
			},

			less_style: {
				files: ['style/**/*.less', '../share/style/**/*.less'],
				tasks: ['less:style']
			},

		},

	});

	grunt.loadNpmTasks('grunt-symlink');
	grunt.loadNpmTasks('grunt-contrib-jade');
	grunt.loadNpmTasks('grunt-contrib-less');

	grunt.loadNpmTasks('grunt-contrib-clean');
	grunt.loadNpmTasks('grunt-contrib-watch');

	grunt.registerTask('default', ['jade','less']);
	grunt.registerTask('configure', ['symlink']);

}