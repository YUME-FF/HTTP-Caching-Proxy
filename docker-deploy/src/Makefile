# all: main

# main: main.cpp httpcommand.cpp httpcommand.h myexception.h helper.cpp helper.h proxy.h proxy.cpp cache.h cache.cpp ResponseInfo.h ResponseInfo.cpp
# 	g++ -O3 -std=c++11 -o main main.cpp httpcommand.cpp helper.cpp proxy.cpp cache.cpp ResponseInfo.cpp -lpthread -lboost_system -lboost_regex

# .PHONY:
# 	clean
# clean:
# 	rm -rf *.o main

CXX = g++
CXXFLAGS = -std=c++11 -O3
LDFLAGS = -lpthread -lboost_system -lboost_regex
SRC = main.cpp httpcommand.cpp helper.cpp proxy.cpp  ResponseInfo.cpp LRUCache.cpp #cache.cpp
TARGET = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
