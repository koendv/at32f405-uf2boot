/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, pa6, pa8 and pb6
  output pwm waveform.
  the master tmrer tmr1 is running at:
  tmr1 frequency = tmr1 counter clock / (tmr1_period + 1) = 750 khz
  and the duty cycle is equal to: tmr1_ccr1/(tmr1_arr + 1) = 50%

  the tmr3 is running at:
  (tmr1 frequency)/ ((tmr3 period +1)* (repetition_counter+1)) = 50 khz and
  a duty cycle equal to tmr3_ccr1/(tmr3_arr + 1) = 33.3%

  the tmr4 is running at:
  (tmr1 frequency)/ ((tmr4 period +1)* (repetition_counter+1)) = 75 khz and
  a duty cycle equal to tmr4_ccr1/(tmr4_arr + 1) = 50%

  for more detailed information. please refer to the application note document AN0085.

