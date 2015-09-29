/**
 * Primitives 3D. 
 * 
 * Placing mathematically 3D objects in synthetic space.
 * The lights() method reveals their imagined dimension.
 * The box() and sphere() functions each have one parameter
 * which is used to specify their size. These shapes are
 * positioned using the translate() function.
 */
 
 
 int iterator = 0;
 float cam_location_X = 0.0;
 float cam_offset_Z = 0.0;
 float cam_offset_theta = 0.0;
 
 float screen_margin = 5;
 
 boolean switch_scene = true;
 boolean center_scene = true;
 
int num_boxes = 200;

float spreadX = 1000;
float spreadY = 300;
float spreadZ = 1000;
float spread_size = 100;

float[] randomsX = new float[num_boxes];
float[] randomsY = new float[num_boxes];
float[] randomsZ = new float[num_boxes];
float[] randoms_size = new float[num_boxes];

boolean flipped = false;
 
 
void setup() {
  //fullScreen();
  size(1200, 800, P3D);
  surface.setResizable(true);
  
  beginCamera();
camera();
//rotateX(PI/6);
endCamera();
  
}

 
void draw () {
//background(150);
background(255,255,0);
lights();

 
//mouse mapping
float input_abs_max = 100;
float mouseX_mapped = map(mouseX,0,width,-input_abs_max,input_abs_max);
float mouseY_mapped = map(mouseY,0,height,-input_abs_max,input_abs_max); // inverted Y
println("MX", mouseX_mapped, "MY", mouseY_mapped);

if ( (mouseY_mapped < -input_abs_max+screen_margin) || (mouseY_mapped > input_abs_max-screen_margin) || (mouseX_mapped < -input_abs_max+screen_margin) || (mouseX_mapped > input_abs_max-screen_margin) ) {
  mouseX_mapped = 0;
  mouseY_mapped = 0;
  println("clipped");
}



float forward_velocity_scale = 0.2;
float forward_velocity = forward_velocity_scale*mouseY_mapped;

float rotational_velocity_scale = 0.05;
float rotational_velocity = rotational_velocity_scale*mouseX_mapped;


cam_offset_Z += forward_velocity;
cam_offset_theta += rotational_velocity;


//println("vel z", forward_velocity);
//println("vel rot", rotational_velocity);

//camera(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
//camera(mouseX, height/2, (height/2) / tan(PI/6),       mouseX, height/2, 0,       0, 1, 0);
//camera(mouseX, mouseY, (height/2) / tan(PI/6),       mouseX, mouseY, 0,       0, 1, 0); // Pan XY


if ( center_scene == true ) {
  cam_offset_Z = 0;
  cam_offset_theta = 0;
  center_scene = false;
}

// Idea: add a button click jump to the eyeX (should look interesting if the X center doesnt change with it)
float eyeX = width/2 - 0; // sin(cam_offset_theta)*cam_offset_Z;
float eyeY = height/2;
float eyeZ = 0 + cam_offset_Z; // cos(cam_offset_theta)*cam_offset_Z;

float centerX = width/2 + sin(cam_offset_theta/cam_offset_Z);
float centerY = height/2;
float centerZ = cam_offset_Z - 100.0;

println("center x", centerX);

if ( centerX > 1000 ) centerX = 1000;
if ( centerX < -1000 ) centerX = -1000;

camera(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0); // Pan XY

//translate(centerX, centerY, centerZ);
//box(30);


//translate(50, 50, 0);
//rotateY(PI/3);
//box(90);


// camera pers.
//float cameraY = height/2.0;
//float fov = cam_location/float(width) * PI/2;
//float cameraZ = cameraY / tan(fov / 2.0);
//float aspect = float(width)/float(height);
//perspective(fov, aspect, cam_location, 10000);
//println(fov, aspect, forward_velocity, cameraZ*10.0);

////noStroke();
//pushMatrix();
//translate(width/2, height/2, 0);
////rotateY(1.25);
////rotateX(-0.4);
////fill(0);
////color(100);
//box(100);
//popMatrix();

if (switch_scene != false) {
  for (int i = 0; i < num_boxes; i++) {
    randomsX[i] = random(-spreadX,spreadX*2);
    randomsY[i] = random(-spreadY,spreadY);
    randomsZ[i] = random(-spreadZ,spreadZ*2);
    randoms_size[i] = random(0,spread_size);
  }
  switch_scene = false;
}


  for (int i = 0; i < num_boxes; i++) {
    
    pushMatrix();
    float boxX = width/2+randomsX[i];
    float boxY = height/2+randomsY[i];
    float boxZ = randomsZ[i];
    translate(boxX, boxY, boxZ);
    
    float distance = dist(boxX, boxY, boxZ, eyeX, eyeY, eyeZ);
    
    color c;
    //if (distance < 1000) {
    //  c = color(200, 204, 0);  // Define color 'c'
    //  fill(c);
    //}
    //else {
    //  c = color(200, 204, 200);  // Define color 'c'
    //  fill(c);
    //}
    
    float R = map(distance,0,2000, 255, 0);
    float G = 0;//map(distance,0,2000, 255, 0);
    float B = map(distance,0,1000, 0, 255);
    c = color(R, G, B, map(distance,0,500, 0, 255));
    fill(c);
    
    //// trying to highlight the axes
    //if ( abs(boxX)<300 ) {
    //  c = color(0, 255, 0);
    //  fill(c);
    //}
    //  if ( abs(boxZ)<300 ) {
    //  c = color(255, 255, 255);
    //  fill(c);
    //}
    
    //rotateY(1.25);
    //rotateX(-0.4);
    //fill(0);

    

    box(randoms_size[i]);
    popMatrix();
    
  }
  



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
  
  println(count);  // count is 2 for double click
}