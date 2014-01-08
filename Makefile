CPPFLAGS = -Wall -g -MMD 
#-fno-stack-protector
LDFLAGS = -g
CXX = g++

OBJS = main.o sim.o user.o topo.o binsearch.o trafficgen.o

SOURCES = main.cpp user.cpp trafficgen.cpp sim.cpp topo.cpp binsearch.cpp

all: coopSim

coopSim: $(OBJS)
	$(CXX) -o $@ $^


-include $(subst .cpp,.d,$(SOURCES))

%.d: %.cpp
	$(CXX) -M $(CPPFLAGS) -MF $@ $< -MT "$*.o $@"

clean:
	rm -f coopSim $(OBJS) *.d *.d.*
