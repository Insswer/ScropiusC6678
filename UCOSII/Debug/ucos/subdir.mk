################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ucos/ucmain.c 

OBJS += \
./ucos/ucmain.obj 

C_DEPS += \
./ucos/ucmain.pp 

C_DEPS__QUOTED += \
"ucos\ucmain.pp" 

OBJS__QUOTED += \
"ucos\ucmain.obj" 

C_SRCS__QUOTED += \
"../ucos/ucmain.c" 


# Each subdirectory must supply rules for building sources it contributes
ucos/ucmain.obj: ../ucos/ucmain.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/ucmain.pp" --obj_directory="ucos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


