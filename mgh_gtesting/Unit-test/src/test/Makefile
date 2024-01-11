CCX = g++
GTEST_PATH=/usr/src/gtest

GTEST_LIB=$(GTEST_PATH)/lib
GTEST_INC=$(GTEST_PATH)/include

SRCS_DIR = ./..
#SRCS = $(SRCS_DIR)/bubbleSort.cpp  \
        bubbleSortTest.cpp

SRCS = $(wildcard *.cpp) $(wildcard $(SRCS_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -L$(GTEST_LIB) -lgtest -lgtest_main -lpthread

INCS = -I$(SRCS_DIR)
INCS += -I$(GTEST_INC)
INCS += -Wno-deprecated

TARGET = outputTest
REPORT = $(TARGET).xml

.PHONY: ut
ut: $(TARGET)
	@echo 'Run test'
	@-./$^ --gtest_output='xml:./$(REPORT)'

$(TARGET): $(OBJS)
	$(CXX) $^ $(CXXFLAGS) $(INCS) -o $(TARGET)

#$(SRCS_DIR)/%.o:$(SRCS_DIR)/%.cpp
#	$(CXX) -c $< -o $@

%.o:%.cpp
	$(CXX) -c $< $(CXXFLAGS) $(INCS) -o $@

.PHONY: clean
clean:
	rm $(TARGET) $(REPORT) *.o

