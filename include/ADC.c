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