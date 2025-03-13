# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O2

# Исходные файлы
SRC = main.cpp
SRCTESTS = test.cpp

# Объектные файлы
OBJ = $(SRC:.cpp=.o)
OBJTESTS = $(SRCTESTS:.cpp=.o)

# Целевые исполняемые файлы
TARGET = differentiator
TARGETTESTS = test.exe

# Правило по умолчанию
all: $(TARGET) $(TARGETTESTS)

# Сборка основной программы
$(TARGET): $(OBJ)
	$(CXX) $^ -o $@
	@echo "Build complete: $(TARGET)"

# Сборка тестов
$(TARGETTESTS): $(OBJTESTS)
	$(CXX) $^ -o $@
	@echo "Build complete: $(TARGETTESTS)"

# Компиляция исходных файлов в объектные
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Запуск тестов
.PHONY: test
test: $(TARGETTESTS)
	./$(TARGETTESTS)

# Очистка скомпилированных файлов
.PHONY: clean
clean:
	rm -f *.o $(TARGET) $(TARGETTESTS)
	@echo "Cleaned up all generated files."

# Удаление промежуточных файлов после сборки
.PHONY: all
all: $(TARGET) $(TARGETTESTS)
	@rm -f $(OBJ) $(OBJTESTS)
	@echo "Removed intermediate object files."
