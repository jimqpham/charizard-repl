# TODO: Add folder paths containing .cpp files if necessary
FOLDERS = CmdLines/ Expressions/
# TODO: Add Testing folder path if necessary
TEST_FOLDER = Testing/

CXX = c++
CXXFLAGS = --std=c++14 -O2
COMPILE = $(CXX) $(CXXFLAGS)

# Create .o file-names from .cpp files for compiling & linking
OBJS := $(foreach FOLDER, $(FOLDERS), $(foreach OBJ, $(wildcard $(FOLDER)*.cpp), $(OBJ:.cpp=.o)))
# Get the testing files
TESTING_FILES = $(wildcard $(TEST_FOLDER)*.h)

msdscript: main.o $(OBJS)
	$(COMPILE) -o $@ main.o $(OBJS)

main.o: main.cpp
	$(COMPILE) -c $< -o $@

$(OBJS): %.o: %.cpp %.h $(TESTING_FILES)
	$(COMPILE) -c $< -o $@

# Clean all the .o files - It won't run automatically
clean:
	rm */*.o
