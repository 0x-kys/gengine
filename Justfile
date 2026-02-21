build:
    g++ -std=c++20 src/*.cc glad/src/glad.c -o gengine-bin \
    -I./include \
    -I./glad/include \
    -lglfw -lGL -ldl -lpthread -lX11

run: build
    ./gengine-bin

clean:
    rm gengine-bin