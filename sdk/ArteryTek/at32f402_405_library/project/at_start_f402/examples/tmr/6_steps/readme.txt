/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to configure
  the tmr1 peripheral to generate 6 steps. a software com event is generated
  each 100 ms: using the systick interrupt. the brake polarity is used at high level.
  the following table describes the tmr1 channels states:
              -----------------------------------------------
             | step1 | step2 | step3 | step4 | step5 | step6 |
   ----------------------------------------------------------
  |channel1  |   1   |   0   |   0   |   0   |   0   |   1   |
   ----------------------------------------------------------
  |channel1n |   0   |   0   |   1   |   1   |   0   |   0   |
   ----------------------------------------------------------
  |channel2  |   0   |   0   |   0   |   1   |   1   |   0   |
   ----------------------------------------------------------
  |channel2n |   1   |   1   |   0   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |channel3  |   0   |   1   |   1   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |channel3n |   0   |   0   |   0   |   0   |   1   |   1   |
   ----------------------------------------------------------
  set-up
  connect the tmr1 pins to an oscilloscope to monitor the different waveforms:
  - tmr1_ch3  ---> pa10
  - tmr1_ch1c ---> pa7
  - tmr1_ch2  ---> pa9
  - tmr1_ch3c ---> pb1
  - tmr1_ch1  ---> pa8
  - tmr1_ch2c ---> pb0

  for more detailed information. please refer to the application note document AN0085.


