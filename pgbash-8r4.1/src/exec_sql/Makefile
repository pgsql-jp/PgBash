#-------------------------------------------------------------------------
# Makefile
#    Makefile for pgbash library
#
#-------------------------------------------------------------------------

EXEC_SQL_DIR= .
PGSQL_INCLUDE= /usr/include

CFLAGS = -O2
CFLAGS+= -I$(PGSQL_INCLUDE) 

OBJS= $(EXEC_SQL_DIR)/exec_sql_init.o   $(EXEC_SQL_DIR)/exec_sql_main.o \
	  $(EXEC_SQL_DIR)/sql_connect.o		$(EXEC_SQL_DIR)/sql_fetch.o  	\
	  $(EXEC_SQL_DIR)/psql_sub.o   		$(EXEC_SQL_DIR)/print_sub.o		\
	  $(EXEC_SQL_DIR)/uncgi_sub.o		$(EXEC_SQL_DIR)/utils.o      	\
	  $(EXEC_SQL_DIR)/sql_set.o 

all: $(OBJS)


$(EXEC_SQL_DIR)/exec_sql_init.o: $(EXEC_SQL_DIR)/exec_sql_init.c $(EXEC_SQL_DIR)/exec_sql.h $(EXEC_SQL_DIR)/version.h
	$(CC) $(CFLAGS) -c $(EXEC_SQL_DIR)/exec_sql_init.c

$(EXEC_SQL_DIR)/exec_sql_main.o: $(EXEC_SQL_DIR)/exec_sql_main.c $(EXEC_SQL_DIR)/exec_sql.h $(EXEC_SQL_DIR)/sqlca.h  $(EXEC_SQL_DIR)/version.h
	$(CC) $(CFLAGS) -c $(EXEC_SQL_DIR)/exec_sql_main.c

$(EXEC_SQL_DIR)/sql_connect.o  : $(EXEC_SQL_DIR)/sql_connect.c   $(EXEC_SQL_DIR)/sql_connect.h
	$(CC) $(CFLAGS) -c $(EXEC_SQL_DIR)/sql_connect.c

$(EXEC_SQL_DIR)/sql_fetch.o    : $(EXEC_SQL_DIR)/sql_fetch.c     $(EXEC_SQL_DIR)/sql_fetch.h
	$(CC) $(CFLAGS) -c $(EXEC_SQL_DIR)/sql_fetch.c

$(EXEC_SQL_DIR)/psql_sub.o     : $(EXEC_SQL_DIR)/psql_sub.c      $(EXEC_SQL_DIR)/psql_sub.h
	$(CC) $(CFLAGS) -c $(EXEC_SQL_DIR)/psql_sub.c

$(EXEC_SQL_DIR)/utils.o        : $(EXEC_SQL_DIR)/utils.c         $(EXEC_SQL_DIR)/utils.h
	$(CC) $(CFLAGS) -c $(EXEC_SQL_DIR)/utils.c

$(EXEC_SQL_DIR)/print_sub.o    : $(EXEC_SQL_DIR)/print_sub.c     $(EXEC_SQL_DIR)/print_sub.h
	$(CC) $(CFLAGS) -c $(EXEC_SQL_DIR)/print_sub.c

$(EXEC_SQL_DIR)/uncgi_sub.o    : $(EXEC_SQL_DIR)/uncgi_sub.c     
	$(CC) $(CFLAGS) -c $(EXEC_SQL_DIR)/uncgi_sub.c

$(EXEC_SQL_DIR)/sql_set.o      : $(EXEC_SQL_DIR)/sql_set.c       $(EXEC_SQL_DIR)/sql_set.h
	$(CC) $(CFLAGS) -c $(EXEC_SQL_DIR)/sql_set.c


clean: 
	rm -f $(OBJS)

