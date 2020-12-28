FaBoGPIO40 faboGPIO;
 void conf_PCA9698(){ 
  faboGPIO.configuration();
  faboGPIO.portMode(0, INPUT);
  faboGPIO.portMode(1, INPUT);
  faboGPIO.portMode(2, INPUT);
  faboGPIO.portMode(3, INPUT);
  faboGPIO.portMode(4, INPUT);
 }