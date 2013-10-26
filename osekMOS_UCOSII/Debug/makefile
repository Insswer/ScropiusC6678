################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./arch/c66xx_vectors.obj" \
"./arch/c66xx_timer.obj" \
"./arch/c66xx_spinlock.obj" \
"./arch/c66xx_rpc.obj" \
"./arch/c66xx_regs.obj" \
"./arch/c66xx_msmc.obj" \
"./arch/c66xx_mpu.obj" \
"./arch/c66xx_memarea.obj" \
"./arch/c66xx_itbl.obj" \
"./arch/c66xx_ipc.obj" \
"./arch/c66xx_init.obj" \
"./arch/c66xx_exception.obj" \
"./arch/c66xx_corepac.obj" \
"./arch/c66xx_cic.obj" \
"./arch/c6678_cache.obj" \
"./demo/osekMRpcFunc.obj" \
"./lib/string.obj" \
"./lib/rb_tree.obj" \
"./os/osek/osekMessage.obj" \
"./os/osek/osekMTask.obj" \
"./os/osek/osekMTarget.obj" \
"./os/osek/osekMSpinlock.obj" \
"./os/osek/osekMRpcKernel.obj" \
"./os/osek/osekMKernel.obj" \
"./os/osek/osekMInterrupt.obj" \
"./os/osek/osekMHook.obj" \
"./os/osek/osekMExecution.obj" \
"./os/osek/osekMEvent.obj" \
"./os/osek/osekMCounter.obj" \
"./os/osek/osekMAlarm.obj" \
"./os/osek/cfg.obj" \
"./os/ucos/core/OS_TIME.obj" \
"./os/ucos/core/OS_TASK.obj" \
"./os/ucos/core/OS_SEM.obj" \
"./os/ucos/core/OS_Q.obj" \
"./os/ucos/core/OS_MUTEX.obj" \
"./os/ucos/core/OS_MEM.obj" \
"./os/ucos/core/OS_MBOX.obj" \
"./os/ucos/core/OS_FLAG.obj" \
"./os/ucos/core/OS_CORE.obj" \
"./os/ucos/port/os_cpu_c.obj" \
"./os/ucos/port/os_cpu_a.obj" \
"./test/ex_test.obj" \
"./ucmain.obj" \
"./oskmain.obj" \
-l"libc.a" \
"../main.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include test/subdir.mk
-include os/ucos/port/subdir.mk
-include os/ucos/core/subdir.mk
-include os/osek/subdir.mk
-include lib/subdir.mk
-include demo/subdir.mk
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
all: osekMOS_UCOSII.out

# Tool invocations
osekMOS_UCOSII.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --diag_warning=225 --abi=eabi -z -m"osekMOS_UCOSII.map" --warn_sections -i"H:/c6678/ccs/ccsv5/tools/compiler/c6000/lib" -i"H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --reread_libs --rom_model -o "osekMOS_UCOSII.out" $(ORDERED_OBJS) --unused_section_elimination=off
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C6000_EXECUTABLE_OUTPUTS__QUOTED) "osekMOS_UCOSII.out"
	-$(RM) "os\ucos\port\os_cpu_a.pp" "arch\c66xx_itbl.pp" "arch\c66xx_regs.pp" "arch\c66xx_vectors.pp" 
	-$(RM) "oskmain.pp" "ucmain.pp" "test\ex_test.pp" "os\ucos\port\os_cpu_c.pp" "os\osek\cfg.pp" "os\osek\osekMAlarm.pp" "os\osek\osekMCounter.pp" "os\osek\osekMEvent.pp" "os\osek\osekMExecution.pp" "os\osek\osekMHook.pp" "os\osek\osekMInterrupt.pp" "os\osek\osekMKernel.pp" "os\osek\osekMRpcKernel.pp" "os\osek\osekMSpinlock.pp" "os\osek\osekMTarget.pp" "os\osek\osekMTask.pp" "os\osek\osekMessage.pp" "lib\rb_tree.pp" "lib\string.pp" "demo\osekMRpcFunc.pp" "arch\c6678_cache.pp" "arch\c66xx_cic.pp" "arch\c66xx_corepac.pp" "arch\c66xx_exception.pp" "arch\c66xx_init.pp" "arch\c66xx_ipc.pp" "arch\c66xx_memarea.pp" "arch\c66xx_mpu.pp" "arch\c66xx_msmc.pp" "arch\c66xx_rpc.pp" "arch\c66xx_spinlock.pp" "arch\c66xx_timer.pp" 
	-$(RM) "os\ucos\core\OS_CORE.pp" "os\ucos\core\OS_FLAG.pp" "os\ucos\core\OS_MBOX.pp" "os\ucos\core\OS_MEM.pp" "os\ucos\core\OS_MUTEX.pp" "os\ucos\core\OS_Q.pp" "os\ucos\core\OS_SEM.pp" "os\ucos\core\OS_TASK.pp" "os\ucos\core\OS_TIME.pp" 
	-$(RM) "oskmain.obj" "ucmain.obj" "test\ex_test.obj" "os\ucos\port\os_cpu_a.obj" "os\ucos\port\os_cpu_c.obj" "os\ucos\core\OS_CORE.obj" "os\ucos\core\OS_FLAG.obj" "os\ucos\core\OS_MBOX.obj" "os\ucos\core\OS_MEM.obj" "os\ucos\core\OS_MUTEX.obj" "os\ucos\core\OS_Q.obj" "os\ucos\core\OS_SEM.obj" "os\ucos\core\OS_TASK.obj" "os\ucos\core\OS_TIME.obj" "os\osek\cfg.obj" "os\osek\osekMAlarm.obj" "os\osek\osekMCounter.obj" "os\osek\osekMEvent.obj" "os\osek\osekMExecution.obj" "os\osek\osekMHook.obj" "os\osek\osekMInterrupt.obj" "os\osek\osekMKernel.obj" "os\osek\osekMRpcKernel.obj" "os\osek\osekMSpinlock.obj" "os\osek\osekMTarget.obj" "os\osek\osekMTask.obj" "os\osek\osekMessage.obj" "lib\rb_tree.obj" "lib\string.obj" "demo\osekMRpcFunc.obj" "arch\c6678_cache.obj" "arch\c66xx_cic.obj" "arch\c66xx_corepac.obj" "arch\c66xx_exception.obj" "arch\c66xx_init.obj" "arch\c66xx_ipc.obj" "arch\c66xx_itbl.obj" "arch\c66xx_memarea.obj" "arch\c66xx_mpu.obj" "arch\c66xx_msmc.obj" "arch\c66xx_regs.obj" "arch\c66xx_rpc.obj" 
	-$(RM) "arch\c66xx_spinlock.obj" "arch\c66xx_timer.obj" "arch\c66xx_vectors.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
