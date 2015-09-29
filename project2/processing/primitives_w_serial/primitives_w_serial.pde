/**
 * Primitives 3D. 
 * 
 * Placing mathematically 3D objects in synthetic space.
 * The lights() method reveals their imagined dimension.
 * The box() and sphere() functions each have one parameter
 * which is used to specify their size. These shapes are
 * positioned using the translate() function.
 */

import processing.serial.*;
Serial myPort;  // Create object from Serial class



float xaxis;
float yaxis;
float zaxis;
float rotangleYaw;
float rotanglePitch;
float moveangle;

float input_yaw = 0.0;
float input_pitch = 0.0;
//float input_yaw = 0.0; // not yet used
//float input_pitch = 0.0; // not yet used

float moveK_max = 10;
float rotateK_max = .01;


boolean[] k = new boolean[200];

float[] body_pose = new float[3];


int iterator = 0;
float cam_location_X = 0.0;
float cam_offset_Z = 0.0;
float cam_offset_theta = 0.0;

float screen_margin = 0;

boolean switch_scene = true;
boolean center_scene = true;


int num_boxes = 500;//500;

float spreadX = 1000;
float spreadY = 300;
float spreadZ = 1000;
float spread_size = 100;

float[] randomsX = new float[num_boxes];
float[] randomsY = new float[num_boxes];
float[] randomsZ = new float[num_boxes];
float[] randoms_size = new float[num_boxes];
float[] coins = new float[num_boxes];

int prev_color_mode = 0;
int color_mode = 0;
color bg = color(255, 255, 255);
color c = color(0, 0, 0);    
    

void setup() {
  //fullScreen();
  size(1200, 800, P3D);
  surface.setResizable(true);

  myPort = new Serial(this, "/dev/cu.LightBlue-Bean", 9600);

  resetCamera();
}


void draw () {
  //background(150);
  background(bg);
  lights();
  
  
  //Serial test
  if(myPort.available() > 0) {
    char inByte = myPort.readChar();
    if(inByte == 'f') {
      // we expect data with this format f:XXXX
      myPort.readChar(); // discard ':'
      byte [] pitchData = new byte[4];
      byte [] rollData = new byte[4];
      myPort.readBytes(pitchData);
      //myPort.readBytes(rollData);
      
      int intbit = 0;
      intbit = (pitchData[3] << 24) | ((pitchData[2] & 0xff) << 16) | ((pitchData[1] & 0xff) << 8) | (pitchData[0] & 0xff);
      float pitch = Float.intBitsToFloat(intbit);
      
      //intbit = 0;
      //intbit = (rollData[3] << 24) | ((rollData[2] & 0xff) << 16) | ((rollData[1] & 0xff) << 8) | (rollData[0] & 0xff);
      //float roll = Float.intBitsToFloat(intbit);
      
      println("Pitch: ", pitch);
      //println("Pitch: ", pitch, "Roll: ", roll);
    }
  }
  
  
  ///////


  //mouse mapping
  float mouse_mappings[] = mouseMapping(); //  { forward velocity, rotational velocity } 0-255

  input_yaw = map(mouse_mappings[0], 0, 255, -rotateK_max, rotateK_max);
  input_pitch = map(mouse_mappings[1], 0 , 255, -rotateK_max, rotateK_max);
  //input_X = map(someInput1, 0 , 255, 0, moveK_max);
  //input_Z = map(someInput2, 0 , 255, 0, moveK_max);

  //println("X", input_yaw, "Y", input_pitch);



  if ( center_scene == true ) {
    resetCamera();
    center_scene = false;
  }


  fill(255);
  sphere(30);


  body_pose = updateCamera(moveK_max, moveK_max, input_pitch,  input_yaw );
  //body_pose = updateCamera(input_X, input_Z, input_pitch,  input_yaw );
  randomBoxes(body_pose);




  /*
noFill();
   stroke(255);
   pushMatrix();
   translate(500, height*0.35, -200);
   sphere(280);
   popMatrix();
   */
}

