# TODO: Add folder paths containing .cpp files if necessary
FOLDERS = CmdLines/ Expressions/ Vals/ Utils/
TEST_FOLDER = Testing/

CXX = c++
CXXFLAGS = --std=c++14 -O2
COMPILE = $(CXX) $(CXXFLAGS)

# Create .o file-names from .cpp files for compiling & linking
OBJS := $(foreach FOLDER, $(FOLDERS), $(foreach OBJ, $(wildcard $(FOLDER)*.cpp), $(OBJ:.cpp=.o)))
# Get the testing files
TESTING_LIB = Testing/catch.h
# Create .o file names from .cpp test files
TEST_OBJS := $(foreach OBJ, $(wildcard $(TEST_FOLDER)*.cpp), $(OBJ:.cpp=.o))

# TODO: Modify the main & testing files if necessary
msdscript: main.o $(OBJS) $(TEST_OBJS)
	$(COMPILE) -o $@ $^

main.o: main.cpp
	$(COMPILE) -c $< -o $@

$(OBJS): %.o: %.cpp %.h $(TESTING_LIB) shared_ptr.h
	$(COMPILE) -c $< -o $@

$(TEST_OBJS): %.o: %.cpp $(TESTING_LIB) $(OBJS) shared_ptr.h
	$(COMPILE) -c $< -o $@

clean:
	rm *.o
	rm */*.o

test: msdscript
	./msdscript --test

# Random Testing Script
test_msdscript: RandomTesting/test_msdscript.o RandomTesting/exec.o
	$(CXX) -o $@ $^

RandomTesting/test_msdscript.o: RandomTesting/test_msdscript.cpp RandomTesting/exec.o
	$(CXX) -c $< -o $@

RandomTesting/exec.o: RandomTesting/exec.cpp
	$(CXX) -c $< -o $@
