CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O2

SRC = main.cpp
SRCTESTS = test.cpp

OBJ = $(SRC:.cpp=.o)
OBJTESTS = $(SRCTESTS:.cpp=.o)

TARGET = differentiator
TARGETTESTS = test.exe

all: $(TARGET) $(TARGETTESTS)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@
	@echo "Build complete: $(TARGET)"

$(TARGETTESTS): $(OBJTESTS)
	$(CXX) $^ -o $@
	@echo "Build complete: $(TARGETTESTS)"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: test
test: $(TARGETTESTS)
	./$(TARGETTESTS)

.PHONY: clean
clean:
	rm -f *.o $(TARGET) $(TARGETTESTS)
	@echo "Cleaned up all generated files."

.PHONY: all
all: $(TARGET) $(TARGETTESTS)
	@rm -f $(OBJ) $(OBJTESTS)
	@echo "Removed intermediate object files."
