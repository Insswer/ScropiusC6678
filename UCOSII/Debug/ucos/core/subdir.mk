################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../ucos/core/OS_CORE.C \
../ucos/core/OS_FLAG.C \
../ucos/core/OS_MBOX.C \
../ucos/core/OS_MEM.C \
../ucos/core/OS_MUTEX.C \
../ucos/core/OS_Q.C \
../ucos/core/OS_SEM.C \
../ucos/core/OS_TASK.C \
../ucos/core/OS_TIME.C 

OBJS += \
./ucos/core/OS_CORE.obj \
./ucos/core/OS_FLAG.obj \
./ucos/core/OS_MBOX.obj \
./ucos/core/OS_MEM.obj \
./ucos/core/OS_MUTEX.obj \
./ucos/core/OS_Q.obj \
./ucos/core/OS_SEM.obj \
./ucos/core/OS_TASK.obj \
./ucos/core/OS_TIME.obj 

C_UPPER_DEPS += \
./ucos/core/OS_CORE.pp \
./ucos/core/OS_FLAG.pp \
./ucos/core/OS_MBOX.pp \
./ucos/core/OS_MEM.pp \
./ucos/core/OS_MUTEX.pp \
./ucos/core/OS_Q.pp \
./ucos/core/OS_SEM.pp \
./ucos/core/OS_TASK.pp \
./ucos/core/OS_TIME.pp 

C_UPPER_DEPS__QUOTED += \
"ucos\core\OS_CORE.pp" \
"ucos\core\OS_FLAG.pp" \
"ucos\core\OS_MBOX.pp" \
"ucos\core\OS_MEM.pp" \
"ucos\core\OS_MUTEX.pp" \
"ucos\core\OS_Q.pp" \
"ucos\core\OS_SEM.pp" \
"ucos\core\OS_TASK.pp" \
"ucos\core\OS_TIME.pp" 

OBJS__QUOTED += \
"ucos\core\OS_CORE.obj" \
"ucos\core\OS_FLAG.obj" \
"ucos\core\OS_MBOX.obj" \
"ucos\core\OS_MEM.obj" \
"ucos\core\OS_MUTEX.obj" \
"ucos\core\OS_Q.obj" \
"ucos\core\OS_SEM.obj" \
"ucos\core\OS_TASK.obj" \
"ucos\core\OS_TIME.obj" 

C_UPPER_SRCS__QUOTED += \
"../ucos/core/OS_CORE.C" \
"../ucos/core/OS_FLAG.C" \
"../ucos/core/OS_MBOX.C" \
"../ucos/core/OS_MEM.C" \
"../ucos/core/OS_MUTEX.C" \
"../ucos/core/OS_Q.C" \
"../ucos/core/OS_SEM.C" \
"../ucos/core/OS_TASK.C" \
"../ucos/core/OS_TIME.C" 


# Each subdirectory must supply rules for building sources it contributes
ucos/core/OS_CORE.obj: ../ucos/core/OS_CORE.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/core/OS_CORE.pp" --obj_directory="ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/core/OS_FLAG.obj: ../ucos/core/OS_FLAG.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/core/OS_FLAG.pp" --obj_directory="ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/core/OS_MBOX.obj: ../ucos/core/OS_MBOX.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/core/OS_MBOX.pp" --obj_directory="ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/core/OS_MEM.obj: ../ucos/core/OS_MEM.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/core/OS_MEM.pp" --obj_directory="ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/core/OS_MUTEX.obj: ../ucos/core/OS_MUTEX.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/core/OS_MUTEX.pp" --obj_directory="ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/core/OS_Q.obj: ../ucos/core/OS_Q.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/core/OS_Q.pp" --obj_directory="ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/core/OS_SEM.obj: ../ucos/core/OS_SEM.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/core/OS_SEM.pp" --obj_directory="ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/core/OS_TASK.obj: ../ucos/core/OS_TASK.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/core/OS_TASK.pp" --obj_directory="ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ucos/core/OS_TIME.obj: ../ucos/core/OS_TIME.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="ucos/core/OS_TIME.pp" --obj_directory="ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


