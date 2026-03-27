CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lUnitTest++
TARGET = route_cipher_tests
SOURCES = routeCipher.cpp tests_route.cpp
OBJS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@echo "=== ТЕСТИРОВАНИЕ ROUTECIPHER ==="
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: run clean
