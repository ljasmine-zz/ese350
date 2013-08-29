#include "mbed.h"

Serial pc(USBTX, USBRX);
//shifter
DigitalOut SER(p21); //serial input is inverted!
DigitalOut OEnot(p22); //can simply connect to ground
DigitalOut Latch(p23); //latch data RCLK
DigitalOut Clock(p24); //clock data SRCLK
DigitalOut SRCLRnot(p25); //clear it when = 0

//decade counter
DigitalOut MR1(p20); //reset for DC1
DigitalOut CP(p19); //increments decade counter
DigitalOut CEnot(p18); //disables DC1 counting when high
DigitalOut MR2(p17); //reset for DC2
DigitalOut CEnot2(p16); //enable for DC2 

//key presses
DigitalIn Num1(p12);
DigitalIn Num2(p13);
DigitalIn Num3(p14);
DigitalIn NumA(p15);

DigitalIn sw1(p5);

#include "LED_board.h"
#include "Ball.h"
#include "Paddle.h"
#define NUM_ROW 16
#define NUM_COL 24

typedef enum {
  RESET, RUN, PAUSE
} STATES;

LED_board board;
Ticker t;
Ticker row_change;
Ticker t1;
char key_pressed;
Ticker detect_key;

void check(){
    key_pressed = 0;
    if(Num1){
        key_pressed = 1;
    }
    if(Num2){
        key_pressed = 2;
    }
    if(Num3){
        key_pressed = 3;
    }
    if(NumA){
        key_pressed = 4;
    }
}

void drawScreen(Paddle thePaddle1, Paddle thePaddle2, Ball theBall1, Ball theBall2, Ball theBall3, Ball theBall4){
    theBall1.draw(board, true);
    theBall2.draw(board, true);
    theBall3.draw(board, true);
    theBall4.draw(board, true);
    thePaddle1.draw(board, true);
    thePaddle2.draw(board, true);
}

void drawScores(Paddle paddle1, Paddle paddle2){
   pc.printf("Player1 score: %d\r\n", paddle1.getScore());
   pc.printf("Player2 score: %d\r\n", paddle2.getScore());
   pc.printf("Player1 lives: %d\r\n", paddle1.getLives());
   pc.printf("Player2 lives: %d\r\n", paddle2.getLives());
}

void ballCollision(Ball ball, Ball ball2, Ball ball3, Ball ball4){
    if(ball.getyInc() == 0 && ball.getxInc() < 0){ //ball moving to the left
        if((ball.getY() == ball2.getY() && ball.getX() - 1 == ball2.getX()) ||
            (ball.getY() == ball3.getY() && ball.getX() - 1 == ball3.getX()) ||
            (ball.getY() == ball4.getY() && ball.getX() - 1 == ball4.getX())){
            ball.reverseX();
        }
    }else if(ball.getyInc() == 0 && ball.getxInc() > 0){ //ball moving to the right
        if((ball.getY() == ball2.getY() && ball.getY() + 1 == ball2.getX()) ||
            (ball.getY() == ball3.getY() && ball.getX() + 1 == ball3.getX()) ||
            (ball.getY() == ball4.getY() && ball.getX() + 1 == ball4.getX())){
            ball.reverseX();
        }
    }
    
    if((ball.getX() + ball.getxInc() == ball2.getX() && ball.getY() + ball.getyInc() == ball2.getY()) ||
        (ball.getX() + ball.getxInc() == ball3.getX() && ball.getY() + ball.getyInc() == ball3.getY()) ||
        (ball.getX() + ball.getxInc() == ball4.getX() && ball.getY() + ball.getyInc() == ball4.getY())){
        ball.reverseX();
        ball.reverseY();
    }
    return;
}

