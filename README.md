# ESP32-S + ST7735 1.8" TFT 範例

本專案示範使用 **ESP32-S 系列開發板** 搭配 **1.8 吋 ST7735 SPI TFT LCD**，
在螢幕上以固定格式顯示四行文字：

1. `HHVS`  （白色）  
2. `IT Dep.`  （黃色）  
3. `ESP32-s`  （綠色）  
4. `ST7735 test`  （藍色）  

顯示內容在上電後自動初始化並顯示，主迴圈中不再重繪畫面，適合作為後續資訊看板或狀態頁的基礎範例。

---

## 1. 專案結構

專案主要結構如下：

```text
ESP32-ST7735_TFT/
├─ platformio.ini          # PlatformIO 設定檔，指定開發板與函式庫
├─ README.md               # 本說明文件
├─ ESP32_ST7735_HHVS_ITDep_Plan_v1.2.md  # 開發計畫書
├─ include/
│  └─ config.h             # GPIO 腳位與系統參數設定
├─ src/
│  └─ main.cpp             # 範例主程式
└─ test/
   └─ README
```

### 1.1 `platformio.ini`

```ini
[env:nodemcu-32s]
platform = espressif32
board = nodemcu-32s
framework = arduino
lib_deps = 
    adafruit/Adafruit GFX Library
    adafruit/Adafruit ST7735 and ST7789 Library
```

- `board = nodemcu-32s`：使用 ESP32 開發板（NodeMCU-32S）。  
- `lib_deps`：指定使用 **Adafruit GFX** 與 **Adafruit ST7735/ST7789** 函式庫，不使用 TFT_eSPI。

### 1.2 `include/config.h`

`config.h` 定義了所有硬體相關參數與系統設定，例如：

```cpp
#define TFT_SCK   18   // SPI Clock
#define TFT_MOSI  23   // SPI MOSI
#define TFT_CS    17   // Chip Select
#define TFT_DC    16   // Data/Command
#define TFT_RST   4    // Reset
#define TFT_BL    21   // Backlight

#define SERIAL_BAUD 115200
#define TFT_INIT_MODE INITR_BLACKTAB
#define TFT_ROTATION 1
```

這些設定對應到實際硬體接線與螢幕初始化方式，若您的模組為其他版本（如 REDTAB / GREENTAB），
可以依據面板規格調整 `TFT_INIT_MODE` 或 `TFT_ROTATION`。

### 1.3 `src/main.cpp`

`main.cpp` 為本範例的核心程式碼，負責：

- 初始化 Serial（除錯訊息輸出）。
- 初始化 TFT 顯示器與背光腳位。
- 設定螢幕方向與背景色。
- 計算文字置中的座標，顯示四行不同顏色的文字。

---

## 2. 硬體需求與接線方式

### 2.1 必要硬體

- ESP32-S 系列開發板（例如 NodeMCU-32S）。
- 1.8 吋 ST7735 SPI TFT LCD 模組。
- Micro USB 連接線（用於燒錄與供電）。

### 2.2 接線說明（ESP32 ↔ TFT）

根據 `config.h` 的設定，接線如下：

| 功能         | TFT 腳位 | ESP32 GPIO | 備註            |
|--------------|----------|------------|-----------------|
| 電源         | VCC      | 3.3V       | 模組供電        |
| 接地         | GND      | GND        | 共地            |
| SPI Clock    | SCK/SCL  | GPIO 18    | `TFT_SCK`       |
| SPI MOSI     | SDA/MOSI | GPIO 23    | `TFT_MOSI`      |
| Reset        | RST/RES  | GPIO 4     | `TFT_RST`       |
| 資料/指令    | DC/RS    | GPIO 16    | `TFT_DC`        |
| 晶片選擇     | CS       | GPIO 17    | `TFT_CS`        |
| 背光         | BL/LED   | GPIO 21    | `TFT_BL`，可 PWM |

> 注意：請避免使用 ESP32 的保留腳或 Boot 敏感腳位（例如 GPIO 0、2、12、34~39 等）。

---

## 3. 開發環境與建置方式

本專案使用 **PlatformIO + Arduino framework**：

### 3.1 安裝環境

1. 安裝 Visual Studio Code。  
2. 安裝 VS Code 擴充套件 **PlatformIO IDE**。  
3. 透過 PlatformIO 開啟本專案資料夾。

### 3.2 自動安裝函式庫

專案已在 `platformio.ini` 中設定 `lib_deps`，第一次建構時 PlatformIO 會自動下載：

- `Adafruit GFX Library`
- `Adafruit ST7735 and ST7789 Library`

使用者無需手動安裝，只需執行建構/上傳即可。

### 3.3 編譯與上傳

在專案根目錄（或 PlatformIO IDE）中，使用下列指令：

```pwsh
# 只編譯
pio run

# 編譯並上傳至 ESP32
pio run --target upload

# 開啟序列埠監控（預設 115200）
pio device monitor
```

