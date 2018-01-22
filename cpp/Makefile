# C PreProcessor
CPPFLAGS = -DNDEBUG
#CPPFLAGS = -DNDEBUG

# C Compiler
CC = gcc

# C Compiler Flags
CFLAGS = -Wall -ansi -pedantic -O3

# C++ Compiler
CXX = g++

# C++ Compiler Flags
CXXFLAGS = -Wall -ansi -pedantic -O3

# Extra flags to give to compilers when they are supposed to invoke the linker, 'ld', such as -L. Libraries (-lfoo) should be added to the LDLIBS variable instead.
LDFLAGS = 

# Library flags or names given to compilers when they are supposed to invoke the linker, 'ld'. LOADLIBES is a deprecated (but still supported) alternative to LDLIBS. Non-library linker flags, such as -L, should go in the LDFLAGS variable.
LDLIBS = -lstdc++ -lm






CPP_FILES = graph.cpp labeled_graph.cpp
OBJ_FILES := $(CPP_FILES:.cpp=.o)
HDR_FILES = graph.hh label_list.hh labeled_graph.hh output_any.hh

PROG =  labeled_graph graph 

labeled_graph: 
labeled_graph.o: labeled_graph.hh label_list.hh

graph: 
graph.o: graph.hh label_list.hh

.PHONY : all
all : $(PROG)


.PHONY : clean
clean:
	$(RM) $(OBJ_FILES)

.PHONY : veryclean
veryclean:
	$(RM) $(PROG) $(OBJ_FILES)

.PHONY : tar
tar:
	tar -czvf src.tar.gz $(HDR_FILES) $(CPP_FILES) Makefile

