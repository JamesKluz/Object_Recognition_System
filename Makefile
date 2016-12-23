########################################
##
## Makefile
## LINUX compilation 
##
##############################################





#FLAGS
C++FLAG = -g -std=c++11

MATH_LIBS = -lm

EXEC_DIR=.


.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 


#First Program (ListTest)

Cpp_OBJ0=image.o p1.o
PROGRAM_0=p1
$(PROGRAM_0): $(Cpp_OBJ0)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ0) $(INCLUDES) $(LIBS_ALL)

Cpp_OBJ1=image.o p2.o
PROGRAM_1=p2
$(PROGRAM_1): $(Cpp_OBJ1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ1) $(INCLUDES) $(LIBS_ALL)

Cpp_OBJ2=image.o p3.o
PROGRAM_2=p3
$(PROGRAM_2): $(Cpp_OBJ2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ2) $(INCLUDES) $(LIBS_ALL)	

Cpp_OBJ3=image.o p4.o
PROGRAM_3=p4
$(PROGRAM_3): $(Cpp_OBJ3)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ3) $(INCLUDES) $(LIBS_ALL)	


all: 
	make $(PROGRAM_0) 
	make $(PROGRAM_1)
	make $(PROGRAM_2)
	make $(PROGRAM_3)

clean:
	(rm -f *.o; rm -f P1; rm -f P2; rm -f P3; rm -f P4;)

(:
