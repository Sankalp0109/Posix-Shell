
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -MMD -MP

TARGET = main

SRCS = main.cpp utils.cpp pipeline.cpp cd.cpp echo.cpp redirection.cpp ls.cpp history.cpp pinfo.cpp process.cpp search.cpp pwd.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

LDLIBS = -lreadline 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

