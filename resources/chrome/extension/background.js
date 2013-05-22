chrome.webRequest.onHeadersReceived.addListener(function(req) {

	var headers = req.responseHeaders

	if ( !headers )
		return

	for(var i = 0; i < headers.length; ++i) {

		if ( headers[i].name=='X-OwNet-CachedOn') {
			// console.log( 'X-OwNet-CachedOn: ' + headers[i].value )

			var cached = new Date(headers[i].value)
			var now = new Date()
			if ( (now - cached)/1000 > 5 ) {
				chrome.tabs.onUpdated.addListener( function showOwnetIcon(tabId, changeInfo, tab) {
					if (tabId == req.tabId) {
						chrome.pageAction.show(tabId)
						chrome.tabs.onUpdated.removeListener(showOwnetIcon)
					}
				})
			}

			return
		}
	}

},{	urls:['http://*/*'], types:['main_frame'] }, ['responseHeaders'])
