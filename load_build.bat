openocd -f stm32f407.cfg -c "program Build/stm32_discovery.elf verify reset exit"