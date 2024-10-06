TARGET = generator

CXX = g++

ifndef VCPKG_ROOT
$(error Set env VCPKG_ROOT)
endif

OS := $(shell uname)

ifeq ($(OS),Linux)
    INCLUDE_DIR := $(VCPKG_ROOT)/installed/x64-linux/include
else ifeq ($(OS),Darwin)
    INCLUDE_DIR := $(VCPKG_ROOT)/installed/arm64-osx/include
else ifeq ($(OS),Windows)
    INCLUDE_DIR := $(VCPKG_ROOT)\installed\x64-windows\include
else
    $(error unknown os: $(OS))
endif

ifneq ("$(wildcard $(INCLUDE_DIR))","")
    CXXFLAGS += -I$(INCLUDE_DIR) -std=c++11 -Wall
else
    $(error Path $(INCLUDE_DIR) doesnt exist. Check vcpkg install and VCPKG_ROOT settings.)
endif

SRCS = main.cpp 

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
