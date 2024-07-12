/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows in halfduplex
  mode how to use polling transfer data. use the mode switch to realize spi and
  i2s communication.
  the pins connection as follow:
  - spi2/i2s2                spi3/i2s3 
    pb9(ws)        <--->     pa15(ws)
    pb1(sck)       <--->     pb3(sck)
    pc3(sd)        <--->     pb5(sd)

  for more detailed information. please refer to the application note document AN0102.
