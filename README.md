ese350
======

ESE350 Final Project Code

A C Program for running the game of Pong on a 16x32 LED panel pieced together by 6 ordinary 8x8 LED panels.
Ball.cpp and Ball.h outline the behavior of the ball while Paddle.cpp and Paddle.h define that of the two paddles.
LED_board.cpp and LED_board.h essentially define an API for the custom 16x24 LED panel, allowing elementary operations to be executed through simple method calls.
vission.cpp and vission.h parse the data received from the Wii-mote infrared light sensor, which we utilized in our project in order to allow the paddles to be controlled by the movement of two infrared LED lights.
Lastly, main.cpp pieces the entire game together.
