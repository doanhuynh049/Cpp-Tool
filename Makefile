CXX = g++				#Compiler C++
CXXFLAGS = -Wall -Wextra -std=c++11	
#enable additional warning and C++11 specifies C++11 standard
LDLIBS = -lgtest -lpthread #specifies the libraries to link against
# -lgtest links against the Google Test library
#-lgtest links against the Google Test library

PRO_DIR := .
INPUT_PATH 	:= $(PRO_DIR)/source
INCLUDE_PATH := $(PRO_DIR)/include
OUTPUT_PATH := $(PRO_DIR)/output

SOURCES := $(INPUT_PATH)/*.cpp $(INPUT_PATH)/*.cc
#$(INPUT_PATH)/factorial.cpp
#SOURCES := $(INPUT_PATH)/main.c $(INPUT_PATH)/edit.
OBJECTS := $(SOURCES:$(INPUT_PATH)/%.c=$(OUTPUT_PATH)/%.o)	# This creates a list of object files from the source files
EXECUTABLE  := $(OUTPUT_PATH)/results 						# The name of the executable

build: $(EXECUTABLE)
	./$(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
#specifies the target test.o and all its dependencies
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)	
$(OUTPUT_PATH)/%.o: $(INPUT_PATH)/%.c $(INCLUDE_PATH)/*.h	#specifies the target test.o and its dependencies.
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I $(INCLUDE_PATH)
clean:
	rm -f $(OUTPUT_PATH)/*

test: 
	${CC} ${INPUT_PATH}/factorial.cpp $(INCLUDE_PATH) 
