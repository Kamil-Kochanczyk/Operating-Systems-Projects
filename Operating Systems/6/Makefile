#
# make: clean, tar, 6
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
ME = MutualExclusion
D = Duplicator
L = Library
#----------------------
EXECME = $(ME)
EXECD = $(D)

OBJME = $(ME).o
OBJD = $(D).o
OBJL = $(L).o

HEADER = $(L).h
#############################
COFLAGS = -Wall -std=c99 -pedantic
LDFLAGS = -Wall -std=c99 -pedantic
CO = gcc
LD = $(CO)
ENDFLAGS = -pthread
#############################
6:
	$(CO) $(COFLAGS) $(L).c $(D).c -o $(D) $(ENDFLAGS)
	$(CO) $(COFLAGS) $(L).c $(ME).c -o $(ME) $(ENDFLAGS)
#############################
.PHONY: clean tar
#=========================================================================
EXECS = $(EXECME) $(EXECD)
#=========================================================================
clean:                                                     
	rm -f *.o  *~ $(EXECS) Number.txt
#=========================================================================
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR))
#=========================================================================
