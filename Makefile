GTEST_VER=1.7.0
MOZJS_VER=38.2.1
MOZJS_VER_SUFFIX=.rc0

build: force_true .jsapi .googletest
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build

all: force_true .jsapi .googletest
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all

test: force_true .jsapi .googletest
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) test

clean: force_true
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean

.autoconf-2.13: force_true
	if [ "${CI}" = "true" ]; then \
		curl ftp://ftp.ripcordsoftware.com/pub/autoconf-2.13-travis-ci-externals-installed.tar.xz -O && \
		tar xfJ autoconf-*; \
	elif [ ! -d externals/autoconf-2.13 ]; then \
		mkdir -p externals && \
		cd externals && \
		mkdir -p installed && \
		curl http://ftp.gnu.org/gnu/autoconf/autoconf-2.13.tar.gz -O && \
		tar xfz autoconf-2.13.tar.gz && \
		cd autoconf-2.13 && \
		./configure --prefix=${PWD}/externals/installed && \
		make && \
		make install; \
	fi

.jsapi: .autoconf-2.13 force_true
	if [ "${CI}" = "true" ]; then \
		curl ftp://ftp.ripcordsoftware.com/pub/mozjs-${MOZJS_VER}-travis-ci-externals-installed.tar.xz -O && \
		tar xfJ mozjs-*; \
	elif [ ! -d externals/mozjs-${MOZJS_VER} ]; then \
		mkdir -p externals && cd externals && \
		mkdir -p installed && \
		if [ ! -f mozjs-${MOZJS_VER}.tar.bz2 ]; then curl https://people.mozilla.org/~sstangl/mozjs-${MOZJS_VER}${MOZJS_VER_SUFFIX}.tar.bz2 -o mozjs-${MOZJS_VER}.tar.bz2; fi && \
		mkdir -p mozjs-${MOZJS_VER} && cd mozjs-${MOZJS_VER} && \
		tar xfj ../mozjs-${MOZJS_VER}.tar.bz2 --strip-components=1 && \
		cd js/src && \
		mkdir build_OPT.OBJ && \
		cd build_OPT.OBJ && \
		export PATH=${PWD}/externals/installed/bin:${PATH} && \
		../configure --prefix=${PWD}/externals/installed --disable-shared-js --disable-tests --enable-exact-rooting && \
		make -j 2 && \
		make install; \
	fi

.googletest: force_true
		if [ "${CI}" = "true" ]; then \
			curl ftp://ftp.ripcordsoftware.com/pub/gtest-${GTEST_VER}-travis-ci-externals-installed.tar.xz -O && \
			tar xfJ gtest-*; \
		elif [ ! -d externals/gtest-${GTEST_VER}/lib/.libs ]; then \
		mkdir -p externals && \
		cd externals && \
		if [ ! -f gtest-${GTEST_VER}.zip ]; then curl https://googletest.googlecode.com/files/gtest-${GTEST_VER}.zip -O; fi && \
		unzip gtest-${GTEST_VER}.zip && \
		cd gtest-${GTEST_VER} && \
		./configure && \
		make -j 2 && \
                if [ ! -d "../installed/include" ]; then mkdir -p ../installed/include; fi && \
                if [ ! -d "../installed/lib" ]; then mkdir -p ../installed/lib; fi && \
		cp -Rf include/* ../installed/include && \
		cp -Rf lib/.libs/* ../installed/lib; \
	fi

force_true:
	true
