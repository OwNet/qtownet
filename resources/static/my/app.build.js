({
	mainConfigFile: "config.js",

	include: [
		"../config",
	],

	out: "js/app.min.js",

	name: '../../share/lib/require',

	wrap: false,

	preserveLicenseComments: false,

	optimize: "uglify",
 })

