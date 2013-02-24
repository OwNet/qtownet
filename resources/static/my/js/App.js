define( function (require) {

	"use_strict"

	var App = {

		initialize: function() {

		},


		showMessage: function(text, msgclass, clear /* = true*/) {
			var $slot = $("#messages")
			var $msg = $("<div>",{ text:text }).addClass("alert").addClass(msgclass)

			if ( clear !==false )
				$slot.html( $msg )
			else
				$slot.append( $msg )

			setTimeout( this.clearMessages, 3000)
		},


		clearMessages: function() {
			$("#messages").hide( "fast", function (){ $(this).html("").show() })
		},

	}

	return App

})