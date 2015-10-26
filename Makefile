all:
	cd src && make && cd ..
	cd test && make && cd ..
clean:
	cd src && make clean && cd ..
	cd test && make clean && cd ..