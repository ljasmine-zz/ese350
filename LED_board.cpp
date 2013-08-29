#include "LED_board.h"

extern Serial pc;

//shifter
extern DigitalOut SER; //serial input is inverted!
extern DigitalOut OEnot; //can simply connect to ground
extern DigitalOut Latch; //latch data RCLK
extern DigitalOut Clock; //clock data SRCLK
extern DigitalOut SRCLRnot; //clear it when = 0

//decade counter
extern DigitalOut MR1;
extern DigitalOut MR2;
extern DigitalOut CP; //increments
extern DigitalOut CEnot; //disables counting when high
extern DigitalOut CEnot2; //disables counting when high

#define CLOCK_DATA do{Clock= 0; Clock = 1;} while(0) //clocks
#define LATCH_DATA do{Latch = 0; Latch = 1;} while(0) //latches
#define CLEAR_DATA SRCLRnot = 0; SRCLRnot = 1; LATCH_DATA
#define DEC_INC do{CP = 0; CP = 1;} while(0) //increments on positive edge
#define PANEL_ON OEnot = 0
#define PANEL_OFF OEnot = 1
#define RESET_DEC1 do{MR1 = 1; MR1 = 0;} while(0);
#define RESET_DEC2 do{MR2 = 1; MR2 = 0;} while(0);
#define NUM_ROW 16
#define NUM_COL 24

LED_board::LED_board(){
    
    lit_row = 0;
    volatile char row_buffer[NUM_ROW][NUM_COL] =  /*{{1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1}};
                                                 {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1} } ; 
                                                  { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };*/
                                                     { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                                     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };
}

int LED_board::get_row(){
    return lit_row;
}

void LED_board::set_LED(int row, int col, bool state){
    if(row >= 0 && row < NUM_ROW && col >= 0 && col < NUM_COL){ //prevent out of bounds
        if(state){ //turn on
            row_buffer[row][col] = 0;
        
        }else{
            row_buffer[row][col] = 1;
        }
    }
}

void LED_board::set_row(int row_index, bool state){
    for(int j = 0; j < NUM_COL; j++){
        set_LED(row_index, j, state);
    }
}

void LED_board::set_col(int col_index, bool state){

    for(int i = 0; i < NUM_ROW; i++){
        set_LED(i, col_index, state);
    }
}

void LED_board::clear_shifter(){
    for(int row = 0; row < NUM_ROW; row++){
        SER = 1;
        CLOCK_DATA;
    }
    LATCH_DATA;
}

void LED_board::set_panel(bool state){
    for(int i = 0; i < NUM_ROW; i++){
        for(int j = 0; j < NUM_COL; j++){
            set_LED(i,j,state);
        }
    }
}

void LED_board::flash_panel(){
    static bool on_off = true;
    if(on_off)
        set_panel(true);
    else
        set_panel(false);
    on_off = !on_off;
}

bool LED_board::LED_state(int x, int y){
     return (row_buffer[x][y]==0);
}

void LED_board::sys_rst(){
    /*//initialization of shifter      
    set_panel(false);
    SRCLRnot = 1; //don't reset (this value is 0 by default)
    
    //initialization of decade counter
    RESET_DEC1; //reset it
    RESET_DEC2;
    CEnot = 0; //enable counting
   
    clear_shifter();
    PANEL_ON; //turn panel off at first*/
    //initialization of shifter      
    set_panel(false);
    SRCLRnot = 1;
    //initialization of decade counter
    RESET_DEC2;
    CEnot = 1; //disable counter1
    CEnot2 = 0; //enable counter2
    
    //increment counter2 to 7
    for(int i = 0; i < 7; i++){
        DEC_INC;
        
    }
    
    CEnot2 = 1; //disable counter2
    CEnot = 0; //enable counter1
    RESET_DEC1; //reset counter1
    
    clear_shifter();
}


