define( function (require){

	"use_strict"

	var $ = require("jquery")


	function jqsub() {
		var str = arguments[0]
		var params

		if (typeof arguments[1] === 'object')
			params = arguments[1]
		else
			params =  Array.prototype.slice.call(arguments,1);


		var lastpos = 0
		var indx = 0
		var arglen = params.length
		var res = ''

		while (indx < arglen && (pos = str.indexOf('?',lastpos)) !== -1 ) {
			res += str.substring(lastpos,pos) + $.escape(params[indx++])
			lastpos = pos+1
		}

		res += str.substring(lastpos)

		return res
	}



	$.escape = function jqueryEscape (str) {
		if (typeof str === 'number') return str
		else return (str===undefined ? '' : str).toString().replace(/[ !"#$%&'()*+,.\/:;<=>?@\[\\\]\^`{|}~]/g,'\\$&')
	}

	$.query = function(query, params, context) {
		return $(jqsub(query,params), context)
	}

})

