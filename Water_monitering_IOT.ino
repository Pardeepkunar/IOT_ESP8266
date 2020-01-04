byte statusLed=13;
byte sensorInterrupt=0;
byte sensorPin=2;

// the hall effect flow sensor output approx 4.5 pulses per second per litre/minute of flow  

float calibrationFactor=4.5;
volatile byte pulseCount;
float flowRate;
unsigned int flowMilliLitres;
unsigned int totalMilliLitres;
unsigned int oldTime;

void setup()
{
  // put setup code for ones time
  // initialize a serial connection for reporting value to the host
  Serial.begin(9600);
  pinMode(statusLed,OUTPUT);
  digitalWrite(statusLed,HIGH);
  pinMode(sensorPin,INPUT);
  digitalWrite(statusLed,HIGH);
  pulseCount=0;
  flowRate=0.0;
  flowMilliLitres=0;
  totalMilliLitres=0;
  oldTime=0;
  attachInterrupt(sensorInterrupt,pulseCounter,FALLING);
 }


 void loop()
 {
  if((millis()- oldTime)>1000) // only process counters ones per second
  {
    // Disable the interrupt while calculating flow rate and sending the value to host
    detachInterrupt(sensorInterrupt);
    //because this loop may not complete in 1 second intervel we calculate the number of milliseconds 
    flowRate=((1000.0/(millis()-oldTime))*pulseCount)/calibrationFactor;

    // Note the time this process pass was executeed.
    // disable interrupts the  mills()
    // at this point , but it will still return the value it was set to just before interupt were disable
    oldTime=millis();

    // divide the flow rate in literes/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to convert milliliters.
    flowMilliLitres=(flowRate/60)*1000;

    // add the millilitres passed in this second to the cumulative total
    totalMilliLitres+=flowMilliLitres;
    unsigned int frac;

    // print the flow rate for this second in litres/ minute 
    Serial.print("flow rate:");
    Serial.print(int(flowRate)); // print the interger part of variable
    Serial.print(".");           // print the decimal point

    // determine the fraction part the 10 multiplier gives us 1 decimal place.
    frac = (flowRate-int(flowRate))*10;
    Serial.print(frac,DEC);       // print the fractional part variable
    Serial.print("L/min");

    //print the number of litres flowed in this second
    Serial.print("Current Liquit Flowing");
    Serial.print(flowMilliLitres);
    Serial.print("mL/sec");

    //print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity");
    Serial.print(totalMilliLitres);
    Serial.println("mL");

    //reset the pulse counter so we start incrementing again
    pulseCount=0;

    //enable the interrupt again how that we finish sending output
    attachInterrupt(sensorInterrupt,pulseCounter,FALLING);
   }
 }
/*
 * interrupt service routine
 */
 void pulseCounter()
 {
  pulseCount++;
 }
