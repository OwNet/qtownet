if ( ! (/\.ownet$/).test(location.hostname) && top===self) {
	var s = document.createElement('script');
	s.src = chrome.extension.getURL("inject.js");
	s.onload = function() {
		this.parentNode.removeChild(this);
	};
	(document.head||document.documentElement).appendChild(s);
}
