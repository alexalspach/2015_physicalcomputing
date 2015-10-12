JSONObject json;

// From Kimono Labs generated API
String quakeURL = "https://www.kimonolabs.com/api/332mapa0?apikey=sYwz8YVXqZK2AtPZqJmxNmwsPkyxZzHP";


void setup() {
  size(640,360);
  loadData();
}

void draw() {
  
}


void loadData() {
  
  json = loadJSONObject(quakeURL);
  println(json);
  
  JSONObject results = json.getJSONObject("results");
  JSONArray jsonQuakes = results.getJSONArray("collection1");
  int numQuakes = jsonQuakes.size();
  
  float w = width/numQuakes;
  
  println("Number of quakes: " + numQuakes);
  
  for (int i=0; i<numQuakes; i++) {
   JSONObject tempQuake = jsonQuakes.getJSONObject(i);
   float magnitude = tempQuake.getFloat("mag_1");
   println("magnitude: " + magnitude);
   
   stroke(0);
   strokeWeight(25);
   line(w*i+25, height, w*i + 25, height - magnitude*30);
   
   textAlign(LEFT);
   float x = w*i + 30;
   float y = height - magnitude*30 - 15;
   pushMatrix();
   translate(x,y);
   rotate(-HALF_PI);
   text(magnitude,0,0);
   popMatrix();
   
  }
  
  
}