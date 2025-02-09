/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to configure
  the tmr1 peripheral to generate 7 pwm signals with 4 different duty cycles
  (50%, 37.5%, 25% and 12.5%).

  the objective is to generate 7 pwm signal at 17.57 khz:
  - tim1_period = (apb2_freq / 17570) - 1
  the channel 1 and channel 1n duty cycle is set to 50%
  the channel 2 and channel 2n duty cycle is set to 37.5%
  the channel 3 and channel 3n duty cycle is set to 25%
  the channel 4 duty cycle is set to 12.5%
  the timer pulse is calculated as follows:
  - channelxpulse = dutycycle * (tim1_period - 1) / 100
  the tim1 waveform can be displayed using an oscilloscope.

  set-up
  connect the tmr1 pins to an oscilloscope to monitor the different waveforms:
  - tmr1_ch1  --->  pa8
  - tmr1_ch1c --->  pa7
  - tmr1_ch2  --->  pa9
  - tmr1_ch2c --->  pb0
  - tmr1_ch3  --->  pa10
  - tmr1_ch3c --->  pb1
  - tmr1_ch4  --->  pa11

  for more detailed information. please refer to the application note document AN0085.

