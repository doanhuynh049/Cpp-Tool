CXX = g++

SRC_DIR = ./src
INCS_DIR = $(SRC_DIR)

SRCS = $(wildcard *.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

TARGET = output

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $< -o $@

%.o:%.cpp
	$(CXX) -c $< -o $@ -I./$(INCS_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET)

