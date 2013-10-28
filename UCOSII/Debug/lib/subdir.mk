################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/rb_tree.c \
../lib/string.c 

OBJS += \
./lib/rb_tree.obj \
./lib/string.obj 

C_DEPS += \
./lib/rb_tree.pp \
./lib/string.pp 

C_DEPS__QUOTED += \
"lib\rb_tree.pp" \
"lib\string.pp" 

OBJS__QUOTED += \
"lib\rb_tree.obj" \
"lib\string.obj" 

C_SRCS__QUOTED += \
"../lib/rb_tree.c" \
"../lib/string.c" 


# Each subdirectory must supply rules for building sources it contributes
lib/rb_tree.obj: ../lib/rb_tree.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="lib/rb_tree.pp" --obj_directory="lib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/string.obj: ../lib/string.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="lib/string.pp" --obj_directory="lib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


