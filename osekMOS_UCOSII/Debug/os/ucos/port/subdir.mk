################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/ucos/port/os_cpu_c.c 

ASM_SRCS += \
../os/ucos/port/os_cpu_a.asm 

ASM_DEPS += \
./os/ucos/port/os_cpu_a.pp 

OBJS += \
./os/ucos/port/os_cpu_a.obj \
./os/ucos/port/os_cpu_c.obj 

C_DEPS += \
./os/ucos/port/os_cpu_c.pp 

ASM_DEPS__QUOTED += \
"os\ucos\port\os_cpu_a.pp" 

C_DEPS__QUOTED += \
"os\ucos\port\os_cpu_c.pp" 

OBJS__QUOTED += \
"os\ucos\port\os_cpu_a.obj" \
"os\ucos\port\os_cpu_c.obj" 

ASM_SRCS__QUOTED += \
"../os/ucos/port/os_cpu_a.asm" 

C_SRCS__QUOTED += \
"../os/ucos/port/os_cpu_c.c" 


# Each subdirectory must supply rules for building sources it contributes
os/ucos/port/os_cpu_a.obj: ../os/ucos/port/os_cpu_a.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/port/os_cpu_a.pp" --obj_directory="os/ucos/port" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/ucos/port/os_cpu_c.obj: ../os/ucos/port/os_cpu_c.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/port/os_cpu_c.pp" --obj_directory="os/ucos/port" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


