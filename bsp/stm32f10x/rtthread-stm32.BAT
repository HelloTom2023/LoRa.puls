SET PATH=D:\Keil\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;d:\Program Files\Git\cmd;C:\Users\snake\AppData\Local\Microsoft\WindowsApps;;d:\Program Files (x86)\SSH Communications Security\SSH Secure Shell
SET CPU_TYPE=STM32F103ZE
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=rtthread-stm32
SET CPU_CLOCK=0x00B71B00
"D:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\build\application.__i"
"D:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\build\startup.__i"
"D:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\build\board.__i"
"D:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\build\stm32f10x_it.__i"
"D:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\build\led.__i"
"D:\Keil\ARM\ARMCC\Bin\ArmCC" --Via ".\build\usart.__i"
"D:\Keil\ARM\ARMCC\Bin\ArmLink" --Via ".\build\rtthread-stm32.lnp"
