CXXFLAGS := -O3 -Wall -Werror -I../include
CXX := clang++
UNAME := $(shell uname)


ifeq ($(UNAME), Linux)
LDFLAGS := -lGL -lsfml-window -lsfml-graphics -lsfml-system
else
LDFLAGS := -framework SFML -framework OpenGL -framework sfml-window \
           -framework sfml-graphics -framework sfml-system
endif


OBJS += $(patsubst %.cpp, %.o, $(wildcard ../src/*.cpp))
OBJS += $(patsubst %.cpp, %.o, $(wildcard ../tests/*.cpp))


../main: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f ../main $(OBJS) 
