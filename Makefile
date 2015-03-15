build: force_true
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) build

all: force_true
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) all

clean: force_true
	cd src/libjsapi && $(MAKE) $(MFLAGS) $(MAKEOVERRIDES) clean

force_true:
	true
