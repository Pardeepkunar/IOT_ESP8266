#include<SoftwareSerial.h>
int srdata;
int prdata;
int pump_status;
SoftwareSerial esp8266(3,4);
#define SSID "pardeep"
#define PASS "12345678"

//******************************sendAT command to esp8266

String sendAT(String command, const int timeout)
{
 String response ="";
 esp8266.print(command);
 long int time = millis();
 while((time+timeout) > millis())
 {
  while(esp8266.available())
  {
    char c = esp8266.read();
    response += c;
  }
 }
 Serial.print(response);
 return response;
}

void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);
  Serial.println("weather moniterig");
  sendAT("AT+RST\r\n",2000);           // reset command
  sendAT("AT\r\n",1000);               // AT command for starting
  sendAT("AT+CWMODE=1\r\n",1000);      // set esp8266 station mode
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n",2000); // for set ssid and pass of hotspot
  while(!esp8266.find("OK"))
  {
        
  }
  sendAT("AT+CIFSR\r\n",1000); // Get the IP address of esp8266
  sendAT("AT+CIPMUX=0\r\n",2000); // connect single cable connect

     pinMode(A0,INPUT);
     pinMode(8,OUTPUT);
  
}
void loop()
{
  srdata=analogRead(A0);
  prdata=map(srdata,0,1023,100,0);
  Serial.print("sensor data");
  Serial.println(prdata);
  String sensor_value= String(prdata);
  if(prdata<50)
  {
    digitalWrite(8,HIGH);
    pump_status=100;
   Serial.println("******************************************************");
    
    
  }
  else
  {
    digitalWrite(8,LOW);
    pump_status=0;
      
  }
  
  String pump = String(pump_status);
  updateTS(sensor_value,pump);
  delay(2000);
  }
  
  void updateTS(String T, String P)
  {
  Serial.println("");
  sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n",1000);
  delay(2000);
  String cmdlen;
  String cmd = "GET /update?key=Z3O4WEJ7GIQ63L8X&field1="+T+"&field2="+P+"\r\n";
  cmdlen = cmd.length();
  sendAT("AT+CIPSEND="+cmdlen+"\r\n",2000);
  esp8266.print(cmd);
  Serial.println("");
  sendAT("AT+CIPCLOSE\r\n",2000);
  Serial.println("");
  delay(1500);
}
