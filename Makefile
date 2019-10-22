CXX = clang++
CFLAGS = -g -Wall -Wextra -pedantic -std=c++17
DEPS = Date.h
TARGET = ddiff
OBJ = $(TARGET).o Date.cpp

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
