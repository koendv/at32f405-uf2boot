/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to
  synchronize tmr peripherals in cascade mode. three timers are used:

  timers synchronisation in cascade mode:
  step1: tmr2 is configured as primary timer:
  - pwm mode is used
  - the tmr2 update event is used as trigger output

  step2: tmr3 is sub for tmr2 and primary for tmr4,
  - pwm mode is used
  - the itr1(tmr2) is used as input trigger
  - gated mode is used, so start and stop of sub counter are controlled by the
    master trigger output signal(tmr2 update event).
  - the tmr3 update event is used as trigger output.

  step3: tmr4 is slave for tmr3,
  - pwm mode is used
  - the itr2(tmr3) is used as input trigger
  - gated mode is used, so start and stop of sub counter are controlled by the
    master trigger output signal(tmr3 overflow event).

  the primary timer tmr2 is running at tmr2 frequency :
  tim2 frequency = (tmr2 counter clock)/ (tmr2 period + 1) = 1 mhz
  and the duty cycle = tmr2_ccr1/(tmr2_arr + 1) = 50%.

  the tmr3 is running at:
  (tmr2 frequency)/ (tmr3 period + 1) = 100 khz and a duty cycle equal to
  tmr3_ccr1/(tmr3_arr + 1) = 50%

  the tmr4 is running at:
  (tmr3 frequency)/ (tmr4 period + 1) = 10 khz and a duty cycle equal to
  tmr4_ccr1/(tmr4_arr + 1) = 50%

  set-up
  connect the following pins to an oscilloscope to monitor the different waveforms:
  - tmr2 ch1 (pa.00)
  - tmr3 ch1 (pa.06)
  - tmr4 ch1 (pb.06) 

  for more detailed information. please refer to the application note document AN0085.
