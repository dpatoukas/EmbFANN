################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/msp430-gcc-6.2.1.16_win32/bin/msp430-elf-gcc.exe" -c -mmcu=msp430fr5994 -I"C:/ti/ccsv7/ccs_base/msp430/include_gcc" -I"C:/Users/patou/workspace_v7/mspFANN/src/include" -I"C:/Users/patou/workspace_v7/mspFANN" -I"C:/ti/ccsv7/tools/compiler/msp430-gcc-6.2.1.16_win32/msp430-elf/include" -Os -g -gdwarf-3 -gstrict-dwarf -Wall -mhwmult=f5series -mlarge -mcode-region=none -mdata-region=none -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


