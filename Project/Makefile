CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra -pedantic -Weffc++ -Wold-style-cast
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SOURCES = main.cpp game.cpp object.cpp
TEST_SOURCES =  Test/test_main.o Test/testscript.cpp Test/main_test.cpp game.cpp Test/avatar_test.cpp
TARGET = play



main:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

memory_main:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)
	valgrind --suppressions=./suppressions.txt ./$(TARGET)
	rm -f $(TARGET)

test_file:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TEST_SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)

