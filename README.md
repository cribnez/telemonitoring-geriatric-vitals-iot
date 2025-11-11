# Sistema de Monitorización Remota de Signos Vitales (IoT)

Este repositorio contiene el firmware y los recursos visuales del proyecto "Diseño de un Sistema Embido Basado en IoT para la Monitorización Remota de Signos Vitales en Pacientes Geriátricos".

El sistema utiliza un ESP32 para capturar datos de sensores biomédicos y los transmite en tiempo real a la plataforma en la nube Ubidots mediante el protocolo MQTT.

## 🩺 Signos Vitales Monitoreados
* **Electrocardiograma (ECG):** Para el análisis de la actividad cardíaca.
* **Oximetría de Pulso (SpO2):** Mide la saturación de oxígeno en sangre.
* **Frecuencia Cardíaca (BPM):** Pulsos por minuto.
* **Temperatura Corporal:** Detección de fiebre o hipotermia.

## 📸 Vistazo del Proyecto

Basándonos en los prototipos y resultados, aquí hay un resumen visual del sistema:

<table align="center">
  <tr>
    <td align="center"><b>Prototipo Físico (Hardware)</b></td>
    <td align="center"><b>Diseño de PCB Personalizado</b></td>
  </tr>
  <tr>
    <td><img src="images/img3.jpg" alt="Prototipo ensamblado" width="100%"></td>
    <td><img src="images/img4.jpg" alt="Diseño de la PCB" width="100%"></td>
  </tr>
  <tr>
    <td align="center"><b>Dashboard General en Ubidots</b></td>
    <td align="center"><b>Señal de ECG en Tiempo Real</b></td>
  </tr>
  <tr>
    <td><img src="images/img1.jpg" alt="Dashboard de Ubidots" width="100%"></td>
    <td><img src="images/img2.jpg" alt="Señal de ECG en Ubidots" width="100%"></td>
  </tr>
</table>

*(Nota: Asegúrate de que tus archivos `img1.jpg` a `img4.jpg` en la carpeta `images` correspondan con las descripciones de arriba.)*

## 🛠️ Stack Tecnológico

| Componente | Tecnología Utilizada |
| :--- | :--- |
| **Microcontrolador** | ESP32 |
| **Sensores** | AD8232 (ECG), MAX30102 (SpO2/BPM), DS18B20 (Temp.) |
| **Plataforma IoT** | Ubidots |
| **Protocolo** | MQTT (Message Queuing Telemetry Transport) |
| **Firmware** | C++ (Arduino IDE) |

## 📂 Estructura del Repositorio
