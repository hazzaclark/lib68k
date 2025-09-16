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

CPU				?= 		M68K_CPU_000
SUPPORTED_CPUS	= 		M68K_CPU_000 M68K_CPU_010

ifeq ($(filter $(CPU),$(SUPPORTED_CPUS)),)
$(error CPU $(CPU) IS UNSUPPORTED -> SUPPORTED MODELS: $(SUPPORTED_CPUS))
endif

## SIMPLE STRING CONCATENATION FOR CPU TYPE SUPPORTED
## MEANS NOTHING IN TERMS OF COMPATIBILITY - JUST LOOKS COOL

CPU_NUM = $(subst M68K_CPU_,,$(CPU))
CPU_CONCATE = 68$(CPU_NUM)

#########################################################################
##				FILE ACCESS			       ##
#########################################################################

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
	$(CC) -o $@ -L. -l68k $(WARNINGS)  

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(WARNINGS) -I$(INC) -DM68K_CPU_TYPE=$(CPU) -DM68K_CPU_LABEL="$(CPU_CONCATE)" -c $< -o $@

clean:
	rm -f $(SRC)/*.o $(TARGET) $(LIBRARY)

.PHONY: clean

#########################################################################
##				MISC.				       ##
#########################################################################

include rules.mk
