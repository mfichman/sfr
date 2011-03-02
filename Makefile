CXX := clang
CXXFLAGS := -g -Wall -Werror -I.
LDFLAGS := -framework SFML -framework OpenGL -framework sfml-window \
    -framework sfml-graphics -framework sfml-system
OBJS += $(patsubst %.cpp, %.o, $(wildcard sfr/*.cpp))
OBJS += $(patsubst %.cpp, %.o, $(wildcard tests/*.cpp))


binaries/main: $(OBJS)
	g++ $(LDFLAGS) -o $@ $^

run: binaries/main
	binaries/main

clean:
	rm -f main $(OBJS) 
