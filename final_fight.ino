///watch video for details of lines below
//static const uint8_t D0   = 16;
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4;
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2;
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;
//static const uint8_t D10  = 1;

//int Pin1 = D1;//IN1 is connected 
//int Pin2 = D2;//IN2 is connected   
//int Pin3 = D3;//IN3 is connected 
//int Pin4 = D4;//IN4 is connected 

 
int pole1[] ={0,0,0,0, 0,1,1,1, 0};//pole1, 8 step values
int pole2[] ={0,0,0,1, 1,1,0,0, 0};//pole2, 8 step values
int pole3[] ={0,1,1,1, 0,0,0,0, 0};//pole3, 8 step values
int pole4[] ={1,1,0,0, 0,0,0,1, 0};//pole4, 8 step values

int poleStep = 0; 
int  dirStatus = 3;// stores direction status 3= stop (do not change)

String buttonTitle1[] ={"SHOOT", "STOP"};//Watch Video for details (on Udemy)
String buttonTitle2[] ={"SHOOT", "STOP"};
String argId[] ={"ccw", "cw"};

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>







//Our HTML webpage contents in program memory
const char IN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<!DOCTYPE html>
<html>

<head>
    <style>
        a{
            display: inline-block;
            
        }
        .layer{
            margin: 1em 0;
            width: 100%;
            height: 4em;
            text-align: center;
        }   
        .arrow{
            width: 4em;
            height: 4em;
            border-left: 5px solid #30475e;
            border-top: 5px solid #30475e;
        }
        .arrow:active,.arrow:hover{
            border-width: 7px;
        }
        .stop-circle{
            width: 4em;
            height: 4em;
            border: 5px solid #f05454;
            background-color: #f05454;
            border-radius: 50%;
            box-shadow: 0 9px #999;
        }
        .stop-circle:hover{
            border-color: #cc4545;
            background-color: #cc4545;
        }
        .stop-circle:active{
            transform: translateY(4px);
            box-shadow: 0 5px #666;
        }
        .up{
            transform: rotate(45deg); 
        }
        .down{
            transform: rotate(-135deg);
        }
        .left{
            transform: rotate(-45deg);
        }
        .right{
            transform: rotate(135deg);
        }
    </style>
</head>

<body>
    <div class="remoteController">
        <div class="first layer">
            <a class="arrow up"  title="Next" href="Forward"></a>
        </div>
        <div class="second layer">
            <a class="arrow left"  title="Previous" href="Left"></a>
            <a href="Stop" class="stop-circle"></a>
            <a class="arrow right"  title="Next" href="Right"></a>
        </div>
        <div class="third layer">
            <a class="arrow down"  title="Next" href="Backward"></a>
        </div>

         <div class="third layer" style="margin-left:100px;">
            <a class="arrow down"  title="Next" href="start"></a>
        </div>
        
    </div>

</body>

</html>
)====="







#include "PageIndex.h"; //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

#define ServoPort D8   //--> Defining Servo Port


#ifndef STASSID
#define STASSID "Narayan"
#define STAPSK  "Krishna12"
#endif

Servo myservo;  // create servo object to control a servo

static const int servoPin = 8;
const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);
String header;
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


