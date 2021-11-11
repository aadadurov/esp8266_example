/*********
  Датчик температуры и влажности. Демо как работать с thingsboard
  Прошивка для модуля на чипе ESP8266
  09/11/2021
*********/

// Подключаем нужные библиотеки
#include <ESP8266WiFi.h> 
#include <WiFiClient.h>
#include <DHT.h>
#include <PubSubClient.h>

// Задаём тип датчика влажности и температуры DHT
#define DHTTYPE DHT22   

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Данные доступа к сети WiFi
const char ssid[64] = "RT-GPON-5EC8"; // Имя сети
const char password[64] = "D28fkf3742"; // Пароль

// Задаём параметры подключения к IoT-платформе Samsung IT Academy  
char thingsboardServer[] = "89.208.220.227"; // Адрес платформы
uint16_t thingsboardServerPort = 1883; // Порт для подключения по протоколу MQTT
#define TOKEN "kuru5uBhitPrtgoVTOqT" // Токен выдаваемый устройству 

// Переменные влажности и температуры
float humidity, temp;

//Задаём GPIO куда подключаем датчик влажности и температуры
const int DHTPin = 0;

// Инициализация датчика влажности и температуры
DHT dht(DHTPin, DHTTYPE);

unsigned long lastSend;

String humidityTemp, celsiusTemp = "";


/*
   Основная функция
*/
void setup(void) {

  delay(1000);
  Serial.begin(115200);

  // Инициализация датчика
  dht.begin();

  WiFi.begin(ssid, password);

  // ждём соединения и выводим статус соединения:
  while (int s = WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  onWiFiConnected();

  Serial.println("");
  Serial.print("Connected to ");  //  "Подключились к "
  Serial.println(ssid);
  Serial.print("IP address: ");  //  "IP-адрес: "
  Serial.println(WiFi.localIP());

  String payload; // Объявили переменную
  // Формируем отличительные параметры для идентификации на портале, чтобы не сомневаться, что пришли именно наши данные
  payload = "{";
  payload += "\"device_started\":";
  payload += "\"mydevicename\"";
  payload += "}";

  char attributes[100]; // Объявили переменную
  // Отправляем телеметрию по MQTT
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/attributes", attributes );
  Serial.println( attributes );

}

/*
   Зацикливаем опрос и отправку данных
*/
void loop(void) {
  if ( millis() - lastSend > 1000 ) { // Обновить и отправить через секунду
    readSensor();
    celsiusTemp = String((int)temp);
    humidityTemp = String((int)humidity);
    // Для отладки
    Serial.print( "Sending temperature and humidity : [" );
    Serial.print( temp ); Serial.print( "," );
    Serial.print( humidity );
    Serial.print( "]   -> " );

    int humidityInt  =  humidity + 0.50;
    int tempInt  =  temp + 0.50;

    // Подготовка телеметрии о темперетуре и влажности в формате JSON
    String payload = "{";
    payload += "\"temperature\":";
    payload += celsiusTemp;
    payload += ",";
    payload += "\"humidity\":";
    payload += humidityTemp;
    payload += "}";

    // Отправляем телеметрию по MQTT
    char telemetry[100];
    payload.toCharArray( telemetry, 100 );
    client.publish( "v1/devices/me/telemetry", telemetry );
    Serial.println( telemetry );
    lastSend = millis();
  }

}

/*
   Чтение данных с датчика
*/
void readSensor() {
  // Читаем данные влажности с датчика
  humidity = dht.readHumidity();
  // Читаем данные температуры (Ц) с датчика
  temp = dht.readTemperature(false);
  // Проверяем на ошибки чтения
  if (isnan(humidity) || isnan(temp)) {
    //      Serial.println("Failed to read from DHT sensor!");
    humidity = 0; temp = 0;
    return;
  }
}

/*
   Вызывается как только устанавливается WiFi-соединение
*/
void onWiFiConnected(void) {
  lastSend = 0;
  String payload;
    String fullLoadMessage;
  payload = "{";
  payload += "\"device_ip\":"; payload += WiFi.localIP().toString();
  payload += "}";
  fullLoadMessage = WiFi.localIP().toString();


  // Отправляем IP адрес по MQTT, чтобы самим знать какой IP устройству назначила точка доступа
  // А также немного отладочной информации, видимой в мониторе последовательного порта при подключении
  Serial.print("ThingsBoard server address: ");
  Serial.println(String(thingsboardServer) + ":" + String(thingsboardServerPort));
  client.setServer( thingsboardServer, thingsboardServerPort );
  Serial.print("\nConnecting to Thingsboard node (device token: " + String(TOKEN) + ")...");
  // Попытка соединения 
  if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
    Serial.print("[DONE]\n");
    char attributes[100];
    // Отправляем
    payload.toCharArray( attributes, 100 );
    client.publish( "v1/devices/me/attributes", attributes );
    Serial.println("Device ip: " + WiFi.localIP().toString());
  } else {
    Serial.print("[FAILED] [ rc = ");
    Serial.print(client.state());
    Serial.println(" : retrying ]");
  }

}



 
