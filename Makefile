cc = c++
flags = -std=c++11 -Wall -O0
files = source/*.cpp
library = library/
pkgconf = pkg-config --cflags --libs Magick++
command = $(cc) $(files) -I $(library) -L $(library) $(flags) `$(pkgconf)` -o bin/szumovski

.PHONY : soft_clean all

all:
	$(command)
clean:
	rm -rf bin
	mkdir bin
