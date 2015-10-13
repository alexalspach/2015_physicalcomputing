boolean DOWNLOAD = true;

PImage webImg;
PImage img;

String dataPath = dataPath("");
String imgPath = "";

void setup() {
  
  size(990,742);
  surface.setResizable(true);
  
  if ( DOWNLOAD == true ) {
  String url = "http://images.nationalgeographic.com/wpf/media-live/photos/000/921/cache/gullfoss-waterfall-iceland-ngpc2015_92122_990x742.jpg";
  // Load image from a web server
  webImg = loadImage(url, "jpg");
  
  println(dataPath);
  imgPath = dataPath + "/" + "natGeoPic.jpg";
  webImg.save(imgPath);
    
    
  //webImg.width;
  
  
  img = loadImage(imgPath);
  image(img, 0, 0);
  
  println(img.width, img.height);


  }
  
  //PImage pimg = loadImg;
  //println(pimg.width);
  //println(pimg.height);
  //int one = webImg.width;
}

void draw() {
  background(255);
  image(img, 0, 0, width, height);
}