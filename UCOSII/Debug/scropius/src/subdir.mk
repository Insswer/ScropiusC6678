################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../scropius/src/main.c \
../scropius/src/sc_int.c \
../scropius/src/sc_os.c 

OBJS += \
./scropius/src/main.obj \
./scropius/src/sc_int.obj \
./scropius/src/sc_os.obj 

C_DEPS += \
./scropius/src/main.pp \
./scropius/src/sc_int.pp \
./scropius/src/sc_os.pp 

C_DEPS__QUOTED += \
"scropius\src\main.pp" \
"scropius\src\sc_int.pp" \
"scropius\src\sc_os.pp" 

OBJS__QUOTED += \
"scropius\src\main.obj" \
"scropius\src\sc_int.obj" \
"scropius\src\sc_os.obj" 

C_SRCS__QUOTED += \
"../scropius/src/main.c" \
"../scropius/src/sc_int.c" \
"../scropius/src/sc_os.c" 


# Each subdirectory must supply rules for building sources it contributes
scropius/src/main.obj: ../scropius/src/main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="scropius/src/main.pp" --obj_directory="scropius/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scropius/src/sc_int.obj: ../scropius/src/sc_int.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="scropius/src/sc_int.pp" --obj_directory="scropius/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scropius/src/sc_os.obj: ../scropius/src/sc_os.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"H:/c6678/ccs/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6600 -g --gcc --include_path="H:/c6678/ccs/ccsv5/tools/compiler/c6000/include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\include" --include_path="H:\c6678\workspace\UCOSII_Double\lib\include" --include_path="H:\c6678\workspace\UCOSII_Double\ucos\include_uc" --include_path="H:\c6678\workspace\UCOSII_Double\scropius\include" --include_path="H:\c6678\workspace\UCOSII_Double\arch\driver\include" --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="scropius/src/sc_os.pp" --obj_directory="scropius/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


