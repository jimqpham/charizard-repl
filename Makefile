INCS = cmdline.h

OBJS = main.o cmdline.o

CXX = c++
CXXFLAGS = --std=c++14 -O2

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

cmdline.o: cmdline.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<