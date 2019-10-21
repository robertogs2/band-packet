// RGB constants
int red = 0;
int yellow = 1;
int green = 2;

// Packet constants
int clk_delay = 0;
int leds_division = 4;
int division = 100/leds_division;

// Lines pins
int pin_clk0 = A4;
int pin_data0 = A5;
int pin_clk1 = A3;
int pin_data1 = A2;
int pin_clk2 = A1;
int pin_data2 = A0;

// RGB pins
int pin_rgb0g = 3;
int pin_rgb0r = 5;
int pin_rgb1g = 6;
int pin_rgb1r = 9;
int pin_rgb2g = 10;
int pin_rgb2r = 11;

// Sign pins
int pin_sign0 = 2;
int pin_sign1 = 4;
int pin_sign2 = 7;

// Buttons pins

int pin_button0 = 8;
int pin_button1 = 12;
int pin_button2 = 13;

String command = "";
int i = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pin_rgb0g, OUTPUT);
  analogWrite(pin_rgb0g, 255);
  pinMode(pin_rgb0r, OUTPUT);
  analogWrite(pin_rgb0r, 255);
  pinMode(pin_rgb1g, OUTPUT);
  analogWrite(pin_rgb1g, 255);
  pinMode(pin_rgb1r, OUTPUT);
  analogWrite(pin_rgb1r, 255);
  pinMode(pin_rgb2g, OUTPUT);
  analogWrite(pin_rgb2g, 255);
  pinMode(pin_rgb2r, OUTPUT);
  analogWrite(pin_rgb2r, 255);

  pinMode(pin_clk0, OUTPUT);
  digitalWrite(pin_clk0, 1);
  pinMode(pin_data0, OUTPUT);
  digitalWrite(pin_data0, 1);
  pinMode(pin_clk1, OUTPUT);
  digitalWrite(pin_clk1, 1);
  pinMode(pin_data1, OUTPUT);
  digitalWrite(pin_data1, 1);
  pinMode(pin_clk2, OUTPUT);
  digitalWrite(pin_clk2, 1);
  pinMode(pin_data2, OUTPUT);
  digitalWrite(pin_data2, 1);
  
  make_clock(pin_clk0, division);
  make_clock(pin_clk1, division);
  make_clock(pin_clk2, division);

  pinMode(pin_sign0, OUTPUT);
  digitalWrite(pin_sign0, 1);
  pinMode(pin_sign1, OUTPUT);
  digitalWrite(pin_sign1, 1);
  pinMode(pin_sign2, OUTPUT);
  digitalWrite(pin_sign2, 1);

  pinMode(pin_button0, INPUT);
  pinMode(pin_button1, INPUT);
  pinMode(pin_button2, INPUT);

}


void make_clock(int pin, int times){
   int i = 0;
   for(;i< times; ++i){
      digitalWrite(pin, 0);
      delay(clk_delay);
      digitalWrite(pin, 1);
      delay(clk_delay);
   }
}

void split(String input, char delimiter, String* results){
  int t = 0;
  int r = 0;
  int i = 0;
  for (i=0; i < input.length(); i++){ 
    if(input.charAt(i) == delimiter && input.charAt(i) != '\n') { 
      results[t] = input.substring(r, i);
      r=(i+1); 
      t++; 
    }
  }  
  results[t] = input.substring(r, i);
}

void rgb_band(int band, int color){
  int pin_rgb_g;
  int pin_rgb_r;
  if (band == 0){pin_rgb_g = pin_rgb0g;pin_rgb_r = pin_rgb0r;}
  else if(band == 1){pin_rgb_g = pin_rgb1g;pin_rgb_r = pin_rgb1r;}
  else if(band == 2){pin_rgb_g = pin_rgb2g;pin_rgb_r = pin_rgb2r;}

  if(color == red){
    analogWrite(pin_rgb_g, 255);  
    analogWrite(pin_rgb_r, 0);  
  }
  else if(color == yellow){
    analogWrite(pin_rgb_g, 0);  
    analogWrite(pin_rgb_r, 20);  
  }
  else if(color == green){
    analogWrite(pin_rgb_g, 0);  
    analogWrite(pin_rgb_r, 255);  
  }
  else{
    analogWrite(pin_rgb_g, 255);  
    analogWrite(pin_rgb_r, 255);  
  }
}

void packet_band(int band, int pos){
  int pin_clk;
  int pin_data;
  if (band == 0){pin_clk = pin_clk0;pin_data = pin_data0;}
  else if(band == 1){pin_clk = pin_clk1;pin_data = pin_data1;}
  else if(band == 2){pin_clk = pin_clk2;pin_data = pin_data2;}

  int times = division;
  digitalWrite(pin_data, 1);
  make_clock(pin_clk, times);
  times = pos/division;
  digitalWrite(pin_data, 0);
  make_clock(pin_clk, 1);
  digitalWrite(pin_data, 1);
  make_clock(pin_clk, times);
}

void sign_band(int band, int value){
  int sign_pin;
  if (band == 0){sign_pin = pin_sign0;}
  else if(band == 1){sign_pin = pin_sign1;}
  else if(band == 2){sign_pin = pin_sign2;}

  if(value == 1){
    digitalWrite(sign_pin, 0);
  }
  else{
    digitalWrite(sign_pin, 1);
  }
  
}

void reset(){
  analogWrite(pin_rgb0g, 255);
  analogWrite(pin_rgb0r, 255);
  analogWrite(pin_rgb1g, 255);
  analogWrite(pin_rgb1r, 255);
  analogWrite(pin_rgb2g, 255);
  analogWrite(pin_rgb2r, 255);

  digitalWrite(pin_clk0, 1);
  digitalWrite(pin_data0, 1);
  digitalWrite(pin_clk1, 1);
  digitalWrite(pin_data1, 1);
  digitalWrite(pin_clk2, 1);
  digitalWrite(pin_data2, 1);
  
  make_clock(pin_clk0, division);
  make_clock(pin_clk1, division);
  make_clock(pin_clk1, division);

  digitalWrite(pin_sign0, 1);
  digitalWrite(pin_sign1, 1);
  digitalWrite(pin_sign2, 1);  
}

void analize_command(String command){
  if(command.length() > 0){
    String split_string[10];
    //Serial.println(command);
    split(command, ':', split_string);
    if(split_string[0] == "rgb"){ // Color of the box
      int band = split_string[1].toInt();
      int color = split_string[2].toInt();
      rgb_band(band, color);
    }
    else if(split_string[0] == "packet"){
      int band = split_string[1].toInt();
      int pos = split_string[2].toInt();
      packet_band(band, pos);
    }
    else if(split_string[0] == "sign"){
      int band = split_string[1].toInt();
      int value = split_string[2].toInt();
      sign_band(band, value);
    }
    else if(split_string[0] == "reset"){
        reset();
    }
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
    delay(3);
    char c = Serial.read();
    if(c == '\n' || c == '\r') break;
    command += c;
  }
  if(command.length() > 0){
    analize_command(command);
    //Serial.println(command);
    Serial.println("pins-" + String(digitalRead(pin_button0)) + "-" + String(digitalRead(pin_button1)) + "-" + String(digitalRead(pin_button2)));
  }
  
  command = "";
  
}
