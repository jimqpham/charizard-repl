# TODO: Add folder paths containing .cpp files if necessary
FOLDERS = CmdLines/ Expressions/

CXX = c++
CXXFLAGS = --std=c++14 -O2
COMPILE = $(CXX) $(CXXFLAGS)

# Create .o file-names from .cpp files for compiling & linking
OBJS := $(foreach FOLDER, $(FOLDERS), $(foreach OBJ, $(wildcard $(FOLDER)*.cpp), $(OBJ:.cpp=.o)))
# Get the testing files
TESTING_LIB = Testing/catch.h

# TODO: Modify the main & testing files if necessary
msdscript: main.o $(OBJS) Testing/tests.o
	$(COMPILE) -o $@ $^

main.o: main.cpp
	$(COMPILE) -c $< -o $@

$(OBJS): %.o: %.cpp %.h $(TESTING_LIB)
	$(COMPILE) -c $< -o $@

# TODO: Modify if necessary 
Testing/tests.o: Testing/tests.cpp $(TESTING_LIB)
	$(COMPILE) -c $< -o $@

# Clean all the .o files - It won't run automatically
clean:
	rm */*.o