int main(void){
    
      //setup
      pc.printf("Hello World!\r\n");
      board.sys_rst();
        
      t.attach(&board, &LED_board::refresh_row, .001);
      detect_key.attach(&check, 0.01);

      Paddle paddle1, paddle2;
      Ball theBall1, theBall2, theBall3, theBall4;
      int count1=0;
      int count2=0;
      int count3=0;
      int count4=0;
      int speed1=10;
      int speed2=30;
      int speed3=30;
      int speed4=10;
      int numBalls = 4;
      bool ball1_en = true, ball2_en = true, ball3_en = true, ball4_en = true;
      
      drawScreen(paddle1, paddle2, theBall1, theBall2, theBall3, theBall4);
      drawScores(paddle1, paddle2);
      STATES state = RESET; // Initial state is RESET
     
      while(1) {
        switch(state) {
          case RESET: // Reset objects, draw the screen, state = PAUSE
            pc.printf("in reset\r\n");
            board.sys_rst();
            paddle1 = Paddle(0, 0, 1, 5, paddle1.getLives(),paddle1.getScore());
            paddle2 = Paddle(23, 11, 1, 5, paddle2.getLives(),paddle2.getScore());
            theBall1 = Ball(6, 6, 1, 1, -1, -1);
            theBall2 = Ball(6, 7, 1, 1, -1, 1);
            theBall3 = Ball(7, 6, 1, 1, 1, -1);
            theBall4 = Ball(7, 7, 1, 1, 1, 1);
            drawScreen(paddle1, paddle2, theBall1, theBall2, theBall3, theBall4);
            drawScores(paddle1, paddle2);
            state = PAUSE;
            
            ball1_en = true; ball2_en = true; ball3_en = true; ball4_en = true;
            numBalls = 4;
            
            pc.printf("P1x: %d, P1y: %d\r\n", paddle1.getX(), paddle1.getY());
            pc.printf("P2x: %d, P2y: %d\r\n", paddle2.getX(), paddle2.getY());    
            break;
          case PAUSE: // Set RGB LED to Red, wait for switch input
            pc.printf("in pause\r\n");

            if(!sw1) {
              while(!sw1);
              state = RUN;
            }
            break;
          case RUN: // Set RGB LED to Blue and run program
            pc.printf("in run\r\n");
         
            if(key_pressed) { // Executes if a key is pressed
              switch(key_pressed) { // Use key enumerator to move paddle1
                case 1:
                  if(paddle1.getY()>0) //prevents paddle from moving off screen
                    paddle1.move(board, -1);
                  break;
                case 2: 
                  if(paddle1.getY()+paddle1.getHeight() < NUM_ROW)
                    paddle1.move(board, 1);
                  break;
                case 3:
                  if(paddle2.getY()>0)
                    paddle2.move(board, -1);
                  break;
                case 4: 
                  if(paddle2.getY()+paddle2.getHeight() < NUM_ROW)
                    paddle2.move(board, 1);
                  break;
              }
            }
            if(++count1 == speed1 && ball1_en) { // Only move the ball once every 5 times through the loop
              count1 = 0;
                if(theBall1.hitP1x((paddle1.getX()+paddle1.getWidth()), paddle1.getY(), paddle1.getHeight())){
                    //pc.printf("hit p1\r\n");
                    theBall1.reverseX();
                }
                if(theBall1.hitP2x(paddle2.getX(), paddle2.getY(), paddle2.getHeight())){
                    theBall1.reverseX();
                }
                if(theBall1.hitX()) { // If the ball hits one of the sides of the screen
                    numBalls--;
                    ball1_en = false;
                    theBall1.draw(board,false);
                }
                if(theBall1.hitY()){ //ball hits top or bottom edges
                    theBall1.reverseY();
                }
                ballCollision(theBall1, theBall2, theBall3, theBall4);
                theBall1.move(board);
            }
            if(++count2 == speed2 && ball2_en){
                count2 = 0;
                if(theBall2.hitP1x((paddle1.getX()+paddle1.getWidth()), paddle1.getY(), paddle1.getHeight())){
                    //pc.printf("hit p1\r\n");
                    theBall2.reverseX();
                }
                if(theBall2.hitP2x(paddle2.getX(), paddle2.getY(), paddle2.getHeight())){
                    //pc.printf("hit p2\r\n");
                    //pc.printf("ballx: %d, bally:%d\r\n", theBall.getX(), theBall.getY());
                    //pc.printf("ballx + wid: %d, bally+ wid:%d\r\n", theBall.getX() + theBall.getWidth(), theBall.getY() + theBall.getHeight());
                    theBall2.reverseX();
                }
                if(theBall2.hitX()) { // If the ball hits one of the sides of the screen
                    numBalls--;
                    ball2_en = false;
                    theBall2.draw(board,false);
                }
                if(theBall2.hitY()){ //ball hits top or bottom edges
                    //pc.printf("Hit y\r\n");
                    theBall2.reverseY();
                }
                ballCollision(theBall2, theBall1, theBall3, theBall4);
                theBall2.move(board);
            }
            if(++count3 == speed3 && ball3_en){
                count3 = 0;
                if(theBall3.hitP1x((paddle1.getX()+paddle1.getWidth()), paddle1.getY(), paddle1.getHeight())){
                    theBall3.reverseX();
                }
                if(theBall3.hitP2x(paddle2.getX(), paddle2.getY(), paddle2.getHeight())){
                    theBall3.reverseX();
                }
                if(theBall3.hitX()) { // If the ball hits one of the sides of the screen
                    numBalls--;
                    ball3_en = false;
                    theBall3.draw(board,false);
                }
                if(theBall3.hitY()){ //ball hits top or bottom edges
                    theBall3.reverseY();
                }
                ballCollision(theBall3, theBall1, theBall2, theBall4);
                theBall3.move(board);
            }
            if(++count4 == speed4 && ball4_en){
                count4 = 0;
                if(theBall4.hitP1x((paddle1.getX()+paddle1.getWidth()), paddle1.getY(), paddle1.getHeight())){
                    theBall4.reverseX();
                }
                if(theBall4.hitP2x(paddle2.getX(), paddle2.getY(), paddle2.getHeight())){
                    theBall4.reverseX();
                }
                if(theBall4.hitX()) { // If the ball hits one of the sides of the screen
                    numBalls--;
                    ball4_en = false;
                    theBall4.draw(board,false);
                }
                if(theBall4.hitY()){ //ball hits top or bottom edges
                    theBall4.reverseY();
                }
                ballCollision(theBall4, theBall1, theBall2, theBall3);
                theBall4.move(board);
            }
            if(numBalls == 0){
                state = RESET;
            }
            break;
        }
        drawScreen(paddle1, paddle2, theBall1, theBall2, theBall3, theBall4);
        drawScores(paddle1, paddle2);
      }
}
