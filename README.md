# at32f405 uf2 boot

This is a uf2 bootloader for the Artery at32f405 arm processor with the application program on external flash.

## Comparison

Compared to internal flash, external flash allows bigger programs at lower cost.

The bootloader has two functions:

- upload programs to external flash
- run programs from external flash

To upload programs to external flash, the bootloader presents itself as a usb disk drive. Files copied to the disk drive are written to flash.

To run programs from external flash, a read from main memory is translated to a read from external flash. This is called eXecute In Place (XIP). Processor cache memory is used to reduce the number of reads.

Processor internal flash:

- fast
- small (256kbyte)
- expensive

External flash:

- slower
- bigger (16mbyte)
- cheaper

## Use

This is a uf2 bootloader for the Artery at32f405 arm processor with Winbond W25Q128JV 16mbyte qspi external flash and high-speed usb.

Connect the at32f405 high-speed usb to a pc. Double-click the reset button to enter device firmware update (DFU).
The bootloader appears as a usb flash drive. The usb flash drive contains the file CURRENT.UF2.
Two actions are defined: reading and writing.

- Copy the file CURRENT.UF2 to your pc to read qspi flash data.
- Copy firmware in uf2 format to CURRENT.UF2 to write firmware to qspi flash. The board reboots when flashing is complete.

## Example app

The [example](example/README.md) directory contains an app to blink a led. 

## Installation

The uf2 bootloader for the external flash is installed in the internal flash.

Install the bootloader using a debugger, or using [dfu-util](https://dfu-util.sourceforge.net/). To install using dfu-util:

- connect the board to usb
- push reset and boot0 buttons
- wait one second
- release the reset button
- wait one second
- release the boot0 button

At this point, the at32f405 ought to be in DFU mode:
```
$ lsusb
...
Bus 003 Device 012: ID 2e3c:df11 Artery-Tech DFU in FS Mode
```

With the firmware ``cherryuf2_at32f405rcu7_qspi_hs.bin`` in your current directory, execute the following command:

```
dfu-util -a 0 -d 2e3c:df11 --dfuse-address 0x08000000 -D cherryuf2_at32f405rcu7_qspi_hs.bin
...
Erase   	[=========================] 100%        23692 bytes
Erase    done.
Download	[=========================] 100%        23692 bytes
Download done.
File downloaded successfully
```
Push the reset button twice in quick succession. The uf2 bootloader usb disk appears.

Linux users: If dfu-util does not see the at32, execute dfu-util as root with ``sudo`` or set up udev-rules:

```
# Artery AT32 DFU
# Copy this file to /etc/udev/rules.d/99-at32.rules and run: sudo udevadm control -R
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2e3c", ATTRS{idProduct}=="df11", TAG+="uaccess", MODE="0664", GROUP="plugdev"
```

Windows users: If dfu-util does not see the at32, download [zadig](https://zadig.akeo.ie/), install the WinUSB driver and run dfu-util again.

## Building

To build the bootloader:

```
cd src
git clone https://github.com/koendv/at32f405-uf2boot
cd  at32f405-uf2boot/ports/ArteryTek/at32f402_405
make BOARD=at32f405rcu7_qspi_hs clean
make BOARD=at32f405rcu7_qspi_hs all
```
The binaries are in the directory ``.build``:
```
$ ls ~/src/at32f405-uf2boot/ports/ArteryTek/at32f402_405/.build/at32f405rcu7_qspi_hs
cherryuf2_at32f405rcu7_qspi_hs.bin
cherryuf2_at32f405rcu7_qspi_hs.elf
cherryuf2_at32f405rcu7_qspi_hs.elf.map
cherryuf2_at32f405rcu7_qspi_hs.hex
```

There are many possible pinouts to connect a qspi flash to a processor.
The source files assume the pinout of this [board](https://oshwlab.com/koendv/at32f405-tool).

Your board may use different pins to connect the qspi flash.
To adapt the code to your board:

- in AT32 Work Bench, configure at least clocks, usb and qspi
- in AT32 Work Bench, click "Generate Code"
- put  wk_system_clock_config() and wk_qspi1_init() from AT32F405RCT7_WorkBench/project/src/at32f402_405_wk_config.c in the file at32f405-uf2boot/ports/ArteryTek/at32f402_405/boards/at32f405rcu7_qspi_hs/wk_config.c

Console is on the serial port, pin A9. Output in dfu mode:

```
dfu
[SFUD]Found a Winbond flash chip. Size is 16777216 bytes.
[SFUD]qspi1 flash device initialized successfully.
```

Output when booting the app:

```
app

 \ | /
- RT -     Thread Operating System
 / | \     5.2.0 build Jul 14 2024 14:02:50
 2006 - 2024 Copyright by RT-Thread team
```

The bootloader has been compiled with [xpack-arm-none-eabi-gcc-12.3.1-1.2](https://xpack.github.io/) toolchain.

## Sources

Notes about the source files.

The software defines two ways to enter the bootloader:

- double-clicking the reset button
- pushing the "user" button when the system boots

For more detailed output, uncomment SFUD_DEBUG_MODE in sfud_cfg.h

The bootloader has been tested with a Winbond W25Q128JV flash chip. If using a different flash chip:

- flash sector size and sector count is defined in board.h
- flash chips are defined in sfud_flash_def.h.
- the qspi read command is in qspi1_xip.c.

The [AT_START_F405](https://www.arterychip.com/en/product/AT32F405.jsp) board uses EN25QH128A-104H flash. If the EN25QH128A-104H flash chip is not recognized, add a line to SFUD_FLASH_CHIP_TABLE in  sfud_flash_def.h:
```
{"EN25QH128A", SFUD_MF_ID_EON, 0x70, 0x18, 16L*1024L*1024L, SFUD_WM_PAGE_256B, 4096, 0x20}, \
```

The bootloader combines [CherryUF2](https://github.com/zhaqian12/Cherryuf2) and [SFUD](https://github.com/armink/SFUD). Differences with upstream CherryUF2:

- [at32f405rcu7_qspi_hs](https://github.com/koendv/at32f405-uf2boot/tree/main/ports/ArteryTek/at32f402_405/boards/at32f405rcu7_qspi_hs): board definition
- [sfud](https://github.com/armink/SFUD): add Serial Flash Universal Driver
- [tinyuf2](https://github.com/adafruit/tinyuf2): update to allow UF2 files bigger than 4mb (ghostfat.c)
- [CherryUSB](https://github.com/cherry-embedded/CherryUSB): SystemCoreClock type change (usb_dc_dwc2.c)
- [at32 sdk](https://github.com/ArteryTek/AT32F402_405_Firmware_Library): install latest qspi drivers to allow "auto input sampling phase correction" (at32f402_405_qspi.c)
