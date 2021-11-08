# NodeMCU_LCD
## Overview
A simple monitor for Bilibili livers based on ESP8266 & TFT_eSPI
## Features
+ Get basic info from Bilibili API for UID you set.
+ Update follower number per seconds.
+ Check if the liver is streaming.
+ Display information on the TFT screen.
## Changelog
### 2021/11/8
+ Add new class for streamer information(`class Streamer`)
+ Code reconstruction with `Streamer`(C++ style reconstruction)
### 2021/11/2
+ Add a small animation on streaming sign
+ Sperate animation update from http information update
+ Attach animation update function to Ticker