all:
	cd src && $(MAKE) && cd ..
	cd test && $(MAKE) && cd ..
clean:
	cd src && $(MAKE) clean && cd ..
	cd test && $(MAKE) clean && cd ..
.PHONY: doc
doc:
	cd doc && $(MAKE) all && cd ..
doc-clean:
	cd doc && $(MAKE) clean && cd ..

