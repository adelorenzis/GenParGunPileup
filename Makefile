CXX=g++
LD=g++

CXXFLAGS += -Wall -std=c++11 -g $(shell root-config --cflags) -I$(WCSIMDIR)/include
LDFLAGS += $(shell root-config --ldflags) $(shell root-config --libs) -lTreePlayer

.PHONY: clean Execs


%o::%cc
	$(CXX) $(CXXFLAGS) -c $? -o $@

GenParGunPileUp.exe: GenParGunPileUp.o VertexGenerator.o Vertex.o
	$(RM) $@
	$(LD) $^ $(LDFLAGS) -o $@
	@echo "$@ done"


clean:
	$(RM) *.o
