import oscP5.*;
import processing.serial.*;
 
Serial serial;
OscSerial osc;
//String serialName = "the name of your serial port";
int serial_port = 4;
 
final int NUMPIXELS = 7; // Constant, cannot be modified 

boolean FIND_ARDUINO = false;
boolean DOWNLOAD = true;

// colors
float[] rA = new float[11];
float[] gA = new float[11];
float[] bA = new float[11];


// Image
PImage webImg;
PImage img;

String dataPath = dataPath("");
String imgPath = dataPath + "/mti_project4/" + "colorPic0.jpg";
//String imgPath = dataPath + "/" + "rainbow2.jpg";
int picture_rot_index = 0;

color c0;

float brightness = 0.1; // 1.0 is full, 0.0 is off

/* Nat Geo API */
// from https://www.npmjs.com/package/nat-geo-api
String natGeoPic_api = "https://natgeoapi.herokuapp.com/api/dailyphoto";
JSONObject json;
String podurl = "";


void setup() {
  
  size(990,742);
  //surface.setResizable(true);
  
  colorMode(RGB);
  
  if ( FIND_ARDUINO == true ) { 
    
   //StringList inventory;
    int numberinos = Serial.list().length;
    for (int i=0; i<numberinos; i++) {
      println(i, ":", Serial.list()[i]);
    }
    exit();
  }
  
  
  serial = new Serial(this, Serial.list()[serial_port], 9600);
  osc = new OscSerial(this, serial);
  //osc.plug(this, "myFunction", "/helloFromArduino");
  
  println(imgPath);
  
  if ( DOWNLOAD == true ) {
    //String url = "http://images.nationalgeographic.com/wpf/media-live/photos/000/921/cache/gullfoss-waterfall-iceland-ngpc2015_92122_990x742.jpg";
    podurl = "http:" + getNGPODurl();
    
    // Load image from a web server
    webImg = loadImage(podurl, "jpg");
    webImg.save(imgPath);
  }
  
  
  img = loadImage(imgPath);
  image(img, 0, 0, width, height);
  
  
}
 
void draw() {
  
  //image(img, 0, 0, width, height);

  c0 = get(mouseX, mouseY);
  
  for (int i=0; i<NUMPIXELS; i++) {
    rA[i] = (red(c0))*brightness;
    gA[i] = (green(c0))*brightness;
    bA[i] = (blue(c0))*brightness;
  }
  
  //rA[0] = (red(c0))*brightness;
  //gA[0] = (green(c0))*brightness;
  //bA[0] = (blue(c0))*brightness;
  
  //rA[1] = (red(c0))*brightness;
  //gA[1] = (green(c0))*brightness;
  //bA[1] = (blue(c0)*brightness);
  
  
  // Timer, much more reliable than millis
  if (frameCount % 15 == 0) {
    //println("time");
    thread("sendColors");
  }
   
  
  
}


void mouseClicked() {

  int num_pics = 6;
  picture_rot_index++;
  if (picture_rot_index > num_pics) picture_rot_index = 0;
  
  imgPath = dataPath + "/mti_project4/" + "colorPic" + str(picture_rot_index) + ".jpg";
  
  img = loadImage(imgPath);
  image(img, 0, 0, width, height);
  
  
}




void sendColors() {
  
 //println("moved!");
    
  OscMessage msg = new OscMessage("/colors");
  
  //println(red(c),blue(c),green(c));
  
  for (int i=0; i<NUMPIXELS; i++) {
    msg.add(int(rA[i]));
    msg.add(int(gA[i]));
    msg.add(int(bA[i]));
  }
   
  //msg.add(int(red(c)));
  //msg.add(int(green(c)));
  //msg.add(int(blue(c)));
  
  ////println(map(mouseX,0,width,0,255));
  //msg.add(0);
  //msg.add(0);
  //msg.add(0);
  
  //msg.add(0);
  //msg.add(0);
  //msg.add(255); 
  
  osc.send(msg);
   
}

 void averaging() {
   int x_start = mouseX-50;
  int x_end = mouseX+50;
  int y_start = mouseY-50;
  int y_end = mouseY+50;  
  image(img, 0, 0);
  loadPixels(); 
  // Since we are going to access the image's pixels too  
  img.loadPixels(); 
  for (int y = y_start; y < y_end; y++) {
    for (int x = x_start; x < x_end; x++) {
      int loc = x + y*width;
      
      // The functions red(), green(), and blue() pull out the 3 color components from a pixel.
      float r = red(img.pixels[loc]);
      float g = green(img.pixels[loc]);
      float b = blue(img.pixels[loc]);
      
      // Image Processing would go here
      // If we were to change the RGB values, we would do it here, 
      // before setting the pixel in the display window.
      
      // Set the display pixel to the image pixel
      //pixels[loc] =  color(255,255,255); //color(r,g,b);          
    }
  }
  updatePixels();
  image(img, 0, 0);
 }
 
 String getNGPODurl() {
  
  json = loadJSONObject(natGeoPic_api);
  println(json);
  //println(json.size());
  
  //JSONObject tempQuake = json.getJSONObject(0);
  String source = json.getString("src");
   
  println(source);
   
  return source;
  
}