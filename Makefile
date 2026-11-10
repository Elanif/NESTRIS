CXX ?= g++
CXXFLAGS ?= -O2 -pipe
CXXFLAGS += -std=c++20 -Iinclude
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lpthread
SRCS = nestris.cpp $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp,build/%.o,$(SRCS))

nestris: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

-include $(OBJS:.o=.d)

clean:
	rm -rf build nestris
.PHONY: clean
