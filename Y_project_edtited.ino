// motor driver connections
#define in1 3  // input1 pin connect to 3 digital pin
#define in3 7  // input3 pin connect to 7 digital pin
#define in2 2  // input2 pin connect to 2 digital pin
#define in4 8  // input4 pin connect to 8 digital pin
#define en1 6  // enable1 pin connect to 6 pwm
#define en2 5  // enable pin connect to 5 pwm

// ir connections
#define IR_l 10  // Ir_l pin connect to 9 digital pin
#define IR_c 11 // Ir_c pin connect to 10 digital pin
#define IR_r 9 // Ir_r pin connect to 11  digital pin

//ultrasonic connections
#define Trig 4 // Trig connect to 4 digital pin
#define echo 12 // Echo connect to 12 digital pin

// led and buzzer connnections
#define Ledyellow_r A4 // 2 leds connected parallel turn on when turn right 
#define Ledyellow_l A3 // leds connnected parallel turn on when it turn left 
#define Ledred_l A2 // ledyellow_l pin connect to 13 turn on when stop
#define Ledred_r A1 // ledyellow_r pin connect to A1 turn on when it stop
#define Ledgreen A5 // ledgreen pin connect to A5 turns green when it is on
//#define buzzer A0  // buzzer pin connect to A0 to make sound when see object

// variables used
char command; // store command sent
unsigned long Time; // store time 
int L_ir;  // store the l ir value
int R_ir;  // store the r ir value
int C_ir;  // store the c ir value
int max_speed = 80;// max speed for motors
int base_speed = 100; // min speed for motors
int Min_speed = 80; //
long ldistance; // store the l distance
long rdistance; // store the right distance
long distance; // store the distance 
String state;  // store the state of the robot


// notes:
// out1 >> motor b down 
// out2 >> motor b up
// out3 >> motor a up
// out4 >> motor a down


/*-----------------------------------------------------------------------------------------------*/
// function to stop motor
/*here the function stop the car and make the leds red turn on*/
void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  digitalWrite(Ledred_r,HIGH);
  digitalWrite(Ledred_l,HIGH);
}
/*---------------------------------------------------------------------------------------------*/


// function to make motor move forward
/*here the function move the car forward and make the leds red turn off*/
void forward(int speed_max) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  analogWrite(en1,speed_max);
  analogWrite(en2,speed_max);

  digitalWrite(Ledred_r,LOW);
  digitalWrite(Ledred_l,LOW);
}

/*----------------------------------------------------------------------------------------------*/
// function to make motor move backward
/*here the function move the car backward and make the leds red turn off and the ledyellow on*/
void backward(int speed_max) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  analogWrite(en1,speed_max);
  analogWrite(en2,speed_max);

  digitalWrite(Ledyellow_l,HIGH);
  digitalWrite(Ledyellow_r,HIGH);
}
/*----------------------------------------------------------------------------------------------*/

// function to make motor move left
/*here the function move the car left and make the leds red turn off and the ledyellow_l on*/
void left(int speed_max,int speed_low) {
  digitalWrite(in1, HIGH);  // left motor forward
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);  // right motor forward
  digitalWrite(in4, LOW);
  analogWrite(en1,speed_low);   // left motor slow
  analogWrite(en2,speed_max);   // right motor fast

  digitalWrite(Ledyellow_l,HIGH);  // turn on left indicator
}
/*----------------------------------------------------------------------------------------------*/
// function to make motor move right
/*here the function move the car right and make the leds red turn off and the ledyellow_r on*/
void right(int speed_max,int speed_low) {
  digitalWrite(in1, HIGH);  // left motor forward
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);  // right motor forward
  digitalWrite(in4, LOW);
  analogWrite(en1,speed_max);   // left motor fast
  analogWrite(en2,speed_low);   // right motor slow

  digitalWrite(Ledyellow_r,HIGH); // turn on right indicator
}
/*----------------------------------------------------------------------------------------------*/


// function blutooth to use it when dont find a line  
void Blutooth(){
  if (Serial.available() > 0) {
    command = Serial.read();
    stopMotors();

    if (command == 'F') forward(max_speed);
    else if (command == 'B') backward(max_speed);
    else if (command == 'L') left(max_speed,base_speed);
    else if (command == 'R') right(max_speed,base_speed);
  }    
}

