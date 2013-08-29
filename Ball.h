#ifndef Ball_H
#define Ball_H

#include "LED_board.h"

class Ball {
  /* This class creates a ball object */
  
  // Attributes
  int x,y,width,height,xInc,yInc;

public:
  // Constructors
  Ball();
  Ball(int _x, int _y, int _w, int _h, int _xi, int _yi);
  
  // Member functions
  void move(LED_board &board);
  void draw(LED_board &board, bool state) const;
  int getX() const;
  int getY() const;
  int getxInc() const;
  int getyInc() const;
  int getWidth() const;
  int getHeight() const;
  bool hitX();
  bool hitLeft();
  bool hitRight();
  bool hitY();
  bool hitP1x(int _x, int _y, int _height);
  bool hitP2x(int _x, int _y, int _height);
  int size() const;
  void reverseX();
  void reverseY();
  void print_ball_position();
};

#endif
