#include "Ball.h"
extern Serial pc;
#define NUM_ROW 16
#define NUM_COL 24

Ball::Ball() {
  x=y=width=height=xInc=yInc=0;
  srand(time(0));
}

Ball::Ball(int _x, int _y, int _w, int _h, int _xi, int _yi){
    x = _x;
    y = _y;
    width = _w;
    height = _h;
    xInc = _xi;
    yInc = _yi;
    srand(time(0));
}

void Ball::print_ball_position(){
    pc.printf("ball x: %d\r\n", x);
    pc.printf("ball y: %d\r\n", y);
}

/* 
 * Member Function move:
 * Description: Colors in the previous ball black
 *  and moves ball to new position.
 */
void Ball::move(LED_board &board) {
  draw(board, false);
  x += xInc; y += yInc;
}

/* 
 * Member Function draw:
 * Description: Draws object on screen
 *  if isBlack, color in black.
 */
void Ball::draw(LED_board &board, bool state) const {
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
        board.set_LED(y+i, x+j, state);
    }
  }
}

int Ball::size() const {
  return width*height;
}

int Ball::getxInc() const{
    return xInc;
}

int Ball::getyInc() const{
    return yInc;
}

int Ball::getX() const {
  return x;
}

int Ball::getY() const {
  return y;
}

int Ball::getWidth() const {
  return width;
}

int Ball::getHeight() const {
  return height;
}

bool Ball::hitX() {
  //return (x <= 0) || (x >= NUM_COL - 1);
  return (x <= 0) || (x + width >= NUM_COL);
}

bool Ball::hitLeft(){
    return (x <= 0);
}

bool Ball::hitRight(){
    return (x + width >= NUM_COL);
}

bool Ball::hitY() {
  //return (y <= 0) || (y >= NUM_COL - 1);
  return (y <= 0) || (y + height >= NUM_ROW);
}

bool Ball::hitP1x(int _x, int _y, int _height) { 
  bool hit = ( xInc < 0 ) && ( x <= _x) && 
                    ( 
                        ( (yInc > 0) && (y <= _y + _height - 1) && (y + height >= _y) ) //if ball is moving down
                        || 
                        ( (yInc < 0) && (y + height > _y) && (y <= _y + _height)) //if ball is moving up
                        ||
                        ( (yInc == 0) && (y + height > _y) && (y <= _y + _height - 1) ) //if ball is moving straight
                    ) ;
  if(hit) {
    if( y < _y + _height / 2) yInc = -1;
    else if( y == _y + _height / 2) yInc = 0;
    else yInc = 1;
  }
  return hit;
}

bool Ball::hitP2x(int _x, int _y, int _height) {
  bool hit = ( xInc > 0 ) && ( x + width >= _x) && 
                    ( 
                        ( (yInc > 0) && (y <= _y + _height - 1) && (y + height >= _y) ) //if ball is moving down
                        || 
                        ( (yInc < 0) && (y + height > _y) && (y <= _y + _height)) //if ball is moving up
                        ||
                        ( (yInc == 0) && (y + height > _y) && (y <= _y + _height - 1) ) //if ball is moving straight
                    ) ;
  if(hit) {
    if( y < _y + _height / 2) yInc = -1;
    else if( y == _y + _height / 2) yInc = 0;
    else yInc = 1;
  }
  return hit;
}

void Ball::reverseX() {
  xInc *= -1;
}

void Ball::reverseY() {
  yInc *= -1;
}
