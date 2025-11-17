/**
 * @file main.cpp
 * @brief ESP32-S + ST7735 1.8" TFT 顯示專案
 * @date 2025-11-17
 * 
 * 功能：在螢幕上顯示四行文字
 * - 第 1 行：HHVS (白色)
 * - 第 2 行：IT Dep. (黃色)
 * - 第 3 行：ESP32-s (綠色)
 * - 第 4 行：ST7735 test (藍色)
 * 
 * 使用 Adafruit GFX + Adafruit ST7735 Library
 */

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "config.h"

// ==================== TFT 物件初始化 ====================
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// ==================== 函式宣告 ====================
void displayText();

// ==================== Setup ====================
void setup() {
  // 初始化 Serial（除錯用）
  Serial.begin(SERIAL_BAUD);
  Serial.println("ESP32-S ST7735 TFT 初始化中...");
  
  // 初始化背光（設為高電位，常亮）
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
  // 初始化 TFT
  tft.initR(TFT_INIT_MODE);
  Serial.println("TFT 初始化完成");
  
  // 設定螢幕旋轉方向
  tft.setRotation(TFT_ROTATION);
  
  // 清除畫面（黑色背景）
  tft.fillScreen(ST77XX_BLACK);
  
  // 顯示文字
  displayText();
  
  Serial.println("顯示完成，進入待機模式");
}

// ==================== Loop ====================
void loop() {
  // 固定顯示，不需重繪
  delay(1000);
}

// ==================== 函式定義 ====================

/**
 * @brief 顯示四行文字，置中對齊
 */
void displayText() {
  // 文字大小設定
  tft.setTextSize(2);
  
  // 取得螢幕尺寸（依旋轉方向）
  int16_t screenWidth = tft.width();
  int16_t screenHeight = tft.height();
  
  // 字元尺寸估算（size=2 時，每字元約 12x16 像素）
  int16_t charWidth = 12;
  int16_t charHeight = 16;
  int16_t lineSpacing = 16; // 行距
  
  // 計算起始 Y 座標（讓四行文字整體置中）
  int16_t totalHeight = charHeight * 4 + lineSpacing * 3;
  int16_t startY = (screenHeight - totalHeight) / 2;
  
  // ==================== 第 1 行：HHVS (白色) ====================
  String line1 = "HHVS";
  int16_t x1 = (screenWidth - line1.length() * charWidth) / 2; // 置中 X 座標
  int16_t y1 = startY; // 第 1 行 Y 座標
  
  tft.setCursor(x1, y1);
  tft.setTextColor(ST77XX_WHITE);
  tft.println(line1);
  
  // ==================== 第 2 行：IT Dep. (黃色) ====================
  String line2 = "IT Dep.";
  int16_t x2 = (screenWidth - line2.length() * charWidth) / 2;
  int16_t y2 = y1 + charHeight + lineSpacing;
  
  tft.setCursor(x2, y2);
  tft.setTextColor(ST77XX_YELLOW);
  tft.println(line2);
  
  // ==================== 第 3 行：ESP32-s (綠色) ====================
  String line3 = "ESP32-s";
  int16_t x3 = (screenWidth - line3.length() * charWidth) / 2;
  int16_t y3 = y2 + charHeight + lineSpacing;
  
  tft.setCursor(x3, y3);
  tft.setTextColor(ST77XX_GREEN);
  tft.println(line3);
  
  // ==================== 第 4 行：ST7735 test (藍色) ====================
  String line4 = "ST7735 test";
  int16_t x4 = (screenWidth - line4.length() * charWidth) / 2;
  int16_t y4 = y3 + charHeight + lineSpacing;
  
  tft.setCursor(x4, y4);
  tft.setTextColor(ST77XX_BLUE);
  tft.println(line4);
  
  Serial.println("文字顯示完成");
}