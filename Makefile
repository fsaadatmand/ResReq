CXX = g++
CFLAGS = -g -Wall -Wextra -pedantic -std=c++17 -lboost_date_time
DEPS = ddiff.h
TARGET = ddiff
OBJ = $(TARGET).o ddiff_functions.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
