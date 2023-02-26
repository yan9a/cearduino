// Name: web_server_led.ino
// By: Yan Naing Aye
// Description: Arduino Uno paired with Ethernet shield v2 as a web server 
// that controls an LED
// http://cool-emerald.blogspot.sg/

#include <SPI.h>
#include <Ethernet2.h>

//Start of Declaration for Hardware ------------------------------------
class MyHardware {
   public:
      int RLED_Pin; 
      int RLED_Val;
      int GLED_Pin;   
      int GLED_Val;
      int SW1_Pin;   
      int SW1_Val;
      MyHardware();
      void begin();
      void update();
};
MyHardware::MyHardware() {
  RLED_Pin=5;   
  RLED_Val=0;
  GLED_Pin=6;   
  GLED_Val=128;
  SW1_Pin=0;   
  SW1_Val=0;
}
void MyHardware::begin() {
  pinMode(RLED_Pin, OUTPUT);
  digitalWrite(RLED_Pin, RLED_Val);
  
  pinMode(GLED_Pin, OUTPUT);
  analogWrite(GLED_Pin, GLED_Val);
  
  pinMode(SW1_Pin, INPUT);
  SW1_Val = digitalRead(SW1_Pin);
}
void MyHardware::update() { 
 //Start of Switch 1 input-------------------- 
  SW1_Val = digitalRead(SW1_Pin);
  //End of Switch 1 ------------------------------------
  
}
MyHardware H;
//End of Declaration for Hardware ------------------------------------

// MAC address that is found on the sticker pasted on Ethernet shield v2
byte mac[]={0x90, 0xA2, 0xDA, 0x0F, 0xF9, 0x51};

// Initialize the Ethernet server (port 80 is default for HTTP)
EthernetServer server(80);
EthernetClient client;

enum ServerState { Chk_END, Chk_NORMAL, Chk_CONTROL };
ServerState s = Chk_END;

char c;

void setup() {
  Serial.begin(9600);
  H.begin();
  
  pinMode(4, OUTPUT);//to disable SD card
  digitalWrite(4, HIGH);
  
  Serial.print("Beginning Ethernet...");
  if (!Ethernet.begin(mac)) { Serial.println("failed."); return; }
  else {Serial.println("success.");}
  
  Serial.println("Starting server...");
  server.begin();
  
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // listen for incoming clients
  client = server.available();
  if (client) {
    Serial.println("New client.");
    while (client.connected()) {
      if (client.available()) {
        c = client.read(); Serial.write(c); //read and echo        
        if (s == Chk_CONTROL) ProcessCmd(c);
        
        //check and update the server state
        if(c=='?') s=Chk_CONTROL; 
        else if(c=='\n'){ 
          if(s==Chk_END) { HTTP_Response(); break;}
          else s=Chk_END;
        } 
        else if(c!='\r') { s=Chk_NORMAL; }
      }
    }
    delay(1);// give the web browser time to receive the data
    client.stop();// close the connection
    Serial.println("Client disconnected.");
    Ethernet.maintain();
  }
  H.update();
}

void ProcessCmd(char c) {
  //Start of red LED digital output--------------------
  if(c=='R') {            
    H.RLED_Val=client.parseInt(); 
    digitalWrite(H.RLED_Pin, H.RLED_Val); 
  }
  //End of red LED ------------------------------------
  
  //Start of Green LED analog output--------------------
  else if(c=='G') {            
    H.GLED_Val=client.parseInt(); 
    analogWrite(H.GLED_Pin, H.GLED_Val);
  }
  //End of Green LED ------------------------------------
}
void HTTP_Response() {
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close"); 
  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name='referrer' content='none'>");
  client.println("<script>");
  client.println("function SendCtrl(c,v){");
  client.println("window.location.href = \"?\"+c+\"=\"+v;");
  client.println("}");
  client.println("</script>");
  client.println("</head>");
  client.println("<body>");
  
  //Start of red LED digital output--------------------
  client.println("<div>");
  char* cStr[]={"255", "128"};
  char* vStr[]={"1", "0"};
  client.println("<br/>Click this button to toggle the red LED: ");
  client.print("<button name='R' type='button' style='background-color: rgb(");
  client.print(cStr[H.RLED_Val]); 
  client.println(",0,0);'onclick='SendCtrl(this.name,this.innerHTML);'>");
  client.println(vStr[(int)H.RLED_Val]);
  client.println("</button><br/>");
  client.println("</div>");
  //End of red LED ------------------------------------
  
  //Start of Green LED analog output--------------------
  client.println("<div>");
  client.println("<br/>Drag the slider to adjust the brightness of green LED: ");
  client.print("<input name='G' type='range' value='");
  client.print(String(H.GLED_Val)); 
  client.println("' min='0' max='255'");
  client.println(" onchange='SendCtrl(this.name,this.value);'>");
  client.println("</div>");
  //End of Green LED ------------------------------------
  
  //Start of Switch 1 input--------------------
  client.println("<div>");
  if(!H.SW1_Val) { client.println("<p style='color:rgb(0,255,0);'>SW1 ON</p>");}
  else { client.println("<p style='color:rgb(128,0,0);'>SW1 OFF</p>");}
  client.println("</div>");
  //End of Switch 1 ------------------------------------
  
  client.println("</body>");
  client.println("</html>");  
}
