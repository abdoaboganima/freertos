#The Compiler Used
CC		:= avr-gcc
#Target MCU
MCU       	:= atmega32
#CPU Clock Frequency
F_CPU     	:= 8000000ul
#Linker
LD        	:= avr-ld
OBJCOPY   	:= avr-objcopy



SOURCES       := $(shell ls FreeRTOS/*.c Drivers/*.c)
INCLUDES      := Drivers FreeRTOS
MAIN_SOURCES  := $(shell ls *.c)

OBJS	      := $(SOURCES:.c=.o)
MAIN_OBJS     := $(MAIN_SOURCES:.c=.o)
MAIN_FUNC     := 
EXEC	      :=

CFLAGS    := -Wall -Wextra -g -Os -fdata-sections -ffunction-sections -fshort-enums -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CPPFLAGS  := $(INCLUDES:%=-I%)
LDFLAGS   := -Xlinker --gc-sections -Xlinker --relax -Wl,-u,vfprintf -lprintf_flt -lm #-Xlinker --strip-all

#VPATH := FreeRTOS Drivers

#`make build-all` for building all the executable files 
.PHONY: build-all
build-all:
	make compile-all
	$(foreach i, $(MAIN_OBJS), make build EXEC=$(shell basename -s .o $(i)) MAINFUNC=$(i);)

#`make build` for build only 
.PHONY: build
build:   $(EXEC).hex


%.o:%.c
	@echo Compiling ---------------------------------------- $@
	@$(CC) -c $(CFLAGS)  $(CPPFLAGS) $< -o $@

#`make x.hex` generate an object file, x.hex from elf file, x.elf,.
%.hex:%.elf
	@echo Genrating hex file --------------------------------- $@
	@$(OBJCOPY) -j .text -j .data -O ihex $< $@

#`make EXECUTABLENAME.elf` for generating the elf file
$(EXEC).elf: $(OBJS) $(MAINFUNC)
	@echo Building Target: $@
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	@avr-size -C $@


#`make compile-all` for compiling all source files
.PHONY: compile-all
compile-all: 	$(OBJS) $(MAIN_OBJS)


.PHONY: test
test:
	@echo The Includes are :  $(INCLUDES)
	@echo
	@echo The Sources  are :  $(SOURCES)
	@echo
	@echo The main functions are :  $(MAIN_SOURCES)
	@echo
	@echo The objects  are :  $(OBJS)
	@echo
	@echo The main objects  are :  $(MAIN_OBJS)


.PHONY: clean
clean:
	@rm -rf $(OBJS) $(PREPROCESSED) $(ASSEMBLIES) $(DEPENDENCIES) $(MAIN_OBJS) \
	*.out *.map *.elf *.map *.hex *.s *.i *.d *.lss *.o *.obj
	@echo CLEANED!

