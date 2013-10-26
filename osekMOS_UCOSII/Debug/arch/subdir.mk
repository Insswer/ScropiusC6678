################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../arch/c6678_cache.c \
../arch/c66xx_cic.c \
../arch/c66xx_corepac.c \
../arch/c66xx_exception.c \
../arch/c66xx_init.c \
../arch/c66xx_ipc.c \
../arch/c66xx_memarea.c \
../arch/c66xx_mpu.c \
../arch/c66xx_msmc.c \
../arch/c66xx_rpc.c \
../arch/c66xx_spinlock.c \
../arch/c66xx_timer.c 

ASM_SRCS += \
../arch/c66xx_itbl.asm \
../arch/c66xx_regs.asm \
../arch/c66xx_vectors.asm 

ASM_DEPS += \
./arch/c66xx_itbl.pp \
./arch/c66xx_regs.pp \
./arch/c66xx_vectors.pp 

OBJS += \
./arch/c6678_cache.obj \
./arch/c66xx_cic.obj \
./arch/c66xx_corepac.obj \
./arch/c66xx_exception.obj \
./arch/c66xx_init.obj \
./arch/c66xx_ipc.obj \
./arch/c66xx_itbl.obj \
./arch/c66xx_memarea.obj \
./arch/c66xx_mpu.obj \
./arch/c66xx_msmc.obj \
./arch/c66xx_regs.obj \
./arch/c66xx_rpc.obj \
./arch/c66xx_spinlock.obj \
./arch/c66xx_timer.obj \
./arch/c66xx_vectors.obj 

C_DEPS += \
./arch/c6678_cache.pp \
./arch/c66xx_cic.pp \
./arch/c66xx_corepac.pp \
./arch/c66xx_exception.pp \
./arch/c66xx_init.pp \
./arch/c66xx_ipc.pp \
./arch/c66xx_memarea.pp \
./arch/c66xx_mpu.pp \
./arch/c66xx_msmc.pp \
./arch/c66xx_rpc.pp \
./arch/c66xx_spinlock.pp \
./arch/c66xx_timer.pp 

ASM_DEPS__QUOTED += \
"arch\c66xx_itbl.pp" \
"arch\c66xx_regs.pp" \
"arch\c66xx_vectors.pp" 

C_DEPS__QUOTED += \
"arch\c6678_cache.pp" \
"arch\c66xx_cic.pp" \
"arch\c66xx_corepac.pp" \
"arch\c66xx_exception.pp" \
"arch\c66xx_init.pp" \
"arch\c66xx_ipc.pp" \
"arch\c66xx_memarea.pp" \
"arch\c66xx_mpu.pp" \
"arch\c66xx_msmc.pp" \
"arch\c66xx_rpc.pp" \
"arch\c66xx_spinlock.pp" \
"arch\c66xx_timer.pp" 

OBJS__QUOTED += \
"arch\c6678_cache.obj" \
"arch\c66xx_cic.obj" \
"arch\c66xx_corepac.obj" \
"arch\c66xx_exception.obj" \
"arch\c66xx_init.obj" \
"arch\c66xx_ipc.obj" \
"arch\c66xx_itbl.obj" \
"arch\c66xx_memarea.obj" \
"arch\c66xx_mpu.obj" \
"arch\c66xx_msmc.obj" \
"arch\c66xx_regs.obj" \
"arch\c66xx_rpc.obj" \
"arch\c66xx_spinlock.obj" \
"arch\c66xx_timer.obj" \
"arch\c66xx_vectors.obj" 

C_SRCS__QUOTED += \
"../arch/c6678_cache.c" \
"../arch/c66xx_cic.c" \
"../arch/c66xx_corepac.c" \
"../arch/c66xx_exception.c" \
"../arch/c66xx_init.c" \
"../arch/c66xx_ipc.c" \
"../arch/c66xx_memarea.c" \
"../arch/c66xx_mpu.c" \
"../arch/c66xx_msmc.c" \
"../arch/c66xx_rpc.c" \
"../arch/c66xx_spinlock.c" \
"../arch/c66xx_timer.c" 

ASM_SRCS__QUOTED += \
"../arch/c66xx_itbl.asm" \
"../arch/c66xx_regs.asm" \
"../arch/c66xx_vectors.asm" 


# Each subdirectory must supply rules for building sources it contributes
arch/c6678_cache.obj: ../arch/c6678_cache.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c6678_cache.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_cic.obj: ../arch/c66xx_cic.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_cic.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_corepac.obj: ../arch/c66xx_corepac.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_corepac.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_exception.obj: ../arch/c66xx_exception.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_exception.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_init.obj: ../arch/c66xx_init.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_init.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_ipc.obj: ../arch/c66xx_ipc.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_ipc.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_itbl.obj: ../arch/c66xx_itbl.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_itbl.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_memarea.obj: ../arch/c66xx_memarea.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_memarea.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_mpu.obj: ../arch/c66xx_mpu.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_mpu.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_msmc.obj: ../arch/c66xx_msmc.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_msmc.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_regs.obj: ../arch/c66xx_regs.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_regs.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_rpc.obj: ../arch/c66xx_rpc.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_rpc.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_spinlock.obj: ../arch/c66xx_spinlock.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_spinlock.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_timer.obj: ../arch/c66xx_timer.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_timer.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

arch/c66xx_vectors.obj: ../arch/c66xx_vectors.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\arch\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\demo" --include_path="H:\c6678\workspace\osekMOS_UCOSII\lib\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\test\include" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_osek" --include_path="H:\c6678\workspace\osekMOS_UCOSII\os\include\include_uc" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="arch/c66xx_vectors.pp" --obj_directory="arch" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


