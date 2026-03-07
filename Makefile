.PHONY: configure debug release clean format

configure:
	cmake --preset debug
	cmake --preset release

debug:
	@[ -d build/debug ] || cmake --preset debug
	cmake --build --preset debug && ./build/debug/game-of-life

release:
	@[ -d build/release ] || cmake --preset release
	cmake --build --preset release && ./build/release/game-of-life

format:
	find src -name '*.cpp' -o -name '*.hpp' -o -name '*.h' | xargs clang-format -i

clean:
	rm -rf build
