################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../os/ucos/core/OS_CORE.C \
../os/ucos/core/OS_FLAG.C \
../os/ucos/core/OS_MBOX.C \
../os/ucos/core/OS_MEM.C \
../os/ucos/core/OS_MUTEX.C \
../os/ucos/core/OS_Q.C \
../os/ucos/core/OS_SEM.C \
../os/ucos/core/OS_TASK.C \
../os/ucos/core/OS_TIME.C 

OBJS += \
./os/ucos/core/OS_CORE.obj \
./os/ucos/core/OS_FLAG.obj \
./os/ucos/core/OS_MBOX.obj \
./os/ucos/core/OS_MEM.obj \
./os/ucos/core/OS_MUTEX.obj \
./os/ucos/core/OS_Q.obj \
./os/ucos/core/OS_SEM.obj \
./os/ucos/core/OS_TASK.obj \
./os/ucos/core/OS_TIME.obj 

C_UPPER_DEPS += \
./os/ucos/core/OS_CORE.pp \
./os/ucos/core/OS_FLAG.pp \
./os/ucos/core/OS_MBOX.pp \
./os/ucos/core/OS_MEM.pp \
./os/ucos/core/OS_MUTEX.pp \
./os/ucos/core/OS_Q.pp \
./os/ucos/core/OS_SEM.pp \
./os/ucos/core/OS_TASK.pp \
./os/ucos/core/OS_TIME.pp 

C_UPPER_DEPS__QUOTED += \
"os\ucos\core\OS_CORE.pp" \
"os\ucos\core\OS_FLAG.pp" \
"os\ucos\core\OS_MBOX.pp" \
"os\ucos\core\OS_MEM.pp" \
"os\ucos\core\OS_MUTEX.pp" \
"os\ucos\core\OS_Q.pp" \
"os\ucos\core\OS_SEM.pp" \
"os\ucos\core\OS_TASK.pp" \
"os\ucos\core\OS_TIME.pp" 

OBJS__QUOTED += \
"os\ucos\core\OS_CORE.obj" \
"os\ucos\core\OS_FLAG.obj" \
"os\ucos\core\OS_MBOX.obj" \
"os\ucos\core\OS_MEM.obj" \
"os\ucos\core\OS_MUTEX.obj" \
"os\ucos\core\OS_Q.obj" \
"os\ucos\core\OS_SEM.obj" \
"os\ucos\core\OS_TASK.obj" \
"os\ucos\core\OS_TIME.obj" 

C_UPPER_SRCS__QUOTED += \
"../os/ucos/core/OS_CORE.C" \
"../os/ucos/core/OS_FLAG.C" \
"../os/ucos/core/OS_MBOX.C" \
"../os/ucos/core/OS_MEM.C" \
"../os/ucos/core/OS_MUTEX.C" \
"../os/ucos/core/OS_Q.C" \
"../os/ucos/core/OS_SEM.C" \
"../os/ucos/core/OS_TASK.C" \
"../os/ucos/core/OS_TIME.C" 


# Each subdirectory must supply rules for building sources it contributes
os/ucos/core/OS_CORE.obj: ../os/ucos/core/OS_CORE.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/core/OS_CORE.pp" --obj_directory="os/ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/ucos/core/OS_FLAG.obj: ../os/ucos/core/OS_FLAG.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/core/OS_FLAG.pp" --obj_directory="os/ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/ucos/core/OS_MBOX.obj: ../os/ucos/core/OS_MBOX.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/core/OS_MBOX.pp" --obj_directory="os/ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/ucos/core/OS_MEM.obj: ../os/ucos/core/OS_MEM.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/core/OS_MEM.pp" --obj_directory="os/ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/ucos/core/OS_MUTEX.obj: ../os/ucos/core/OS_MUTEX.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/core/OS_MUTEX.pp" --obj_directory="os/ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/ucos/core/OS_Q.obj: ../os/ucos/core/OS_Q.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/core/OS_Q.pp" --obj_directory="os/ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/ucos/core/OS_SEM.obj: ../os/ucos/core/OS_SEM.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/core/OS_SEM.pp" --obj_directory="os/ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/ucos/core/OS_TASK.obj: ../os/ucos/core/OS_TASK.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/core/OS_TASK.pp" --obj_directory="os/ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

os/ucos/core/OS_TIME.obj: ../os/ucos/core/OS_TIME.C $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="os/ucos/core/OS_TIME.pp" --obj_directory="os/ucos/core" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