void mouseClicked() {

  int count = mouseEvent.getClickCount();

  if ( count == 1 ) switch_scene = true;
  if ( count == 2 ) center_scene = true;

  //println(count);  // count is 2 for double click
}



void keyPressed() {
  if (key == ESC) {
    key = 0;
    exit();
  }
  k[keyCode] = true;
}

void keyReleased() {
  k[keyCode] = false;
}


float[] updateCamera(float moveKx, float moveKz, float rotateKpitch, float rotateKyaw) {

  // Thanks http://www.saic.edu/~rauyeu/Processing/SketchFarm/Farm.html for the base camera code.

  if (k[87]) {
    // w
    zaxis += moveKz*cos(-moveangle);
    xaxis -= moveKx*sin(-moveangle);
  }    
  if (k[83]) {
    //println("83");
    // s
    zaxis -= moveKz*cos(-moveangle);
    xaxis += moveKx*sin(-moveangle);
  }    
  if (k[65]) {
    //println("65");
    // a
    xaxis += moveKx*(cos(moveangle));
    zaxis -= moveKz*(sin(moveangle));
  }    
  if (k[68]) {
    //println("68");
    // d
    xaxis -= moveKx*(cos(moveangle));
    zaxis += moveKz*(sin(moveangle));
  }    
  //if (k[38]) {
  //  // up arrow
  //  //yaxis += moveK;
  //  rotanglePitch -= rotateKpitch*PI;
  //}    
  //if (k[40]) {
  //  // down arrow
  //  //yaxis -= moveK;
  //  rotanglePitch += rotateKpitch*PI;
  //}    
  //if (k[37]) {
  //  println("37");
  //  // left arrowproc
  //  rotangleYaw -= rotateKyaw*PI;
  //  moveangle += rotateKyaw*PI;
  //}    
  //if (k[39]) {
  //  println("39");
  //  // right arrow
  //  rotangleYaw += rotateKyaw*PI;
  //  moveangle -= rotateKyaw*PI;
  //}
  
  rotanglePitch -= rotateKpitch*PI;
  rotangleYaw += rotateKyaw*PI;
  moveangle -= rotateKyaw*PI;


  //println(rotangleYaw, rotanglePitch);
  //println(rotanglePitch % PI);
  
  float angle_limit_margin = 0.05;
  if ( rotanglePitch >= (PI/2-angle_limit_margin) )  rotanglePitch = (PI/2 - angle_limit_margin);  
if ( rotanglePitch <= (-PI/2+angle_limit_margin) )  rotanglePitch = (-PI/2 + angle_limit_margin);  
//yaxis = 20;

//if ((abs(rotanglePitch)%(2*PI)) < (PI/2)) {
  camera(  xaxis, yaxis, zaxis, 
          xaxis+cos(rotangleYaw), yaxis+tan(rotanglePitch), zaxis+sin(rotangleYaw), 
          0, 1, 0);
//}
//else{
//   camera(  xaxis, yaxis, zaxis, 
//         xaxis+cos(rotangleYaw), yaxis-tan(-rotanglePitch), zaxis+sin(rotangleYaw), 
//         0, 1, 0);
//}

  float[] body_pose = {xaxis, yaxis, zaxis};
  return body_pose;
  
  

  
}

float[] mouseMapping() {
  //mouse mapping
  float input_min = 0;
  float input_max = 255;
  float mouse_mappings[] = {0.0, 0.0};

  float mouseX_mapped = map(mouseX, 0, width, input_min, input_max);
  float mouseY_mapped = map(mouseY, height, 0, input_min, input_max); // inverted Y

  if ( (mouseY_mapped < input_min+screen_margin) || (mouseY_mapped > input_max-screen_margin) || (mouseX_mapped < input_min+screen_margin) || (mouseX_mapped > input_max-screen_margin) ) {
    mouseX_mapped = 0;
    mouseY_mapped = 0;
  }

  //float forward_velocity = forward_velocity_scale*mouseY_mapped;
  //float rotational_velocity = rotational_velocity_scale*mouseX_mapped;

  mouse_mappings[0] = mouseX_mapped;
  mouse_mappings[1] = mouseY_mapped;

  return mouse_mappings;
}



