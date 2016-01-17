
#include <Keypad.h>
#include <LiquidCrystal.h>

#define N 5
#define INF 0x7FFF

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

const int MAXDESTINATIONS = 4;
const int NODESCOUNT = 5;

int state = STATE_START;
int destinationsCount = 0;
int currentDestination = -1;
int destinations[MAXDESTINATIONS] = { -1, -1, -1 };

int pathCursor = -1;

int globalPath[N*N];
int globalPathCount = 0;

/**
 * 
 * 
 */

// alex function definitions
void getPath(int P[][N], int i, int j, int paths[], int k);    //, int paths[], int k
void node_compression(int dist[][N], int p[][N], int path[N], int paths[N][N][N], int compressed_nodes[][N], int nodes_of_interest[], int mapping_of_nodes[N], int nodes_of_interest_count);

// constantina function definitions
int finalizePath( int *minimum_path, int dist[N][N],int  ARRAY_SIZE, int id);
void swap(int *x, int *y);
void permute(int *a, int l, int r, int dist[N][N],int *minimum_distance, int *minimum_path, int ARRAY_SIZE, int id);


int CalculatePath(int id) {

  Serial.println("CalculatePath");
  
  // alex variables
  int dist[N][N];  // For some N
  int p[N][N];
  int path[N];
  int paths[N][N][N];
  int compressed_nodes[N][N];
  int i, j, k;
  int nodes_of_interest[N];
  int mapping_of_nodes[N];
  int nodes_of_interest_count;
  int final_path[N*N];

  // constantinas variables
  int min_dist= INF;
  int minimum_path[N];

  // NODES OF INTEREST
  for (i = 0; i < N; ++i) {
    nodes_of_interest[i] = 0;
    mapping_of_nodes[i] = -1;
  }

  // alex main
  nodes_of_interest_count = destinationsCount + 1;

  nodes_of_interest[id] = 1;
  
  for (int d = 0; d < destinationsCount; d++) {
      nodes_of_interest[destinations[d]] = 1;
  }
  
  /*nodes_of_interest[0] = 1;
  nodes_of_interest[1] = 0;
  nodes_of_interest[2] = 1;
  nodes_of_interest[3] = 1;
  nodes_of_interest[4] = 1;*/

  mapping_of_nodes[0] = id;
  
  for (int d = 0; d < destinationsCount; d++) {
      mapping_of_nodes[d + 1] = destinations[d];
  }
  

  /*
  mapping_of_nodes[0] = 0;
  mapping_of_nodes[1] = 2;
  mapping_of_nodes[2] = 3;
  mapping_of_nodes[3] = 4;
  mapping_of_nodes[4] = -1;*/
  

  for (int k = 0; k < N; k++)
  {
    //Serial.println(nodes_of_interest[k]);
    Serial.println(mapping_of_nodes[k]);
  }


  for (i = 0; i < N*N; ++i) {
    final_path[i] = -1;
  }

  // CREATE TOPOLOGY
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      if (i == j) {
        dist[i][j] = 0;
        compressed_nodes[i][j] = 0;
      } else {
        dist[i][j] = INF;
        compressed_nodes[i][j] = INF;
      }
    }
  }
  
  dist[0][1] = 1;
  dist[1][2] = 1;
  dist[1][4] = 1;
  dist[2][1] = 1;
  dist[2][3] = 1;
  dist[3][2] = 1;
  dist[3][4] = 1;
  dist[4][1] = 1;
  dist[4][3] = 1;
  dist[1][0] = 1;
  dist[2][4] = 1;
  dist[4][2] = 1;

  // init parent matrix
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if (dist[i][j] != 0 && dist[i][j] != INF) {
        p[i][j] = i;
      } else {
        p[i][j] = -1;
      }
    }
  }
  
  node_compression(dist, p, path, paths, compressed_nodes, nodes_of_interest, mapping_of_nodes, nodes_of_interest_count);

  //// Serial.print("\n\n\n");

  for (i = 0; i < nodes_of_interest_count; ++i) {
    for (j = 0; j < nodes_of_interest_count; ++j) {
      //int idx = nodes_of_interest[i];
      //if (idx != -1 && paths[idx] == ) {
      // Serial.print("%d\t", compressed_nodes[i][j]);
      //}
    }
    // Serial.print("\n");
  }

  // printf("DIO SOU TOUTA\n");
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      if (compressed_nodes[i][j] == INF) {
        // printf("INF\t");
      } else {
        // printf("%d\t", compressed_nodes[i][j]);
      }
    }
    // printf("\n");
  }

  // printf("%d\n", nodes_of_interest_count);
  // printf("OS DAME\n");

  // constantinas main
  int id_mapping = -1;
  for (i = 0; i < N; ++i) {
    if (mapping_of_nodes[i] == id) {
      id_mapping = i;
    }
  }

  if (id_mapping == -1) {
    // printf("SHOULD NEVER HAPPEN\n");
    return 1;
  }
  
  min_dist = finalizePath(minimum_path, compressed_nodes, nodes_of_interest_count, id_mapping);
  
    // printf("Minimum distance: %d\n", min_dist);
  // printf("%d %d %d \n", minimum_path[0],minimum_path[1],minimum_path[2]);

  for (i = 0; i < N; i++) {
    // printf("%d ", minimum_path[i]);
  } 
  // printf("\n");
  
  
  int idx = 0;
  j = 0;
  while(idx < N-1 && minimum_path[idx] != -1 && minimum_path[idx+1] != -1) {
    i = 1;
    while (i < N && paths[mapping_of_nodes[minimum_path[idx+1]]][mapping_of_nodes[minimum_path[idx]]][i] != -1) {
      final_path[j] = paths[mapping_of_nodes[minimum_path[idx+1]]][mapping_of_nodes[minimum_path[idx]]][i];
      // printf("paths[mapping_of_nodes[minimum_path[idx+1]]][mapping_of_nodes[minimum_path[idx]]][i] : %d\n", paths[mapping_of_nodes[minimum_path[idx+1]]][mapping_of_nodes[minimum_path[idx]]][i]);
      // printf("mapping_of_nodes[minimum_path[idx]] : %d\n", mapping_of_nodes[minimum_path[idx]]);
      // printf("mapping_of_nodes[minimum_path[idx+1]] : %d\n", mapping_of_nodes[minimum_path[idx+1]]);
      // printf("minimum_path[idx] : %d\n", minimum_path[idx]);
      // printf("minimum_path[idx+1] : %d\n", minimum_path[idx+1]);
      // printf("idx : %d\n", idx);
      // printf("idx+1 : %d\n", idx+1);
      // printf("i : %d\n", i);
      // printf("j : %d\n", j);

      i++;
      j++;
    }
    idx++;
  }

  globalPathCount = 0;
  for (i = 0; i < N*N; i++) {
    if (final_path[i] == -1) {
      break;
    }

    globalPath[globalPathCount] = final_path[i];
    globalPathCount++;
    
    Serial.print(final_path[i], DEC);
    Serial.print("-");
  }
  
  return 0;
}


