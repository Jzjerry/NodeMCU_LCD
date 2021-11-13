# NodeMCU_LCD

## Overview
面向Vup&Vtber观众的低成本硬件小模块，目标为仅基于一个LCD屏幕和一个ESP8266开发板获取并显示包含粉丝数和开播情况等信息。

A simple monitor based on ESP8266 & TFT_eSPI for Bilibili streamer's information display.

<img src=".\pic\demo.jpg" alt="demo" style="zoom:50%;" />

画饼：将来会基于该项目和这两个元件出衍生一系列的其他小模块，例如：追番助手、弹幕姬、邮箱显示器、RSS订阅显示器...

Future Plan: Build a series of modules derived from the current one, such as Bangumi(Anime) Subscriber, Danmaku(Chat) Displayer, E-mail Box, RSS Box, etc..

## Features

+ Get basic info from Bilibili API for UID you set.
+ Update follower number per seconds.
+ Check if the liver is streaming.
+ Display information on the TFT screen.

## Usage

+ Clone this Repository.
+ Open it in VS Code & PlatformIO.
+ Create `include/wifi_config.h` and specify your wifi ssid & password.(referring `wifi_config_template.h`)
+ Change the `String UID` in `src/main.cpp` into your Bilibili UID or whoever's Bilibili UID.
+ Build it. Flash it. Done. (the wiring between ESP8266 board and LCD Screen differs)

## Changelog
### 2021/11/9
+ Add new class for user information(`class Simp`)
+ Now you can obtain the Streamer UID from your account by wearing the streamer's medal(Bilibili Fan medal)
+ Small adjustment in display
+ New includes hierarchy, trying to make it easier to port to other streaming platforms.
### 2021/11/8
+ Add new class for streamer information(`class Streamer`)
+ Code reconstruction with `Streamer`(C++ style reconstruction)
### 2021/11/2
+ Add a small animation on streaming sign
+ Sperate animation update from http information update
+ Attach animation update function to Ticker

## Hardware

+ NodeMCU (AIthinker version, Esp8266-12F)
+ 1.3' inch 240x240 TFT LCD Screen(ST7789)

## Acknowledgment

+ TFT_eSPI@Bodmer (https://github.com/Bodmer/TFT_eSPI)
+ lvgl@lvgl (https://github.com/lvgl/lvgl)
+ ArduinoJson@bblanchon (https://github.com/bblanchon/ArduinoJson)
+ bilibili-API-collect@SocialSisterYi (https://github.com/SocialSisterYi/bilibili-API-collect)