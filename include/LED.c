#define LED 2

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