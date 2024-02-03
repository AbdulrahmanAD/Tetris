#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Blynk variables

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6S19pX3Dm"
#define BLYNK_TEMPLATE_NAME "TETRIS Template"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "dlink-M961-2.4G-b4cc";
char pass[] = "yeket24886";
char auth[] = "QFhKCE3-Fs0SSXvVfxjjnm9OI2HepiTa";

int blynkLeft = 0;
int blynkRight = 0;
int blynkDown = 0;
int blynkRotate = 0;

#define WIDTH 64
#define HEIGHT 128
Adafruit_SSD1306 Oled(128, 64, &Wire, -1);

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0


//the first index indicates the number of rotations each shape has, the second index is 2 because of x and y (coordinates of the shape), the third index is three because all shapes take 4 blocks
//square shape
const char shapes_Sq[1][2][4] = {{
                                    {0, 1, 0, 1}, {0, 0, 1, 1}
                                  }};

//I shape
const char shapes_l[2][2][4] = {{
                                    {0, 1, 2, 3}, {0, 0, 0, 0}
                                  },
                                  {
                                    {0, 0, 0, 0}, {0, 1, 2, 3}
                                  }};  

//Z shape 
const char shape_Z[2][2][4] = {{
                                      {0, 0, 1, 1}, {0, 1, 1, 2}
                                    },
                                    {
                                      {0, 1, 1, 2}, {1, 1, 0, 0}
                                    }};

//S shape, (the mirror of Z shape)
const char shape_S[2][2][4]{{
                                    {1, 1, 0, 0}, {0, 1, 1, 2}
                                  },
                                  {
                                    {0, 1, 1, 2}, {0, 0, 1, 1}
                                  }};

//L shape
const char shapes_L_l[4][2][4] = {{
                                    {0, 0, 0, 1}, {0, 1, 2, 2}
                                  },
                                  {
                                    {0, 1, 2, 2}, {1, 1, 1, 0}
                                  },
                                  {
                                    {0, 1, 1, 1}, {0, 0, 1, 2}
                                  },
                                  {
                                    {0, 0, 1, 2}, {1, 0, 0, 0}
                                  }};

//T shape
const char shapes_T[4][2][4] = {{
                                    {0, 0, 1, 0},{0, 1, 1, 2}
                                  },
                                  {
                                    {0, 1, 1, 2},{1, 0, 1, 1}
                                  },
                                  {
                                    {1, 0, 1, 1},{0, 1, 1, 2}
                                  },
                                  {
                                    {0, 1, 1, 2},{0, 0, 1, 0}
                                  }};


const short TOP_MARG = 19; 
const short LEFT_MARG = 3; 
const short SIZE = 5; 
const short TYPES = 6;   
const int MELODY_LENGTH = 10; 
const int MELODY_NOTES[MELODY_LENGTH] = {262, 294, 330, 262};
const int MELODY_DURATIONS[MELODY_LENGTH] = {500, 500, 500, 500};
int click = 1047; 
int click_duration = 100;
int erase = 2093;
int erase_duration = 100;
word currentType, nextType, rotation;
short shapeX, shapeY;
short shape[2][4];
int interval = 20, score;
long timer, delayer; 
boolean grid[10][18];
boolean b1, b2, b3;

String difficulty = "none";

int scoreEasy = 0;
int scoreHard = 0;

int tempo=144; 

int melody[] = {
  
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8

};

int SPEAKER_PIN = 13;
const int inX = 35; // analog input for x-axis
const int inY = 34; // analog input for y-axis
int xValue = 0; // variable to store x value
int yValue = 0; // variable to store y value
const int inPressed = 32; // input for detecting whether the joystick/button is pressed
int notPressed = 0; // variable to store the button's state => 1 if not pressed

int RED = 2;
int GREEN = 0;
int BLUE = 4;