或在 VS Code 左側的 PlatformIO 工具列中，使用「Build」、「Upload」、「Monitor」按鈕操作。

---

## 4. 程式運作說明

### 4.1 `setup()` 流程

`main.cpp` 中的 `setup()` 大致流程如下：

1. `Serial.begin(SERIAL_BAUD);`  
   - 開啟序列埠，以便在序列監控視窗中看到除錯訊息。

2. 設定背光腳位並點亮：
   - `pinMode(TFT_BL, OUTPUT);`
   - `digitalWrite(TFT_BL, HIGH);`  → 讓背光常亮。

3. 初始化 TFT：
   - `tft.initR(TFT_INIT_MODE);` 依照面板類型初始化（例如 `INITR_BLACKTAB`）。
   - `tft.setRotation(TFT_ROTATION);` 設定螢幕旋轉方向。
   - `tft.fillScreen(ST77XX_BLACK);` 將整個螢幕填成黑色背景。

4. 呼叫 `displayText();` 顯示四行文字。

### 4.2 `loop()` 行為

```cpp
void loop() {
  // 固定顯示，不需重繪
  delay(1000);
}
```

- 主迴圈中不進行任何重繪，只是簡單 `delay(1000);`。
- 顯示內容在 `setup()` 中完成後即保持不變，適合作為看板型應用的基底範例。

### 4.3 文字顯示與置中演算法

在 `displayText()` 中：

1. 設定文字大小：
   ```cpp
   tft.setTextSize(2);
   ```
   Adafruit GFX 預設字型在 `setTextSize(2)` 時，每個字元大約為 12x16 像素（寬 x 高）。

2. 取得螢幕寬高：
   ```cpp
   int16_t screenWidth = tft.width();
   int16_t screenHeight = tft.height();
   ```

3. 根據字元尺寸與行距計算四行文字的總高度，讓整體垂直置中：
   ```cpp
   int16_t charWidth = 12;
   int16_t charHeight = 16;
   int16_t lineSpacing = 16;
   int16_t totalHeight = charHeight * 4 + lineSpacing * 3;
   int16_t startY = (screenHeight - totalHeight) / 2;
   ```

4. 每一行的 X 座標計算方式：
   ```cpp
   int16_t x = (screenWidth - line.length() * charWidth) / 2;
   ```
   - `line.length()`：字串長度。  
   - `line.length() * charWidth`：整行文字的像素寬度。  
   - 用螢幕寬度減去文字寬度再除以 2，即可讓文字「水平置中」。

5. 每行使用不同顏色顯示：
   ```cpp
   tft.setTextColor(ST77XX_WHITE);   // 第 1 行
   tft.setTextColor(ST77XX_YELLOW);  // 第 2 行
   tft.setTextColor(ST77XX_GREEN);   // 第 3 行
   tft.setTextColor(ST77XX_BLUE);    // 第 4 行
   ```

---

## 5. 常見問題與調整建議

### 5.1 螢幕不亮或全白

- 檢查：
  - VCC 是否接 3.3V（部份模組可接 5V，視模組規格而定）。
  - GND 是否確實共地。
  - `TFT_BL`（背光）是否有設定為 HIGH。

### 5.2 顯示方向顛倒或左右顛倒

- 調整 `config.h` 中的：
  ```cpp
  #define TFT_ROTATION 1
  ```
  可嘗試改為 0、1、2、3 其中之一，觀察畫面方向是否正確。

### 5.3 顏色怪異或畫面亂碼

- 確認模組是使用 ST7735 控制器，且 `TFT_INIT_MODE` 與實際面板相符：
  ```cpp
  #define TFT_INIT_MODE INITR_BLACKTAB
  ```
  若畫面顏色偏差，可嘗試改用 `INITR_GREENTAB` 或 `INITR_REDTAB` 等模式。

### 5.4 字太小或太大

- 調整 `displayText()` 中：
  ```cpp
  tft.setTextSize(2);
  ```
  例如改為 `1` 或 `3`，並同步調整 `charWidth` / `charHeight` 估算值，使置中運算合理。

---

## 6. 延伸應用方向

在此範例基礎上，可以進一步：

- 加入多頁顯示，透過按鍵或定時切換不同頁面的文字或圖形。
- 顯示感測器數值（溫度、溼度、電壓等）。
- 做為機房設備標示牌，顯示設備名稱、IP、位置編號等資訊。
- 搭配 Wi-Fi / MQTT，做成小型狀態看板或告警顯示器。

---

## 7. 總結

本專案提供一個簡單、清楚的 ESP32 + ST7735 TFT 顯示範例：

- 採用 Adafruit 官方函式庫，安裝與使用方式標準。  
- GPIO 腳位配置遵守 ESP32 的使用原則，避開敏感腳位。  
- 範例程式碼結構清楚，適合初學者理解與擴充。

您可以在此基礎上依需求修改顯示內容、字體大小、顏色，或加入更多互動與網路功能，打造自己的專屬資訊看板。