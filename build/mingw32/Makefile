
INCLUDE=-I/c/Source/yacg/include
CFLAGS=-g3 -Wall -pedantic -ftabstop=4 -DALLEGRO_STATICLINK -D_DEBUG -DDEBUGMODE=1
LDFLAGS=-Wl,--subsystem,console -lalld_s -lgdi32 -lwinmm -ldinput -lole32 -lddraw -ldsound -ldxguid -lstdc++

all: precompiled test

clean:
	-rm src/*.o 2>/dev/null
	-rm test/*.o 2>/dev/null
	-rm test/*.exe 2>/dev/null

distclean: clean
	find . -name '*.gch' -exec rm {} \;

src/Box.o: src/Box.cpp include/yacg/Box.hpp
	gcc $(CFLAGS) $(INCLUDE) -c src/Box.cpp -o src/Box.o

src/yacg.o: src/yacg.cpp
	gcc $(CFLAGS) $(INCLUDE) -c src/yacg.cpp -o src/yacg.o

src/Plane.o: src/Plane.cpp include/yacg/Plane.hpp
	gcc $(CFLAGS) $(INCLUDE) -c src/Plane.cpp -o src/Plane.o

src/Theme.o: src/Theme.cpp src/utility.hpp src/dump.hpp include/yacg/Theme.hpp \
		include/yacg/Theme.inl src/ThemeImpl.hpp
	gcc $(CFLAGS) $(INCLUDE) -c src/Theme.cpp -o src/Theme.o

src/Window.o: src/Window.cpp include/yacg/Window.hpp
	gcc $(CFLAGS) $(INCLUDE) -c src/Window.cpp -o src/Window.o

test/Main.o: test/Main.cpp include/yacg/Theme.hpp
	gcc $(CFLAGS) $(INCLUDE) -c test/Main.cpp -o test/Main.o

test: test/test.exe 

test/test.exe: test/Main.o src/yacg.o src/Plane.o src/Theme.o src/Box.o src/Window.o src/Plane.o
	gcc $(CFLAGS) -o test/test.exe test/Main.o src/yacg.o src/Theme.o src/Box.o \
			src/Window.o src/Plane.o $(LDFLAGS) -Wl,-E



# Precompiled header output

precompiled: src/precompiled.hpp.gch

src/precompiled.hpp.gch: src/precompiled.hpp.gch
	gcc $(CFLAGS) $(INCLUDE) -x c++ -c src/precompiled.hpp -o src/precompiled.hpp.gch

# Preprocess and assembly output

src/yacg.i:
	gcc $(CFLAGS) $(INCLUDE) -E src/yacg.cpp -o src/yacg.i

src/yacg.s:
	gcc $(CFLAGS) $(INCLUDE) -S src/yacg.cpp -o src/yacg.s

test/Main.i:
	gcc $(CFLAGS) $(INCLUDE) -E test/Main.cpp -o test/Main.i

test/Main.s:
	gcc $(CFLAGS) $(INCLUDE) -S test/Main.cpp -o test/Main.s

src/Theme.i: src/Theme.cpp
	gcc $(CFLAGS) $(INCLUDE) -E src/Theme.cpp -o src/Theme.i

src/Theme.s:
	gcc $(CFLAGS) $(INCLUDE) -S src/Theme.cpp -o src/Theme.s



browse:
	ebrowse include/yacg.hpp include/yacg/Theme.hpp include/yacg/Theme.inl \
		include/yacg/Box.hpp include/yacg/BoxTheme.hpp \
		test/Main.cpp \
		src/yacg.cpp src/Box.cpp src/Theme.cpp src/ThemeImpl.hpp \
		src/dump.hpp src/utility.hpp src/precompiled.hpp
