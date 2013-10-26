################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../demo/osekMRpcFunc.c 

OBJS += \
./demo/osekMRpcFunc.obj 

C_DEPS += \
./demo/osekMRpcFunc.pp 

C_DEPS__QUOTED += \
"demo\osekMRpcFunc.pp" 

OBJS__QUOTED += \
"demo\osekMRpcFunc.obj" 

C_SRCS__QUOTED += \
"../demo/osekMRpcFunc.c" 


# Each subdirectory must supply rules for building sources it contributes
demo/osekMRpcFunc.obj: ../demo/osekMRpcFunc.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="demo/osekMRpcFunc.pp" --obj_directory="demo" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


