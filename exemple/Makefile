COMP_ARMEL= arm-linux-gnueabi-g++
COMP_ARMHF= arm-linux-gnueabihf-g++
COMP_PATH_ARMEL= armel/
COMP_PATH_ARMHF= armhf/
CFLAGS= -std=c++11
NAME_PROG= main

# PERSO
SOURCES= main.cpp ../CI2C.cpp
OBJETS= main.o CI2C.o
FILES= $(OBJETS) $(NAME_PROG)
# COMPILATION ARMEL
armel : prog_armel
	mkdir -p $(COMP_PATH_ARMEL)
	mv $(FILES) -t $(COMP_PATH_ARMEL)

prog_armel : prog_armel_cpp
	$(COMP_ARMEL) $(CFLAGS) -o $(NAME_PROG) $(OBJETS)

prog_armel_cpp : $(SOURCES)
	$(COMP_ARMEL) $(CFLAGS) -c $(SOURCES)

# COMPILATION ARMHF
armhf : prog_armhf
	mkdir -p $(COMP_PATH_ARMHF)
	mv $(FILES) -t $(COMP_PATH_ARMEL)

prog_armhf : prog_armhf_cpp
	$(COMP_ARMHF) $(CFLAGS) -o $(NAME_PROG) $(OBJETS)

prog_armhf_cpp : $(SOURCES)
	$(COMP_ARMHF) $(CFLAGS) -c $(SOURCES)

# AUTRE
clean :
	rm -f *.o
	rm -f $(COMP_PATH)*.o

mrproper : clean
	rm -f $(NAME_PROG)
	rm -fr $(COMP_PATH)