void displayFrame(){
  //Oled.drawLine(starting x, starting y, ending x, ending y, color:WHITE);
  Oled.drawLine(0, 15, WIDTH, 15, WHITE);
  Oled.drawRect(1, 0, WIDTH - 1, HEIGHT, WHITE);
  drawNextShape();
  char text[6];
  //this is like the function to_string:
  itoa(score, text, 10);
  //drawText(text, numlength(score), x, y);
  drawText(text, numlength(score), 7, 4);
}
void displayBlocks(){
  for(short x = 0; x < 10; x++)
    for(short y = 0; y < 18; y++)
      if(grid[x][y])
        Oled.fillRect(LEFT_MARG + (SIZE + 1)*x, TOP_MARG + (SIZE + 1)*y, SIZE, SIZE, WHITE);
}
void drawShape(short x, short y){
  for(short i = 0; i < 4; i++)
    Oled.fillRect(LEFT_MARG + (SIZE + 1)*(x + shape[0][i]), TOP_MARG + (SIZE + 1)*(y + shape[1][i]), SIZE, SIZE, WHITE);
}
void drawNextShape(){
  short nShape[2][4];
  copyShape(nShape, nextType, 0);
  for(short i = 0; i < 4; i++)
    Oled.fillRect(50 + 3*nShape[0][i], 4 + 3*nShape[1][i], 2, 2, WHITE);
}
void refresh(){
  Oled.clearDisplay();
  displayFrame();
  displayBlocks();
  drawShape(shapeX, shapeY);
  Oled.display();
}
void turnOnLEDs(){
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 0);
  delay(100);
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);
  delay(100);
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 0);
  delay(100);
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);
  delay(100);
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 0);
  delay(100);   
}
void scanAndClearLines(){
  boolean full;
  for(short y = 17; y >= 0; y--){
    full = true;
    for(short x = 0; x < 10; x++){
      full = full && grid[x][y];
    }
    if(full){
      turnOnLEDs();
      clearLine(y);
      //y++ to check for: if multiple lines are full
      y++;
    }
  }
}
void clearLine(short lineIndex){
  tone(SPEAKER_PIN, erase, 1000 / erase_duration); 
  delay(100);
  noTone(SPEAKER_PIN);
  //this shifts the line to be removed and all the lines above it will be shifted down
  for(short y = lineIndex; y >= 0; y--){
    for(short x = 0; x < 10; x++){
      grid[x][y] = grid[x][y-1];
    }
  }
  //the for loop below is effictive only for the top row, to make sure that it is empty when all the rows below it are shifted
  for(short x = 0; x < 10; x++){
    grid[x][0] = 0;
  }
  //invert Oled is an oled function just for visual effect
  Oled.invertDisplay(true);
  delay(50);
  Oled.invertDisplay(false);
  score += 10;
  Blynk.virtualWrite(V4, score);
}
boolean horizontalPathIsBlocked(short shape[2][4], int amount){
  for(short i = 0; i < 4; i++){
    short newX = shapeX + shape[0][i] + amount;
    short newY = shapeY + shape[1][i];
    if(newX > 9 || newX < 0 || grid[newX][newY])
      return true;
  }
  return false;
}
boolean pathIsBlocked(){
  for(short i = 0; i < 4; i++){
    short y = shapeY + shape[1][i] + 1;
    short x = shapeX + shape[0][i];
    if(y > 17 || grid[x][y])
      return true;
  }
  return false;
}
void generate(){
  currentType = nextType;
  nextType = random(TYPES);
  if(currentType != 5)
    shapeX = random(9);
  else
    shapeX = random(7);
  shapeY = 0;
  rotation = 0;
  copyShape(shape, currentType, rotation);
}
void copyShape(short shape[2][4], short type, short rotation){
  switch(type){
  case 0: //L_l
    for(short i = 0; i < 4; i++){
      shape[0][i] = shapes_L_l[rotation][0][i];
      shape[1][i] = shapes_L_l[rotation][1][i];
    }
    break;
  case 1: //S_l
    for(short i = 0; i < 4; i++){
      shape[0][i] = shape_Z[rotation][0][i];
      shape[1][i] = shape_Z[rotation][1][i];
    }
    break;
  case 2: //S_r
    for(short i = 0; i < 4; i++){
      shape[0][i] = shape_S[rotation][0][i];
      shape[1][i] = shape_S[rotation][1][i];
    }
    break;
  case 3: //Sq
    for(short i = 0; i < 4; i++){
      shape[0][i] = shapes_Sq[0][0][i];
      shape[1][i] = shapes_Sq[0][1][i];
    }
    break;
    case 4: //T
    for(short i = 0; i < 4; i++){
      shape[0][i] = shapes_T[rotation][0][i];
      shape[1][i] = shapes_T[rotation][1][i];
    }
    break;
    case 5: //l
    for(short i = 0; i < 4; i++){
      shape[0][i] = shapes_l[rotation][0][i];
      shape[1][i] = shapes_l[rotation][1][i];
    }
    break;
  }
}
short getMaxRotation(short type){
  if(type == 1 || type == 2 || type == 5)
    return 2;
  else if(type == 0 || type == 4)
    return 4;
  else if(type == 3)
    return 1;
  else
    return 0;
}
boolean canRotate(short rotation){
  //if the shape can be rotated and don't collide with the borders ex: L
  short shape[2][4];
  copyShape(shape, currentType, rotation);
  return !horizontalPathIsBlocked(shape, 0);
}
short numlength(int num){
  short len = 1;
  while(num >= 10){
    num /= 10;
    len++;
  }
  return len;
}
void drawText(char text[], short length, int x, int y){
  Oled.setTextSize(1);
  Oled.setTextColor(WHITE);
  Oled.setCursor(x, y);    
  Oled.cp437(true);     
  for(short i = 0; i < length; i++)
    Oled.write(text[i]);
}
boolean isGameOver() {
  for(int x = 0; x < 10; x++) {
      if(grid[x][0]) return true;
  }
  return false;
}
void resetGame() {
  analogWrite(RED, 0);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);
  delay(200);
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);
  delay(200);
  analogWrite(RED, 0);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);
  delay(200);
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);
  delay(200);
  analogWrite(RED, 0);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);
  delay(200);
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);
  delay(200);

  tone(SPEAKER_PIN, 300, 500);
  delay(500);  
  noTone(SPEAKER_PIN);
  tone(SPEAKER_PIN, 300, 500);
  delay(250);  
  noTone(SPEAKER_PIN);
  delay(1000);

  for(int x = 0; x < 10; x++) {
      for(int y = 0; y < 18; y++) {
          grid[x][y] = false;
      }
  }
  score = 0;
  Blynk.virtualWrite(V4, score);
  interval = 400;
  nextType = random(TYPES);
  generate();
}


