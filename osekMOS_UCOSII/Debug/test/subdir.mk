################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../test/ex_test.c 

OBJS += \
./test/ex_test.obj 

C_DEPS += \
./test/ex_test.pp 

C_DEPS__QUOTED += \
"test\ex_test.pp" 

OBJS__QUOTED += \
"test\ex_test.obj" 

C_SRCS__QUOTED += \
"../test/ex_test.c" 


# Each subdirectory must supply rules for building sources it contributes
test/ex_test.obj: ../test/ex_test.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="test/ex_test.pp" --obj_directory="test" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


