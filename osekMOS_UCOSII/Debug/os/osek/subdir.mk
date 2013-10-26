################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/osek/cfg.c \
../os/osek/osekMAlarm.c \
../os/osek/osekMCounter.c \
../os/osek/osekMEvent.c \
../os/osek/osekMExecution.c \
../os/osek/osekMHook.c \
../os/osek/osekMInterrupt.c \
../os/osek/osekMKernel.c \
../os/osek/osekMRpcKernel.c \
../os/osek/osekMSpinlock.c \
../os/osek/osekMTarget.c \
../os/osek/osekMTask.c \
../os/osek/osekMessage.c 

OBJS += \
./os/osek/cfg.obj \
./os/osek/osekMAlarm.obj \
./os/osek/osekMCounter.obj \
./os/osek/osekMEvent.obj \
./os/osek/osekMExecution.obj \
./os/osek/osekMHook.obj \
./os/osek/osekMInterrupt.obj \
./os/osek/osekMKernel.obj \
./os/osek/osekMRpcKernel.obj \
./os/osek/osekMSpinlock.obj \
./os/osek/osekMTarget.obj \
./os/osek/osekMTask.obj \
./os/osek/osekMessage.obj 

C_DEPS += \
./os/osek/cfg.pp \
./os/osek/osekMAlarm.pp \
./os/osek/osekMCounter.pp \
./os/osek/osekMEvent.pp \
./os/osek/osekMExecution.pp \
./os/osek/osekMHook.pp \
./os/osek/osekMInterrupt.pp \
./os/osek/osekMKernel.pp \
./os/osek/osekMRpcKernel.pp \
./os/osek/osekMSpinlock.pp \
./os/osek/osekMTarget.pp \
./os/osek/osekMTask.pp \
./os/osek/osekMessage.pp 

C_DEPS__QUOTED += \
"os\osek\cfg.pp" \
"os\osek\osekMAlarm.pp" \
"os\osek\osekMCounter.pp" \
"os\osek\osekMEvent.pp" \
"os\osek\osekMExecution.pp" \
"os\osek\osekMHook.pp" \
"os\osek\osekMInterrupt.pp" \
"os\osek\osekMKernel.pp" \
"os\osek\osekMRpcKernel.pp" \
"os\osek\osekMSpinlock.pp" \
"os\osek\osekMTarget.pp" \
"os\osek\osekMTask.pp" \
"os\osek\osekMessage.pp" 

OBJS__QUOTED += \
"os\osek\cfg.obj" \
"os\osek\osekMAlarm.obj" \
"os\osek\osekMCounter.obj" \
"os\osek\osekMEvent.obj" \
"os\osek\osekMExecution.obj" \
"os\osek\osekMHook.obj" \
"os\osek\osekMInterrupt.obj" \
"os\osek\osekMKernel.obj" \
"os\osek\osekMRpcKernel.obj" \
"os\osek\osekMSpinlock.obj" \
"os\osek\osekMTarget.obj" \
"os\osek\osekMTask.obj" \
"os\osek\osekMessage.obj" 

C_SRCS__QUOTED += \
"../os/osek/cfg.c" \
"../os/osek/osekMAlarm.c" \
"../os/osek/osekMCounter.c" \
"../os/osek/osekMEvent.c" \
"../os/osek/osekMExecution.c" \
"../os/osek/osekMHook.c" \
"../os/osek/osekMInterrupt.c" \
"../os/osek/osekMKernel.c" \
"../os/osek/osekMRpcKernel.c" \
"../os/osek/osekMSpinlock.c" \
"../os/osek/osekMTarget.c" \
"../os/osek/osekMTask.c" \
"../os/osek/osekMessage.c" 


# Each subdirectory must supply rules for building sources it contributes
os/osek/cfg.obj: ../os/osek/cfg.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/cfg.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMAlarm.obj: ../os/osek/osekMAlarm.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMAlarm.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMCounter.obj: ../os/osek/osekMCounter.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMCounter.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMEvent.obj: ../os/osek/osekMEvent.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMEvent.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMExecution.obj: ../os/osek/osekMExecution.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMExecution.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMHook.obj: ../os/osek/osekMHook.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMHook.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMInterrupt.obj: ../os/osek/osekMInterrupt.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMInterrupt.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMKernel.obj: ../os/osek/osekMKernel.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMKernel.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMRpcKernel.obj: ../os/osek/osekMRpcKernel.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMRpcKernel.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMSpinlock.obj: ../os/osek/osekMSpinlock.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMSpinlock.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMTarget.obj: ../os/osek/osekMTarget.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMTarget.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMTask.obj: ../os/osek/osekMTask.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMTask.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/osek/osekMessage.obj: ../os/osek/osekMessage.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/osek/osekMessage.pp" --obj_directory="os/osek" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


