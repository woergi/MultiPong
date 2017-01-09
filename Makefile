
all:
	+$(MAKE) -C src
	mv src/multipong .

clean:
	+$(MAKE) -C src clean

