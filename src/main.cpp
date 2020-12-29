

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
#define ADDR 0x40 >> 1

// Configurações do WiFi em BH
const char *ssid = "R2Maziero";        // Nome da rede WiFi
const char *password = "camilarenato"; // Senha da rede WiFi

// Configurações do Servidor NTP
const char *servidorNTP = "a.st1.ntp.br"; // Servidor NTP para pesquisar a hora

const int fusoHorario = -10800;        // Fuso horário em segundos (-03h = -10800 seg)
const int taxaDeAtualizacao = 1800000; // Taxa de atualização do servidor NTP em milisegundos

WiFiUDP ntpUDP; // Declaração do Protocolo UDP
NTPClient timeClient(ntpUDP, servidorNTP, fusoHorario, 60000);

//protótipos de função
void conf_PCA9698();
void vLePca(void *pvParameters);
void vLed(void *pvParameters);
void vhoraNTP(void *pvParameters);
void vcertLePCA9698(void *pvParameters);
void vTaskADC(void *pvParameters);

unsigned long lastDebounceTime = 0; // última vez que o botão foi pressionado
unsigned long debounceDelay = 1000; // O intervalo, igual ao delay do código anterior
TaskHandle_t lePCA9698Handle, acendLEDHandle, horaNTPHandle, certLePCA9698Handle, xtaskADCHandle;
QueueHandle_t xfilaPCA;
SemaphoreHandle_t mutex;

void setup()
{
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

  xfilaPCA = xQueueCreate(10, sizeof(int));
  if (xfilaPCA == NULL)
  {
    Serial.println("Não foi possível criar a fila xfilaPCA");
  }

  mutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(vLePca, "lePCA", configMINIMAL_STACK_SIZE + 1024, NULL, 4, &lePCA9698Handle, 1);
  xTaskCreatePinnedToCore(vLed, "led", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &acendLEDHandle, 0);
  xTaskCreatePinnedToCore(vhoraNTP, "horaNTP", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &horaNTPHandle, 0);
  xTaskCreatePinnedToCore(vcertLePCA9698, "cerTLePCA9698", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &certLePCA9698Handle, 0);
  xTaskCreatePinnedToCore(vTaskADC, "TaskADC", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xtaskADCHandle, 0);
}

void loop()
{
}

