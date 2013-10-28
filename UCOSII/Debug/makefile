################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./arch/c66xx_vectors.obj" \
"./arch/c66xx_rpc.obj" \
"./arch/c66xx_regs.obj" \
"./arch/c66xx_msmc.obj" \
"./arch/c66xx_mpu.obj" \
"./arch/c66xx_memarea.obj" \
"./arch/c66xx_ipc.obj" \
"./arch/c66xx_init.obj" \
"./arch/c66xx_exception.obj" \
"./arch/c66xx_corepac.obj" \
"./arch/c66xx_cic.obj" \
"./arch/c6678_cache.obj" \
"./arch/driver/c66xx_timer.obj" \
"./arch/driver/c66xx_spinlock.obj" \
"./lib/string.obj" \
"./lib/rb_tree.obj" \
"./scropius/src/sc_os.obj" \
"./scropius/src/sc_int.obj" \
"./scropius/src/main.obj" \
"./ucos/core/OS_TIME.obj" \
"./ucos/core/OS_TASK.obj" \
"./ucos/core/OS_SEM.obj" \
"./ucos/core/OS_Q.obj" \
"./ucos/core/OS_MUTEX.obj" \
"./ucos/core/OS_MEM.obj" \
"./ucos/core/OS_MBOX.obj" \
"./ucos/core/OS_FLAG.obj" \
"./ucos/core/OS_CORE.obj" \
"./ucos/port/os_irqhandler.obj" \
"./ucos/port/os_inc_vector.obj" \
"./ucos/port/os_cpu_c.obj" \
"./ucos/port/os_cpu_a.obj" \
"./ucos/ucmain.obj" \
-l"libc.a" \
"../main.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include ucos/subdir.mk
-include ucos/port/subdir.mk
-include ucos/core/subdir.mk
-include scropius/src/subdir.mk
-include lib/subdir.mk
-include arch/driver/subdir.mk
-include arch/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: UCOSII_Double.out

# Tool invocations
UCOSII_Double.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --diag_warning=225 --abi=eabi -z -m"UCOSII_Double.map" --warn_sections -i"H:/c6678/ccs/ccsv5/tools/compiler/c6000/lib" -i"H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --reread_libs --rom_model -o "UCOSII_Double.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C6000_EXECUTABLE_OUTPUTS__QUOTED) "UCOSII_Double.out"
	-$(RM) "ucos\port\os_cpu_a.pp" "ucos\port\os_inc_vector.pp" "arch\c66xx_regs.pp" "arch\c66xx_vectors.pp" 
	-$(RM) "ucos\ucmain.pp" "ucos\port\os_cpu_c.pp" "ucos\port\os_irqhandler.pp" "scropius\src\main.pp" "scropius\src\sc_int.pp" "scropius\src\sc_os.pp" "lib\rb_tree.pp" "lib\string.pp" "arch\driver\c66xx_spinlock.pp" "arch\driver\c66xx_timer.pp" "arch\c6678_cache.pp" "arch\c66xx_cic.pp" "arch\c66xx_corepac.pp" "arch\c66xx_exception.pp" "arch\c66xx_init.pp" "arch\c66xx_ipc.pp" "arch\c66xx_memarea.pp" "arch\c66xx_mpu.pp" "arch\c66xx_msmc.pp" "arch\c66xx_rpc.pp" 
	-$(RM) "ucos\core\OS_CORE.pp" "ucos\core\OS_FLAG.pp" "ucos\core\OS_MBOX.pp" "ucos\core\OS_MEM.pp" "ucos\core\OS_MUTEX.pp" "ucos\core\OS_Q.pp" "ucos\core\OS_SEM.pp" "ucos\core\OS_TASK.pp" "ucos\core\OS_TIME.pp" 
	-$(RM) "ucos\ucmain.obj" "ucos\port\os_cpu_a.obj" "ucos\port\os_cpu_c.obj" "ucos\port\os_inc_vector.obj" "ucos\port\os_irqhandler.obj" "ucos\core\OS_CORE.obj" "ucos\core\OS_FLAG.obj" "ucos\core\OS_MBOX.obj" "ucos\core\OS_MEM.obj" "ucos\core\OS_MUTEX.obj" "ucos\core\OS_Q.obj" "ucos\core\OS_SEM.obj" "ucos\core\OS_TASK.obj" "ucos\core\OS_TIME.obj" "scropius\src\main.obj" "scropius\src\sc_int.obj" "scropius\src\sc_os.obj" "lib\rb_tree.obj" "lib\string.obj" "arch\driver\c66xx_spinlock.obj" "arch\driver\c66xx_timer.obj" "arch\c6678_cache.obj" "arch\c66xx_cic.obj" "arch\c66xx_corepac.obj" "arch\c66xx_exception.obj" "arch\c66xx_init.obj" "arch\c66xx_ipc.obj" "arch\c66xx_memarea.obj" "arch\c66xx_mpu.obj" "arch\c66xx_msmc.obj" "arch\c66xx_regs.obj" "arch\c66xx_rpc.obj" "arch\c66xx_vectors.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
