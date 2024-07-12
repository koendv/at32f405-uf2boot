/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows in halfduplex
  mode how to use interrupt transfer data.
  the pins connection as follow:
  - pc7 as mck out
  - i2s2 slaver              i2s3 master
    pb9(ws)        <--->     pa15(ws)
    pb1(sck)       <--->     pb3(sck)
    pc3(sd)        <--->     pb5(sd)

  for more detailed information. please refer to the application note document AN0102.
