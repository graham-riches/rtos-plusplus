################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include Src/HWM/USART/subdir.mk
-include Src/HWM/System/subdir.mk
-include Src/HWM/SPI/subdir.mk
-include Src/HWM/I2S/subdir.mk
-include Src/HWM/I2C/subdir.mk
-include Src/HWM/HAL/subdir.mk
-include Src/HWM/GPIO/subdir.mk
-include Src/HWM/DMA/subdir.mk
-include Src/HWM/ADC/subdir.mk
-include Src/APP/Utilities/subdir.mk
-include Src/APP/Events/subdir.mk
-include Src/APP/Debug/subdir.mk
-include Src/APP/Audio/subdir.mk
-include Src/APP/Accelerometer/subdir.mk
-include Src/APP/subdir.mk
-include Drivers/STM32F4xx_StdPeriph_Driver/src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: STM32_discovery_test.elf

# Tool invocations
STM32_discovery_test.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU G++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -L"D:\Code\C\stm32-discovery\Lib" -specs=nosys.specs -specs=nano.specs -u _printf_float -T"../STM32F407VGTx_FLASH.ld" -Wl,-Map=output.map -Wl,--gc-sections -fno-exceptions -fno-rtti -lm -o "STM32_discovery_test.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) STM32_discovery_test.elf
	-@echo ' '

post-build:
	-@echo 'Generating hex and Printing size information:'
	-arm-none-eabi-objcopy -O ihex "STM32_discovery_test.elf" "STM32_discovery_test.hex" && arm-none-eabi-size "STM32_discovery_test.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
