#ifndef Paddle_H
#define Paddle_H

#include "LED_board.h"

class Paddle {
  /* This class creates a paddle object */
 
  // Attributes
  int x,y,width,height,lives,score;

public: 
  // Constructors
  Paddle();
  Paddle(int _y, int _x, int _w, int _h, int _l, int _s);
  
  // Member functions
  void move(LED_board &board, int increment);
  void moveCPU(LED_board &board, int _y);
  void draw(LED_board &board, bool state) const;
  bool loseLife();
  void addPoint();
  int size() const;
  int getWidth() const;
  int getHeight() const;
  int getX() const;
  int getY() const;
  int getLives() const;
  int getScore() const;
  void setLives(int l);
  
};

#endif
