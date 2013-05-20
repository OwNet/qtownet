(function () {

	"use_strict"


	/* Utils  */
	var $ = {
		pageId: 0
	}


	$.setCSS = function(element, styles) {
		for (var key in styles)
			element.style[key] = styles[key]
	}

	$.addCss = function (cssCode) { // TODO REFACTOR
		var styleElement = document.createElement("style");
		styleElement.type = "text/css";

		if (styleElement.styleSheet) {
			styleElement.styleSheet.cssText = cssCode;
		}
		else {
			styleElement.appendChild(document.createTextNode(cssCode));
		}
		document.getElementsByTagName("head")[0].appendChild(styleElement);
	}

	$.onDocumentReady = function (callback, document) {
	    document || (document = window.document)
		document.addEventListener("DOMContentLoaded", function once() {
			document.removeEventListener("DOMContentLoaded", once, false)
			callback()
		}, false);
	}

	$.getEncodedPageUri = function () {
		return encodeURIComponent(document.location.href.replace(/#.*$/, ""));
	}
	$.getEncodedReferrerUri = function () {
		return encodeURIComponent(document.referrer.replace(/#.*$/, ""));
	}

	$.loadScript = function (url, callback) {
		var body = document.body;
		var script = document.createElement('script');
		script.type = 'text/javascript';
		script.src = url;

		var callback_called = 0;
		if (document.all) {
			script.onreadystatechange = function () {
				if ((script.readyState === "loaded" || script.readyState === "complete") && callback_called == 0) {
					callback();
					callback_called = 1;
				}
			};
		}
		else {
			script.onload = function () {
				callback();
				callback_called = 1;
			};
		}

		body.appendChild(script);
	}

	$.getRandomId = function() {
		return Math.floor((Math.random() * 1000) + 1);
	}

	$.setPageId = function (id) {
		this.pageId = id;
	}
	$.getPageId = function () {
		return this.pageId;
	}

	//$.isNotOwnet = function () {
	//	return document.location.href.match(/[a-zA-Z]+.ownet\/api\/[a-zA-Z]+/) === null;
	//}

	//$.isFromOwnet = function () {
	//	return document.referrer.match(/[a-zA-Z]+.ownet\/api\/[a-zA-Z]+/) !== null;
	//}

	$.urlEquals = function (a, b) {
		return (a.replace(/\//g, "") === b.replace(/\//g, ""));
	}

	var PrefetchContact = {
		apiUri               : "http://prefetch.ownet/api/prefetch/",
		hasRequestedPrefetch : 0,
		requestTimeout       : 0,
		hasReportedClose     : 0,
		hasReportedPrefetch  : 0,
		TIMEOUT_DELAY_SECS   : 60,
		startRequestTimeout: function () {
		    this.requestTimeout = setTimeout(function () { PrefetchContact.requestPrefetch(); }, this.TIMEOUT_DELAY_SECS * 1000);
		},
		stopRequestTimeout: function() {
			if (this.requestTimeout) clearTimeout(this.requestTimeout);
		},
		requestPrefetch: function () {
		   
		    if ($.getPageId() != 0) {
		       
		        var count = document.links.length;

                if (count > 0) {
                    predictions = [];
                    var xF = Math.floor(count * 0.35);
                    if (xF >= 0 && xF < count && document.links[xF].href && document.links[xF].href.match(/^http:.*/) !== null)
                        predictions[predictions.length] = document.links[xF].href;

                    var yF = Math.floor(count * 0.5);
                    if (yF > xF && yF < count && document.links[yF].href && document.links[yF].href.match(/^http:.*/) !== null)
                        predictions[predictions.length] = document.links[yF].href;

                    var zF = Math.floor(count * 0.65);
                    if (zF > yF && zF < count && document.links[zF].href && document.links[zF].href.match(/^http:.*/) !== null)
                        predictions[predictions.length] = document.links[zF].href;

                    Ownet.sendMessage("prefetch", { page: $.getPageId(), links: predictions.toString() });
                }
			}
			else {
				this.startRequestTimeout();
			}
		},

		reportClose: function () {
			try {
				if (this.hasReportedClose == 0 && $.getPageId() != 0) {
					this.stopRequestTimeout();
					$.ajaxGet(this.apiUri + "close/?pid=" + $.getPageId() + "&gid=" + $.getRandomId(), function (xmlHttp) {
						if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
							return true;
						}
						return true;
					});
					this.hasReportedClose = 1;
				}
			}
			catch (e) {
			}
		}

		//reportPrefetch: function () {
		//	try {
		//		if (this.hasReportedPrefetch == 0 && document.referrer.match(/[a-zA-Z]+.ownet\/api\/prefetch/) !== null) {
		//			owNetGLOBAL.loadScript(this.apiUri + "done/?page=" + $.getEncodedPageUri() + "&gid=" + $.getRandomId(), function () {
		//				return true;
		//			});
		//			this.hasReportedPrefetch = 1;
		//		}
		//	}
		//	catch (e) {
		//	}
		//}
	}

	var HistoryContact = {
		hasReportedVisit: 0,
		apiUri: "http://api.ownet/api/history/",

		reportVisit: function () {
			try {
				if (this.hasReportedVisit == 0) {
					$.loadScript(this.apiUri + "visit/?page=" + $.getEncodedPageUri() + "&ref=" + $.getEncodedReferrerUri() + "&gid=" + $.getRandomId(), function () {
						if (owNetPAGEID !== null) {
							$.setPageId(owNetPAGEID);
						}
					});
					this.hasReportedVisit = 1;
				}
			}
			catch (e) {
				// throw e;
			}
		}
	}

	var CacheSwitch = { 
        isSwitchedOn: 0,
        switchable : null,
        init : function(switchable) {
            if (this.switchable == null) {
                this.switchable = switchable;
                Ownet.sendMessage("caching:check", { url : document.location.href.replace(/#.*$/, "") });
            }
        },
	    doSwitch : function() {
	        if (this.isSwitchedOn == 0) {
	            Ownet.sendMessage("caching:change", { url:  document.location.href.replace(/#.*$/, ""), settings: true });
	            this.switchable.switchOn();
	            this.isSwitchedOn = 1;
	        }
	        else {
	            Ownet.sendMessage("caching:change", { url : document.location.href.replace(/#.*$/, ""), settings: false });
	            this.switchable.switchOff();
	            this.isSwitchedOn = 0;
	        }
	    },
	    checkSwitch: function (settings) {
	        if (settings) {
	            this.switchable.switchOn();
	            this.isSwitchedOn = 1;
	        }
	        else {
	            this.switchable.switchOff();
	            this.isSwitchedOn = 0;
	        }
	    }
	}

	var HighlightSwitch = {
		isSwitchedOn: 0,
        switchable: null,
		highlightedLinks: [],
		init: function (switchable) {
		    this.switchable = switchable;
		    $.addCss("a.OwNetHIGHLIGHT { border: 2px solid #F49B04; }");

		},
		receiveLinks: function (linksobj) {
		    if (Array.isArray(linksobj)) {
		        for (var i = 0; i < linksobj.length; ++i) {
		            linksobj[i] = decodeURIComponent(linksobj[i]);
		        }
		    }
		    else {
		        linksobj = [];
		    }
			return linksobj;

		},
		doSwitch: function () {
			if (this.isSwitchedOn == 0) {  /* switch on */
				this.highlightedLinks = [];
                    
				var x = [];
				for (var i in document.links) {
				    if (document.links[i].href && document.links[i].href.match(/^http:.*/) !== null && x.indexOf(document.links[i].href) < 0) {
				        x[x.length] = document.links[i].href; 
				    }
				} // JSON.stringify({ links: x.toString() });
				Ownet.sendMessage("cached", { links : x });
			}
			else {  /* switch off*/
				this.switchOff();
			}
		},
		switchOn: function (links) {
		    links = this.receiveLinks(links);
		    this.highlightedLinks = [];
		    if (links.length > 0) {
		        for (var i = 0; i < document.links.length; ++i) {
		            for (var j = 0; j < links.length; ++j) {
		                if ($.urlEquals(links[j], document.links[i].href)) {
		                    document.links[i].className += " OwNetHIGHLIGHT";

		                    this.highlightedLinks[this.highlightedLinks.length] = document.links[i];
		                    break;
		                }
		            }
		        }
		        this.switchable.switchOn();
		        this.isSwitchedOn = 1;
		    }
		 
		},
		switchOff: function () {
			var reg = new RegExp("(\\s|^)" + "OwNetHIGHLIGHT" + "(\\s|$)");

			for (var i = 0; i < this.highlightedLinks.length; ++i) {
				this.highlightedLinks[i].className = this.highlightedLinks[i].className.replace(reg, "");
			}

			this.highlightedLinks = [];
			this.isSwitchedOn = 0;
			this.switchable.switchOff();
		}
	};

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

			cache_settings: {
			    icon: 'owetab_cache_exc_off.png',
				alt: 'Cache',
				title: 'Open options to configure caching.',
				onclick: 'toggleCacheSettings',
                init: 'initToggleCacheSettings'
			},

			offline_links: {
				icon: 'owetab_off.png',
				alt: 'Highlight',
				title: 'Highlight links on this webpage which are available offline.',
				onclick: 'toggleOfflineLinks',
				init: 'initToggleOfflineLinks'
			},

			page_rating: {
				icon: 'owetab_star.png',
				text: 'Show OwNet',
				alt: 'Recomend',
				title: 'Open OwNet box for rating, recommending and tagging this webpage.',
				onclick: 'togglePageActions',
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
				'border-bottom-right-radius' : '5px',
				'background-image': 'linear-gradient(to bottom, rgba(252, 195, 98, 0.8), rgba(244, 155, 4, 0.8))',
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
				'border': 'none',
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
				'top': '40px',
				'z-index': '999998',
				'background': 'transparent',
			},

		},

		events: {
			'OwNet:ready' : 'onOwnetReady',
			'OwNet:iframe:resize' : 'iframeResize',
			'OwNet:iframe:close': 'iframeClose',
			'OwNet:highlight': 'highlightLinks',
            'OwNet:caching:checked': 'cachingChecked'
		},

		/* public */
		initialize: function() {

			this._initEvents()

			this.root = document.createElement('div')
			document.body.appendChild(this.root)

			window.addEventListener("message", function(e) {
				Ownet.receiveMessage(e)
			}, false)

			this._initIFrame()
		},

		sendMessage: function (name, data) {
		    this.iframe.contentWindow.postMessage({ name: name, data: data, OwNet: true }, this.iframeHost)
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

		togglePageActions: function() {
			this._toggleTab('page_actions')
		},


		initToggleCacheSettings: function (icon) {
		    CacheSwitch.init({ switchOn: function () { icon.src = icon.src.replace("off.png", "on.png"); }, switchOff: function () { icon.src = icon.src.replace("on.png", "off.png"); } });
		},

		toggleCacheSettings: function () {
		    CacheSwitch.doSwitch();
		},

		initToggleOfflineLinks: function (icon) {
		    HighlightSwitch.init({ switchOn: function () { icon.src = icon.src.replace("off.png", "on.png"); }, switchOff: function () { icon.src = icon.src.replace("on.png", "off.png"); } });
		},

		toggleOfflineLinks: function () {
		    HighlightSwitch.doSwitch();
		},

		sendPageInfo: function() {
			var data = {
				uri: location.hostname + (location.pathname !== '/' ? location.pathname:''),
				title: document.title,
			}
			this.sendMessage('page', data)
		},

		iframeResize: function(rect) {
			this.iframe.setAttribute("width", rect.width)
			this.iframe.setAttribute("height", rect.height)
		},

		/* events */
		onOwnetReady: function() {
			this._initControls()
			this.sendPageInfo()
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
			var controlsRoot = document.createElement('div')
			  , controlsShadowRoot = controlsRoot.webkitCreateShadowRoot()
			  , controlsBox = document.createElement('div')
			  , controls = document.createElement('div')

			$.setCSS( controls, this.style.controls)
			$.setCSS( controlsBox, this.style.controlsBox)

			this.root.appendChild(controlsRoot)
			controlsShadowRoot.appendChild(controlsBox)
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

				if (link.init) {
				    self[link.init](img);
				}

				if (link.href)
					a.setAttribute('href', link.href)

				if (link.text) {
					var span = document.createElement('span')
					span.textContent = link.text
					$.setCSS(span, this.style.linkText)
					a.appendChild(span)
				}

			}

			document.body.appendChild(this.root);
		},

		_initIFrame: function() {
			var box = document.createElement('div')
			$.setCSS(box, this.style.iframeBox)
			this.iframeBox = box

			var iframe = document.createElement('iframe')
			iframe.setAttribute("src", this.iframeHost + this.iframePath)
			iframe.setAttribute("frameborder", "0")
			iframe.setAttribute("allowtransparency",true)

			box.appendChild(iframe)
			this.root.appendChild(box)

			this.iframe = iframe
		},

		_toggleTab: function(tab) {
			if (this.activeTab === tab)
				this.sendMessage('close')
			else {
				this.activeTab = tab
				this.iframeBox.style.display = 'block'
				this.sendMessage('show',tab)
			}
		},

		cachingChecked: function (data) {
		    CacheSwitch.checkSwitch(data && data.is_exception);
		},

		highlightLinks: function (data) {
		    HighlightSwitch.switchOn(data.split(','));
		},

		iframeClose: function() {
			this.activeTab = null
			this.iframeBox.style.display = 'none'
		}
	}



	//if ($.isFromOwnet()) {
	//	$.onDocumentReady(function () {
	//		PrefetchContact.reportPrefetch();
	//	});
	//}
	//else if ($.isNotOwnet()) {
		HistoryContact.reportVisit();
		Ownet.initialize();

		PrefetchContact.startRequestTimeout();
	//}
}());
