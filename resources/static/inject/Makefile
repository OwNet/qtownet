SHELL := bash
OS := $(shell uname)
BIN := ../../../tools


ifeq ("$(wildcard js/share)","")
ifneq ($(OS), Windows_NT)
$(shell ln -s ../../share js/share)
else
$(error  Please run `mklink /D "js\share" "..\..\share"` as administrator first)
endif
endif


ifeq ($(OS), Windows_NT)
FIND := find2
else
FIND := find
endif


JADE := node $(BIN)/jade/bin/jade
JADE_FLAGS := --space-delim
JADE_CL_FLAGS :=  $(JADE_FLAGS)  --client  --client-amd
JADE_FILES := $(shell $(FIND) templates -type f -name "*.jade")
JADE_CLIENT_FILES := $(shell $(FIND) templates-client -type f -name "*.jade")
TMP := $(JADE_CLIENT_FILES:.jade=.js)
JADE_CLIENT_FILES_JS :=  $(patsubst templates-client/%,js/tpl/%,$(TMP))


JS_CHECK = node $(BIN)/jscheck/jscheck
JS_CHECK_FILES := $(shell $(FIND) js -type f -name "*.js" -and -not -name "app.min.js" -and -not -path "js/tpl/*"  -and -cnewer Makefile)


SASS := sass
SASS_FILES := $(shell $(FIND) style js/share/style -type f -name "*.sass" -o -name "*.scss")


RJS := node $(BIN)/requirejs/bin/r.js


ifeq ($(RELEASE), 1)
JADE_FLAGS :=  $(JADE_FLAGS)  --obj '{"config":{"debug":false}}'
JADE_CL_FLAGS := $(JADE_CL_FLAGS)  --no-debug
SASS_FLAGS :=  $(SASS_FLAGS)  -t compressed
else
JADE_FLAGS :=  $(JADE_FLAGS)  --obj '{"config":{"debug":true}}'  --pretty  --error-format '%f:%l: Error: %m'
JADE_CL_FLAGS := $(JADE_CL_FLAGS)  --pretty
SASS_FLAGS :=  $(SASS_FLAGS)  --debug-info
# SASS_ERR_FORMATTER :=  2>&1 | sed '$$d' | tr -d '\n' | sed 's/^\(.*\)\s*on line \([0-9]\+\) of \([^ ,]\+\).*/\3:\2: \1\n/' 1>&2; exit $${PIPESTATUS[0]};
SILENT :=  >/dev/null
endif



all: index.html  css/style.css  js/app.min.js  $(JS_CHECK_FILES)  $(JADE_CLIENT_FILES_JS)
	@ touch Makefile
ifdef OUT
	@ mkdir -p "$(OUT)"  "$(OUT)/css"  "$(OUT)/js"  "$(OUT)/img"
	@ cp index.html "$(OUT)"
	@ cp -r css "$(OUT)/"
	@ cp -r img "$(OUT)/"

ifeq ($(RELEASE), 1)
	@ cp js/app.min.js  "$(OUT)/js/"
else
	@ cp -r js  "$(OUT)/"
endif

endif


index.html: $(JADE_FILES)
	@ # echo "build index.html"
	@ $(JADE) $(JADE_FLAGS)  --out . templates/index.jade $(SILENT)


css/style.css: $(SASS_FILES)
	@ # echo "build style.css"
ifeq ("$(wildcard css)","")
	@ mkdir css
endif
	@ sass $(SASS_FLAGS) style/style.sass:css/style.css $(SASS_ERR_FORMATTER)


$(JADE_CLIENT_FILES_JS): $(JADE_CLIENT_FILES)
	@ # echo "build templates-client"
	@ $(JADE) $(JADE_CL_FLAGS)  --out js/tpl  templates-client $(SILENT)


js/app.min.js: $(JS_CHECK_FILES) $(JADE_CLIENT_FILES_JS)
ifeq ($(RELEASE), 1)
	@ # echo "build js/app.min.js"
	@ $(RJS) -o app.build.js
endif



$(JS_CHECK_FILES): FORCE
	@ $(JS_CHECK) "$@"


clean:
	@ rm -f index.html css/style.css js/app.min.js
	@ rm -rf js/tpl release



.PHONY: clean

FORCE:
