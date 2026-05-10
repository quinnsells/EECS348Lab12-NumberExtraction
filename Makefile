CXX     = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET  = NumberExtractor
SRC     = extractNumber.cpp
 
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)
 
clean:
	rm -f $(TARGET)