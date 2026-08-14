BUILDDIR ?= build

all: build

build:
	mkdir -p $(BUILDDIR)
	cd src && $(MAKE) && cd ..
	cd test && $(MAKE) test && cd ..
	$(MAKE) $(BUILDDIR)/libbparser.so $(BUILDDIR)/test

$(BUILDDIR)/libbparser.so: src/libbparser.so
	cp -f src/libbparser.so $(BUILDDIR)/
$(BUILDDIR)/test: test/test
	cp -f test/test $(BUILDDIR)/

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

