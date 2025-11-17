# ESP32-S + 1.8 吋 ST7735 SPI TFT 顯示專案  
**開發計畫書（Development Plan）**  
版本：v1.2（Library 改用 Adafruit 方案）  
日期：2025-11-17  

---

## 1. 專案概要

### 1.1 專案名稱
ESP32-S ST7735 1.8" TFT 雙行文字顯示專案

### 1.2 專案目標
使用 ESP32-S 系列 MCU 搭配 1.8 吋 ST7735 SPI TFT LCD，於螢幕上固定顯示兩行文字：

- 第一行：`HHVS`  
- 第二行：`IT Dep.`  

系統上電後自動初始化顯示，無需額外輸入。

### 1.3 使用情境
- 機房或設備標示（如機櫃、伺服器、網路設備識別）。  
- 也可做為後續多頁顯示、狀態看板的基礎專案。

---

## 2. 系統架構

### 2.1 系統方塊圖（概念）

- ESP32-S MCU  
  ↕ SPI  
- ST7735 1.8" TFT LCD  

### 2.2 資料流說明
1. ESP32-S 上電。  
2. 初始化 SPI 與 ST7735（透過 Adafruit Library）。  
3. 設定畫面方向、背景色、字型、字體大小。  
4. 顯示 `HHVS` 與 `IT Dep.`。  
5. 螢幕固定輸出文字，主迴圈維持待機狀態。  

---

## 3. 硬體規格與接線

### 3.1 MCU

- 系列：ESP32-S / ESP32-S2 / ESP32-S3  
- 電壓：3.3 V I/O  
- 介面需求：  
  - 1 組 SPI（SCK、MOSI）  
  - GPIO 用於 DC、RST、CS、BL  

---

### 3.2 TFT LCD 模組（ST7735）

- 尺寸：1.8 吋  
- 解析度：128 × 160 pixels（常見規格）  
- 控制器：ST7735 / ST7735S  
- 介面：SPI  
- 腳位：VCC、GND、SCK、MOSI、RST、DC、CS、BL  

---

### 3.3 GPIO 接腳配置（依 ESP32 GPIO 原則修正）

避免：  
- Input-only 腳：34, 35, 36, 39  
- 系統保留腳：0, 6, 7, 8, 9, 10, 11  
- Boot 敏感腳：2, 12  
- DHT 不穩定腳：5, 26  

#### ✔ 最終腳位配置

| 功能         | TFT 腳位 | ESP32-S GPIO | 備註 |
|--------------|----------|--------------|------|
| VCC          | VCC      | 3.3 V        | — |
| GND          | GND      | GND          | — |
| SPI Clock    | SCL/SCK  | **GPIO 18**  | 標準 SPI SCK |
| SPI MOSI     | SDA/MOSI | **GPIO 23**  | 標準 MOSI |
| Reset        | RES/RST  | **GPIO 4**   | 安全 GPIO |
| Data/Command | DC/RS    | **GPIO 16**  | 無 Boot 功能 |
| Chip Select  | CS       | **GPIO 17**  | 適合作為 CS |
| Backlight    | BL/LED   | **GPIO 21**  | 可 PWM 或常亮 |

---

## 4. 開發環境與 Library

### 4.1 開發工具

- IDE：Visual Studio Code  
- 建構系統：PlatformIO  
- Framework：Arduino Core for ESP32  

### 4.2 使用 Library（固定採用 Adafruit 方案）

本專案 **明確採用 Adafruit 官方 Library 組合，不使用客製化 TFT_eSPI**：  

- `Adafruit_GFX`  
- `Adafruit_ST7735`  

#### 4.2.1 Library 安裝方式（建議）

1. 在 PlatformIO 的 `platformio.ini` 中加入：

```ini
lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit ST7735 and ST7789 Library
```

2. 於 `main.cpp` 中包含：

```cpp
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
```

3. 以建構子指定 CS、DC、RST 腳位，例如：

```cpp
#define TFT_CS   17
#define TFT_DC   16
#define TFT_RST  4

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
```

4. 初始化時使用對應面板設定，例如（依實際模組選擇）：  

```cpp
tft.initR(INITR_BLACKTAB); // 或 REDTAB / GREENTAB 等
```

> 實際使用參數依模組規格（常見為 BLACKTAB / GREENTAB）調整。

---

## 5. 專案結構（建議）

```text
/ESP32_ST7735_HHVS_ITDep
├─ platformio.ini
└─ src
   └─ main.cpp          # 主程式，負責初始化與顯示文字
└─ include
   └─ config.h          # 腳位定義、常數
```

---

## 6. 功能需求

### 6.1 顯示文字

- 第 1 行：`HHVS`  
- 第 2 行：`IT Dep.`  
- 第 3 行：`ESP32-s` 
- 第 4 行：`ST7735 test`
- 文字對齊方式：置中
- 字型：預設字型（Adafruit GFX 預設）
- 文字顏色:
  - 第1行: 白色 (`ST77XX_WHITE`)
  - 第2行: 黃色 (`ST77XX_YELLOW`)
  - 第3行: 綠色 (`ST77XX_GREEN`)
  - 第4行: 藍色 (`ST77XX_BLUE`)
- 建議字型大小：`setTextSize(2)`  
- 顏色：背景黑（`ST77XX_BLACK`）  

---

## 7. 系統流程

1. MCU 上電 / 重置。  
2. 初始化 Serial（115200 bps，用於除錯）。  
3. 初始化 TFT（`tft.initR(...)`、`setRotation()`）。  
4. `fillScreen(ST77XX_BLACK)` 清除畫面。  
5. 設定文字顏色與大小（`setTextColor`、`setTextSize`）。  
6. 進入 `loop()`，不需重繪畫面。  

---

## 8. 文件維護

- 本文件為 v1.2 版：  
  - Library 明確固定採用 Adafruit GFX + Adafruit ST7735。  
  - 不使用客製化 TFT_eSPI。  
- 若後續變更 Library 或面板型號，需同步更新：  
  - 第 4 章（開發環境與 Library）  
  - 以及程式中初始化參數（例如 `initR()` 模式）。  
