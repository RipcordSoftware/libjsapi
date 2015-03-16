build: force_true .jsapi
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build

all: force_true .jsapi
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all

clean: force_true
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean

.autoconf-2.13:
	if [ ! -d externals/autoconf-2.13 ]; then \
		mkdir -p externals && \
		cd externals && \
		mkdir -p installed && \
		curl http://ftp.gnu.org/gnu/autoconf/autoconf-2.13.tar.gz -O && \
		tar xvfz autoconf-2.13.tar.gz && \
		cd autoconf-2.13 && \
		./configure --prefix=${PWD}/externals/installed && \
		make && \
		make install; \
	fi

.jsapi: .autoconf-2.13 force_true
	if [ ! -d externals/mozjs-31.5.0 ]; then \
		mkdir -p externals && \
		cd externals && \
		mkdir -p installed && \
		if [ ! -f mozjs-31.5.0.tar.bz2 ]; then curl https://people.mozilla.org/~sstangl/mozjs-31.5.0.tar.bz2 -O; fi && \
		tar xvfj mozjs-31.5.0.tar.bz2 && \
		cd mozjs-31.5.0 && \
		cd js/src && \
		mkdir build_OPT.OBJ && \
		cd build_OPT.OBJ && \
		export PATH=${PWD}/externals/installed/bin:${PATH} && \
		../configure --prefix=${PWD}/externals/installed --disable-shared-js && \
		make -j 2 && \
		make install; \
	fi

force_true:
	true
