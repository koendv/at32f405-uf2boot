/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to configur
  the tmr peripheral in output high mode with the corresponding interrupt requests
  for each channel.

  tmr2 configuration:
  tmr2clk = system_core_clock / 8,
  the objective is to get tmr2 counter clock at 1 khz:
  - prescaler = (tim2clk / tmr2 counter clock) - 1
  and generate 4 signals with 4 different delays:
  tmr2_ch1 delay = ccr1_val/tmr2 counter clock = 1000 ms
  tmr2_ch2 delay = ccr2_val/tmr2 counter clock = 500 ms
  tmr2_ch3 delay = ccr3_val/tmr2 counter clock = 250 ms
  tmr2_ch4 delay = ccr4_val/tmr2 counter clock = 125 ms

  connect the following pins to an oscilloscope to monitor the different waveforms:
  - pc6
  - pc7
  - pc8
  - pc9

  for more detailed information. please refer to the application note document AN0085.
