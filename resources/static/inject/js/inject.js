(function() {

	"use_strict"


	/* Utils  */
	var $ = {}

	$.setCSS = function(element, styles) {
		for (var key in styles)
			element.style[key] = styles[key]
	}

	$.onDocumentReady = function(callback, document) {
		document || (document = window.document)
		document.addEventListener("DOMContentLoaded", function once() {
			document.removeEventListener("DOMContentLoaded", once, false)
			callback()
		}, false);
	}

	var Events = {
		__callbacks__ : {},
		on: function(event, callback, context) {
			if ( ! this.__callbacks__[event] ) this.__callbacks__[event] = []
			this.__callbacks__[event].push( {callback:callback, ctx:context})
		},
		trigger: function(event) {
			var clb
			if ( (clb = this.__callbacks__[event]) != null ) {
				var args = Array.prototype.slice.call(arguments,1)
				for (var i=0; i<clb.length; i++)
						clb[i].callback.apply(clb[i].ctx, args)
			}
		},
	}

	/* Ownet */
	var Ownet = {

		/* attributes */
		root       :    null,
		activeTab  :    null,

		ifrmae     :    null,
		iframeBox  :    null,
		iframeHost :    'http://inject.ownet',
		iframePath  :   '/index.html',

		imgBaseURI :    'http://inject.ownet/img/',

		links: {
			home: {
				icon: 'owetab_home.png',
				alt: 'home',
				title: 'Visit OwNet homepage.',
				href: 'http://my.ownet',
			},

			refresh: {
				icon: 'owetab_refresh.png',
				alt: 'Refresh',
				title: 'Download new version of current webpage.',
			},

			cache_settings: {
				icon: 'owetab_caching.png',
				alt: 'Cache',
				title: 'Open options to configure caching.',
				onclick: 'toggleCacheSettings',
			},

			offline_links: {
				icon: 'owetab_off.png',
				alt: 'Highlight',
				title: 'Highlight links on this webpage which are available offline.',
			},

			page_rating: {
				icon: 'owetab_star.png',
				text: 'Show OwNet',
				alt: 'Recomend',
				title: 'Open OwNet box for rating, recommending and tagging this webpage.',
				onclick: 'togglePageRatings',
			},

			close: {
				icon: 'close.png',
				alt: 'Close OwNet box',
				title: 'Close OwNet box',
				style: 'close',
				onclick: 'hide'
			}
		},

		style: {
			controlsBox: {
				'position': 'fixed',
				'top': '0',
				'left': '0',
				'z-index': '999999',
			},

			controls: {
				'line-height': '18px',
				'text-align': 'left',
				'padding': '7px 0px 9px 10px',
				'margin': '0px',
				'border-bottom': '2px solid #ffe566',
				'border-right': '2px solid #ffe566',
				'border-left': 'none',
				'border-top': 'none',
				'background': 'rgba(255, 244, 190, 0.8)',
				'-webkit-touch-callout': 'none',
				'-webkit-user-select': 'none',
				'-khtml-user-select': 'none',
				'-moz-user-select': 'none',
				'-ms-user-select': 'none',
				'user-select': 'none',
			},

			link: {
				'cursor': 'pointer',
			},

			icon: {
				'margin-bottom': '-4px',
				'margin-right': '4px',
				'width': '16px',
				'height': '16px',
			},

			linkText: {
				'font-family': 'Helvetica, Arial, Tahoma, Verdana',
				'font-weight': 'bold',
				'font-size': '10px',
				'line-height': '10px',
				'color': '#2490b2',
			},

			close: {
				'cursor': 'pointer',
				'margin-left': '33px',
			},

			iframeBox: {
				'position': 'fixed',
				'display' : 'none',
				'left': '0px',
				'top': '30px',
				'background-color': 'rgb(255, 244, 190)',
				'border': '3px solid rgb(196, 196, 196)',
				'z-index': '999998',
			},

			iframeTitleBar: {
				'text-align': 'right',
				'height': '20px',
			},

			ifrmaeClose: {
				'margin-top': '3px',
				'margin-right': '3px',
				'width': '16px',
				'height': '16px',
			},
		},

		events: {
			'OwNet:ready' : 'onOwnetReady',
		},

		/* public */
		initialize: function() {

			this._initEvents()

			window.addEventListener("message", function(e) {
				Ownet.receiveMessage(e)
			}, false)

			this._initControls()
			this._initIFrame()
		},

		sendMessage: function(name,data) {
			this.iframe.contentWindow.postMessage( {name:name, data:data, OwNet:true}, this.iframeHost)
		},

		receiveMessage: function(event) {
			if (event.origin !==  'http://inject.ownet')
				return
			var msg = event.data
			this.trigger('OwNet:'+msg.name, msg.data)
		},

		hide: function() {
			this.root.style.display = 'none'
		},

		togglePageRatings: function() {
			this._toggleTab('page_rating')
		},

		toggleCacheSettings: function() {
			this._toggleTab('cache_settings')
		},

		sendURI: function() {
			this.sendMessage('URI', location.hostname + (location.pathname !== '/' ? location.pathname:'') )
		},

		/* events */
		onOwnetReady: function() {
			this.sendURI()
		},

		/* private */
		_initEvents: function() {
			for (var key in Events)
				this[key] = Events[key]

			var events = this.events
			for (var key in events)
				this.on(key, this[events[key]], this)
		},

		_initControls: function() {
			this.root = document.createElement('div')

			var controls = document.createElement('div')
			  , controlsBox = document.createElement('div')

			$.setCSS( controls, this.style.controls)
			$.setCSS( controlsBox, this.style.controlsBox)

			this.root.appendChild(controlsBox)
			controlsBox.appendChild(controls)

			var self = this

			for (var key in this.links) {

				var link = this.links[key]
				  , a = document.createElement('a')
				  , img = document.createElement('img')

				a.appendChild(img)
				controls.appendChild(a)

				$.setCSS(a, (link.style? this.style[link.style] : this.style.link))
				$.setCSS(img, this.style.icon)

				img.setAttribute('src', this.imgBaseURI + link.icon)
				img.setAttribute('alt', link.alt)
				img.setAttribute('title', link.title)

				if (link.onclick)
					a.onclick = self[link.onclick].bind(this)

				if (link.href)
					a.setAttribute('href', link.href)

				if (link.text) {
					var span = document.createElement('span')
					span.textContent = link.text
					$.setCSS(span, this.style.linkText)
					a.appendChild(span)
				}

			}

			document.getElementsByTagName("body")[0].appendChild(this.root);
		},

		_initIFrame: function(onload) {
			var box = document.createElement('div')
			$.setCSS(box, this.style.iframeBox)
			this.iframeBox = box


			var title_bar = document.createElement('div')
			$.setCSS(title_bar, this.style.iframeTitleBar)

			var close_link = document.createElement('a')
			close_link.style.cursor = 'pointer'
			close_link.onclick = this._closeIFrameTab.bind(this)

			var img_close = document.createElement('img')
			img_close.setAttribute('src',this.imgBaseURI + this.links.close.icon)
			img_close.setAttribute('alt','Close OwNet box')
			img_close.setAttribute('title','Close OwNet box')
			$.setCSS(img_close, this.style.ifrmaeClose)

			close_link.appendChild(img_close)
			title_bar.appendChild(close_link)
			box.appendChild(title_bar)

			var iframe = document.createElement('iframe')
			iframe.setAttribute("src", this.iframeHost + this.iframePath)
			iframe.setAttribute("width", "460")
			iframe.setAttribute("height", "460");
			iframe.setAttribute("scrolling", "auto")
			iframe.setAttribute("frameborder", "0")

			box.appendChild(iframe)
			this.root.appendChild(box)

			this.iframe = iframe
		},

		_toggleTab: function(tab) {
			if (this.activeTab === tab)
				this._closeIFrameTab()
			else {
				this.activeTab = tab
				this.sendMessage('tabselect',tab)
				this.iframeBox.style.display = 'block'
			}
		},

		_closeIFrameTab: function() {
			this.activeTab = null
			this.iframeBox.style.display = 'none'
		}
	}

	if (top === self) { // do not apply for iframes
		$.onDocumentReady(function() {
			Ownet.initialize()
		})
	}

}());