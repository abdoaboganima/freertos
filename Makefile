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

OBJS	      := $(SOURCES:%.c=bin/%.o)      #Replaces every source file with an associated object file in bin directory
MAIN_OBJS     := $(MAIN_SOURCES:%.c=bin/%.o)
EXECUTABLES   := $(MAIN_OBJS:%.o=%.elf)
HEX_FILES     := $(MAIN_OBJS:%.o=%.hex)

CFLAGS    := -Wall -Wextra -g -Os -fdata-sections -ffunction-sections -fshort-enums -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CPPFLAGS  := $(INCLUDES:%=-I%)
LDFLAGS   := -Xlinker --gc-sections -Xlinker --relax -Wl,-u,vfprintf -lprintf_flt -lm -Xlinker --strip-all



#`make build-all` for building all the executable files 
.PHONY: build-all
build-all: $(HEX_FILES)

#`make build` for build only 
.PHONY: build
build:   $(MAINFUNC).hex


bin/%.o:%.c
	@mkdir -p $(INCLUDES:%=bin/%)
	@echo Compiling ---------------------------------------- $@
	@$(CC) -c $(CFLAGS)  $(CPPFLAGS) $< -o $@

#`make x.hex` generate an object file, x.hex from elf file, x.elf,.
%.hex:%.elf
	@echo Genrating hex file ------------------------------- $@
	@$(OBJCOPY) -j .text -j .data -O ihex $< $@

#`make EXECUTABLENAME.elf` for generating the elf file
%.elf: $(OBJS) %.o #The object of the file that contains the main function
	@echo Building Target: $@
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
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
	@echo The hex files     are :  $(HEX_FILES)


.PHONY: clean
clean:
	@rm -rf $(OBJS) $(PREPROCESSED) $(ASSEMBLIES) $(DEPENDENCIES) $(MAIN_OBJS) \
	$(EXECUTABLES) $(HEX_FILES) bin/ *.out *.map *.elf *.map *.hex *.s *.i *.d *.lss *.o *.obj
	@echo CLEANED!

