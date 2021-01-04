
#include <PubSubClient.h>


unsigned long lastDebounceTime = 0; // última vez que o botão foi pressionado
unsigned long debounceDelay = 1000; // O intervalo, igual ao delay do código anterior

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
      int tempoLer = 300;
      int tempoModo = 40;

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
              vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 1:
              Serial.println("a2:" + String(Port_a[1]));
              vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 2:
              Serial.println("a3:" + String(Port_a[2]));
              vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 3:
              Serial.println("a4:" + String(Port_a[3]));
              vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 4:
              Serial.println("a5:" + String(Port_a[4]));
              vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 5:
              Serial.println("a6:" + String(Port_a[5]));
              vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 6:
              Serial.println("a7:" + String(Port_a[6]));
              vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 7:
              Serial.println("a8:" + String(Port_a[7]));
              vTaskDelay(pdMS_TO_TICKS(tempoLer));
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
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 1:
            Serial.println("b2:" + String(Port_b[1]));
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 2:
            Serial.println("b3:" + String(Port_b[2]));
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 3:
            Serial.println("b4:" + String(Port_b[3]));
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 4:
             Serial.println("b5:" + String(Port_b[4]));
             vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 5:
            Serial.println("b6:" + String(Port_b[5]));
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 6:
            Serial.println("b7:" + String(Port_b[6]));
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 7:
            Serial.println("b8:" + String(Port_b[7]));
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
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
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 1:
            Serial.println("c2:" + String(Port_clow[1]));
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 2:
            Serial.println("c3:" + String(Port_clow[2]));
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
              break;
            case 3:
            Serial.println("c4:" + String(Port_clow[3]));
            vTaskDelay(pdMS_TO_TICKS(tempoLer));
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
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 5:
             Serial.println("c6:" + String(Port_chigh[5]));
             vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 6:
             Serial.println("c7:" + String(Port_chigh[6]));
             vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 7:
            Serial.println("c8:" + String(Port_chigh[7]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
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
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 1:
            Serial.println("d2:" + String(Port_d[1]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 2:
            Serial.println("d3:" + String(Port_d[2]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 3:
            Serial.println("d4:" + String(Port_d[3]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 4:
            Serial.println("d5:" + String(Port_d[4]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 5:
            Serial.println("d6:" + String(Port_d[5]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 6:
            Serial.println("d7:" + String(Port_d[6]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 7:
            Serial.println("d8:" + String(Port_d[7]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
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
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 1:
            Serial.println("e2:" + String(Port_e[1]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 2:
            Serial.println("e3:" + String(Port_e[2]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 3:
            Serial.println("e4:" + String(Port_e[3]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 4:
            Serial.println("e5:" + String(Port_e[4]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 5:
            Serial.println("e6:" + String(Port_e[5]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
              break;
            case 6:
            Serial.println("e7:" + String(Port_e[6]));
            case 7:
            Serial.println("e8:" + String(Port_e[7]));
            vTaskDelay(pdMS_TO_TICKS(tempoModo));
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