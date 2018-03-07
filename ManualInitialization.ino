/*  
 *  GPRS/GSM Quadband Module (SIM900)
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  a
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           2.0
 *  Design:            David Gascón 
 *  Implementation:    Alejandro Gallego & Marcos Martinez
 */
#include <SoftwareSerial.h>


int8_t answer = 0;

char data[512];
int data_size;

char aux_str[100];
char aux;
int x = 0;

SoftwareSerial mySerial(2, 3);

char pin[]="";
char apn[]="m2mde.telefonica.com";
char user_name[]="";
char password[]="";

char url[ ]="test.libelium.com/test-get-post.php?a=1&b=2";


void setup(){
    Serial.begin(9600);
    mySerial.begin(9600);
    answer = sendATcommand("AT", "OK", 2000);
    Serial.println(answer);
    while(answer == 0) {
      answer = sendATcommand("AT", "OK", 2000);
      Serial.println(answer);
      Serial.println("Not Connected, trying again...");  
    }
    Serial.println("Connected");
    
    while (sendATcommand2("AT+CREG?", "+CREG: 0,2", "+CREG: 0,5", 2000) == 0);
    Serial.println("Registered");
    
    snprintf(aux_str, sizeof(aux_str), "AT+QIREGAPP=\"%s\",\"\",\"\"", apn);
    sendATcommand(aux_str, "OK", 2000);

    Serial.println("GPRS Activated");
}

void loop(){
}


int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( mySerial.available() > 0) mySerial.read();    // Clean the input buffer

    mySerial.println(ATcommand);    // Send the AT command 
    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(mySerial.available() != 0){    
            response[x] = mySerial.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - previous) < timeout));    
    
    Serial.print("Response: ");
    Serial.println(response);
    return answer;
}



int8_t sendATcommand2(char* ATcommand, char* expected_answer1, 
char* expected_answer2, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( mySerial.available() > 0) mySerial.read();    // Clean the input buffer

    mySerial.println(ATcommand);    // Send the AT command 


    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{        
        if(mySerial.available() != 0){    
            response[x] = mySerial.read();
            x++;
            // check if the desired answer 1 is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
            // check if the desired answer 2 is in the response of the module
            if (strstr(response, expected_answer2) != NULL)    
            {
                answer = 2;
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));    
    Serial.println(response);
    return answer;
}
