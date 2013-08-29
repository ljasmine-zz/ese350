#ifndef LED_board_H
#define LED_board_H

#include "mbed.h"

class LED_board {
  // Attributes
  volatile char row_buffer[16][24];
    
public:
  // Constructors
  LED_board();
  int lit_row;
  
  
  // Member functions
    
    int get_row();
    void set_panel(bool state);
    void flash_panel();
    
    void set_row(int row_index, bool state);
    void set_col(int col_index, bool state);
    bool LED_state(int x, int y);
    
    void clear_shifter();
    void sys_rst();
    void array_rst();
    void shift_decoder(int row1, int row2);
    void load_row_data(int row);
    bool row_has_data(int row);
    void refresh_row();
    void set_LED(int row, int col, bool state);
    void turn_on_line(int num_pts, int *points);
    void draw_line (int row1, int col1, int row2, int col2) ;
    void light_row(int row_index);
    void light_col(int col_index);
    void shift_through_rows();
    void shift_through_cols();
    void print_board();
};
#endif
