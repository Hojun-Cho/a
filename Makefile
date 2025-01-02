PROG = yo

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

CFLAGS = -g

${PROG}: y.tab.o $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $^

$(SRCS): y.tab.h

y.tab.o y.tab.h: cc.y
	yacc -d cc.y
	cc -c $(CFLAGS) -o y.tab.o y.tab.c

$(OBJS): dat.h fn.h yo.h

clean:
	rm -f $(PROG) $(OBJS) y.tab.*