/*---------------------------------------------------------------------------------------------*/
// distance function
long Distance(){
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  
  Time = pulseIn(echo,HIGH);
  return distance = 0.0343*Time /2;
}
/*----------------------------------------------------------------------------------------------*/
// function for behaviour
/*here the function make a descion when the car reads all white and want it complete following the line*/
void behaviour_func(String state){
  if (state == "left"){
     left(max_speed,base_speed);
    }
  else if (state == "right"){
     right(max_speed,base_speed);
    }
  else if (state == "forward"){
    forward(max_speed);
    }
   else {
     state = "backward";
    }
}
/*----------------------------------------------------------------------------------------------*/
// obstacle-avoidance function
// (no changes here, it works as is)
void Obstacle_avoidance() {
    stopMotors();
    delay(500);         

    backward(base_speed);
    delay(400); 
    stopMotors();
    delay(100);

    // Check right distance
    right(max_speed, base_speed/2);
    delay(800);
    rdistance = Distance();
    stopMotors();
    delay(100);

    // Check left distance
    left(max_speed, base_speed/2);
    delay(1600);
    ldistance = Distance();
    stopMotors();
    delay(100);

    if(ldistance > rdistance) {
        left(max_speed, base_speed/2);
        delay(400);
        stopMotors();
        delay(100);

        forward(max_speed);
        delay(800);
        stopMotors();
        delay(100);

        right(max_speed, base_speed/2);
        delay(400);
        stopMotors();
    } else {
        right(max_speed, base_speed/2);
        delay(400);
        stopMotors();
        delay(100);

        forward(max_speed);
        delay(800);
        stopMotors();
        delay(100);

        left(max_speed, base_speed/2);
        delay(400);
        stopMotors();
    }

    while(!(digitalRead(IR_l) == 1 && digitalRead(IR_c) == 0 && digitalRead(IR_r) == 1)) {
        L_ir = digitalRead(IR_l);
        C_ir = digitalRead(IR_c);
        R_ir = digitalRead(IR_r);

        if(L_ir == 1 && R_ir == 1) {
            forward(base_speed);
        } else if(L_ir == 0) {
            left(base_speed, base_speed/2);
        } else if(R_ir == 0) {
            right(base_speed, base_speed/2);
        } else {
            forward(base_speed);
        }
        delay(50);
    }

    stopMotors();
}


/*------------------------------------------------------------------------------------------------*/
// function IR 
void IR(){
  L_ir = digitalRead(IR_l); // read left ir
  R_ir = digitalRead(IR_r); // read right ir
  C_ir = digitalRead(IR_c); // read center ir

 if(C_ir==1 && R_ir == 0 && L_ir == 0)
 {
    forward(max_speed);
 }
 else if (R_ir == 0 && L_ir == 0){
    forward(max_speed);
    state = "forward";
  }
 else if(L_ir == 1 && R_ir == 0)
  {
     left(max_speed,base_speed);  
     state = "left";
  }
 else if (L_ir == 0  && R_ir == 1)
  {
    right(max_speed,base_speed);
    state = "right";
  }
 else if(L_ir == 0 && C_ir == 0 && R_ir == 0) // fixed (was commas before)
 {
   behaviour_func(state);  // fixed (no extra String keyword)
 }
}
/*-------------------------------------------------------------------------------------------------*/


void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en2,OUTPUT);
  pinMode(en1,OUTPUT);
  
  pinMode(Ledred_r,OUTPUT);
  pinMode(Ledred_l,OUTPUT);
  pinMode(Ledyellow_l,OUTPUT);
  pinMode(Ledyellow_r,OUTPUT);
  //pinMode(buzzer,OUTPUT);
  
  pinMode(Trig,OUTPUT);
  pinMode(echo,INPUT);
  
  digitalWrite(en1,LOW);
  digitalWrite(en2,LOW);

  //noTone(buzzer);
  digitalWrite(Ledred_r,LOW);
  digitalWrite(Ledred_l,LOW);
  digitalWrite(Ledyellow_l,LOW);
  digitalWrite(Ledyellow_r,LOW);
  digitalWrite(Ledgreen,LOW);
}
void loop() {
  if (Serial.available() > 0) {
    Blutooth();
    return;
  }
  distance = Distance();
  if(distance < 15) { // threshold in cm
    Obstacle_avoidance();
    return;
  }
  IR();
}