void getPath(int P[][N], int i, int j, int path[], int k) { 
  if (i == j) {
    //// printf("%d ", i);
    path[k] = i;
    ++k;
  }
  else if (P[i][j] == -1) {
    //// printf("No Path");
    path[k] = -2;
    k = 0;
  }
  else {
    path[k] = j;
    ++k;
    getPath(P, i, P[i][j], path, k); 
    //// printf("%d ", j);
  }
}

void node_compression(int dist[][N], int p[][N], int path[N], int paths[N][N][N], int compressed_nodes[][N], int nodes_of_interest[], int mapping_of_nodes[N], int nodes_of_interest_count) {

  int i, j, k;

  // Input data into dist, where dist[i][j] is the distance from i to j.
   
    for ( k = 0; k < N; k++ )
        for ( i = 0; i < N; i++ )
          for ( j = 0; j < N; j++ ) {
              //dist[i][j] = min( dist[i][j], dist[i][k] + dist[k][j] );
              if (dist[i][k] == INF || dist[k][j] == INF) {
          continue;                 
        }
        if (dist[i][j] > dist[i][k] + dist[k][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
          p[i][j] = p[k][j];
        }
      }
  // Now the shortest distance between i and j will be in dist[i][j].

  // printf("\nShortest Paths\n--------------------------------\n\n");
  // printf("\t");
    for (int i = 0; i < N; ++i) { 
    // printf("%d\t", i);
  }
  // printf("\n");
  for (int i = 0; i < N; ++i) {
    // printf("%d\t", i);
    for (int j = 0; j < N; ++j) {
      // printf("%d\t", dist[i][j]);
    }
    // printf("\n");
  }



  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      if (nodes_of_interest[i] == 1 && nodes_of_interest[j] == 1) {
        for (k = 0; k < N; ++k) {
          path[k] = -1;
        }
        // printf("Path from %d to %d: ", i, j );
        getPath(p, i, j, path, 0);
        int len = 0;
        while (path[++len]!=-1);
        // printf("len = %d\n", len);
        int interesting = 1;
        if (len == 1) {
          interesting = 0;
        }
        for (k = 1; k < (len-1); ++k) {
          if (nodes_of_interest[path[k]] == 1) {
            interesting = 0;
          }
        }
        if (interesting == 1) {
          int x, y;
          for (k = 0; k < N; ++k) {
            if (mapping_of_nodes[k] == i) {
              x = k;
            }
            if (mapping_of_nodes[k] == j) {
              y = k;
            }
          }
          compressed_nodes[x][y] = dist[i][j];
          for (k = 0; k < N; ++k) {
            // printf("%d\t", path[k]);
            paths[i][j][k] = path[k];
          }
          // printf("\n");
        }
        
      } //else {
        //for (k = 0; k < N; ++k) {
        //  path[k] = -5;
        //}
      //}
    }
  }
  

  for (i = 0; i < nodes_of_interest_count; ++i) {
    for (j = 0; j < nodes_of_interest_count; ++j) {
      //int idx = nodes_of_interest[i];
      //if (idx != -1 && paths[idx] == ) {
      // printf("%d\t", compressed_nodes[i][j]);
      //}
    }
    // printf("\n");
  }
}

