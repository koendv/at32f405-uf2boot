This is a bootloader for [arm_can_tool](http://oshwlab.com/koendv/arm_can_tool)

Processor is AT32F405RCT7-7, with 16Mbyte qspi execute-in-place flash running at 108MHz.

Differences with at32f405rcu7_qspi_hs:

- The LED is at pin PC2.
- The user button to start the boot loader is pin PB12.
  Pin PB12 is the center contact of the joystick.
- Console output is UART6, TX on pin PC6.

## elf file

To build:

```
$ cd at32f405-uf2boot/ports/ArteryTek/at32f402_405
$ make BOARD=arm_can_tool clean
$ make BOARD=arm_can_tool all
```

This gives .elf and .bin files.

## dfu file

To convert elf to dfu, first build the `elf2dfuse` tool:

```
cd at32f405-uf2boot/ports/ArteryTek/elf2dfuse
make
```

Then convert .elf to .dfu

```
cd at32f405-uf2boot/ports/ArteryTek/at32f402_405/.build/arm_can_tool/
elf2dfuse cherryuf2_arm_can_tool.elf cherryuf2_arm_can_tool.dfu
```

This creates the file `cherryuf2_arm_can_tool.dfu` file in the directory `.build/arm_can_tool`

## installing the bootloader

Install the bootloader 

- with a debugger such as openocd or black magic probe, using SWD

- with the `dfu-util` command, using USB

### install bootloader with dfu-util

- press _boot_ and _reset_ buttons at the same time

- wait one second

- release _reset_ button, keep pressing _boot_ button

- wait one second

- release _boot_ button

- on linux, type `lsusb` to see the usb devices. The board should appear as a DFU device: _2e3c:df11 Artery-Tech DFU in FS Mode_

- With the board in DFU, use the following command:

```shell
sudo dfu-util -a 0  -R -D cherryuf2_arm_can_tool.dfu
```

The terminal shows

```
dfu-util 0.11

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2021 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to http://sourceforge.net/p/dfu-util/tickets/

Match vendor ID from file: 2e3c
Match product ID from file: df11
Multiple alternate interfaces for DfuSe file
Opening DFU capable USB device...
Device ID 2e3c:df11
Device DFU version 011a
Claiming USB DFU Interface...
Setting Alternate Interface #0 ...
Determining device status...
DFU state(2) = dfuIDLE, status(0) = No error condition is present
DFU mode device DFU version 011a
Device returned transfer size 2048
DfuSe interface name: "Internal Flash   "
File contains 1 DFU images
Parsing DFU image 1
No target name
Image for alternate setting 0, (1 elements, total size = 26328)
Setting Alternate Interface #0 ...
Parsing element 1, address = 0x08000000, size = 26320
Erase   	[=========================] 100%        26320 bytes
Erase    done.
Download	[=========================] 100%        26320 bytes
Download done.
```

This installs the bootloader in the at32f405 flash.

## updating board firmware

Once the _uf2boot_ bootloader installed, firmware in uf2 format can be written to qspi flash.

To update board firmware in qspi flash:

- connect the board to usb
- push the _reset_ button
- push the small joystick down
- wait one second
- reselese the _reset_ button
- usb storage appears in the pc. The usb storage contains a file _CURRENT.UF2_
- release the joystick
- copy the new firmware in UF2 format to the file CURRENT.UF2 in the usb storage
  
  ```shell
  cp firmware.uf2 /media/$USER/CherryUF2/CURRENT.UF2
  ```

This completes updating the firmware
