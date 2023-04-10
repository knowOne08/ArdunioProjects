#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const char* ssid = "DataAcessPoint"; // Replace with your desired access point name
const char* password = "123456789"; // Replace with your desired access point password
IPAddress local_IP(192, 168, 4, 22); // Replace with your desired IP address
IPAddress gateway(192, 168, 4, 9); // Replace with your desired gateway IP address
IPAddress subnet(255, 255, 255, 0); // Replace with your desired subnet mask

Adafruit_MPU6050 mpu;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("Starting access point...");
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.println("Access point started");

  server.begin();
  Serial.println("HTTP server started");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  delay(100);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  String data = "Acceleration X: " + String(a.acceleration.x) + " m/s^2\n";
  data += "Acceleration Y: " + String(a.acceleration.y) + " m/s^2\n";
  data += "Acceleration Z: " + String(a.acceleration.z) + " m/s^2\n";
  data += "Net Acceleration: " + String(sqrt(((a.acceleration.x)*(a.acceleration.x))+((a.acceleration.y)*(a.acceleration.y))+((a.acceleration.z)*(a.acceleration.z))));
  data += "Rotation X: " + String(g.gyro.x) + " rad/s\n";
  data += "Rotation Y: " + String(g.gyro.y) + " rad/s\n";
  data += "Rotation Z: " + String(g.gyro.z) + " rad/s\n";
  data += "Temperature: " + String(temp.temperature) + " degC\n";

  String response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Connection: close\r\n"
    "Refresh: 5\r\n"
    "\r\n"
    + data;

  client.print(response);

  Serial.println("Response sent");
  delay(500);
}