void vLePca(void *pvParameters)
{
  int Port_a[8] = {0,0,0,0,0,0,0,0};
  int Port_b[8] = {0,0,0,0,0,0,0,0};
  int Port_clow[4] = {0,0,0,0};
  int Port_chigh[8] = {0,0,0,0,0,0,0,0};
  int Port_d[8] = {0,0,0,0,0,0,0,0};
  int Port_e[8] = {0,0,0,0,0,0,0,0};

  while (true)
  {
   // xSemaphoreTake(mutex, portMAX_DELAY);
    for (int i = 0; i < 40; i++)
    {
      int gpio = faboGPIO.digitalRead(i);
      int tempo = 10;

//**************************************IMPORTANTE PARA TESTES*************************/
      if (gpio == HIGH)  //para imprimir apenas os níveis e modos das portas quando 1
      {                  //SE COMENTAR lè e mostra a leitura de todos independente do nível
//*************************************************************************************/
        if ((millis() - lastDebounceTime) > debounceDelay)
        {
          if (i >= 0 && i <= 7)
          {
            int pa = i;
            Port_a[pa] = gpio;

            //entrar com código de emissão MQTT aqui

//****************para usar como teste no desenvolvimento****************************/
            switch (pa)
            {
            case 0:
              Serial.println("a1:" + String(Port_a[0]));
              break;
            case 1:
              Serial.println("a2:" + String(Port_a[1]));
              break;
            case 2:
              Serial.println("a3:" + String(Port_a[2]));
              break;
            case 3:
              Serial.println("a4:" + String(Port_a[3]));
              break;
            case 4:
              Serial.println("a5:" + String(Port_a[4]));
              break;
            case 5:
              Serial.println("a6:" + String(Port_a[5]));
              break;
            case 6:
              Serial.println("a7:" + String(Port_a[6]));
              break;
            case 7:
              Serial.println("a8:" + String(Port_a[7]));
              break;           
            default:
           // Serial.println(pa + 1);
            //Serial.println("o pino a" + String(pa + 1) + " está em nível: " + Port_a[pa]);
           // Serial.println("---------------------------");
              break;
            }
            vTaskDelay(pdMS_TO_TICKS(tempo));
            Port_a[pa] = 0;
          } 
          if (i >= 8 && i <= 15)
          {
            int pb = i - 8;
            Port_b[pb] = gpio;

            //entrar com código de emissão MQTT aqui

//**********************************************************************/          
            switch (pb)
            {
            case 0:
            Serial.println("b1:" + String(Port_b[0]));
              break;
            case 1:
            Serial.println("b2:" + String(Port_b[1]));
              break;
            case 2:
            Serial.println("b3:" + String(Port_b[2]));
              break;
            case 3:
            Serial.println("b4:" + String(Port_b[3]));
              break;
            case 4:
             Serial.println("b5:" + String(Port_b[4]));
              break;
            case 5:
            Serial.println("b6:" + String(Port_b[5]));
              break;
            case 6:
            Serial.println("b7:" + String(Port_b[6]));
              break;
            case 7:
            Serial.println("b8:" + String(Port_b[7]));
              break; 
            default:
            //Serial.println(pb + 1);
            //Serial.println("o pino b" + String(pb + 1) + " está em nível: " + Port_b[pb]);
            //Serial.println("---------------------------");
              break;
            }
            vTaskDelay(pdMS_TO_TICKS(tempo));
            Port_b[pb] = 0;
          }

          if (i >= 16 && i <= 19)
          {
            int pc = i - 16;
            Port_clow[pc] = gpio;

            //entrar com código de emissão MQTT aqui

//**********************************************************************/
            switch (pc)
            {
            case 0:
            Serial.println("c1:" + String(Port_clow[0]));
              break;
            case 1:
            Serial.println("c2:" + String(Port_clow[1]));
              break;
            case 2:
            Serial.println("c3:" + String(Port_clow[2]));
              break;
            case 3:
            Serial.println("c4:" + String(Port_clow[3]));
              break; 
            default:
            Serial.println(pc + 1);
            Serial.println("o pino c" + String(pc + 1) + " está em nível: " + Port_clow[pc]);
            Serial.println("---------------------------");
            break;
            }
            vTaskDelay(pdMS_TO_TICKS(tempo));
            Port_clow[pc] = 0;
          }

          if (i >= 20 && i <= 23)
          {
            int pc = i - 16;
            Port_chigh[pc] = gpio;

            //entrar com código de emissão MQTT aqui


            switch (pc)
            {
            case 4:
            Serial.println("c5:" + String(Port_chigh[4]));
              break;
            case 5:
             Serial.println("c6:" + String(Port_chigh[5]));
              break;
            case 6:
             Serial.println("c7:" + String(Port_chigh[6]));
              break;
            case 7:
            Serial.println("c8:" + String(Port_chigh[7]));
              break; 
            default:
           // Serial.println(pc + 1);
           // Serial.println("A porta c" + String(pc + 1) + " está no modo: " + Port_chigh[pc]);
           // Serial.println("---------------------------");
              break;
            }
            vTaskDelay(pdMS_TO_TICKS(tempo));
            Port_chigh[pc] = 0;
          }
          if (i >= 24 && i <= 31)
          {
            int pd = i - 24;
            Port_d[pd] = gpio;

            //entrar com código de emissão MQTT aqui

//**********************************************************************/  
            switch (pd)
            {
            case 0:
            Serial.println("d1:" + String(Port_d[0]));
              break;
            case 1:
            Serial.println("d2:" + String(Port_d[1]));
              break;
            case 2:
            Serial.println("d3:" + String(Port_d[2]));
              break;
            case 3:
            Serial.println("d4:" + String(Port_d[3]));
              break;
            case 4:
            Serial.println("d5:" + String(Port_d[4]));
              break;
            case 5:
            Serial.println("d6:" + String(Port_d[5]));
              break;
            case 6:
            Serial.println("d7:" + String(Port_d[6]));
              break;
            case 7:
            Serial.println("d8:" + String(Port_d[7]));
              break; 
            default:
           // Serial.println(pd + 1);
           // Serial.println("o pino d" + String(pd + 1) + " está em modo: " + Port_d[pd]);
           // Serial.println("---------------------------");
              break;
            }
            vTaskDelay(pdMS_TO_TICKS(tempo));
            Port_d[pd] = 0;
          }
          if (i >= 32 && i <= 39)
          {
            int pe = i - 32;
            Port_e[pe] = gpio;

            //entrar com código de emissão MQTT aqui

//**********************************************************************/  
            switch (pe)
            {
            case 0:
            Serial.println("e1:" + String(Port_e[0]));
              break;
            case 1:
            Serial.println("e2:" + String(Port_e[1]));
              break;
            case 2:
            Serial.println("e3:" + String(Port_e[2]));
              break;
            case 3:
            Serial.println("e4:" + String(Port_e[3]));
              break;
            case 4:
            Serial.println("e5:" + String(Port_e[4]));
              break;
            case 5:
            Serial.println("e6:" + String(Port_e[5]));
              break;
            case 6:
            Serial.println("e7:" + String(Port_e[6]));
              break;
            case 7:
            Serial.println("e8:" + String(Port_e[7]));
              break; 
            default:
           // Serial.println(pe + 1);
           // Serial.println("o pino e" + String(pe + 1) + " está em modo: " + Port_e[pe]);
           // Serial.println("---------------------------");
              break;
            }
            vTaskDelay(pdMS_TO_TICKS(tempo));
            Port_e[pe] = 0;
          }
        }
      }//
      vTaskDelay(pdMS_TO_TICKS(25));
    }
   // xSemaphoreGive(mutex);
   // vTaskDelay(25);
  }
}
void vLed(void *pvParameters)
{
  while (1)
  {
    //xSemaphoreTake(mutex,portMAX_DELAY);
    digitalWrite(LED, HIGH);
    vTaskDelay(pdMS_TO_TICKS(100));
    digitalWrite(LED, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));
    //xSemaphoreGive(mutex);
    //vTaskDelay(50);
  }
}
void vhoraNTP(void *pvParameters)
{
  while (1)
  {
    //xSemaphoreTake(mutex,portMAX_DELAY);
    timeClient.update();
    String horario = timeClient.getFormattedTime();

    //Serial.println("Hora do teste: " + String(horario));
    Serial.println(horario);

    vTaskDelay(pdMS_TO_TICKS(1000));
    //xSemaphoreGive(mutex);
    //vTaskDelay(pdMS_TO_TICKS(20));
  }
}
void vcertLePCA9698(void *pvParameters)
{
  while (1)
  {

    vTaskDelay(25);
  }
}
void vTaskADC(void *pvParameters)
{
  while (1)
  {
    //xSemaphoreTake(xSemaphore,portMAX_DELAY);
    //xSemaphoreTake(mutex, portMAX_DELAY);
    float adcvalue = analogRead(34);
    float x = adcvalue * 1 / 1023; //*(3/1023);
    Serial.println("ADC: " + String(x));
    vTaskDelay(pdMS_TO_TICKS(1000));
    //xSemaphoreGive(mutex);
    //vTaskDelay(50);
  }
}
