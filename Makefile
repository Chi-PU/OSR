CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS  = -lprotobuf -lssl -lcrypto -lpq

SRCS = Online_Star_Rail.cpp socket.cpp connect_manager.cpp protobuf.cpp \
       game.pb.cc auth.cpp characters.cpp menu.cpp data.cpp ssl.cpp
OBJS = $(SRCS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)
TARGET = osr_client

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

proto:
	protoc --cpp_out=. game.proto

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean proto
