SHELL:=bash

GTEST_VER:=1.7.0
MOZJS_VER:=50.1.0
MOZJS_VER_SUFFIX:=

MOZJS_CONFIG_FLAGS:=--disable-jemalloc --disable-shared-js --disable-tests --enable-install-strip --without-intl-api

CACHE_PATH:=~/Downloads

EXTERNALS:=$(CURDIR)/externals
INSTALLED:=$(EXTERNALS)/installed
INSTALLED_INC:=$(INSTALLED)/include
INSTALLED_LIB:=$(INSTALLED)/lib
INSTALLED_DIRS:=$(EXTERNALS) $(INSTALLED) $(INSTALLED_INC) $(INSTALLED_LIB)

MOZJS_ARCHIVE_NAME:=mozjs-$(MOZJS_VER).tar.bz2
MOZJS_ARCHIVE_PATH:=$(EXTERNALS)/$(MOZJS_ARCHIVE_NAME)
MOZJS_SOURCE_PATH:=$(EXTERNALS)/mozjs-$(MOZJS_VER)
MOZJS_BUILD_PATH:=$(MOZJS_SOURCE_PATH)/js/src/build_OPT.OBJ
MOZJS_H:=$(INSTALLED_INC)/mozjs/jsapi.h
MOZJS_LIB:=$(INSTALLED_LIB)/libjs_static.ajs

GTEST_ARCHIVE_NAME:=gtest-$(GTEST_VER).zip
GTEST_ARCHIVE_PATH:=$(EXTERNALS)/$(GTEST_ARCHIVE_NAME)
GTEST_H:=$(INSTALLED_INC)/gtest/gtest.h
GTEST_LIB:=$(INSTALLED_LIB)/libgtest.a

.PHONY: build all test clean .jsapi .googletest
.PRECIOUS: $(MOZJS_ARCHIVE_PATH) $(GTEST_ARCHIVE_PATH) $(MOZJS_LIB) $(GTEST_LIB)
.NOTPARALLEL: test

build all test: .jsapi .googletest
	cd src/libjsapi && $(MAKE) $@

clean:
	cd src/libjsapi && $(MAKE) $@

.jsapi: $(MOZJS_LIB)

.googletest: $(GTEST_LIB)

$(MOZJS_LIB): $(MOZJS_ARCHIVE_PATH)
	mkdir -p $(INSTALLED_DIRS) && \
	cd $(MOZJS_BUILD_PATH) && \
	$(MAKE) MAKEOVERRIDES= && \
	$(MAKE) install MAKEOVERRIDES= && \
	cp -fp dist/sdk/lib/libmozglue.a $(INSTALLED_LIB) && \
	cd $(INSTALLED_INC) && \
	((test -L mozjs && rm mozjs) || true) && ln -s mozjs-?? mozjs

$(MOZJS_ARCHIVE_PATH):
	mkdir -p $(INSTALLED_DIRS) && \
	cd $(EXTERNALS) && \
	(test -f $(CACHE_PATH)/$(MOZJS_ARCHIVE_NAME) && cp -fp $(CACHE_PATH)/$(MOZJS_ARCHIVE_NAME) .) || curl ftp://ftp.ripcordsoftware.com/pub/mozjs-$(MOZJS_VER)$(MOZJS_VER_SUFFIX).tar.bz2 -o $(MOZJS_ARCHIVE_NAME) && \
	mkdir -p $(MOZJS_SOURCE_PATH) && cd $(MOZJS_SOURCE_PATH) && \
	tar xfj ../$(MOZJS_ARCHIVE_NAME) --strip-components=1 && \
	mkdir -p $(MOZJS_BUILD_PATH) && cd $(MOZJS_BUILD_PATH) && \
	CC="$(CC)" CXX="$(CXX)" ../configure --prefix=$(INSTALLED) $(MOZJS_CONFIG_FLAGS)

$(GTEST_LIB): $(GTEST_ARCHIVE_PATH)
	mkdir -p $(INSTALLED_DIRS) && \
	cd $(EXTERNALS)/gtest-$(GTEST_VER) && \
	$(MAKE) MAKEOVERRIDES= && \
	cp -Rfp include/* $(INSTALLED_INC) && \
	cp -Rfp lib/.libs/* $(INSTALLED_LIB)

$(GTEST_ARCHIVE_PATH):
	mkdir -p $(INSTALLED_DIRS) && \
	cd $(EXTERNALS) && \
	(test -f $(CACHE_PATH)/$(GTEST_ARCHIVE_NAME) && cp -fp $(CACHE_PATH)/$(GTEST_ARCHIVE_NAME) .) || curl ftp://ftp.ripcordsoftware.com/pub/gtest-$(GTEST_VER).zip -o $(GTEST_ARCHIVE_NAME) && \
	unzip -o $(GTEST_ARCHIVE_NAME) && \
	cd gtest-$(GTEST_VER) && \
	CC="$(CC)" CXX="$(CXX)" ./configure
