:start
esptool.exe  --baud 921600 --before default_reset --after hard_reset write_flash 0x0 pong_0x0.bin

@echo Press any key, keep finding device program
pause
goto start