String page = ""; //For the Web Server
String page2="";  //For updating Status of motor 1
String page3="";  //For updating status of motor 2

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 
String p = IN_page;
   //Robojax.com ESP32 Relay Motor Control
 String HTML ="<!DOCTYPE html>\
  <html>\
  <head>\
  \t\n<title>Robojax 28BYJ-48 Motor Control</title>\
  \t\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  \n<style>\
 \nhtml,body{\t\nwidth:100%\;\nheight:100%\;\nmargin:0}\n*{box-sizing:border-box}\n.colorAll{\n\tbackground-color:#90ee90}\n.colorBtn{\n\tbackground-color:#add8e6}\n.btn{\n\tmargin:5px\;\nborder:none\;\ndisplay:inline-block\;\nvertical-align:middle\;\ntext-align:center\;\nwhite-space:nowrap}\n";
   
  HTML +="</style>\n\n</head>\n\n<body>\n<h1> </h1>\n";

   if(dirStatus ==2){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\"></span></h2>\n";    
   }else if(dirStatus ==1){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\"></span></h2>\n";      
   }else{
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\"></span></h2>\n";    
   }
      if(dirStatus ==1){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[0];
        HTML += "=off\">";
        HTML +=buttonTitle1[0]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[0];
        HTML += "=on\">";       
        HTML +=buttonTitle2[0];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";  
       //Watch details at my Arduino Course at Udemy.com
      if(dirStatus ==2){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[1];
        HTML += "=off\">";
        HTML +=buttonTitle1[1]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[1];
        HTML += "=on\">";       
        HTML +=buttonTitle2[1];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";     

  HTML +="\t\n</body>\n</html>\n";
  server.send(200, "text/html", p+s+HTML+page+page2+page3); 

}
void handleServo(){
  String POS = server.arg("servoPOS");
  int pos = POS.toInt();
  myservo.write(pos);   //--> Move the servo motor according to the POS value
  delay(15);
  Serial.print("Servo Angle:");
  Serial.println(pos);
  server.send(200, "text/plane","");
}






void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);

}





/*
 * motorControl()
 * updates the value of "dirStatus" varible to 1, 2 or 3
 * returns nothing
 * written by Ahmad Shamshiri
 * on Wednesday April 19, 2020 at 18:03 in Ajax, Ontario, Canada
 * www.robojax.com
 */
void motorControl() {

//Watch details at my Arduino Course at Udemy.com
    if(server.arg(argId[0]) == "on")
    {
      dirStatus = 1;// CCW 
               
    }else if(server.arg(argId[0]) == "off"){
      dirStatus = 3;  // motor OFF 
          
    }else if(server.arg(argId[1]) == "on"){
      dirStatus = 2;  // CW  
          
    }else if(server.arg(argId[1]) == "off"){
      dirStatus = 3;  // motor OFF
          
    }  

  

  handleRoot();
}//motorControl end