void LED_board::shift_decoder(int row1, int row2){
    if(row1 == row2) return; //if same row then don't need to increment decade counter
    if((row1 < 9) && (row2 < 9)){ //moving from DC1 to DC1
        if(row2 > row1){
            for(int i = 0; i < row2 - row1; i++){
                DEC_INC;
            }
        }else{
            RESET_DEC1;
            for(int i = 0; i < row2; i++){
                DEC_INC;
            }
        }   
    }else if((row1 >= 9) && (row2 >= 9)){ //moving from DC2 to DC2
        if(row2 > row1){
            for(int i = 0; i < row2 - row1; i++){
                DEC_INC;
            }
        }else{
            RESET_DEC2;
            for(int i = 0; i < row2; i++){
                DEC_INC;
            }
        }
    }else if(row2 > row1){ //moving from DC1 to DC2
        for(int i = 0; i < 9-row1; i++){ //increment DC1 to 10 and turn DC1 off
            DEC_INC;
        }
        CEnot = 1; //disable counting on DC1
        CEnot2 = 0; //enable counting on DC2
        RESET_DEC2; //DC2 reset to 0
        for(int i = 0; i < row2-9; i++){
            DEC_INC;
        }
    }else{ //moving DC2 to DC1
        for(int i = 0; i < 17-row1; i++){ //increment DC2 to 
            DEC_INC;
        }
        CEnot2 = 1; //disable counting on DC2
        CEnot = 0; //enable counting on DC1
        RESET_DEC1; //DC1 reset to 0
        for(int i = 0; i < row2; i++){
            DEC_INC;
        }
    }
}
bool LED_board::row_has_data(int row){
    for(int col = 0; col < NUM_COL; col++){
        if(  ((row_buffer[row][col]) & 0x1) == 0  ){
            return true;
        }
    }
    return false;
}

void LED_board::load_row_data(int row){
    for(int col = 0; col < NUM_COL; col++){
        SER = ((row_buffer[row][col]) & 0x1);
        //pc.printf("//SER data = %d\r\n", (row_buffer[row][col]) & 0x1);
        CLOCK_DATA;
    }
}

void LED_board::refresh_row(){
    static int count = 0;
    int next_row = (lit_row + 1) % NUM_ROW;
    while(!row_has_data(next_row) && (count < NUM_ROW)){
        count++;
        next_row = (next_row + 1) % NUM_ROW;
    }
    count = 0;
    
    load_row_data(next_row);
    LATCH_DATA;
    shift_decoder(lit_row, next_row);
    lit_row = next_row;
}

void LED_board::turn_on_line(int num_pts, int *points){
    for(int i = 0; i <= num_pts * 2; i += 2){
       set_LED(points[i], points[i+1], true); //turns on
    }
}

void LED_board::draw_line (int row1, int col1, int row2, int col2) 
{
  int dx, dy, sx, sy, err, e2, i;
  int points [NUM_ROW * NUM_COL]; //128 is the largest # of pixels, so size should be 128*2
  
  //led_reset();
  
  dx = abs(col2 - col1);
  dy = abs(row2 - row1);
  if(col1 < col2){
    sx = 1;
  }
  else sx = -1;

  if(row1 < row2){
    sy = 1;
  }
  else sy = -1;

  err = dx - dy;
  points[0] = col1;
  points[1] = row1;
  i = 2;
  while((col1 != col2) || (row1 != row2)){
    e2 = 2*err;
    if(e2 > (-dy)){
      err = err - dy;
      col1 = col1 + sx;
    }
    if(e2 < dx){
      err = err + dx;
      row1 = row1 + sy;
    }
    points[i] = col1;
    points[i+1] = row1;
    i = i + 2;
  } 
  i = (i - 1)/2; //overcounted 2, add 1 to get length, / by 2 to get #points
  
  turn_on_line(i, points);
}

void LED_board::light_row(int row_index){
    if((row_index < 0) || (row_index > (NUM_ROW-1))) return;
    for(int i = 0; i < NUM_ROW; i++){
        for(int j = 0; j < NUM_COL; j++){
            if(i == row_index){
                set_LED(i, j, true); //turn on
            }else{
                set_LED(i, j, false); //turn off
            }
        }
    }
}

void LED_board::light_col(int col_index){
    if((col_index < 0) || col_index > (NUM_COL - 1)) return;
    for(int i = 0; i < NUM_ROW; i++){
        for(int j = 0; j < NUM_COL; j++){
            if(j == col_index){
                set_LED(i, j, true); //turn on
            }else{
                set_LED(i, j, false); //turn on
            }
        } 
    }
}

void LED_board::shift_through_rows(){
    static int current_row = 0;
    if(current_row > (NUM_ROW-1)){
        current_row = 0;
    }
    
    light_row(current_row);
    current_row++;
}

void LED_board::shift_through_cols(){
    static int current_col = 0;
    if(current_col > (NUM_COL-1)){
        current_col = 0;
    }
    
    light_col(current_col);
    current_col++;
}

void LED_board::print_board(){
   for(int i = 0; i < NUM_ROW; i++){
        for(int j = 0; j < NUM_COL; j++){
            pc.printf("row_buffer[%d][%d] = %x\r\n", i, j, row_buffer[i][j]);
        }
    }
}
