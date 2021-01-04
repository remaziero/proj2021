

//GIT
//crie um repositório no git e pegue o endereço do repositório
// o endereço tá no botão 'código' = https://github.com/remaziero/proj2021.git
//De volta ao VSCODE - control shift g
// va nos tres pontinhos e inicialize o terminal E DIGITE
//git remote add origin https://github.com/remaziero/proj2021.git

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FaBoGPIO40_PCA9698.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <PubSubClient.h>

#include "conf_PCA9698.h"
#include "confWifiMqttNtp.h"
#include "confRTOS.h"

#include "vLePCA9698.c"
#include "LED.c"
#include "ADC.c"

// MQTT
#define TOPICO_SUBSCRIBE "MQTTEnvia" //tópico MQTT de escuta
#define TOPICO_PUBLISH "MQTTRecebe"  //tópico MQTT de envio de informações para Broker
#define ID_MQTT "ARN"                //id mqtt (para identificação de sessão)

const char *BROKER_MQTT = "iot.eclipse.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883;                      // Porta do Broker MQTT

WiFiClient MetroDF;         // Cria o objeto MeroDF
PubSubClient MQTT(MetroDF); // Instancia o Cliente MQTT passando o objeto MetroDF

//protótipos de função
void conf_PCA9698();
void vLePca(void *pvParameters);
void vLed(void *pvParameters);
void vhoraNTP(void *pvParameters);
void vTaskADC(void *pvParameters);
void initWifi();
void reconectWiFi();
void mqtt_callback(char *topic, byte *payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);

void setup()
{
  //conf_PCA9698();
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  initWifi();

  timeClient.forceUpdate(); //Força o Update.

  // Iniciar cliente de aquisição do tempo
  timeClient.begin();
  timeClient.forceUpdate(); //Força o Update.

  xfilaPCA = xQueueCreate(10, sizeof(int));
  if (xfilaPCA == NULL)
  {
    Serial.println("Não foi possível criar a fila xfilaPCA");
  }

  mutex = xSemaphoreCreateMutex();

  // xTaskCreatePinnedToCore(vLePca, "lePCA", configMINIMAL_STACK_SIZE + 1024, NULL, 4, &lePCA9698Handle, 1);
  // xTaskCreatePinnedToCore(vLed, "led", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &acendLEDHandle, 0);
  // xTaskCreatePinnedToCore(vhoraNTP, "horaNTP", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &horaNTPHandle, 0);
  // xTaskCreatePinnedToCore(vTaskADC, "TaskADC", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xtaskADCHandle, 0);
}

void loop()
{
}

String horario;

void vhoraNTP(void *pvParameters)
{
  while (1)
  {
    //xSemaphoreTake(mutex,portMAX_DELAY);
    timeClient.update();
    horario = timeClient.getFormattedTime();

    //Serial.println("Hora do teste: " + String(horario));
    Serial.println(horario);

    vTaskDelay(pdMS_TO_TICKS(1000));
    //xSemaphoreGive(mutex);
    //vTaskDelay(pdMS_TO_TICKS(20));
  }
}

//*********************BROKER*MQTT**MÁQUINA QBEX conectado via WIFI*********************
const char *mqttServer = "192.168.1.105";
const int mqttPort = 1883;     // 1955;           // Aqui mude para sua porta fornecida pelo ACL
const char *mqttUser = "";     //  Aqui o nome de usuario fornecido pelo ACL
const char *mqttPassword = ""; //  Aqui sua senha fornecida pelo ACL

const char *usu = "METRO_DF/ARN";
const char *estacao = "ARN"; //client ID

long previousMillis = 0;
int intervalo = 500;

void initWifi()
{
  WiFi.begin(ssid, password);

  // Aguardando conexão do WiFi
  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("WiFi conectado. Endereço IP: ");
  Serial.println(WiFi.localIP());
}