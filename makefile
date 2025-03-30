## COPYRIGHT (C) HARRY CLARK 2024
## MOTOROLA 68000 STANDALONE EMULATION LIBRARY

#########################################################################
##				PREFIXES			       ##
#########################################################################

CC 			=		$(PREFIX)gcc
AR			=		$(PREFIX)ar
WARNINGS	=		-std=c99 -Wall -Wextra -Wparentheses -Werror -pedantic
SRC			= 		src
INC			=		inc

#########################################################################
##				FILE ACCESS			       ##
#########################################################################

68K_FILES	=		$(wildcard $(SRC)/*.c)
68K_OBJS	=		$(68K_FILES:$(SRC)/%.c=$(SRC)/%.o)

#########################################################################
##			    TARGET LINKING			       ##
#########################################################################

TARGET 		= 		lib68k
LIBRARY		= 		lib68k.so     

all: $(TARGET)

$(LIBRARY): $(68K_OBJS)
	$(AR) rcs $@ $^

$(TARGET): $(LIBRARY)
	$(CC) -o $@ -L. -l68k $(WARNINGS)  

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(WARNINGS) -I$(INC) -c $< -o $@

clean:
	rm -f $(SRC)/*.o $(TARGET) $(LIBRARY)

.PHONY: clean

#########################################################################
##				MISC.				       ##
#########################################################################

include rules.mk
