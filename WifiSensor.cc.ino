#include <WiFiNINA.h>
#include <Arduino_MKRENV.h>
#include <ArduinoHttpClient.h>
#include <ArduinoLowPower.h>


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "[SSID]";        // your network SSID (name)
char pass[] = "[PASS]";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

WiFiClient wifi;
HttpClient client(wifi, "IP_ADDR", 8080);

float celsius;
float humidity;
float pressure;
float illuminance;

void setup() {
  Serial.println("enter setup");
  Serial.begin(115200);

  // check if the WiFi module works
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  Serial.println("connecting...");
  while ( status != WL_CONNECTED) {
    connect();

    // wait 10 seconds for connection:
    delay(10000);

    Serial.println("init environmental sensors...");
    if (!ENV.begin()) {
      Serial.println("Failed to initialize MKR ENV shield!");
      while (1);
    }
    Serial.println("...done");
  }
  WiFi.lowPowerMode();

  // you're connected now, so print out the status:
//  printWiFiStatus();
}

void connect() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  status = WiFi.begin(ssid, pass);
}

void disconnect() {
  status = WL_IDLE_STATUS;
  WiFi.disconnect();
}

// the loop function runs over and over again forever
void loop() {
  sampleTemp();
  putTemp();
  putState();

  LowPower.deepSleep(300 * 1000);
}

void sampleTemp() {
  celsius = ENV.readTemperature();
  humidity = ENV.readHumidity();
  pressure    = ENV.readPressure();
  illuminance = ENV.readIlluminance();
}

// this method makes a HTTP connection to the server:
void putTemp() {
  String contentType = "application/json;charset=UTF-8";
  String path = "/temp/";
  String body = "{\"celsius\":" + String(celsius) + "," 
    + "\"humidity\":" + String(humidity) + "," 
    + "\"pressure\":" + String(pressure) + "," 
    + "\"illuminance\":" + String(illuminance) + "}";

  client.put(path, contentType, body);
  int statusCode = client.responseStatusCode();
  Serial.println(client.responseBody());
}

void putState() {
  String ssid = WiFi.SSID();

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  String ipAddr = String(ip[0] + String(".") + ip[1] + String(".") + ip[2] + String(".") + ip[3]);

  // print the received signal strength:
  long rssi = WiFi.RSSI();

  String contentType = "application/json;charset=UTF-8";
  String path = "/status/";
  String body = "{\"ssid\":\"" + ssid + "\","
    + "\"address\":\"" + ipAddr + "\"," 
    + "\"state\":\"" + "ONLINE" + "\"," 
    + "\"signal\":" + String(rssi) + "}"; 
  
  client.put(path, contentType, body);
  int statusCode = client.responseStatusCode();
  Serial.println(client.responseBody());
}

//void printWiFiStatus() {
//  // print the SSID of the network you're attached to:
//  Serial.print("SSID: ");
//  Serial.println(WiFi.SSID());
//
//  // print your WiFi shield's IP address:
//  IPAddress ip = WiFi.localIP();
//  Serial.print("IP Address: ");
//  Serial.println(ip);
//
//  // print the received signal strength:
//  long rssi = WiFi.RSSI();
//  Serial.print("signal strength (RSSI):");
//  Serial.print(rssi);
//  Serial.println(" dBm");
//}
