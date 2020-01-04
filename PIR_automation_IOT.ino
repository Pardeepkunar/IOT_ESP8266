#include<SoftwareSerial.h>
int data;
int data1;
boolean lock=true;
int counter=0;
int Light_status=100;

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

void connectwifi()
{
  sendAT("AT\r\n",1000);               // AT command for starting
  sendAT("AT+CWMODE=1\r\n",1000);      // set esp8266 station mode
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n",2000); // for set ssid and pass of hotspot
  while(!esp8266.find("OK"))
  {
        
  }
  sendAT("AT+CIFSR\r\n",1000); // Get the IP address of esp8266
  sendAT("AT+CIPMUX=0\r\n",1000); // connect single cable connect

}


void setup() 
{
 Serial.begin(9600);
 esp8266.begin(9600);
 pinMode(10,INPUT);
 pinMode(11,INPUT);
 pinMode(12,OUTPUT);
 pinMode(13,OUTPUT);
 pinMode(8,OUTPUT);
 Serial.print("calibration sensor");
 for(int i=0;i<10;i++)
 {
  Serial.print(".");
  delay(1000);
 }
  Serial.print("Done");
  Serial.print("Sensor activate");
  digitalWrite(12,HIGH);
  delay(2000);
  digitalWrite(12,LOW);
  delay(50);
  
}

void loop() 
{
  data=digitalRead(10);
  data1=digitalRead(11);
  
  if(data==HIGH && lock==true)
  {
    lock=false;
    digitalWrite(12,HIGH);
    delay(500);
     digitalWrite(12,LOW);
     Serial.print("Sensor IN");
     counter++;
     if(counter>0)
     {
       digitalWrite(8,HIGH);
       Light_status=100;
       passTS(counter, Light_status);
     }
    delay(4000);
    lock=true;
    
  }
 if(data1==HIGH && lock==true)
  {
    lock=false;
    digitalWrite(12,HIGH);
    delay(500);
     digitalWrite(12,LOW);
     Serial.print("Sensor OUT");
     counter--;
     if(counter==0)
     {
       digitalWrite(8,LOW);
       Light_status=10;
     }
    passTS(counter, Light_status);
    delay(4000);
    lock=true;
    
  }
 
}
void passTS(int c,int l)
{
  String counter1=String(c);
  Serial.print("number of persion");
  Serial.println(c);

  String Light_status1=String(l);
  Serial.print("Light Status");
  Serial.println(l);
  updateTS(counter1,Light_status1);
 delay(3000);
}
void updateTS(String S,String L)
  {
  Serial.println("");
  sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n",1000);
  delay(2000);
  String cmdlen;
  String cmd = "GET /update?key=ULIT5XSHD739PXDA&field1="+S+"&field2="+L+"\r\n";
  cmdlen = cmd.length();
  sendAT("AT+CIPSEND="+cmdlen+"\r\n",2000);
  esp8266.print(cmd);
  Serial.println("");
  sendAT("AT+CIPCLOSE\r\n",2000);                  
  Serial.println("");
  delay(1000);
  
}
