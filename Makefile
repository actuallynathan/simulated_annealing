OBJS	= build/main.o build/table.o
SOURCE	= src/main.cpp src/file.cpp src/table.cpp
HEADER	= inc/
OUT	= annealer
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

build/main.o: src/main.cpp
	@mkdir -p $(@D)
	$(CC) $(FLAGS) src/main.cpp -I$(HEADER) -o build/main.o -std=c++17


build/table.o: src/table.cpp
	@mkdir -p $(@D)
	$(CC) $(FLAGS) src/table.cpp -I$(HEADER) -o build/table.o -std=c++17


clean:
	rm -f $(OBJS) $(OUT)
