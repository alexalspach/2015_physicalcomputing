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

boolean PITCH_CONTROL = true;

// For camera motion
float xaxis;
float yaxis;
float zaxis;
float rotangleYaw;
float rotanglePitch;
float moveangle;

// Hold input values after mapping
float input_yaw = 0.0;
float input_pitch = 0.0;
float input_fwd = 0.0;

// Gains for movement speed / magnitude
float moveK_max = 10.0;
float rotateK_max = 0.02;

// For keyboard commands
boolean[] k = new boolean[200];

// Current position and pose (of camera)
float[] body_pose = new float[3];
float cam_location_X = 0.0;
float cam_offset_Z = 0.0;
float cam_offset_theta = 0.0;

// Used to zero motion when mouse leaves screen
float screen_margin = 0;

// Uses to trigger new colors and boxes, and to center view in world
boolean switch_scene = true;
boolean center_scene = true;

// Values controlling number of boxes and size of box "universe"
int num_boxes = 500;
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

int command[] = {0,0,0,0};

//pitch low pass filter
int pitch_windowSize = 3;
float[] pitch_data = new float[pitch_windowSize];
float pitch_total = 0.0;
int pitch_dataIndex = 0;
float pitch_filtered = 0.0;


void setup() {
  //fullScreen();
  size(1200, 800, P3D);
  surface.setResizable(true);
  myPort = new Serial(this, "/dev/cu.LightBlue-Bean", 9600);
  resetCamera();
  
  for (int thisReading = 0; thisReading < pitch_windowSize; thisReading++)
    pitch_data[thisReading] = 0;
    
}


void draw () {
  //background(150);
  background(bg);
  lights();

  int cmd_fwd, cmd_yaw, cmd_pitch, cmd_clk;
  command = getSerial(4);
  cmd_fwd = command[0];
  // Only update if there is data available
  if ( cmd_fwd > -1 ) {
    
    cmd_yaw = command[1];
    cmd_pitch = command[2];
    cmd_clk = command[3];
    
    pitch_filtered = filterPitch(cmd_pitch);
    cmd_pitch = int(pitch_filtered);
    
    //println(cmd_yaw);
    println(cmd_pitch);
  
    //mouse mapping
    //float mouse_mappings[] = mouseMapping(); //  { forward velocity, rotational velocity } 0-255
    //input_yaw = map(mouse_mappings[0], 0, 255, -rotateK_max, rotateK_max);
    //input_pitch = map(mouse_mappings[1], 0, 255, -rotateK_max, rotateK_max);
    //input_X = map(someInput1, 0 , 255, 0, moveK_max);
    //input_Z = map(someInput2, 0 , 255, 0, moveK_max);
  
    input_fwd = map(cmd_fwd, 0, 254, -moveK_max, moveK_max);
    input_yaw = map(cmd_yaw, 0, 254, -rotateK_max, rotateK_max);
    if ( PITCH_CONTROL == true ) {
      input_pitch = map(cmd_pitch, 0, 254, -rotateK_max, rotateK_max);
    }
    else {
      input_pitch = 0.0;
    }
    
    if ( (center_scene == true) ) {
      resetCamera();
      center_scene = false;
    }
    
    if (cmd_clk == 1) switch_scene = true;
  
    }
  
    fill(255);
    sphere(30);
  
    body_pose = updateCamera(0, input_fwd, input_pitch, input_yaw );
    //body_pose = updateCamera(input_X, input_Z, input_pitch,  input_yaw );
    randomBoxes(body_pose);

}


float[] updateCamera(float moveKx, float moveKz, float rotateKpitch, float rotateKyaw) {

  // Thanks http://www.saic.edu/~rauyeu/Processing/SketchFarm/Farm.html for the base camera code.

  zaxis += moveKz*cos(-moveangle);
  xaxis -= moveKz*sin(-moveangle);
    
  rotanglePitch = -rotateKpitch*PI*15;
  rotangleYaw -= rotateKyaw*PI;
  moveangle += rotateKyaw*PI;

  //println(rotangleYaw, rotanglePitch);
  //println(rotanglePitch % PI);

  float angle_limit_margin = 0.05;
  if ( rotanglePitch >= (PI/2-angle_limit_margin) )  rotanglePitch = (PI/2 - angle_limit_margin);  
  if ( rotanglePitch <= (-PI/2+angle_limit_margin) )  rotanglePitch = (-PI/2 + angle_limit_margin);  

  camera(  xaxis, yaxis, zaxis, 
    xaxis+cos(rotangleYaw), yaxis+tan(rotanglePitch), zaxis+sin(rotangleYaw), 
    0, 1, 0);

  float[] body_pose = {xaxis, yaxis, zaxis};
  return body_pose;
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


int randomBoxes(float[] body_pose) {

  if (switch_scene != false) {
    for (int i = 0; i < num_boxes; i++) {

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
    println("color mode:", color_mode);
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

  float R, G, B;
  
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
}


int[] getSerial(int num_bytes) {

  int[] packet = new int[num_bytes];

  //Enough data available ?
  if (myPort.available() > num_bytes) {

    //delay(2);
    int inByte0 = myPort.read();

    // Start byte defined as 255 in Arduino and processing code (defined by me)
    if ( inByte0 == 255 ) { 

      for (int i = 0; i<4; i++) {
        inByte0 = myPort.read();
        packet[i] = inByte0;
      }
      
      //print( packet[0] );
      //print("\t");
      //print( packet[1] );
      //print("\t");
      //print( packet[2] );
      //print("\t");
      //print( packet[3] );
      //print("\n");
      
      return packet;
    }

  }

  int noData[] = {-1,-1,-1,-1};
  return noData;
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

  mouse_mappings[0] = mouseX_mapped;
  mouse_mappings[1] = mouseY_mapped;

  return mouse_mappings;
}


float filterPitch(float raw) {

  pitch_total = pitch_total - pitch_data[pitch_dataIndex];
  pitch_data[pitch_dataIndex] = raw;
  pitch_total = pitch_total + pitch_data[pitch_dataIndex];
  pitch_dataIndex = pitch_dataIndex + 1;

  if (pitch_dataIndex >= pitch_windowSize)
    pitch_dataIndex = 0;

  return ( pitch_total / pitch_windowSize );

}


/*
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

*/