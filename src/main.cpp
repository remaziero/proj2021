

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
#include "Wire.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>

#include "conf_PCA9698.h"
#define LED 2
#define ADDR  0x40 >> 1

// Configurações do WiFi
const char* ssid     = "R2Maziero"; // Nome da rede WiFi
const char* password = "camilarenato"; // Senha da rede WiFi

// Configurações do Servidor NTP
const char* servidorNTP = "a.st1.ntp.br"; // Servidor NTP para pesquisar a hora
 
const int fusoHorario = -10800; // Fuso horário em segundos (-03h = -10800 seg)
const int taxaDeAtualizacao = 1800000; // Taxa de atualização do servidor NTP em milisegundos
 
WiFiUDP ntpUDP; // Declaração do Protocolo UDP
NTPClient timeClient(ntpUDP, servidorNTP, fusoHorario, 60000);
  
//protótipos de função
void conf_PCA9698();
void vLePca(void * pvParameters);
void vLed(void * pvParameters);
void vOled(void * pvParameters);

unsigned long lastDebounceTime = 0; // última vez que o botão foi pressionado
unsigned long debounceDelay = 1000;   // O intervalo, igual ao delay do código anterior
TaskHandle_t lePCA9698Handle, acendLEDHandle, OledHandle;
QueueHandle_t xfilaPCA;
SemaphoreHandle_t mutex;



void setup() {
  //conf_PCA9698();
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
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

  // Iniciar cliente de aquisição do tempo
  timeClient.begin();

  xfilaPCA=xQueueCreate(40,sizeof(int));
  if(xfilaPCA==NULL){
    Serial.println("Não foi possível criar a fila xfilaPCA");
  }

  mutex=xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(vLePca, "lePCA", configMINIMAL_STACK_SIZE+1024,NULL,4,&lePCA9698Handle,1);
  xTaskCreatePinnedToCore(vLed, "led", configMINIMAL_STACK_SIZE+1024,NULL,1,&acendLEDHandle,1);
  xTaskCreatePinnedToCore(vOled,"Oled",configMINIMAL_STACK_SIZE+1024,NULL,1,&OledHandle,0);
}

void loop() {


}


void vLePca(void * pvParameters){
  int PortBit[]={};
  int Port_a[]={};
  int Port_b[]={};
  int Port_clow[]={};
  int Port_chigh[]={};
  int Port_d[]={};
  int Port_e[]={};

    while (true)
  {
    xSemaphoreTake(mutex,portMAX_DELAY);
    for(int i = 0; i < 40; i++) {
      int gpio = faboGPIO.digitalRead(i);
      if (gpio == HIGH) {
        if ((millis() - lastDebounceTime) > debounceDelay) {      
          //Serial.println("PCA9698 lendo porta "+String(i+1) +" no estado: " + String(gpio));
          PortBit[i]=gpio;
          if(i>=0 && i<=7){
          Serial.println("A porta a"+String(i)+" com nível: "+PortBit[i]);
          Port_a[i]=gpio;
          }
          if(i>=8 && i<=15){
          Serial.println("A porta b"+String(i)+" com nível: "+PortBit[i]);
          Port_b[i]=gpio;
          }
          if(i>=16 && i<=19){
          Serial.println("A porta c"+String(i)+" com nível: "+PortBit[i]);
          Port_clow[i]=gpio;
          }
          if(i>=20 && i<=23){
          Serial.println("A porta c"+String(i)+" modo: "+PortBit[i]);
          Port_chigh[i]=gpio;
          }
          if(i>=24 && i<=31){
          Serial.println("A porta d"+String(i)+" modo: "+PortBit[i]);
          Port_d[i]=gpio;
          }
          if(i>=31 && i<=39){
          Serial.println("A porta d"+String(i)+" modo: "+PortBit[i]);
          Port_e[i]=gpio;
          }
        }    
      }
      vTaskDelay(pdMS_TO_TICKS(25));
    }
    xSemaphoreGive(mutex);
    vTaskDelay(25);     
  }
}
void vLed(void * pvParameters){
  while (1)
  {
  //xSemaphoreTake(mutex,portMAX_DELAY);
  digitalWrite(LED,HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));
  digitalWrite(LED,LOW);
  vTaskDelay(pdMS_TO_TICKS(1000)); 
  //xSemaphoreGive(mutex);
  //vTaskDelay(50);
  }
}
void vOled(void * pvParameters){
  while (1)
  {
    //xSemaphoreTake(mutex,portMAX_DELAY);
    timeClient.update();
    String horario = timeClient.getFormattedTime();
    Serial.println("Hora do teste: "+String(horario));
    Serial.println("----------------------");
 
    vTaskDelay(pdMS_TO_TICKS(1000));
    //xSemaphoreGive(mutex);
    //vTaskDelay(pdMS_TO_TICKS(20));
  }
}
