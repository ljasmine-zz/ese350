#include "Paddle.h"

Paddle::Paddle() {
  int x=y=width=height=score=0;
  lives = 3;
}

Paddle::Paddle(int _x, int _y, int _w, int _h, int _l, int _s)
 : x(_x), y(_y), width(_w), height(_h), lives(_l), score(_s) {}
 
/* 
 * Member Function move:
 * Description: Colors in the previous paddle black
 *  and moves paddle to new position.
 */
void Paddle::move(LED_board &board, int increment) {
  draw(board, false);
  y += increment;
}

void Paddle::draw(LED_board &board, bool state) const {
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
        board.set_LED(y+i, x+j, state);
    }
  }
}

bool Paddle::loseLife() {
  return --lives;
}

void Paddle::addPoint() {
  ++score;
}

int Paddle::size() const {
  return width*height;
}

int Paddle::getWidth() const {
  return width;
}

int Paddle::getHeight() const {
  return height;
}

int Paddle::getX() const {
  return x;
}

int Paddle::getY() const {
  return y;
}

int Paddle::getLives() const {
  return lives;
}

int Paddle::getScore() const {
  return score;
}

void Paddle::setLives(int l) {
  lives = l;
}