//void fire(){
//
//    digitalWrite(D1,HIGH);
//    digitalWrite(D2,LOW);
//  }


 void Forward() 
 {

  String s = MAIN_page; //Read HTML contents
 
String p = IN_page;
 String HTML ="<!DOCTYPE html>\
  <html>\
  <head>\
  \t\n<title>Robojax 28BYJ-48 Motor Control</title>\
  \t\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  \n<style>\
 \nhtml,body{\t\nwidth:100%\;\nheight:100%\;\nmargin:0}\n*{box-sizing:border-box}\n.colorAll{\n\tbackground-color:#90ee90}\n.colorBtn{\n\tbackground-color:#add8e6}\n.a{\n\tdisplay:inline-block}\n.btn{\n\tmargin:5px\;\nborder:none\;\ndisplay:inline-block\;\nvertical-align:middle\;\ntext-align:center\;\nwhite-space:nowrap}\n";
   
  HTML +="</style>\n\n</head>\n\n<body>\n<h1></h1>\n";

   if(dirStatus ==2){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\"></span></h2>\n";    
   }else if(dirStatus ==1){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\"></span></h2>\n";      
   }else{
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\"></span></h2>\n";    
   }
      if(dirStatus ==1){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[0];
        HTML += "=off\">";
        HTML +=buttonTitle1[0]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[0];
        HTML += "=on\">";       
        HTML +=buttonTitle2[0];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";  
       //Watch details at my Arduino Course at Udemy.com
      if(dirStatus ==2){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[1];
        HTML += "=off\">";
        HTML +=buttonTitle1[1]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[1];
        HTML += "=on\">";       
        HTML +=buttonTitle2[1];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";     

  HTML +="\t\n</body>\n</html>\n";


  digitalWrite(D5,HIGH);
    digitalWrite(D6,LOW);//fwd
    
digitalWrite(D0,HIGH);
    digitalWrite(D7,LOW);



  
//    digitalWrite(D5,HIGH);
//    digitalWrite(D6,LOW);
    page2="<center><p> motor 1 Status : Forward </p></center>";
    server.send(200,"text/html",p+s+HTML+page+page2+page3);
    delay(200);
  }
  void Left()
  {

    String s = MAIN_page; //Read HTML contents
 
String p = IN_page;
 String HTML ="<!DOCTYPE html>\
  <html>\
  <head>\
  \t\n<title>Robojax 28BYJ-48 Motor Control</title>\
  \t\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  \n<style>\
 \nhtml,body{\t\nwidth:100%\;\nheight:100%\;\nmargin:0}\n*{box-sizing:border-box}\n.colorAll{\n\tbackground-color:#90ee90}\n.colorBtn{\n\tbackground-color:#add8e6}\n.a{\n\tdisplay:inline-block}\n.btn{\n\tmargin:5px\;\nborder:none\;\ndisplay:inline-block\;\nvertical-align:middle\;\ntext-align:center\;\nwhite-space:nowrap}\n";
   
  HTML +="</style>\n\n</head>\n\n<body>\n<h1> </h1>\n";

   if(dirStatus ==2){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\"></span></h2>\n";    
   }else if(dirStatus ==1){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\"></span></h2>\n";      
   }else{
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor OFF</span></h2>\n";    
   }
      if(dirStatus ==1){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[0];
        HTML += "=off\">";
        HTML +=buttonTitle1[0]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[0];
        HTML += "=on\">";       
        HTML +=buttonTitle2[0];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";  
       //Watch details at my Arduino Course at Udemy.com
      if(dirStatus ==2){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[1];
        HTML += "=off\">";
        HTML +=buttonTitle1[1]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[1];
        HTML += "=on\">";       
        HTML +=buttonTitle2[1];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";     

  HTML +="\t\n</body>\n</html>\n";

    
    page3="<center><p> motor 2 Status : Left</p></center>";
    server.send(200,"text/html",p+s+HTML+page+page2+page3);

digitalWrite(D6, HIGH);
    digitalWrite(D5,LOW);//back

digitalWrite(D0,HIGH);
    digitalWrite(D7,LOW);//right
    
//    digitalWrite(D7,HIGH);
//    digitalWrite(D8,LOW);
    delay(200);
  }



  
  void Right()
   { 
    String s = MAIN_page; //Read HTML contents
 
String p = IN_page;
String HTML ="<!DOCTYPE html>\
  <html>\
  <head>\
  \t\n<title>Robojax 28BYJ-48 Motor Control</title>\
  \t\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  \n<style>\
 \nhtml,body{\t\nwidth:100%\;\nheight:100%\;\nmargin:0}\n*{box-sizing:border-box}\n.colorAll{\n\tbackground-color:#90ee90}\n.colorBtn{\n\tbackground-color:#add8e6}\n.a{\n\tdisplay:inline-block}\n.btn{\n\tmargin:5px\;\nborder:none\;\ndisplay:inline-block\;\nvertical-align:middle\;\ntext-align:center\;\nwhite-space:nowrap}\n";
   
  HTML +="</style>\n\n</head>\n\n<body>\n<h1>Robojax 28BYJ-48 Stepper Motor Control </h1>\n";

   if(dirStatus ==2){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor Running in CW</span></h2>\n";    
   }else if(dirStatus ==1){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor Running in CCW</span></h2>\n";      
   }else{
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor OFF</span></h2>\n";    
   }
      if(dirStatus ==1){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[0];
        HTML += "=off\">";
        HTML +=buttonTitle1[0]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[0];
        HTML += "=on\">";       
        HTML +=buttonTitle2[0];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";  
       //Watch details at my Arduino Course at Udemy.com
      if(dirStatus ==2){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[1];
        HTML += "=off\">";
        HTML +=buttonTitle1[1]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[1];
        HTML += "=on\">";       
        HTML +=buttonTitle2[1];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";     

  HTML +="\t\n</body>\n</html>\n";
    
    page3="<center><p> motor 2 Status : Right</p></center>";
    server.send(200,"text/html",p+s+HTML+page+page2+page3);


digitalWrite(D5,HIGH);
    digitalWrite(D6,LOW);//fwd


digitalWrite(D7,HIGH);
    digitalWrite(D0,LOW);//left



    
//    digitalWrite(D0,HIGH);
//    digitalWrite(D7,LOW);
    delay(200);
  }


 void Start()
   { 
    String s = MAIN_page; //Read HTML contents
 
String p = IN_page;
String HTML ="<!DOCTYPE html>\
  <html>\
  <head>\
  \t\n<title>Robojax 28BYJ-48 Motor Control</title>\
  \t\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  \n<style>\
 \nhtml,body{\t\nwidth:100%\;\nheight:100%\;\nmargin:0}\n*{box-sizing:border-box}\n.colorAll{\n\tbackground-color:#90ee90}\n.colorBtn{\n\tbackground-color:#add8e6}\n.a{\n\tdisplay:inline-block}\n.btn{\n\tmargin:5px\;\nborder:none\;\ndisplay:inline-block\;\nvertical-align:middle\;\ntext-align:center\;\nwhite-space:nowrap}\n";
   
  HTML +="</style>\n\n</head>\n\n<body>\n<h1>Robojax 28BYJ-48 Stepper Motor Control </h1>\n";

   if(dirStatus ==2){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor Running in CW</span></h2>\n";    
   }else if(dirStatus ==1){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor Running in CCW</span></h2>\n";      
   }else{
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor OFF</span></h2>\n";    
   }
      if(dirStatus ==1){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[0];
        HTML += "=off\">";
        HTML +=buttonTitle1[0]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[0];
        HTML += "=on\">";       
        HTML +=buttonTitle2[0];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";  
       //Watch details at my Arduino Course at Udemy.com
      if(dirStatus ==2){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[1];
        HTML += "=off\">";
        HTML +=buttonTitle1[1]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[1];
        HTML += "=on\">";       
        HTML +=buttonTitle2[1];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";     

  HTML +="\t\n</body>\n</html>\n";
    
    page3="<center><p> motor 2 Status : Right</p></center>";
    server.send(200,"text/html",p+s+HTML+page+page2+page3);


digitalWrite(D1,HIGH);
    digitalWrite(D2,LOW);//fwd


delay(700);

digitalWrite(D1,LOW);
    digitalWrite(D2,LOW);


    
//    digitalWrite(D0,HIGH);
//    digitalWrite(D7,LOW);
    delay(200);
  }




  
   void Backward()
  {

    String s = MAIN_page; //Read HTML contents
 
String p = IN_page;
String HTML ="<!DOCTYPE html>\
  <html>\
  <head>\
  \t\n<title>Robojax 28BYJ-48 Motor Control</title>\
  \t\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  \n<style>\
 \nhtml,body{\t\nwidth:100%\;\nheight:100%\;\nmargin:0}\n*{box-sizing:border-box}\n.colorAll{\n\tbackground-color:#90ee90}\n.colorBtn{\n\tbackground-color:#add8e6}\n.a{\n\tdisplay:inline-block}\n.btn{\n\tmargin:5px\;\nborder:none\;\ndisplay:inline-block\;\nvertical-align:middle\;\ntext-align:center\;\nwhite-space:nowrap}\n";
   
  HTML +="</style>\n\n</head>\n\n<body>\n<h1>Robojax 28BYJ-48 Stepper Motor Control </h1>\n";

   if(dirStatus ==2){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor Running in CW</span></h2>\n";    
   }else if(dirStatus ==1){
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor Running in CCW</span></h2>\n";      
   }else{
    HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">Motor OFF</span></h2>\n";    
   }
      if(dirStatus ==1){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[0];
        HTML += "=off\">";
        HTML +=buttonTitle1[0]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[0];
        HTML += "=on\">";       
        HTML +=buttonTitle2[0];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";  
       //Watch details at my Arduino Course at Udemy.com
      if(dirStatus ==2){
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[1];
        HTML += "=off\">";
        HTML +=buttonTitle1[1]; //motor ON title
      }else{
        HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[1];
        HTML += "=on\">";       
        HTML +=buttonTitle2[1];//motor OFF title   
      }   
     HTML +="</a>\t\n\t</div>\n\n";     

  HTML +="\t\n</body>\n</html>\n";
    
    page2="<center><p> motor 1 Status : Backward</p></center>";
    server.send(200, "text/html", p+s+HTML+page+page2+page3);

digitalWrite(D6, HIGH);
    digitalWrite(D5,LOW);//back

digitalWrite(D7,HIGH);
    digitalWrite(D0,LOW);//left
    
//    digitalWrite(D6, HIGH);
//    digitalWrite(D5,LOW);
    delay(200); 
  }



  void setup(void) {

  //Robojax.com 28BYJ-48 Steper Motor Control

   myservo.attach(ServoPort); //--> attaches the servo on D1 to the servo objec
 pinMode(D1, OUTPUT);   // inputs for motor 1
  pinMode(D2,OUTPUT);   

// page = "<center><h1>Motor Control Web Server</h1><body><p><a href=\"Forward\"><button>Forward</button></a><p><a href=\"Backward\"><button>Backward</button></a></p><p><a href =\"Left\"><button>Left</button></a>&nbsp;<a href=\"Stop\"><button>Stop</button></a><a href=\"Right\"><button>Right</button></a></p></body></center>";
  pinMode(D5, OUTPUT);   // inputs for motor 1
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);    // inputs for motor 2 
  pinMode(D0,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);  // For status of WiFi connection
  delay(1000);


 
  Serial.begin(115200);//initialize the serial monitor
  Serial.println("");

  //Robojax.com 28BYJ-48 Steper Motor Control
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: http://");
  Serial.println(WiFi.localIP());

//multicast DNS   //Robojax.com 28BYJ-48 Steper Motor Control
  if (MDNS.begin("robojaxESP8266")) {
    Serial.println("MDNS responder started");
    Serial.println("access via http://robojaxESP8266");
  }

  server.on("/", handleRoot);
  server.on("/setPOS",handleServo); //--> Sets servo position from Web request
     server.on("/Forward",Forward);
  server.on("/Backward",Backward);
  server.on("/Left",Left);
  server.on("/Right",Right);
  server.on("/start",Start);

  server.on("/Stop",[](){                              // turns all the motor input pins low
   page2="<center><p> motor 1 Status : Off</p></center>";
   page3="<center><p> motor 2 Status : off</p></center>";
  server.send(200,"text/html",page+page2+page3);
//    digitalWrite(D1,LOW);
//    digitalWrite(D2,LOW);
//    
//    digitalWrite(D5,LOW);
//    digitalWrite(D6,LOW);
//    digitalWrite(D7,LOW);
//    digitalWrite(D0,LOW);
    delay(200);
  });
  
  server.on("/motor", HTTP_GET, motorControl); 
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  
  server.handleClient();
  MDNS.update();
  //Watch details at my Arduino Course at Udemy.com
if(dirStatus ==1){ 
   poleStep++; 
 }else if(dirStatus ==2){ 
   poleStep--; 
 }
 if(poleStep>7){ 
   poleStep=0; 
 } 
 if(poleStep<0){ 
   poleStep=7; 
 } 

   delay(1);
   //Robojax.com 28BYJ-48 Steper Motor Control  
}