void playTetrisThemeSong() {
  int notes=sizeof(melody)/sizeof(melody[0])/2; 

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(SPEAKER_PIN, melody[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    
    // stop the waveform generation before the next note.
    noTone(SPEAKER_PIN);
  }

}

void displayTetrisTitle() {
  Oled.clearDisplay();
  Oled.setTextSize(1);
  Oled.setTextColor(WHITE);
  Oled.setCursor(15, 30);
  Oled.print("TETRIS");

  Oled.setCursor(30, 64);
  Oled.print("by:");

  Oled.setCursor(30, 74);
  Oled.print("Abd");

  Oled.setCursor(30, 84);
  Oled.print("Sami");

  Oled.setCursor(30, 94);
  Oled.print("Motaz");
  Oled.display();
}

void showTetrisTitle() {
  displayTetrisTitle();
  playTetrisThemeSong();
  Oled.clearDisplay();
  Oled.display();
}


void displayDifficulties1() {

  char text[6];
  itoa(scoreEasy, text, 10);

   Oled.clearDisplay();
  Oled.setTextColor(WHITE);

  drawText(text, numlength(scoreEasy), 15, 20);

  Oled.setTextSize(2);
  Oled.setCursor(12, 32);
  Oled.print("EASY");

  Oled.setTextSize(1);
  Oled.setCursor(20, 48);
  //Oled.print("--->");

  Oled.setCursor(20, 72);
  //Oled.print("<---");

  Oled.setTextSize(2);
  Oled.setCursor(12, 88);
  Oled.print("HARD");

  itoa(scoreHard, text, 10);

  drawText(text, numlength(scoreHard), 15, 105);

  //Oled.drawLine(0, 64, WIDTH, 64, WHITE);
  Oled.drawRect(1, 0, WIDTH - 1, 64, WHITE);

  Oled.display();

}

void displayDifficulties2() {

  char text[6];
  itoa(scoreEasy, text, 10);

   Oled.clearDisplay();
  Oled.setTextColor(WHITE);

  drawText(text, numlength(scoreEasy), 15, 20);

  Oled.setTextSize(2);
  Oled.setCursor(12, 32);
  Oled.print("EASY");

  Oled.setTextSize(1);
  Oled.setCursor(20, 48);
  //Oled.print("--->");

  Oled.setCursor(20, 72);
  //Oled.print("<---");

  Oled.setTextSize(2);
  Oled.setCursor(12, 88);
  Oled.print("HARD");

  itoa(scoreHard, text, 10);

  drawText(text, numlength(scoreHard), 15, 105);

  //Oled.drawLine(0, 64, WIDTH, 64, WHITE);
  Oled.drawRect(1, 64, WIDTH - 1, HEIGHT - 2, WHITE);

  Oled.display();

}

void selectDifficulty() {

  displayDifficulties1();

  int choice = 0;
  
  while(true) {

    Blynk.run();
    xValue = analogRead(inX);
    notPressed = digitalRead(inPressed);

    if(xValue < 500|| blynkDown) {
      choice = 1;
      displayDifficulties1();
      tone(SPEAKER_PIN, click, 1000 / click_duration);
      delay(100);
      noTone(SPEAKER_PIN);
    }
    if(xValue > 3500 || blynkRight) {
      choice = 2;
      displayDifficulties2();
      tone(SPEAKER_PIN, click, 1000 / click_duration);
      delay(100);
      noTone(SPEAKER_PIN);
    }
      
    if(!notPressed)
      break;

  }

  if(choice == 1) {
    difficulty = "easy";
    interval = 400;
  }
    
  if(choice == 2) {
    difficulty = "hard";
    interval = 120;
  }
    

}

void setup() {
  Serial.begin(9600);
  pinMode(inX, INPUT); // setup x input
  pinMode(inY, INPUT); // setup y input
  pinMode(inPressed, INPUT_PULLUP);
  // pinMode(RED, OUTPUT);
  // pinMode(GREEN, OUTPUT);
  // pinMode(BLUE, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  
  Serial.println("Here 1");  // SSD1306_SWITCHCAPVCC = generate Oled voltage from 3.3V internally
  if(!Oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  Oled.setRotation(1);
  Oled.clearDisplay();

  Blynk.begin(auth, ssid, pass);

  showTetrisTitle();
  displayFrame();
  Oled.display();
  randomSeed(analogRead(A3));
  nextType = random(TYPES);
  generate();
  timer = millis();    
  
}

void loop() {
  //Serial.printf("Here 4");
  analogWrite(RED, 255);
  analogWrite(BLUE, 255);
  analogWrite(GREEN, 255);

  if(isGameOver()) {
    

    if(difficulty == "hard" && score > scoreHard)
      scoreHard = score;
    if(difficulty == "easy" && score > scoreEasy)
      scoreEasy = score;

    resetGame();
    difficulty = "none";
  }

  if(difficulty == "none") {
    selectDifficulty();
    resetGame();

  }

  
  Blynk.run();
  xValue = analogRead(inX); // reading x value [range 0 -> 1023]
  yValue = analogRead(inY); // reading y value [range 0 -> 1023]
  notPressed = digitalRead(inPressed); // reading button state: 1 = not pressed, 0 = pressed

  if(millis() - timer > interval){
    scanAndClearLines();
    refresh();
    if(pathIsBlocked()){
      for(short i = 0; i < 4; i++)
        grid[shapeX + shape[0][i]][shapeY + shape[1][i]] = 1;
      generate();
    }else
      shapeY++;
    timer = millis();
  }
  if(yValue > 3500 || blynkLeft){
    tone(SPEAKER_PIN, click, 1000 / click_duration);
    delay(100);
    noTone(SPEAKER_PIN);
    //the if statement below is to make the shape go to the left only one time
    if(b1){
      if(!horizontalPathIsBlocked(shape, -1)){
        shapeX--;
        refresh();
      }
      b1 = false;
    }
  }else{
    b1 = true;
  }
  if(yValue < 500 || blynkRight){
    tone(SPEAKER_PIN, click, 1000 / click_duration);
    delay(100);
    noTone(SPEAKER_PIN);
    if(b2){
      if(!horizontalPathIsBlocked(shape, 1)){
        shapeX++;
        refresh();
      }
      b2 = false;
    }
  }else{
    b2 = true;
  }
  if(xValue > 3500 || blynkDown) {
    interval = 20;
  } else{
    if(difficulty == "easy")
      interval = 400;
    else if(difficulty == "hard")
      interval = 120;
  }
  if(!notPressed || blynkRotate){
    tone(SPEAKER_PIN, click, 1000 / click_duration);
    delay(100);
    noTone(SPEAKER_PIN);
    if(b3){
      if(rotation == getMaxRotation(currentType) - 1 && canRotate(0)){
        rotation = 0;
      }else if(canRotate(rotation + 1)){
        rotation++;
      }  
      copyShape(shape, currentType, rotation);
      refresh();
      b3 = false;
      delayer = millis();
    }
  }else if(millis() - delayer > 50){
    b3 = true;
  }

  
}



BLYNK_WRITE(V0) {
  blynkLeft = param.asInt();
}
BLYNK_WRITE(V1) {
  blynkRight = param.asInt();
}
BLYNK_WRITE(V2) {
  blynkRotate = param.asInt();
}
BLYNK_WRITE(V3) {
  blynkDown = param.asInt();
}









