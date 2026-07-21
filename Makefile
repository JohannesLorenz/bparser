BUILDDIR ?= build

all: build

build:
	mkdir -p $(BUILDDIR)
	cd src && $(MAKE) && cd ..
	cd test && $(MAKE) test && cd ..
	cp -f src/libbparser.so test/test $(BUILDDIR)/

run: build
	cd test && $(MAKE) run && cd ..

clean:
	cd src && $(MAKE) clean && cd ..
	cd test && $(MAKE) clean && cd ..
	rm -rf $(BUILDDIR)

.PHONY: all build run clean doc
doc:
	cd doc && $(MAKE) all && cd ..
doc-clean:
	cd doc && $(MAKE) clean && cd ..

