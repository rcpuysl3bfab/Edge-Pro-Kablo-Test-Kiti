# Edge Pro Kablo Test Kiti

Arduino Mega 2560 tabanlı profesyonel kablo test cihazı. Edge Pro sistemindeki çeşitli kabloların bütünlüğünü ve bağlantılarını test eder.

## Özellikler

- **11 Farklı Kablo Tipi Testi**: Hareket, Heater, Loadcell, Z-Ref, Y-Ref, Ortam Sensörü, IR Temp, Projektor, PC-Led kabloları
- **CVR Limit Switch Testi**: CVR1 ve CVR2 limit switch'lerinin çalışma durumunu kontrol eder
- **128x64 OLED Ekran**: Menü tabanlı kullanıcı arayüzü
- **Rotary Encoder**: Kolay menü navigasyonu
- **Buzzer Geri Bildirimi**: Test sonuçları için sesli uyarı
- **Çoklu Okuma**: Her pin için 3 kez okuma yaparak güvenilir sonuçlar

## Donanım Gereksinimleri

- **Mikrodenetleyici**: Arduino Mega 2560
- **OLED Ekran**: SSD1306 (128x64, I2C)
- **Rotary Encoder**: 3 pinli (CLK, DT, SW)
- **Buzzer**: Pasif buzzer (Pin 30)
- **Test Pinleri**: 
  - Send Pinleri: 22-28
  - Receive Pinleri: 40-46
- **CVR Butonları**: 
  - CVR1: Top (Pin 11), Bottom (Pin 10)
  - CVR2: Top (Pin 13), Bottom (Pin 12)

## Pin Bağlantıları

### Rotary Encoder
- `ENCODER_CLK`: Pin 3
- `ENCODER_DT`: Pin 2
- `ENCODER_SW`: Pin 4 (Pull-up)

### OLED Ekran (I2C)
- `SDA`: Pin 20 (I2C)
- `SCL`: Pin 21 (I2C)
- `Adres`: 0x3C

### Buzzer
- `BUZZER_PIN`: Pin 30

### Test Pinleri

#### Send Pinleri (Çıkış)
- Pin 22-28: Test sinyali gönderimi için kullanılır

#### Receive Pinleri (Giriş)
- Pin 40-46: Test sinyali alımı için kullanılır

### CVR Limit Switch'ler
- **CVR1**:
  - Top Limit: Pin 11 (Pull-up)
  - Bottom Limit: Pin 10 (Pull-up)
- **CVR2**:
  - Top Limit: Pin 13 (Pull-up)
  - Bottom Limit: Pin 12 (Pull-up)

## Test Edilen Kablolar

### 1. Hareket S. Kablosu
- **Pin Sayısı**: 6
- **Send Pinleri**: 22, 23, 24, 25, 26, 27
- **Receive Pinleri**: 40, 41, 42, 43, 44, 45

### 2. Heater Data Kablosu
- **Pin Sayısı**: 3
- **Send Pinleri**: 22, 23, 24
- **Receive Pinleri**: 40, 41, 42

### 3. Loadcell Kablosu
- **Pin Sayısı**: 6
- **Send Pinleri**: 22, 23, 24, 25, 26, 27
- **Receive Pinleri**: 40, 41, 42, 43, 44, 45

### 4. Z-Ref Kablosu
- **Pin Sayısı**: 3
- **Send Pinleri**: 22, 23, 24
- **Receive Pinleri**: 40, 41, 42

### 5. Ortam Sensörü
- **Pin Sayısı**: 6
- **Send Pinleri**: 22, 23, 24, 25, 26, 27
- **Receive Pinleri**: 40, 41, 42, 43, 44, 45

### 6. Y-Ref Kablosu
- **Pin Sayısı**: 4
- **Send Pinleri**: 22, 23, 24, 25
- **Receive Pinleri**: 40, 41, 42, 43

### 7. IR Temp Sensor
- **Pin Sayısı**: 5
- **Send Pinleri**: 22, 23, 24, 25, 26
- **Receive Pinleri**: 40, 41, 42, 43, 44

### 8. Projektor Data
- **Pin Sayısı**: 7
- **Send Pinleri**: 22, 23, 24, 25, 26, 27, 28
- **Receive Pinleri**: 40, 41, 42, 43, 44, 45, 46

### 9. PC-Led Kablosu
- **Pin Sayısı**: 4
- **Send Pinleri**: 22, 23, 24, 25
- **Receive Pinleri**: 40, 41, 42, 43

### 10. CVR1 Limit Test
- **Tip**: Buton testi
- **Butonlar**: Top (Pin 11), Bottom (Pin 10)
- **Fonksiyon**: Limit switch'lerin basılı/basılı değil durumunu gösterir

### 11. CVR2 Limit Test
- **Tip**: Buton testi
- **Butonlar**: Top (Pin 13), Bottom (Pin 12)
- **Fonksiyon**: Limit switch'lerin basılı/basılı değil durumunu gösterir

## Kullanım

