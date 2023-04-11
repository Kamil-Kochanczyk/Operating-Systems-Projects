#
# make: clean, tar, zad5, 5a, 5b
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------
MAIN = 5
PRODUCER = Producer
CONSUMER = Consumer
IN = Input
OUT = Output
FIFO = FIFO
#----------------------
EXECM = $(MAIN)
EXECP = $(PRODUCER)
EXECC = $(CONSUMER)

OBJM = $(MAIN).o
OBJP = $(PRODUCER).o
OBJC = $(CONSUMER).o
#############################
COFLAGS = -Wall -O
LDFLAGS = -Wall -O
CO = gcc
LD = $(CO)
#############################
%.o: %.c
	$(CO) $(COFLAGS) -c $<
#############################
$(EXECM): $(OBJM)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXECP): $(OBJP)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
$(EXECC): $(OBJC)
	 $(LD) -o $@ $(LDFLAGS) $^
#############################
zad5:
	(make $(EXECM); make $(EXECP); make $(EXECC))
#############################
5a:
	(./$(EXECM) $(PRODUCER) $(CONSUMER) $(IN); diff -s $(IN) $(OUT))
#############################
5b:
	mkfifo $(FIFO)
	(xterm -hold -title $(PRODUCER) -e ./$(PRODUCER) $(FIFO) $(IN) &)
	(xterm -hold -title $(CONSUMER) -e ./$(CONSUMER) $(FIFO) &)
#############################
.PHONY: clean tar
#=========================================================================
EXECS = $(EXECM) $(EXECP) $(EXECC)
#=========================================================================
clean:                                                     
	rm -f *.o  *~ $(EXECS) $(OUT) $(FIFO)
#=========================================================================
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR))
#=========================================================================
