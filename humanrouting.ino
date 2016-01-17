
#include <Keypad.h>
#include <LiquidCrystal.h>

/*
 * Keyboard
 */
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 35, 49, 51, 53 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 31, 32, 33, 34 }; 

// 31, 32, 33, 34
// 35, 49, 51, 53

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define ledpin 13

/*
 * LCD
 */
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*
 *  Our code
 * 
 */
const int STATE_START = 0;
const int STATE_ENTER_DEST = 1;
const int STATE_SHOW_PATH = 2;

const int MAXDESTINATIONS = 3;
const int NODESCOUNT = 9;

int state = STATE_START;
int destinationsCount = 0;
int currentDestination = -1;
int destinations[MAXDESTINATIONS] = { -1, -1, -1 };

int pathCursor = -1;

class NodeObj {
 public:
   NodeObj(){
      _nodeId = "";
      _distance = 0;
      _dir = 0;
   }
   NodeObj(String nodeId, int distance, int dir)
   {
      _nodeId = nodeId;
      _distance = distance;
      _dir = dir;
   }

   String getNodeTextMain()
   {
      // 5 chars 
      String t = "Node:";

      // 4 chars
      t.concat(_nodeId);

      // max 5 chars
      t.concat(this->getDistance());
      return t;
   }

   String getNodeTextSec()
   {
      // 4 chars 
      String t = "Dir:";
      
      return t;
   }

   String getDistance()
   {
      String d = "";
      if (_distance < 1000)
      {
          d.concat(_distance);
          d.concat("m");
      } else {
          d.concat(_distance / 1000);
          d.concat("km");
      }
      return d;
   }

   String getDirection()
   {
      
   }
   
 private:
   String _nodeId;

   // distance and direction to reach nodeId
   int _distance;
   int _dir;
};

bool DestinationExists(int destination)
{
  for (int i = 0; i < min(destinationsCount, MAXDESTINATIONS); i++)
  {
    if (destinations[i] == destination) {
      return true;
    }
  }
  return false;
}

void PushDestination(int destination)
{
  // Exists
  if (DestinationExists(destination)) {
    return;
  }

  // Add Destination
  destinations[destinationsCount] = destination;
  destinationsCount ++;
}

void PopDestination()
{
  destinations[destinationsCount] = -1;
  destinationsCount --;
}

void CleanDestinations()
{
  destinationsCount = 0;
  for (int i = 0; i < MAXDESTINATIONS; i++) {
    destinations[i] = -1;
  }
}

String GetDestinationText()
{
  String t = "";
  for (int i = 0; i < min(destinationsCount, MAXDESTINATIONS); i++)
  {
    t.concat(destinations[i] + 1);
    if (i == MAXDESTINATIONS - 1) {
      t.concat(".");
    } else {
      t.concat(",");
    }
  }
  return t;
}

void CalculatePath()
{
  
}

String transformText(String text)
{
  int len = 16 - text.length();
  String space = "";
  for( int i = 0; i < len; i++)
  {
    space.concat(" ");
  }

  text.concat(space);
  return text;
}

void printMain(String text)
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print(transformText(text));
}

void printInput(String input)
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print(transformText(input));  
}

/*
 * SETUP
 */
void setup() {
  // put your setup code here, to run once:
  
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin, HIGH);
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello!");
}

/*
 * LOOP
 */
void loop() {
  // put your main code here, to run repeatedly:
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '*':
        if (state == STATE_SHOW_PATH) {
          pathCursor = max(pathCursor - 1, -1);
        }
        break;
      case '#':
        if (state == STATE_START) {
          state = STATE_ENTER_DEST;
        } else if (state == STATE_ENTER_DEST) {
          // calculate path
          Serial.println("Calculate Path");
          CalculatePath();
          state = STATE_SHOW_PATH;
        } else if (state == STATE_SHOW_PATH) {
          pathCursor = min(pathCursor + 1, 10);
        }
        break;
      case 'D': 
        if (state == STATE_ENTER_DEST) {
          // Clean path
          CleanDestinations();
        } else if (state == STATE_SHOW_PATH) {
          // RESET
          state = STATE_START;
          pathCursor = -1;
          CleanDestinations();
        }
        break;
      case '0':
        if (state == STATE_ENTER_DEST) {
          PopDestination();
        }
        break;
      default:
        if (state == STATE_ENTER_DEST) {
          if ('0' < key && key <= '9') {
            currentDestination = key - 49; 
            PushDestination(key - 49); // '1' becomes 0
          }
        }
    }
  }
  
  if (state == STATE_START)
  {
    printMain("Hello! Lost?");
    printInput("Press # to start");
    return;
  }

  if (state == STATE_ENTER_DEST)
  {
    String allDestinations = GetDestinationText();
    
    printMain("Enter & press #");
    printInput(allDestinations);
    return;
  }

  if (state == STATE_SHOW_PATH)
  {
    if (pathCursor < 0) {
      String maintext = "Path to ";
      maintext.concat(GetDestinationText());

      printMain(maintext);
      printInput("Next: #  Back: *");
    }

    if (pathCursor >= 0) {

      String main = "Node No: ";
      main.concat(pathCursor);
      
      printMain(main);
      printInput("");
    }
    return;
  }
}


