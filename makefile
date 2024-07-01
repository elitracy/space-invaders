all:
	cmake -B build
	cmake --build build

run: 
	./build/space_invaders

clean:
	rm -rf ./build
