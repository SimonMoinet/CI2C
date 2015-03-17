CC=g++
CFLAGS=-std=c++11
# PERSO #
SOURCES=CI2C.cpp
OBJETS=$(SOURCES:.cpp=.o)
COMP_PATH=Compile

all : $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)
	mkdir -p $(COMP_PATH)
	mv $(OBJETS) -t $(COMP_PATH)	

# AUTRE
clean :
	rm -fr $(COMP_PATH)