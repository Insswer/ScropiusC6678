################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ucos/port/os_cpu_c.c \
../ucos/port/os_irqhandler.c 

ASM_SRCS += \
../ucos/port/os_cpu_a.asm \
../ucos/port/os_inc_vector.asm 

ASM_DEPS += \
./ucos/port/os_cpu_a.pp \
./ucos/port/os_inc_vector.pp 

OBJS += \
./ucos/port/os_cpu_a.obj \
./ucos/port/os_cpu_c.obj \
./ucos/port/os_inc_vector.obj \
./ucos/port/os_irqhandler.obj 

C_DEPS += \
./ucos/port/os_cpu_c.pp \
./ucos/port/os_irqhandler.pp 

ASM_DEPS__QUOTED += \
"ucos\port\os_cpu_a.pp" \
"ucos\port\os_inc_vector.pp" 

C_DEPS__QUOTED += \
"ucos\port\os_cpu_c.pp" \
"ucos\port\os_irqhandler.pp" 

OBJS__QUOTED += \
"ucos\port\os_cpu_a.obj" \
"ucos\port\os_cpu_c.obj" \
"ucos\port\os_inc_vector.obj" \
"ucos\port\os_irqhandler.obj" 

ASM_SRCS__QUOTED += \
"../ucos/port/os_cpu_a.asm" \
"../ucos/port/os_inc_vector.asm" 

C_SRCS__QUOTED += \
"../ucos/port/os_cpu_c.c" \
"../ucos/port/os_irqhandler.c" 


# Each subdirectory must supply rules for building sources it contributes
ucos/port/os_cpu_a.obj: ../ucos/port/os_cpu_a.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/port/os_cpu_a.pp" --obj_directory="ucos/port" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/port/os_cpu_c.obj: ../ucos/port/os_cpu_c.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/port/os_cpu_c.pp" --obj_directory="ucos/port" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/port/os_inc_vector.obj: ../ucos/port/os_inc_vector.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/port/os_inc_vector.pp" --obj_directory="ucos/port" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/port/os_irqhandler.obj: ../ucos/port/os_irqhandler.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/port/os_irqhandler.pp" --obj_directory="ucos/port" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


