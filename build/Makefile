CXXFLAGS := -g -Wall -Werror -I.
CXX := g++
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LDFLAGS := -lGL -lsfml-window -lsfml-graphics -lsfml-system
else
LDFLAGS := -framework SFML -framework OpenGL -framework sfml-window \
           -framework sfml-graphics -framework sfml-system
endif


OBJS += $(patsubst %.cpp, %.o, $(wildcard SFR/*.cpp))
OBJS += $(patsubst %.cpp, %.o, $(wildcard Tests/*.cpp))


main: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

run: main
	./main

clean:
	rm -f main $(OBJS) 
