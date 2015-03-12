#include <SPI.h>
#include <Ethernet.h>
 
boolean incoming = 0;
 
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
IPAddress ip(192,168,11,2);
EthernetServer server(80);
 
void setup()
{
  pinMode(2, OUTPUT);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
}
 
void loop()
{
  int port = -1;
  int command = -1;
  EthernetClient client = server.available();
  if (client) 
  {
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available())
      {
        char c = client.read();

        if(c == 'p' && client.read() == '=')
        {
          c = client.read();
          port = c - 48;
          c = client.read();
          if(c != '&')
          {
            port = 10;
            port += c - 48; 
            c = client.read();
          }
  
          if(c == '&')
          {
            c = client.read();            
            if(c == 'c' && client.read() == '=')
            {
              command = client.read() - 48;
            }
            else
            {
              client.stop();
              return;
            }
                  
            switch(command)
            {
              case 0:
                analogWrite(port, 0);
                break;
              case 1:
                analogWrite(port, 255);
                break;
             default:
               client.stop();
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.print("port: ");
            client.println(port);
            client.print("command: ");
            client.println(command);
            delay(1);
            client.stop();
            return;
          }
          else
          {
            client.stop();
            return;
          }
        }
      }
    }
  }
}
