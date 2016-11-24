SHELL:=/bin/bash

GTEST_VER:=1.7.0
MOZJS_VER:=45.0.2
MOZJS_VER_SUFFIX:=

MOZJS_CONFIG_FLAGS:=--disable-jemalloc --disable-shared-js --disable-tests --enable-install-strip --without-intl-api

EXTERNALS:=$(CURDIR)/externals
INSTALLED:=$(EXTERNALS)/installed

.PHONY: build all test clean .jsapi .googletest
.NOTPARALLEL: test

build all test: .jsapi .googletest
	cd src/libjsapi && $(MAKE) $@

clean:
	cd src/libjsapi && $(MAKE) $@

.jsapi:
	if [ "${CI}" = "true" ]; then \
		curl ftp://ftp.ripcordsoftware.com/pub/mozjs-$(MOZJS_VER)-travis-ci-externals-installed.tar.xz -O && \
		tar xfJ mozjs-*; \
	elif [ ! -d externals/mozjs-$(MOZJS_VER) ]; then \
		mkdir -p $(EXTERNALS) && cd $(EXTERNALS) && \
		mkdir -p $(INSTALLED) && \
		if [ ! -f mozjs-$(MOZJS_VER).tar.bz2 ]; then curl -L https://people-mozilla.org/~sfink/mozjs-$(MOZJS_VER)$(MOZJS_VER_SUFFIX).tar.bz2 -o mozjs-$(MOZJS_VER).tar.bz2; fi && \
		mkdir -p mozjs-$(MOZJS_VER) && cd mozjs-$(MOZJS_VER) && \
		tar xfj ../mozjs-$(MOZJS_VER).tar.bz2 --strip-components=1 && \
		cd js/src && \
		mkdir build_OPT.OBJ && \
		cd build_OPT.OBJ && \
		../configure --prefix=$(INSTALLED) $(MOZJS_CONFIG_FLAGS) && \
		$(MAKE) && \
		$(MAKE) install && \
		cp -f dist/sdk/lib/libmozglue.a $(INSTALLED)/lib && \
		pushd $(INSTALLED)/include && ((test -L mozjs && rm mozjs) || true) && ln -s mozjs-?? mozjs && popd; \
	fi

.googletest:
	if [ "${CI}" = "true" ]; then \
		curl ftp://ftp.ripcordsoftware.com/pub/gtest-$(GTEST_VER)-travis-ci-externals-installed.tar.xz -O && \
		tar xfJ gtest-*; \
	elif [ ! -d externals/gtest-$(GTEST_VER)/lib/.libs ]; then \
		mkdir -p $(EXTERNALS) && cd $(EXTERNALS) && \
		mkdir -p $(INSTALLED) && \
		if [ ! -f gtest-$(GTEST_VER).zip ]; then curl ftp://ftp.ripcordsoftware.com/pub/gtest-$(GTEST_VER).zip -O; fi && \
		unzip gtest-$(GTEST_VER).zip && \
		cd gtest-$(GTEST_VER) && \
		./configure && \
		$(MAKE) && \
		if [ ! -d "../installed/include" ]; then mkdir -p ../installed/include; fi && \
		if [ ! -d "../installed/lib" ]; then mkdir -p ../installed/lib; fi && \
		cp -Rf include/* ../installed/include && \
		cp -Rf lib/.libs/* ../installed/lib; \
	fi

