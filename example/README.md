## Blink

This is an example application that blinks a LED.
The operating system used is rt-thread.

## source

The difference between xip and running from internal flash is that qspi flash is at address 0x90000000, while internal flash is at 0x08000000. The source needs to:

- link the app at address 0x90000000
- set the interrupt vector to 0x90000000.

In the file blink/board/linker_scripts/link.lds:

```
{
    ROM (rx) : ORIGIN = 0x90000000, LENGTH =  16M /* 16MB flash */
    RAM (rw) : ORIGIN = 0x20000000, LENGTH =  96k /* 96KB sram */
}
```
In the file blink/board/src/board.c:

```
void system_clock_config(void)
{
  /*  Set interrupt vector table address */
  SCB->VTOR = 0x90000000;

```

Check the led pin used in the file applications/main.c:

```
#define LED0_PIN    GET_PIN(C, 8)
```
And we are ready to build the app.

## build

To build the example app, first clone rt-thread

```
git clone https://github.com/RT-Thread/rt-thread
```

Copy the blink directory to rt-thread/bsp/at32/

In the directory rt-thread/bsp/at32/blink, run the following commands:

```
scons -c
scons
```

This produces a binary file rtthread.elf. To convert this file to the uf2 format the bootloader understands, first convert the .elf file to binary:

```
arm-none-eabi-objcopy -O binary rtthread.elf rtthread.bin
```

This produces a file rtthread.bin. To convert this binary file to uf2, use [uf2utils](https://github.com/josuah/uf2-utils):

```
bin2uf2 -f 0xf35c900d -o rtthread.uf2 0x90000000 rtthread.bin
```

In this command, 0xf35c900d is the "family id", and 0x90000000 is the load address. The "family id" is the same value as the UF2_FAMILY_ID in the bootloader file port.mk. The "family id" is an arbitrary value upon which application and bootloader agree.

It is possible to inspect the .uf2 file:

```
$ uf2dump rtthread.uf2 | less
...
uint32_t targetAddr=0x90000000
...
0x90000000: DC 06 00 20 69 07 00 90 B1 07 00 90 CD 02 00 90 B1 07 00 90 B1 07 00 90 B1 07 00 90 00 00 00 00
```

The firmware loads at 0x90000000 and begins with the interrupt vector. The interrupt vector is an array of pointers to functions. The second element of the array (here: 0x90000769) is the address of the application entry point, Reset_Handler. 0x90000769 is an odd number. This means the handler begins at 0x90000768, and is thumb code.

## flash

With the bootloader installed in the at32f405 board, double-click the reset button. A usb disk connects. To flash the firmware, copy the .uf2 file to the usb disk:

```
cp rtthread.uf2 /media/koen/CherryUF2/CURRENT.UF2
```

The led of the at32f405 board now blinks, and the console has a shell prompt:

```
dfu                                                                        
[SFUD]Found a Winbond flash chip. Size is 16777216 bytes.                       
[SFUD]qspi1 flash device initialized successfully.                              
...............                                                                 
app                                                                             
                                                                                
 \ | /                                                                          
- RT -     Thread Operating System                                              
 / | \     5.2.0 build Jul 18 2024 08:26:56                                     
 2006 - 2024 Copyright by RT-Thread team                                        
msh >
```