// CONSTANTINA ALG

void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void permute(int *a, int l, int r, int dist[N][N],int *minimum_distance, int *minimum_path, int ARRAY_SIZE, int id)
{
   int i;
   int c1,c2;
   int distance=0;
   if (l == r){
      if (id == a[0]) {
          // printf("%d %d %d \n", a[0],a[1],a[2]);
          // printf("%d %d\n", dist[a[0]][a[1]], dist[a[1]][a[2]] );
          // printf("======\n");
          for (c1=0; c1<ARRAY_SIZE-1; ++c1){
              if (distance == INF || dist[a[c1]][a[c1+1]] == INF) {
                distance = INF;
              } else {
                distance = distance + dist[a[c1]][a[c1+1]] ;
              }
            
          } 
        // printf("distance is: %d\n", distance);
        // printf("======\n");
        if (*minimum_distance > distance){
          *minimum_distance = distance;
          for (c2=0; c2<ARRAY_SIZE; ++c2){
            minimum_path[c2] = a[c2];
        }
          // printf("Current Min Path\n\n");
          // printf("%d %d %d\n", a[0],a[1],a[2]);
          // printf("%d %d %d\n", minimum_path[0],minimum_path[1],minimum_path[2]);
          // printf("\n\n");
      }
    }
  } 
   else
   {
       for (i = l; i <= r; i++)
       {
          swap((a+l), (a+i));
          permute(a, l+1, r, dist,minimum_distance, minimum_path, ARRAY_SIZE, id);
          swap((a+l), (a+i)); 
       }
   }
}

int finalizePath( int *minimum_path, int dist[N][N],int  ARRAY_SIZE, int id){
  int str[N] ;
  int i=0;
  int minimum = INF;
  for (i=0; i<N; i++){
    minimum_path[i]=-1;
    str[i]=i;
  }
    permute(str, 0, ARRAY_SIZE-1, dist, &minimum, minimum_path, ARRAY_SIZE, id);
    
    // printf("Minimum distance: %d\n", minimum);
  // printf("%d %d %d \n", minimum_path[0],minimum_path[1],minimum_path[2]);
  return minimum;
  
}

/**
 *  END
 * 
 */
 
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
          // Serial.println("Calculate Path");
          CalculatePath(0);
          state = STATE_SHOW_PATH;
        } else if (state == STATE_SHOW_PATH) {
          pathCursor = min(pathCursor + 1, globalPathCount);
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
      case '1':
        break;
      default:
        if (state == STATE_ENTER_DEST) {
          if ('1' <= key && key <= '5') {
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

    if (0 <= pathCursor && pathCursor < globalPathCount) {

      String main = "Node No: ";
      
      main.concat(globalPath[pathCursor] + 1);
      
      printMain(main);
      printInput("");
    }
    return;
  }
}


