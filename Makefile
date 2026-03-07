.PHONY: configure debug release clean

configure:
	cmake --preset debug
	cmake --preset release

debug:
	cmake --build --preset debug && ./build/debug/game-of-life

release:
	cmake --build --preset release && ./build/release/game-of-life

clean:
	rm -rf build
