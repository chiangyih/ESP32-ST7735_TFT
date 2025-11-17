/**
 * @file config.h
 * @brief GPIO 腳位配置與系統常數定義
 * @date 2025-11-17
 * 
 * ESP32-S + ST7735 1.8" TFT 專案
 * 腳位配置避開 ESP32 保留腳與敏感腳位
 */

#ifndef CONFIG_H
#define CONFIG_H

// ==================== SPI 與 TFT 腳位定義 ====================

// SPI 標準腳位
#define TFT_SCK   18    // SPI Clock (SCL)
#define TFT_MOSI  23    // SPI MOSI (SDA)

// TFT 控制腳位
#define TFT_CS    17    // Chip Select
#define TFT_DC    16    // Data/Command (RS)
#define TFT_RST   4     // Reset

// 背光腳位
#define TFT_BL    21    // Backlight (可選用 PWM 控制亮度)

// ==================== 系統設定 ====================

// Serial 鮑率
#define SERIAL_BAUD 115200

// TFT 初始化模式 (依實際模組調整)
// 可選: INITR_BLACKTAB, INITR_GREENTAB, INITR_REDTAB
#define TFT_INIT_MODE INITR_BLACKTAB

// 螢幕旋轉方向 (0-3)
// 0: 無旋轉, 1: 順時針90度, 2: 180度, 3: 逆時針90度
#define TFT_ROTATION 3

#endif // CONFIG_H
