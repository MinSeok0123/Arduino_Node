#include <ArduinoJson.h>
#include <ESP8266WiFi.h> // ESP 8266 와이파이 라이브러리

int count = 0;
//MAC주소
uint8_t MAC_array[6];
char MAC_char[18];
String buf;

char ssid[] = "bssm_guest";
char pass[] = "bssm_guest";
int status = WL_IDLE_STATUS;


WiFiClient client;
String line;
String li;

///////////////미세먼지 핀/////////////////////
int Vo = A0;

int V_LED = 3;

float Vo_value = 0;

float Voltage = 0;

float dustDensity = 0;

////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  //MAC주소
  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i) {
    sprintf(MAC_char, "%s%02x:", MAC_char, MAC_array[i]);
  }
  delay(500);
  WiFi.begin("bssm_guest", "bssm_guest"); // 공유기 이름과 비밀번호

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) // 와이파이 접속하는 동안 "." 출력
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); // 접속된 와이파이 주소 출력
  ///////////////////////////////
  pinMode(V_LED, OUTPUT);

  pinMode(Vo, INPUT);
  ///////////////////////////////
}

void loop() {

  ///////////////////////////////
  digitalWrite(V_LED, LOW);

  delayMicroseconds(280);

  Vo_value = analogRead(Vo);

  delayMicroseconds(40);

  digitalWrite(V_LED, HIGH);

  delayMicroseconds(9680);



  Voltage = Vo_value * 5.0 / 1023.0;

  dustDensity = (Voltage - 0.3) / 0.005; 

  ///////////////////////////////
  String jsondata = "";
  int re = client.connect("192.168.10.220", 8000);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["macadress"] = MAC_char;
  root["Volatage"] = Voltage;
  root["Dust Density"] = dustDensity;
  root.printTo(jsondata);
  Serial.println(jsondata); //jsondata를 String으로 출력
  re = client.connect("192.168.10.220", 8000);
  if (re) {
    client.println("GET /test HTTP/1.1");
    client.println("Host: 192.168.10.220:8000");
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsondata.length());
    client.println();
    client.print(jsondata);
    li = client.readString();
    Serial.println(li);
    for (int i = 0; li[i] != '\0'; i++) {
      if (count > 7) {
        buf += li[i];
      }
      if (li[i] == '\r') {
        count++;
      }
    }
    Serial.println(buf);
    buf = "";
    count = 0;
  }
}
