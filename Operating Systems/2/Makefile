# make zad2, clean, tar

DIR = `basename $(CURDIR)`
NAME = zad2
EXEC = $(NAME)

COFLAGS = -Wall -O
CO = gcc

$(EXEC): $(NAME).o
	$(CO) $(COFLAGS) -o $@ $^

$(NAME).o: $(NAME).c
	$(CO) $(COFLAGS) -c $^

clean:
	rm $(EXEC) *.o
	
tar:
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR))
