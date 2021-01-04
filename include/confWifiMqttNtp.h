// Configurações do WiFi em BH
const char *ssid = "R2Maziero";        // Nome da rede WiFi
const char *password = "camilarenato"; // Senha da rede WiFi

// Configurações do Servidor NTP
const char *servidorNTP = "a.st1.ntp.br"; // Servidor NTP para pesquisar a hora

const int fusoHorario = -10800;        // Fuso horário em segundos (-03h = -10800 seg)
const int taxaDeAtualizacao = 1800000; // Taxa de atualização do servidor NTP em milisegundos

WiFiUDP ntpUDP; // Declaração do Protocolo UDP
NTPClient timeClient(ntpUDP, servidorNTP, fusoHorario, 60000);

/*
Os códigos de retorno de CONNACK são:
0  - Conexão aceita
1 - Conexão recusada, versão do protocolo inaceitável
2 - Conexão recusada, identificador rejeitado
3 - Conexão recusada, servidor indisponível
4 - Conexão recusada, usuário e/ou senha inválidos
5 - Conexão recusada, não autorizado
 */
//char* ESP8266Client = "MQTT_ARN";

//****DEFINIÇÃO DOS TÓPICOS PARA O BROKER*************/
//identificador de publicação no terminal





 


