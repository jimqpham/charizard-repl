INCS = cmdline.h

OBJS = main.o cmdline.o

CXX = c++
CXXFLAGS = --std=c++14 -O2

sum: $(OBJS)
	$(CXX) $(CXXFLAGS) -o sum $(OBJS)

main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

cmdline.o: cmdline.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<