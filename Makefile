all:
	cd src && $(MAKE) && cd ..
	cd test && $(MAKE) && cd ..
clean:
	cd src && $(MAKE) clean && cd ..
	cd test && $(MAKE) clean && cd ..
