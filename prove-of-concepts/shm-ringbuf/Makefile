all:consumer producer

CPPFLAGS+=-std=c++03 -Wall -pedantic
CPPFLAGS+=-g -O0
CPPFLAGS+=-isystem ~/custom/boost/

LDFLAGS+=-L ~/custom/boost/stage/lib/ -Wl,-rpath,/home/sehe/custom/boost/stage/lib
LDFLAGS+=-lboost_system -lrt -lpthread

%:%.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)
	