int randomBoxes(float[] body_pose) {


  if (switch_scene != false) {
    for (int i = 0; i < num_boxes; i++) {
      //randomsX[i] = random(-spreadX, spreadX);
      //randomsY[i] = random(-spreadY, spreadY);
      //randomsZ[i] = random(-spreadZ, spreadZ);
      //randoms_size[i] = random(0, spread_size);
      
      randomsX[i] = randomGaussian()*1000;
      randomsY[i] = randomGaussian()*300;
      randomsZ[i] = randomGaussian()*1000;
      
      randoms_size[i] = randomGaussian()*spread_size;
      
      //coins[i] = random(0, 1);
    }
    
    prev_color_mode = color_mode;
    while (color_mode == prev_color_mode ) {
      color_mode = int(random(5)); // choose new color mode, cannot be the same as the previous color mode
    }
    println("color mode:",color_mode);
    switch_scene = false;
  }



  for (int i = 0; i < num_boxes; i++) {

    
    
    
    pushMatrix();
    float boxX = randomsX[i];//width/2+randomsX[i];
    float boxY = randomsY[i];//height/2+randomsY[i];
    float boxZ = randomsZ[i];
    translate(boxX, boxY, boxZ);

    float distance = dist(boxX, boxY, boxZ, body_pose[0], body_pose[1], body_pose[2]);
    
    
    applyNewColors(distance);

    

    fill(c);



    //// Adds bxes and spheres.. too slow
    //if (coins[i] > 0.5) box(randoms_size[i]);
    //else sphere(randoms_size[i]);

    box(randoms_size[i]);
    popMatrix();
  }

  return 1;
}

void resetCamera() {
  xaxis = 0;
  yaxis = 0;
  zaxis = 0;
  rotangleYaw = PI/2;
  rotanglePitch = 0;
}

void applyNewColors(float distance) {
 
  float R,G,B;

  switch(color_mode) {
   
    case 0:
      // 49 25 255
      // 26 255 0
      // 204 93 20
      bg = color(49, 25, 255);
      R = map(distance, 0, 2000, 26, 204);
      G = map(distance, 0, 2000, 255, 93);
      B = map(distance, 0, 2000, 0, 20);
      break;
    case 1:
      //255 29 232
      //4 255 231
      //204 169 23    
      bg = color(255, 29, 232);
      R = map(distance, 0, 2000, 204, 4);
      G = map(distance, 0, 2000, 169, 255);
      B = map(distance, 0, 2000, 23, 231);
      break;
    case 2:
      //255 104 76
      //47 115 255
      //142 204 17   
      bg = color(255, 104, 76);
      R = map(distance, 0, 2000, 47, 142);
      G = map(distance, 0, 2000, 115, 204);
      B = map(distance, 0, 2000, 255, 17);
      break;     
    case 3:
      //219 255 249
      //255 209 49
      //202 17 204
      bg = color(219, 255, 249);
      R = map(distance, 0, 2000, 255, 202);
      G = map(distance, 0, 2000, 209, 17);
      B = map(distance, 0, 2000, 49, 204);
      break;  
    case 4:
      bg = color(0, 0, 0);
      R = map(distance, 0, 2000, 255, 0);
      G = map(distance, 0, 2000, 255, 255);
      B = map(distance, 0, 2000, 255, 0 );
      break;        
    default:
      bg = color(255, 255, 255);
      R = 100;
      G = 100;
      B = 100;
      break;
  }
  
      
    c = color(R, G, B, map(distance, 0, 500, 0, 255));
    
    

  

  
  
  //219 255 249
  //255 209 49
  //202 17 204
  
  
  
  
  
}