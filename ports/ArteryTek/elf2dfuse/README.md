elf2dfuse
=========

This tool is a possible aid for STM32 developers who want to generate a DfuSe image directly from a STM32 ELF object file.

## Limitations

DFU (and DfuSe) images must encode the VID:PID of the target being programmed.  The source code has #defines for the VID:PID to be 0x2e3c:0xdf11 (Artery-Tech DFU in FS Mode)

## Sample Usage

```
elf2dfuse myapp.elf myapp.dfu
```

