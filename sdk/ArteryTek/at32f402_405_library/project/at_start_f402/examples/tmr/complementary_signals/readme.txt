/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to configure
  the tmr1 peripheral to generate three complementary tmr1 signals, to insert
  a defined dead time value, to use the brake feature and to lock the desired
  parameters.

  tmr1 configuration to:

  step1: generate 3 complementary pwm signals with 3 different duty cycles:
         tim1clk is fixed to apb2_freq, the tmr1 prescaler is equal to
         0 so the tmr1 counter clock used is apb2_freq.

  the objective is to generate pwm signal at 17.57 khz:
  - tim1_period = (apb2_freq / 17570) - 1

  the three duty cycles are computed as the following description:

  the channel 1 duty cycle is set to 50% so channel 1n is set to 50%.
  the channel 2 duty cycle is set to 25% so channel 2n is set to 75%.
  the channel 3 duty cycle is set to 12.5% so channel 3n is set to 87.5%.
  the timer pulse is calculated as follows:
  - channelxpulse = dutycycle * (tim1_period - 1) / 100

  step2: insert a dead time equal to 11/apb2_freq ns
  step3: configure the brake feature, active at high level, and using the automatic
         output enable feature
  step4:  use the locking parameters level1.

  the tmr1 waveform can be displayed using an oscilloscope.

  set-up
  connect the tmr1 pins to an oscilloscope to monitor the different waveforms:
  - tmr1_ch1  pin (pa.08)
  - tmr1_ch1c pin (pa.7)
  - tmr1_ch2  pin (pa.09)
  - tmr1_ch2c pin (pb.0)
  - tmr1_ch3  pin (pa.10)
  - tmr1_ch3c pin (pb.1)

- connect the tmr1 brake pin tmr1_stpin pin (pb.12) to the gnd. to generate a
  brake event, switch this pin level from 0v to 3.3v.

  for more detailed information. please refer to the application note document AN0085.

