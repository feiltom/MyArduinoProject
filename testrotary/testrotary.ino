// usually the rotary encoders three pins have the ground pin in the middle
enum PinAssignments {
  encoderPinA = D4,   // right
  encoderPinB = D3   // left
//  clearButton = D8    // another two pins
};

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating=false;      // debounce management
static int clearButton = 15;
// interrupt service routine vars
boolean A_set = false;              
boolean B_set = false;


void setup() {
  digitalWrite(clearButton, HIGH);

  pinMode(encoderPinA, INPUT); 
  pinMode(encoderPinB, INPUT); 
  pinMode(clearButton, INPUT);
 // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  //digitalWrite(clearButton, HIGH);

// encoder pin on interrupt 0 (pin 2)
  attachInterrupt(D4, doEncoderA, CHANGE);
// encoder pin on interrupt 1 (pin 3)
  attachInterrupt(D3, doEncoderB, CHANGE);

  Serial.begin(115200);  // output
      Serial.println("start:");

}

// main loop, work is done by interrupt service routines, this one only prints stuff
void loop() { 
  rotating = true;  // reset the debouncer

  if (lastReportedPos != encoderPos) {
    Serial.print("Index:");
    Serial.println(encoderPos, DEC);
    lastReportedPos = encoderPos;
  }
  /*if (digitalRead(clearButton) == LOW )  {
    encoderPos = 0;
        Serial.print("Index:");
    Serial.println(digitalRead(clearButton));
  }*/
}

// Interrupt on A changing state
void doEncoderA(){
  // debounce
  if ( rotating ) delay (1);  // wait a little until the bouncing is done

  // Test transition, did things really change? 
  if( digitalRead(encoderPinA) != A_set ) {  // debounce once more
    A_set = !A_set;

    // adjust counter + if A leads B
    if ( A_set && !B_set ) 
      encoderPos += 1;

    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB(){
  if ( rotating ) delay (1);
  if( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if( B_set && !A_set ) 
      encoderPos -= 1;

    rotating = false;
  }
}
