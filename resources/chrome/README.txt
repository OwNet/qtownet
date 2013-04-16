Linux:
	staci spustit start-ownet-chrome.sh

Windows:
	Vytvorte si shortcut a ako target dajte toto ( <...> treba nahradit ako mate u seba absolutne cesty):
	"<Path to chrome.exe>" --proxy-server="http=127.0.0.1:8081" --user-data-dir="<current dir>\user-data-dir"


Ked sa vam bude pustat chrome s proxy tak treba este zapnut tu extension.
1. V Chrome chodte do settings -> extensions
2. Zaskrtnite developer mode (v pravo hore)
3. Dajte load unpacket extension a dajte tam adresar extension

Ked sa zmeni inject.js je potrebne ist do settings/extensions a pri Ownet dat reload.