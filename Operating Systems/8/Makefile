#
# make: clean, tar, 8, run8
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
S = Server
C = Client
L = MQLibrary
#----------------------
EXECS = $(S)
EXECC = $(C)
EXECL = $(L)

OBJS = $(S).o
OBJC = $(C).o
OBJL = $(L).o

HEADER = $(L).h
#############################
COFLAGS = -Wall -std=c99 -pedantic
LDFLAGS = -Wall -std=c99 -pedantic
CO = gcc
LD = $(CO)
ENDFLAGS = -lrt
#############################
8:
	$(CO) $(COFLAGS) $(L).c $(S).c -o $(S) $(ENDFLAGS)
	$(CO) $(COFLAGS) $(L).c $(C).c -o $(C) $(ENDFLAGS)
#############################
run8:
	(xterm -hold -title Server -e ./Server &)
	(sleep 0.1)
	(xterm -hold -title Client1 -e ./Client &)
	(sleep 0.1)
	(xterm -hold -title Client2 -e ./Client &)
	(sleep 0.1)
	(xterm -hold -title Client3 -e ./Client &)
#############################
.PHONY: clean tar
#=========================================================================
EXECUTABLES = $(EXECS) $(EXECC)
#=========================================================================
clean:                                                     
	rm -f *.o  *~ $(EXECUTABLES)
#=========================================================================
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR))
#=========================================================================

