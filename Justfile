build:
    cmake -B build
    cmake --build build

run: build
    ./build/gengine-bin

clean:
    rm -rf build/
    rm -f gengine-bin