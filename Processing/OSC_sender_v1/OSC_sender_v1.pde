/**
 * OSC streamer test for LED node
 */
 
import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation;

color c;

void setup() {
  size(400,400);
  frameRate(15);
  oscP5 = new OscP5(this,12000);
  myRemoteLocation = new NetAddress("10.29.169.229",8888);
}

void draw() {
  background(0);
  fill(255,0,0);
  rect(0,0,100,100);
  fill(0,255,0);
  rect(100,0,100,100);
  fill(0,0,255);
  rect(200,0,100,100);
  fill(255);
  rect(300,0,100,100);
  
  sendColor();
}

void sendColor() {
  OscMessage myMessage = new OscMessage("/led1");
  
  for (int i = 0; i < 60; i++){
    c = get(mouseX+i, mouseY);
    myMessage.add(int(red(c)));
    myMessage.add(int(blue(c)));
    myMessage.add(int(green(c)));
    println(i, red(c), green(c), blue(c));
    line(mouseX, mouseY, mouseX+i, mouseY);
  }
  oscP5.send(myMessage, myRemoteLocation); 
  println("OSC message send");
}
