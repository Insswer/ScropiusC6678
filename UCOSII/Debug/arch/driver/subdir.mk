################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../arch/driver/c66xx_spinlock.c \
../arch/driver/c66xx_timer.c 

OBJS += \
./arch/driver/c66xx_spinlock.obj \
./arch/driver/c66xx_timer.obj 

C_DEPS += \
./arch/driver/c66xx_spinlock.pp \
./arch/driver/c66xx_timer.pp 

C_DEPS__QUOTED += \
"arch\driver\c66xx_spinlock.pp" \
"arch\driver\c66xx_timer.pp" 

OBJS__QUOTED += \
"arch\driver\c66xx_spinlock.obj" \
"arch\driver\c66xx_timer.obj" 

C_SRCS__QUOTED += \
"../arch/driver/c66xx_spinlock.c" \
"../arch/driver/c66xx_timer.c" 


# Each subdirectory must supply rules for building sources it contributes
arch/driver/c66xx_spinlock.obj: ../arch/driver/c66xx_spinlock.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/driver/c66xx_spinlock.pp" --obj_directory="arch/driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/driver/c66xx_timer.obj: ../arch/driver/c66xx_timer.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/driver/c66xx_timer.pp" --obj_directory="arch/driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


