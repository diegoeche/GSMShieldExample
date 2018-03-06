#include <GSM.h>

// PIN number if necessary
#define PINNUMBER ""

// APN information obrained from your network provider
#define GPRS_APN       "m2mde.telefonica.com" // replace with your GPRS APN
#define GPRS_LOGIN     "" // replace with your GPRS login
#define GPRS_PASSWORD  "" // replace with your GPRS password

// initialize the library instances
GSMClient client;
GPRS gprs;
GSM gsmAccess;

// This example downloads the URL "http://arduino.cc/latest.txt"

char server[] = "arduino.cc"; // the base URL
char path[] = "/latest.txt"; // the path
int port = 80; // the port, 80 for HTTP

void setup()
{
  // initialize serial communications
  Serial.begin(9600);
  Serial.println("Starting Arduino web client.");
  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // pass the PIN of your SIM as a parameter of gsmAccess.begin()
  while(notConnected)
  {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("Connected to GSM");
  
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.available() && !client.connected())
  {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    for(;;);
  }
}
 
