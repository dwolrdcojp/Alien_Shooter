# which compiler to use 
CC = g++

#compiler flags
# -g   Adds debugging information to the executable file
# -Wall    turns on most, but not all, compiler warnings
CFLAGS = -g -Wall -std=c++17

# define directories for header files 
INCLUDES = -I../include -I/usr/local/include

# define library paths
LFLAGS = -L/usr/local/lib

# define the libraries to link to the executable 
LIBS = -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network

#define the C++ source files
SRCS = *.cpp

# the build target executable 
TARGET = program 

%.o: %.cpp
	$(CC) -c $< -o $@

%.o: %.hpp
	$(CC) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SRCS) $(LFLAGS) $(LIBS)  

clean:
	$(RM) *.o *-- $(TARGET)
