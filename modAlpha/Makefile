CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lUnitTest++
TARGET = alpha_tests
SOURCES = modAlphaCipher.cpp tests_alpha.cpp
OBJS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@echo "=== ТЕСТИРОВАНИЕ MODALPHACIPHER ==="
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: run clean
