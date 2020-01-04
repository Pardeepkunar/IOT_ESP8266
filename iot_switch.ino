//************************ 1 part one for basic start**************

#include<SoftwareSerial.h>
String temp;
SoftwareSerial esp8266(3,4);
#define SSID "pardeep"
#define PASS "12345678"

String sendAT(String command, const int timeout)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while((time + timeout)>millis())
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

//************************ 2 part second for basic start**************

void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);
  Serial.println("weather moniterig");
  
  sendAT("AT+RST\r\n",2000);
  sendAT("AT\r\n",1000);
  sendAT("AT+CWMODE=1\r\n",1000);      // set esp8266 station mode
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n",2000);
 /* while(!esp8266.find("OK"))
  {
    
  }*/
  sendAT("AT+CIFSR\r\n",1000);
  sendAT("AT+CIPMUX=0\r\n",2000); 
}

//************************ 3 part third for basic start**************

void loop()
{
  int switc_h = digitalRead(5);
  if(switc_h == 1)
  {
     Serial.print("on");
     temp = 100;
  }
  else
  {
     Serial.print("off");
     temp = 50;
  }
  updateTS(temp); 
  delay(100);
  
}
void updateTS(String T)
{
  Serial.println("");
  sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n",1000);
  delay(2000);
  String cmdlen;
  String cmd = "GET /update?key=Z3O4WEJ7GIQ63L8X&field1="+T+"\r\n";
  cmdlen = cmd.length();
  sendAT("AT+CIPSEND="+cmdlen+"\r\n",2000);
  esp8266.print(cmd);
  Serial.println("");
  sendAT("AT+CIPCLOSE\r\n",2000);
  Serial.println("");
  delay(1500);
}
