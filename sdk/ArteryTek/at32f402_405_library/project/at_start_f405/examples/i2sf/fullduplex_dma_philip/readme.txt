/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to constitute
  a fullduplex i2s module by i2sf module.
  the pins distribution as follow:
  - fullduplex i2s
  - i2sf5 master        spi i2s slaver
  - pb6(ws)      <--->     pa4(ws)
  - pb7(ck)      <--->     pa5(ck) 
  - pb9(sd)      <--->     pa7(sd)
  - pb8(sdext)   <--->     pb4(sdext)
  - pc8(mck) 

  for more detailed information. please refer to the application note document AN0102.
