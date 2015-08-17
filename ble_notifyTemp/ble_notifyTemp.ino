#include <WiFi.h>
#include<stdlib.h>

char NotifyFlg;

WiFiServer server(3000);

void setup(){
  
    Serial.begin(9600);

    server.begin();
    Serial.print("Connected to TCP. My address:");
    IPAddress myAddress(127,0,0,1);
    Serial.println(myAddress);
    
    pinMode(A0,INPUT);
    
}

void loop(){
  
    int rawVal;
    float temp;
    String str;
    char cstr[16];
    
    WiFiClient client = server.available();
    
    
    if (client.available() > 0) { 
      
      NotifyFlg = client.read(); 
      Serial.println(NotifyFlg);
      
      if(NotifyFlg == '1'){
      
          for (int i = 0; i < 30; i++){
            rawVal = rawVal + analogRead(0);
          }
          
          rawVal = rawVal / 30;
        
          temp = log(10000.0*((1024.0/rawVal-1)));
          temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
          temp = temp - 273.15;
              
          dtostrf(temp,3,1,cstr);
          client.write(cstr);
          
          Serial.print("send data :");
          Serial.println(cstr);
          
          NotifyFlg = '0';

      }
   }
}


char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
    char fmt[20];
    sprintf(fmt, "%%%d.%df", width, prec);
    sprintf(sout, fmt, val);
    return sout;
}

