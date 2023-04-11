#
# make: clean, tar, 7
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
PARENT = Parent
P = Producer
C = Consumer
SL = SemaphoreLibrary
SML = SharedMemoryLibrary
#----------------------
EXECPARENT = $(PARENT)
EXECP = $(P)
EXECC = $(C)

OBJPARENT = $(PARENT).o
OBJP = $(P).o
OBJC = $(C).o
OBJSL = $(SL).o
OBJSML = $(SML).O

HEADERSL = $(SL).h
HEADERSML = $(SML).h
#############################
COFLAGS = -Wall -std=gnu99 -pedantic
LDFLAGS = -Wall -std=gnu99 -pedantic
CO = gcc
LD = $(CO)
ENDFLAGS = -pthread -lrt
#############################
7:
	$(CO) $(COFLAGS) $(SL).c $(SML).c $(PARENT).c -o $(PARENT) $(ENDFLAGS)
	$(CO) $(COFLAGS) $(SL).c $(SML).c $(P).c -o $(P) $(ENDFLAGS)
	$(CO) $(COFLAGS) $(SL).c $(SML).c $(C).c -o $(C) $(ENDFLAGS)
#############################
.PHONY: clean tar
#=========================================================================
EXECS = $(EXECPARENT) $(EXECP) $(EXECC)
#=========================================================================
clean:                                                     
	rm -f *.o  *~ $(EXECS) Output
#=========================================================================
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR))
#=========================================================================
