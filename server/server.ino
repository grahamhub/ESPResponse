#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define RED_LED 4 // GPIO4 - D2 
#define BLUE_LED 5 // GPIO5 - D1
#define GREEN_LED 14 // GPIO14 - D5 
#define BUTTON 12 // GPIO12 - D6 
#define API_PORT 8080 // Port number for API 

const char* ssid = "fake_ssid"; // your router's SSID
const char* password = "fake_password"; // your router's password

int redValue; 
int greenValue;
int blueValue;

bool requestAccepted;
bool forbidden;
bool invalidArgs;

ESP8266WebServer server(API_PORT); // instance of WebServer with port number 8080

int getIdxOfMax(int arr[], int n) {
  int idx = 0;
  int largest = arr[0];

  for (int i = 1; i < n; i++) {
    if (arr[i] > largest) {
      largest = arr[i];
      idx = i;
    }
  }

  return idx;
}

void toggleLEDs() {
  digitalWrite(RED_LED, redValue);
  digitalWrite(GREEN_LED, greenValue);
  digitalWrite(BLUE_LED, blueValue);
}

String getActiveLED() {
  if (redValue) {
    return "RED";
  } else if (greenValue) {
    return "GREEN";
  } else if (blueValue) {
    return "BLUE";
  } else {
    return "NONE";
  }
}

bool btnChanged(int lastState) {
  if (lastState == HIGH && digitalRead(BUTTON) == LOW) {
    return true;
  } else {
    return false;
  }
}

void waitForBtn() {
  forbidden = true;
  requestAccepted = false;
  long starttime = millis();
  long endtime = starttime;
  int btnState = digitalRead(BUTTON);
  while ((endtime - starttime) <= 10000) { // wait 10 secs
    yield(); // ESP processors have a very low blocking threshold so make sure to yield during long-blocking loops
    if (btnChanged(btnState)) {
      Serial.println("Button pressed");
      requestAccepted = true;
      forbidden = false;
      return;
    }
    endtime = millis();
    btnState = digitalRead(BUTTON);
  }

  Serial.println("Request rejected");
}

int getStatus() {
  if (requestAccepted) {
    return 202;
  }

  else if (invalidArgs) {
    return 400;
  }

  else if (forbidden) {
    return 403;
  }

  return 200;
}

String buildJson() {
  String json = "{\"message\": ";

  if (requestAccepted) {
    json += "\"Request received. Active LED: ";
    json += getActiveLED();
    json += ".\"";
  } else if (invalidArgs) {
    json += "\"Invalid query parameters/arguments.\"";
  }
  else if (forbidden) {
    json += "\"Request was rejected.\"";
  }

  json += "}";

  return json;
}

void handleArgs() {
  if (forbidden) {
    requestAccepted = false;
    return;
  }

  String red = server.arg("r");
  String green = server.arg("g");
  String blue = server.arg("b");
  
  if (red == "" || green == "" || blue == "") {
    invalidArgs = true;
    requestAccepted = false;
    return;
  }

  requestAccepted = true;

  int queryArgs[3] = {atoi(red.c_str()), atoi(green.c_str()), atoi(blue.c_str())};
  int whichRgb = getIdxOfMax(queryArgs, 3);

  switch(getIdxOfMax(queryArgs, 3)) {
    case 0:
      redValue = 1;
      greenValue = 0;
      blueValue = 0;
      break;
    case 1:
      redValue = 0;
      greenValue = 1;
      blueValue = 0;
      break;
    case 2:
      redValue = 0;
      greenValue = 0;
      blueValue = 1;
      break;
    default:
      redValue = 0;
      greenValue = 0;
      blueValue = 0;
  }
}

void buildResponse() {
  digitalWrite(LED_BUILTIN, LOW);
  waitForBtn();
  handleArgs();
  if (requestAccepted) {
    toggleLEDs();
  }
  digitalWrite(LED_BUILTIN, HIGH);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(getStatus(), "text/json", buildJson());
}

void handleNotFound() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(404, "text/json", "{\"error\": \"Resource does not exist\"}");
}

void handleTest() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/json", "{\"message\": \"Test request received.\"}");
}

void serverRouting() {
  server.on(F("/test"), HTTP_GET, handleTest);
  server.on(F("/leds"), HTTP_GET, buildResponse);
}

void setup() 
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  redValue = 0;
  greenValue = 0;
  blueValue = 0;
  toggleLEDs();
  
  Serial.print("\n");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); // Configure ESP8266 to use station mode
  WiFi.begin(ssid, password); // Connect to Wi-Fi based on credentials
  while(WiFi.status() != WL_CONNECTED) // Wifi connection process indicator
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("*");
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }
  Serial.print("\n");
  Serial.print("Connected to Wi-Fi: ");
  Serial.println(WiFi.SSID());
  Serial.print("Using local IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);

  serverRouting();
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP Server started");
}

void loop()
{
 server.handleClient();
}