### Başlangıç
1. Cihazı açtığınızda logo ekranı görüntülenir ve açılış melodisi çalar
2. Ana menü ekrana gelir

### Menü Navigasyonu
- **Encoder Çevirme**: Menüde yukarı/aşağı hareket
- **Encoder Butonu**: Seçili öğeyi seçer veya test sırasında menüye döner

### Kablo Testi
1. Encoder ile test edilecek kablo tipini seçin
2. Encoder butonuna basın
3. Test otomatik olarak başlar
4. Her pin için sonuç gösterilir:
   - **DOGRU**: Pin bağlantısı sağlam
   - **Kopuk!**: Pin bağlantısı kopuk veya hatalı
5. Test sonuçları 3 saniye gösterilir
6. Otomatik olarak menüye dönülür

### CVR Limit Testi
1. "CVR1 Limit Test" veya "CVR2 Limit Test" seçin
2. Encoder butonuna basın
3. Ekranda Top ve Bottom butonlarının durumu gösterilir:
   - **Basildi**: Buton basılı
   - **Basilmadi**: Buton basılı değil
4. Encoder butonuna basarak menüye dönün

## Test Mantığı

### Pin Testi
1. Her send pininden sırayla HIGH sinyali gönderilir
2. Karşılık gelen receive pininden 3 kez okuma yapılır
3. Üç okuma da HIGH ise pin **DOGRU** kabul edilir
4. Herhangi biri LOW ise pin **Kopuk** olarak işaretlenir

### Güvenilirlik
- Her pin için 3 kez okuma yapılması, geçici gürültü veya bağlantı sorunlarını filtreler
- 10ms delay'ler ile pin durumlarının stabilize olması sağlanır

## Kütüphaneler

Bu proje aşağıdaki kütüphaneleri kullanır:
- `Wire.h` - I2C iletişimi için
- `Adafruit_GFX.h` - Grafik kütüphanesi
- `Adafruit_SSD1306.h` - SSD1306 OLED ekran sürücüsü

### PlatformIO ile Kurulum

`platformio.ini` dosyasına eklenmesi gerekenler:

```ini
lib_deps =
  adafruit/Adafruit SSD1306 @ ^2.5.7
  adafruit/Adafruit GFX Library @ ^1.11.9
```

## Derleme ve Yükleme

### PlatformIO ile

```bash
# Projeyi derle
pio run

# Arduino Mega 2560'a yükle
pio run --target upload

# Serial monitörü aç
pio device monitor
```

### Arduino IDE ile

1. Gerekli kütüphaneleri yükleyin:
   - Adafruit SSD1306
   - Adafruit GFX Library
2. `main.cpp` dosyasını açın
3. Board: Arduino Mega 2560 seçin
4. Port'u seçin
5. Upload butonuna tıklayın

## Sorun Giderme

### OLED Ekran Çalışmıyor
- I2C bağlantılarını kontrol edin (SDA=20, SCL=21)
- Ekran adresinin 0x3C olduğundan emin olun
- Serial monitörde "OLED bulunamadı!" hatası görürseniz bağlantıları kontrol edin

### Encoder Çalışmıyor
- CLK ve DT pinlerinin doğru bağlandığından emin olun
- Buton pininin pull-up olduğundan emin olun (Pin 4)

### Test Sonuçları Yanlış
- Send ve Receive pinlerinin doğru bağlandığından emin olun
- Kabloların temiz ve sağlam olduğundan emin olun
- Pin numaralarının doğru eşleştiğini kontrol edin

### Buzzer Çalışmıyor
- Pin 30'un doğru bağlandığından emin olun
- Buzzer'ın pasif buzzer olduğundan emin olun

## Proje Yapısı

```
Edge Pro Kablo Test Kiti/
├── src/
│   └── main.cpp          # Ana program kodu
├── logo.h                # Logo bitmap verisi
├── platformio.ini        # PlatformIO yapılandırması
└── README.md             # Bu dosya
```

## Teknik Detaylar

### Test Algoritması
- Her pin için sıralı test yapılır
- Send pininden HIGH gönderilir
- 10ms bekleme (pin stabilizasyonu)
- Receive pininden 3 kez okuma (500μs aralıklarla)
- Sonuç değerlendirmesi

### Menü Sistemi
- Scroll edilebilir menü (6 öğe görünür)
- Encoder ile navigasyon
- Seçili öğe ">" işareti ile gösterilir

### Ekran Güncellemesi
- Menü değiştiğinde ekran güncellenir
- Test sırasında sonuçlar gösterilir
- CVR testinde anlık buton durumu gösterilir

## Lisans

Bu proje özel kullanım içindir.

## Geliştirici Notları

- Test süresi: Her pin için yaklaşık 20ms
- Toplam test süresi: Pin sayısı × 20ms + 3 saniye gösterim
- Encoder debounce: 5ms
- Serial baud rate: 9600

## Versiyon Geçmişi

- **v1.0**: İlk sürüm
  - 11 kablo tipi testi
  - CVR limit switch testi
  - OLED menü sistemi
  - Buzzer geri bildirimi
