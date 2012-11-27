owNetAVAILABLEURIS = [];

owNetGLOBAL = {
    localUri: "http://prefetch.ownet/api/prefetch/",
    prefetchUri : "http://prefetch.ownet/api/prefetch/",
    encodedPageUri: encodeURIComponent(document.location.href.replace(/#.*$/, "")),
    encodedReferrerUri: encodeURIComponent(document.referrer.replace(/#.*$/, "")),
    getRandomLink: function() {
        var num = Math.floor(Math.random()*document.links.length + 1);
        return encodeURIComponent(document.links[num].href.replace(/#.*$/, ""));
    },
    pageUriChanged: function () {
        var encuri = encodeURIComponent(document.location.href.replace(/#.*$/, ""));
        if (this.encodedPageUri !== encuri) {
            this.encodedPageUri = encuri;
            return true;
        }
        return false;
    },
    updatePageUri: function () {
        this.encodedPageUri = encodeURIComponent(document.location.href.replace(/#.*$/, ""));
    },
    clearNode: function (elem) {
        while (elem && elem.hasChildNodes()) {
            if (elem.firstChild.hasChildNodes())
                this.clearNode(elem.firstChild);
            else
                elem.clearChild(elem.firstChild);
        }
    },
    removeNode: function (node, parent) {
        while (node && node.hasChildNodes()) {
            this.removeNode(node.firstChild, node);
        }
        if (node && parent) {
            parent.removeChild(node);
        }
    },
    ajaxGet: function (url, callback) {
        var xmlHttp = null;
        try {
            xmlHttp = new XMLHttpRequest();
        }
        catch (e) {
            try {
                xmlHttp = new ActiveXObject("Msxml2.XMLHTTP");
            }
            catch (e) {
                xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
            }
        }

        if (xmlHttp == null) {
            return false;
        }
        xmlHttp.onreadystatechange = function () {
            callback(xmlHttp);
        };
        xmlHttp.open("GET", url, true);
        xmlHttp.send();
        return true;
    },
    loadScript: function (url, callback) {
        var head = document.getElementsByTagName('head')[0];
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

        head.appendChild(script);
    },
    loadCss: function (url) {
        var head = document.getElementsByTagName("head")[0];
        var css = document.createElement("link");
        css.setAttribute("rel", "stylesheet");
        css.setAttribute("type", "text/css");
        css.setAttribute("href", url);
        css.setAttribute("media", "screen");
        head.appendChild(css);
    },
    addCss: function (cssCode) {
        var styleElement = document.createElement("style");
        styleElement.type = "text/css";

        if (styleElement.styleSheet) {
            styleElement.styleSheet.cssText = cssCode;
        }
        else {
            styleElement.appendChild(document.createTextNode(cssCode));
        }
        document.getElementsByTagName("head")[0].appendChild(styleElement);
    },
    isArray: function () {
        if (arguments && typeof arguments[0] === "object") {
            var criterion = arguments[0].constructor.toString().match(/array/i);
            return (criterion != null);
        }
        return false;
    },
    urlEquals: function (a, b) {
        return (a.replace(/\//g, "") === b.replace(/\//g, ""));
    },
    fadeOut: function (target, callback) {
        var step = 10;
        var call = (arguments && arguments.length > 1) ? callback : null;
        target.style.opacity = 1.0;

        var anim = function () {
            if (step > 0) {
                step -= 2;
                target.style.opacity = step / 10.0;
                setTimeout(anim, 50);
            }
            else {
                target.style.display = "none";
                if (call !== null)
                    call();
            }
        };
        anim();
    },
    fadeIn: function (target, callback) {
        var step = 0;
        var call = (arguments && arguments.length > 1) ? callback : null;
        target.style.opacity = 0.0;
        target.style.display = "block";

        var anim = function () {
            if (step < 10) {
                step += 2;
                target.style.opacity = step / 10.0;

                setTimeout(anim, 50);
            }
            else {
                if (call !== null)
                    call();
            }
        };
        anim();
    },
    HighlightSwitch:
    {
        isSwitchedOn: 0,
        switchObj: null,
        availableUris: null,
        highlightedLinks: [],
        receiveLinks: function (linksobj) {
            if (owNetGLOBAL.isArray(linksobj)) {
                this.availableUris = linksobj;
                for (var i = 0; i < this.availableUris.length; ++i) {
                    this.availableUris[i] = decodeURIComponent(this.availableUris[i]);
                }
            }

        },
        doSwitch: function () {
            if (this.isSwitchedOn == 0) {  /* switch on */
                owNetAVAILABLEURIS = null;  // check the client everytime
                this.availableUris = null; 
                if (this.availableUris === null || this.availableUris.length === 0 || owNetGLOBAL.pageUriChanged() === true) {
                    owNetGLOBAL.updatePageUri();
                    this.availableUris = null;
                    this.highlightedLinks = [];
                    owNetGLOBAL.loadScript(owNetGLOBAL.localUri + "cache/links?page=" + owNetGLOBAL.encodedPageUri + "&gid=" + Math.floor((Math.random() * 1000) + 1), function () {
                        owNetGLOBAL.HighlightSwitch.receiveLinks(owNetAVAILABLEURIS); owNetGLOBAL.HighlightSwitch.switchOn();
                    }
                    );
                    return;
                }
                this.switchOn();
            }
            else {  /* switch off*/
                this.switchOff();
            }
        },
        switchOn: function () {
            for (var i = 0; i < document.links.length; ++i) {
                if (this.availableUris.owNetContains(document.links[i].href, owNetGLOBAL.urlEquals)) {
                    document.links[i].className += " owNetHighlight";
                    this.highlightedLinks[this.highlightedLinks.length] = document.links[i];
                }
            }
            this.isSwitchedOn = 1;
            this.switchObj.switchOn();
        },
        switchOff: function () {
            var reg = new RegExp("(\\s|^)" + "owNetHighlight" + "(\\s|$)");

            for (var i = 0; i < this.highlightedLinks.length; ++i) {
                this.highlightedLinks[i].className = this.highlightedLinks[i].className.replace(reg, "");
            }
            this.highlightedLinks = [];
            this.switchObj.switchOff();
            this.isSwitchedOn = 0;
        },
        assignSwitch: function (obj) {
            this.switchObj = obj;
            return function () {
                owNetGLOBAL.HighlightSwitch.doSwitch();
            };
        }
    },
    ProxyContact:
    {
        hasReportedPrimary: 0,
        hasReportedPrefetch: 0,
        hasReportedClose: 0,
        isRefreshed: 0,
        pageId: -1,
        reportPrimary: function () {
            try {
                if (this.hasReportedPrimary == 0) {
                    owNetGLOBAL.loadScript(owNetGLOBAL.prefetchUri + "visit?page=" + owNetGLOBAL.encodedPageUri + "&ref=" + owNetGLOBAL.encodedReferrerUri + "&gid=" + Math.floor((Math.random() * 1000) + 1), function () {
                        if (owNetPAGEID !== null)
                        {
                            owNetGLOBAL.ProxyContact.pageId = owNetPAGEID;
                        }
                    });
                    this.hasReportedPrimary = 1;
                }
            }
            catch (e) {
            }
        },
    	sendRandomLink : function() {
             owNetGLOBAL.loadScript(owNetGLOBAL.prefetchUri + "link?from=" + this.pageId + "&to=" + owNetGLOBAL.getRandomLink() + "&gid=" + Math.floor((Math.random() * 1000) + 1), function () {
	        	return true;
             });
	    },
 	    reportLink:function () {
            try {
                if (this.hasReportedPrimary == 1) {
		            this.sendRandomLink();
		            this.sendRandomLink();
		            this.sendRandomLink();
                }
            }
            catch (e) {

            }
        },
        reportClose: function () {
            try {
                if (this.hasReportedClose == 0) {
                    /*         owNetGLOBAL.loadScript(owNetGLOBAL.localUri + "cancel?page=" + owNetGLOBAL.encodedPageUri, function () {
                    return true;
                    });*/
                    owNetGLOBAL.ajaxGet(owNetGLOBAL.localUri + "close?page=" + this.pageId, function (xmlHttp) {
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
        },
        startRefresh: function () {
            owNetGLOBAL.loadScript(owNetGLOBAL.localUri + "refresh/start", function () {
                window.location.reload();
            });
        },
        stopRefresh: function () {
            try {
                this.stopRefreshTimeout();
                if (this.isRefreshed == 0) {
                    owNetGLOBAL.loadScript(owNetGLOBAL.localUri + "refresh/stop", function () {
                        return true;
                    });
                    this.isRefreshed = 1;
                }
            }
            catch (e) {
                throw e;
            }
        },
        refreshTimeout: 0,
        reportTimeout: 0,
        startRefreshTimeout: function () {
            this.refreshTimeout = setTimeout(this.stopRefresh, 1000 * 60 * 2);
        },
        stopRefreshTimeout: function () {
            if (this.refreshTimeout) clearTimeout(this.refreshTimeout);
        },
        startReportTimeout: function () {
            this.reportTimeout = setTimeout(this.reportLink, 1000 * 10);
        },
        stopReportTimeout: function () {
            if (this.reportTimeout) clearTimeout(this.reportTimeout);
        },
        reportPrefetch: function () {
            try {
                if (this.hasReportedPrefetch == 0) {
                   // var target = owNetGLOBAL.encodedReferrerUri;

                   // if (target === null || target.match(/\S/g) === null)
                    target = owNetGLOBAL.encodedPageUri;

                    owNetGLOBAL.loadScript(owNetGLOBAL.localUri + "done?page=" + target + "&gid=" + Math.floor((Math.random() * 1000) + 1), function () {
                        return true;
                    });
                    this.hasReportedPrefetch = 1;
                }
            }
            catch (e) {
            }
        },
    }

};



Array.prototype.owNetContains = function (item, comparer) {
    var compfunc = (comparer && typeof comparer === "function") ? comparer : null;
    if (item)
    {
        for (var i = 0; i < this.length; ++i)
        {
            if (compfunc != null)
            {
                if (compfunc(this[i], item)) 
                     return true;
            }
            else if (this[i] === item)
                return true;
        }
    }
    return false;
};


(function () {
    try 
    {
        if (top === self) 
        {
            if (document.referrer.match(/prefetch.ownet\/prefetch/) !== null) {

                if (document.addEventListener) {
                    document.addEventListener("DOMContentLoaded", function () {
                        document.removeEventListener("DOMContentLoaded", arguments.callee, false);
                        owNetGLOBAL.ProxyContact.reportPrefetch("proxy");
                    }, false);
                }
                else if (document.attachEvent) {
                    document.attachEvent("onreadystatechange", function () {
                        if (document.readyState === "complete") {
                            document.detachEvent("onreadystatechange", arguments.callee);
                            owNetGLOBAL.ProxyContact.reportPrefetch("proxy");
                        }
                    });
                }
            }
            else {

                owNetGLOBAL.ProxyContact.reportPrimary();

                if (document.addEventListener) {
                    document.addEventListener("DOMContentLoaded", function () {
                        document.removeEventListener("DOMContentLoaded", arguments.callee, false);
                        owNetGLOBAL.ProxyContact.reportLink();
                    }, false);
                }
                else if (document.attachEvent) {
                    document.attachEvent("onreadystatechange", function () {
                        if (document.readyState === "complete") {
                            document.detachEvent("onreadystatechange", arguments.callee);
                            owNetGLOBAL.ProxyContact.reportLink();
                        }
                    });
                }


                if (window.addEventListener) {
                    window.addEventListener("unload", function () {
                        owNetGLOBAL.ProxyContact.reportClose();
                    }, false);
                    window.addEventListener("beforeunload", function () {
                        owNetGLOBAL.ProxyContact.reportClose();
                    }, false);
                }
                else if (window.attachEvent) {
                    window.attachEvent("onunload", function () {
                        owNetGLOBAL.ProxyContact.reportClose();
                    });
                    window.attachEvent("onbeforeunload", function () {
                        owNetGLOBAL.ProxyContact.reportClose();
                    });
                }
            }
        }
    }
    catch (e) 
    {
        throw e;
    }
})();

