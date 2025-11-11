#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "UbidotsEsp32Mqtt.h"

#define ONE_WIRE_BUS 25 
MAX30105 particleSensor;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18b20sensor(&oneWire);

int miVector[30] = {800, 1100, 1000, 900, 950, 970,740,1200,600,800,4100,0,1000,800,1100,1700, 2000,2000,950, 900,1000,1100,980,1000,1100,870,850,800};
const int LO1 =13;
const int LO2 =14;

long int x=0;
float temperatura;
float ecg;
float bpm;
float spo2 = 0;
float beatAVG=0;

const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

float beatsPerMinute;
int beatAvg;

#define SAMPLE_WINDOW 100
uint32_t redBuffer[SAMPLE_WINDOW];
uint32_t irBuffer[SAMPLE_WINDOW];
int sampleIndex = 0;

//*Definimos el Token que nos brinda la plataforma Ubidots para hacer la conexión*/
const char *UBIDOTS_TOKEN = "TU_TOKEN_DE_UBIDOTS";
/*Definimos SSID y PASSWORD de nuestra red WiFi*/
const char* WIFI_SSID = "NOMBRE_DE_TU_WIFI";
const char* WIFI_PASS = "PASSWORD_DE_TU_WIFI";   
/*Definimos el nombre de nuestro dispositivo, de Ubidots*/
const char *DEVICE_LABEL = "esp32-salud";
/*Definimos las variables que se medirán y que serán publicadas en la plataforma Ubidots*/
const char *VARIABLE_LABEL_1 = "temperature"; 
const char *VARIABLE_LABEL_2 = "ecg";
const char *VARIABLE_LABEL_3 = "bpm";
const char *VARIABLE_LABEL_4 = "spo2";
//const char *VARIABLE_LABEL_5 = "x1";    
/*Definimos la frecuencia de publicación*/
const int PUBLISH_FREQUENCY = 30000;
/*Definimos la frecuencia de ACTUALIZACION DE SENSOR TEMPERATURA*/
//const int SENSOR_FREQUENCY  = 120000; 
unsigned long timer;
unsigned long timer2;
Ubidots ubidots(UBIDOTS_TOKEN);

void setup() {
  Serial.begin(115200);
  delay(500);
  DS18b20sensor.begin();
  DS18b20sensor.setResolution(9);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  pinMode(13, LO2); 
  pinMode(14, LO1); 
  timer = millis();
  //Serial.println("Iniciando sensor MAX30102...");
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false)
  {
    Serial.println("Error: Sensor MAX30102 no detectado.");
    while (1);
  }
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x32);
  particleSensor.setPulseAmplitudeIR(0x32);
  particleSensor.setPulseAmplitudeGreen(0);
 // Serial.println("Sensores listos.");
   DS18b20sensor.requestTemperatures();
  float tempC = DS18b20sensor.getTempCByIndex(0);
  temperatura=tempC;
}
/*La librería nos da una función de Subscripción para mensajes de regreso desde la plataforma Ubidots hacia nuestro circuito*/
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
void loop()
{
  long irValue = particleSensor.getIR();
  long redValue = particleSensor.getRed();

  // Guardar datos para SpO2
  redBuffer[sampleIndex] = redValue;
  irBuffer[sampleIndex] = irValue;
  sampleIndex++;
    // BPM
      if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
    if (sampleIndex == SAMPLE_WINDOW) {
    float SpO2 = calculateSpO2(redBuffer, irBuffer, SAMPLE_WINDOW);
    spo2=SpO2;
    Serial.println("IR=       " + String(irValue));
    Serial.println("Red=      " + String(redValue));
    Serial.println("BPM=      " + String(beatsPerMinute));
    Serial.println("Avg BPM=  " + String(beatAvg));
    Serial.println("SpO2=     " + String(SpO2)+" %");
    Serial.println("Temp:     " + String(temperatura));
    Serial.println("ECG:      " + String(analogRead(33)));
    Serial.println("Freq:     " + String(PUBLISH_FREQUENCY));
    sampleIndex = 0;
        if(redValue<10000){
      const int zero =0;
    ubidots.add(VARIABLE_LABEL_3, zero);
    ubidots.add(VARIABLE_LABEL_4, zero);
    Serial.println("BPM : " + String(zero));
    Serial.println("SPO2: " + String(zero));
    }else{
    ubidots.add(VARIABLE_LABEL_3, beatAvg);
    ubidots.add(VARIABLE_LABEL_4, spo2);
    Serial.println("BPM : " + String(beatAvg));
    Serial.println("SPO2: " + String(spo2));
    }
    ubidots.publish(DEVICE_LABEL);
    ubidots.loop();
  }

  if ((millis() - timer) > PUBLISH_FREQUENCY){ 
    // Lectura de temperatura
  DS18b20sensor.requestTemperatures();
  float tempC = DS18b20sensor.getTempCByIndex(0);
  if (tempC >= 10.00 || tempC <= 50.0) {
     temperatura=tempC;
  }else{
        temperatura=0;}
      if (!ubidots.connected())  /*Definimos que, si no se conecta a la plataforma Ubidots, se pasa la función Reconnect() para volver a establecer la conexión*/
  {
    ubidots.reconnect();
  }

    Serial.println("Enviando datos a Ubidots: ");
    Serial.println("Temp: " + String(temperatura));
    ubidots.add(VARIABLE_LABEL_1, temperatura);

    
    Serial.println("-----------------------------------------");
    for (int i = 0; i < 27; i++) {

            if (!ubidots.connected())  /*Definimos que, si no se conecta a la plataforma Ubidots, se pasa la función Reconnect() para volver a establecer la conexión*/
  {
    ubidots.reconnect();
  }
          if((digitalRead(13) == 1)||(digitalRead(14) == 1)||analogRead(33)<=50||analogRead(33)>=3900){
            Serial.print("ECG desconectado ->");
            Serial.println("ECG No. " + String(i) + "   :" + "00");
            ubidots.add(VARIABLE_LABEL_2, 0);     // inicio automatico de ECG
            //ubidots.add(VARIABLE_LABEL_2, miVector[i]);
            
                  }else{
                    Serial.println("ECG No. " + String(i) + "   :" + String(miVector[i]));
                    ubidots.add(VARIABLE_LABEL_2, miVector[i]);
                  }
          //ubidots.add(VARIABLE_LABEL_5, x);
          ubidots.publish(DEVICE_LABEL);
          ubidots.loop();
          delay(1000);
          x++;
        }
    Serial.println("-----------------------------------------");
    timer = millis();
  }

}
// Función para calcular SpO2 
float calculateSpO2(uint32_t *red, uint32_t *ir, int length) {
  double sumRedAC = 0, sumIrAC = 0;
  double sumRedDC = 0, sumIrDC = 0;

  for (int i = 1; i < length; i++) {
    sumRedAC += abs((int)red[i] - (int)red[i - 1]);
    sumIrAC += abs((int)ir[i] - (int)ir[i - 1]);

    sumRedDC += red[i];
    sumIrDC += ir[i];
  }

  double avgRedDC = sumRedDC / (length - 1);
  double avgIrDC = sumIrDC / (length - 1);

  double R = (sumRedAC / avgRedDC) / (sumIrAC / avgIrDC);
  float SpO2 = 110 - 25 * R;

  if (SpO2 > 100) SpO2 = 100;
  if (SpO2 < 0) SpO2 = 0;

  return SpO2;
}
