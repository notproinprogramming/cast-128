CXX = g++
CXXFLAGS = -Wall -std=c++17

OBJS_COMMON = cast-128.o s-boxes.o fileutil.o

all: prog test

prog: $(OBJS_COMMON) prog.o
	$(CXX) $(CXXFLAGS) -o prog $(OBJS_COMMON) prog.o

test: $(OBJS_COMMON) test.o
	$(CXX) $(CXXFLAGS) -o test $(OBJS_COMMON) test.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o prog test
