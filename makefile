## COPYRIGHT (C) HARRY CLARK 2024
## MOTOROLA 68000 STANDALONE EMULATION LIBRARY

#########################################################################
##				PREFIXES			       ##
#########################################################################

CC			=		$(PREFIX)gcc
AR			=		$(PREFIX)ar
WARNINGS	=		-std=gnu99 -Wall -Wextra -Wparentheses -Werror -pedantic -O0
SRC			= 		src
INC			=		inc

include hooks.mk

#########################################################################
##				FILE ACCESS			       ##
#########################################################################

CFLAGS      =       $(WARNINGS) $(OPT_LEVEL) $(TRACE_FLAGS)
68K_FILES	=		$(wildcard $(SRC)/*.c)
68K_OBJS	=		$(68K_FILES:$(SRC)/%.c=$(SRC)/%.o)

#########################################################################
##			    TARGET LINKING			       ##
#########################################################################

TARGET 		= 		lib68k.exe  
LIBRARY		= 		lib68k.so lib68k.a     

all: $(TARGET)

$(LIBRARY): $(68K_OBJS)
	$(AR) rcs $@ $^

$(TARGET): $(LIBRARY)
	$(CC) -o $@ -L. -l68k $(CFLAGS)  

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

clean:
	rm -f $(SRC)/*.o $(TARGET) $(LIBRARY)

.PHONY: clean

#########################################################################
##				MISC.				       ##
#########################################################################

include rules.mk
