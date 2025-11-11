# 🩺 Sistema de Monitorización Remota de Signos Vitales (IoT)

Este repositorio documenta el proyecto "Diseño de un Sistema Embebido Basado en IoT para la Monitorización Remota de Signos Vitales en Pacientes Geriátricos", desarrollado en la Universidad Politécnica de Chiapas.

El sistema emplea un microcontrolador ESP32 para adquirir, procesar y transmitir cuatro signos vitales clave a la plataforma en la nube **Ubidots** a través de **MQTT**.

---

## 📸 Galería del Proyecto

### 1. 📸 Galería del Producto Final

El dispositivo final integra todos los componentes electrónicos (ESP32, sensores, batería) dentro de una carcasa ergonómica impresa en 3D, diseñada para ser un monitor portátil, funcional y fácil de usar.

**<table align="center" style="border: none;">
  <tr style="border: none;">
    <td align="center">
      <b>[Descripción de la Imagen 1]</b><br>
      <img src="images/img1.jpg" alt="Vista 1 del diseño final" width="400">
    </td>
    <td align="center">
      <b>[Descripción de la Imagen 2]</b><br>
      <img src="images/img2.jpg" alt="Vista 2 del diseño final" width="400">
    </td>
  </tr>
  <tr style="border: none;">
    <td align="center">
      <b>[Descripción de la Imagen 3]</b><br>
      <img src="images/img3.jpg" alt="Vista 3 del diseño final" width="400">
    </td>
    <td align="center">
      <b>[Descripción de la Imagen 4]</b><br>
      <img src="images/img4.jpg" alt="Vista 4 del diseño final" width="400">
    </td>
  </tr>
</table>**

### 2. Visualización de Datos (Dashboard Ubidots)

Los datos se envían en tiempo real a Ubidots para su monitorización remota por personal médico o cuidadores.

<table align="center" style="border: none;">
  <tr style="border: none;">
    <td align="center">
      <b>Dashboard Principal (SpO2, BPM, Temp)</b><br>
      <img src="images/dashboard.jpeg" alt="Dashboard de Ubidots con SpO2, BPM y Temperatura" width="400">
    </td>
    <td align="center">
      <b>Señal de ECG en Tiempo Real</b><br>
      <img src="images/ecg.jpeg" alt="Señal de ECG en vivo en Ubidots" width="400">
    </td>
  </tr>
</table>

---

## 🛠️ Stack Tecnológico

| Categoría | Componente / Tecnología |
| :--- | :--- |
| **Microcontrolador** | ESP32 |
| **Plataforma IoT** | Ubidots (Broker MQTT y Dashboard) |
| **Comunicación** | WiFi y Protocolo MQTT |
| **Sensor ECG** | Módulo AD8232 |
| **Sensor Oximetría/Pulso**| Módulo MAX30102 |
| **Sensor Temperatura** | Sensor DS18B20 (sumergible) |
| **Firmware** | C++ (Arduino IDE) |

---

## 🚀 Guía de Inicio Rápido

El código fuente para el ESP32 se encuentra en la carpeta `/firmware`.

### 1. Librerías Requeridas (Arduino IDE)
Asegúrate de tener instaladas las siguientes librerías a través del Gestor de Librerías:
* `PubSubClient` (para MQTT)
* `Ubidots` (por Ubidots)
* `OneWire` (para el DS18B20)
* `DallasTemperature` (para el DS18B20)
* `MAX30105` (Librería de SparkFun para el MAX30102)

### 2. Configuración de Credenciales
Antes de compilar, abre el archivo `.ino` y modifica las siguientes variables con tus datos:

```cpp
// -- Configuración WiFi --
const char* WIFI_SSID = "NOMBRE_DE_TU_WIFI";
const char* WIFI_PASS = "PASSWORD_DE_TU_WIFI";


// -- Configuración Ubidots --
const char* UBIDOTS_TOKEN = "TU_TOKEN_DE_UBIDOTS";
const char* DEVICE_LABEL = "esp32"; // O el nombre de tu dispositivo
```
