################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/cfg.c \
../source/osekMAlarm.c \
../source/osekMCounter.c \
../source/osekMEvent.c \
../source/osekMExecution.c \
../source/osekMHook.c \
../source/osekMInterrupt.c \
../source/osekMKernel.c \
../source/osekMRpcKernel.c \
../source/osekMSpinlock.c \
../source/osekMTarget.c \
../source/osekMTask.c \
../source/osekMessage.c 

OBJS += \
./source/cfg.obj \
./source/osekMAlarm.obj \
./source/osekMCounter.obj \
./source/osekMEvent.obj \
./source/osekMExecution.obj \
./source/osekMHook.obj \
./source/osekMInterrupt.obj \
./source/osekMKernel.obj \
./source/osekMRpcKernel.obj \
./source/osekMSpinlock.obj \
./source/osekMTarget.obj \
./source/osekMTask.obj \
./source/osekMessage.obj 

C_DEPS += \
./source/cfg.pp \
./source/osekMAlarm.pp \
./source/osekMCounter.pp \
./source/osekMEvent.pp \
./source/osekMExecution.pp \
./source/osekMHook.pp \
./source/osekMInterrupt.pp \
./source/osekMKernel.pp \
./source/osekMRpcKernel.pp \
./source/osekMSpinlock.pp \
./source/osekMTarget.pp \
./source/osekMTask.pp \
./source/osekMessage.pp 

C_DEPS__QUOTED += \
"source\cfg.pp" \
"source\osekMAlarm.pp" \
"source\osekMCounter.pp" \
"source\osekMEvent.pp" \
"source\osekMExecution.pp" \
"source\osekMHook.pp" \
"source\osekMInterrupt.pp" \
"source\osekMKernel.pp" \
"source\osekMRpcKernel.pp" \
"source\osekMSpinlock.pp" \
"source\osekMTarget.pp" \
"source\osekMTask.pp" \
"source\osekMessage.pp" 

OBJS__QUOTED += \
"source\cfg.obj" \
"source\osekMAlarm.obj" \
"source\osekMCounter.obj" \
"source\osekMEvent.obj" \
"source\osekMExecution.obj" \
"source\osekMHook.obj" \
"source\osekMInterrupt.obj" \
"source\osekMKernel.obj" \
"source\osekMRpcKernel.obj" \
"source\osekMSpinlock.obj" \
"source\osekMTarget.obj" \
"source\osekMTask.obj" \
"source\osekMessage.obj" 

C_SRCS__QUOTED += \
"../source/cfg.c" \
"../source/osekMAlarm.c" \
"../source/osekMCounter.c" \
"../source/osekMEvent.c" \
"../source/osekMExecution.c" \
"../source/osekMHook.c" \
"../source/osekMInterrupt.c" \
"../source/osekMKernel.c" \
"../source/osekMRpcKernel.c" \
"../source/osekMSpinlock.c" \
"../source/osekMTarget.c" \
"../source/osekMTask.c" \
"../source/osekMessage.c" 


# Each subdirectory must supply rules for building sources it contributes
source/cfg.obj: ../source/cfg.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/cfg.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMAlarm.obj: ../source/osekMAlarm.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMAlarm.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMCounter.obj: ../source/osekMCounter.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMCounter.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMEvent.obj: ../source/osekMEvent.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMEvent.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMExecution.obj: ../source/osekMExecution.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMExecution.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMHook.obj: ../source/osekMHook.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMHook.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMInterrupt.obj: ../source/osekMInterrupt.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMInterrupt.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMKernel.obj: ../source/osekMKernel.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMKernel.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMRpcKernel.obj: ../source/osekMRpcKernel.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMRpcKernel.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMSpinlock.obj: ../source/osekMSpinlock.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMSpinlock.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMTarget.obj: ../source/osekMTarget.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMTarget.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMTask.obj: ../source/osekMTask.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMTask.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/osekMessage.obj: ../source/osekMessage.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_8.20\include" --include_path="H:\c6678\workspace\osekMOS_8.20\arch\include" --include_path="H:\c6678\workspace\osekMOS_8.20\demo" --include_path="H:\c6678\workspace\osekMOS_8.20\lib\include" --include_path="H:\c6678\workspace\osekMOS_8.20\test\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="source/osekMessage.pp" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


