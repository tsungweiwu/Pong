// main.c

#include "gba.h"
#include <stdbool.h>

void sync() {
  // only modify VRAM if in V_BUFFER
  // typically we would put logic, physics, ai
  while (REG_DISPLAY_VCOUNT >= 160);
  while (REG_DISPLAY_VCOUNT < 160);
}

// changes color
uint16 makeColor(uint8 r, uint8 g, uint8 b) { //RBG to HEX Function
  return (r & 0x1f) | ((g & 0x1f) << 5) | ((b & 0x1f) << 10);
}

// if value of ball is less than 0, then return 0 or else value is greater
// than max you return the max else the value of ball
uint32 clamp(uint32 value, uint32 min, uint32 max) {
  return (value < min ? min : (value > max ? max : value));
}

// draws out the shape of a rectangle
void drawRect(struct Rect rect, uint16 color) {
  for (int y = 0; y < rect.h; y++) {
    for (int x = 0; x < rect.w; x++) {
      SCREENBUFFER[(rect.y + y) * SCREEN_WIDTH + rect.x + x] = color;
    }
  }
}

void drawSegment(bool statement, struct Rect rect){
    if(statement){
        drawRect(rect, makeColor(255, 255, 255));// uff rgb to hex
    }
}

// number sequence for count down
void init7Segments() {
  // a, b, c, d, e, f, g
  a.w = i.w = 16;
  a.h = i.h = 4;
  f.w = n.w = 4;
  f.h = n.h = 16;

  // i, j, k, l, m, n, o
  b = c = e = f;
  d = g = a;

  j = k = m = n;
  l = o = i;

  // location of the 7 segment display
  a.x = d.x = e.x = f.x = g.x = SCREEN_WIDTH / 5.5;
  b.x = c.x = a.x + a.w - a.h;

  i.x = l.x = m.x = n.x = o.x = SCREEN_WIDTH / 4 * 3;
  j.x = k.x = i.x + i.w - i.h;

  // height of the display
  a.y = b.y = f.y = i.y = j.y = n.y = 0;

  c.y = e.y = g.y = b.y + b.h - a.h;
  d.y = g.y + b.h - a.h;

  k.y = m.y = o.y = j.y + j.h - i.h;
  l.y = o.y + j.h - i.h;
}

// erases or just sets all to white
void clear7Segments() {
  drawRect(a, makeColor(0, 0, 0));
  drawRect(b, makeColor(0, 0, 0));
  drawRect(c, makeColor(0, 0, 0));
  drawRect(d, makeColor(0, 0, 0));
  drawRect(e, makeColor(0, 0, 0));
  drawRect(f, makeColor(0, 0, 0));
  drawRect(g, makeColor(0, 0, 0));

  drawRect(i, makeColor(0, 0, 0));
  drawRect(j, makeColor(0, 0, 0));
  drawRect(k, makeColor(0, 0, 0));
  drawRect(l, makeColor(0, 0, 0));
  drawRect(m, makeColor(0, 0, 0));
  drawRect(n, makeColor(0, 0, 0));
  drawRect(o, makeColor(0, 0, 0));
}

// draws the number count down
void draw7Segments(uint8 num, uint8 num2) {
  clear7Segments();
  // first digit
  bool w, x, y, z;
  // second digit
  bool w2, x2, y2, z2;

  w = (num >= 8 ? ((num -= 8) ? 1 : 1) : 0);
  x = (num >= 4 ? ((num -= 4) ? 1 : 1) : 0);
  y = (num >= 2 ? ((num -= 2) ? 1 : 1) : 0);
  z = (num >= 1 ? ((num -= 1) ? 1 : 1) : 0);

  w2 = (num2 >= 8 ? ((num2 -= 8) ? 1 : 1) : 0);
  x2 = (num2 >= 4 ? ((num2 -= 4) ? 1 : 1) : 0);
  y2 = (num2 >= 2 ? ((num2 -= 2) ? 1 : 1) : 0);
  z2 = (num2 >= 1 ? ((num2 -= 1) ? 1 : 1) : 0);

  // this is where the crazy if statements go
  drawSegment(w || y || (!x && !z) || (x && z), a);
  drawSegment((y && z) || (!y && !z) || !x, b);
  drawSegment(x || !y || z, c);
  drawSegment(w || (y && !z) || (!x && !z) || (!x && y) || (x && !y && z), d);
  drawSegment((y && !z) || (!x && !z), e);
  drawSegment(w || (x && !y) || (x && !z) || (!y && !z), f);
  drawSegment(w || (!y && x) || (y && !z) || (y && !x), g);
  // this is where the crazy if statements go
  drawSegment(w2 || y2 || (!x2 && !z2) || (x2 && z2), i); //first horizontal this
  drawSegment((y2 && z2) || (!y2 && !z2) || !x2, j); // second vertical
  drawSegment(x2 || !y2 || z2, k); //forth vertical
  drawSegment(w2 || (y2 && !z2) || (!x2 && !z2) || (!x2 && y2) || (x2 && !y2 && z2), l); //third horizontal this
  drawSegment((y2 && !z2) || (!x2 && !z2), m); //third vertical
  drawSegment(w2 || (x2 && !y2) || (x2 && !z2) || (!y2 && !z2), n); //first vertical
  drawSegment(w2 || (!y2 && x2) || (y2 && !z2) || (y2 && !x2), o); // second horizontal
}

void initGame() {
  // setting up the position and size of player 1
  player.h = 32;
  player.w = 8;
  player.x = 0;
  player.y = 0;

  // setting up the position and size of player 2
  player2.h = 32;
  player2.w = 8;
  player2.x = SCREEN_WIDTH - player2.w;
  player2.y = 0;

  // setting up the position and size of the ball
  ball.h = 8;
  ball.w = 8;
  prevBall = ball;
}

int main() {
  REG_DISPLAY = VIDEOMODE | BGMODE;
  // initialize the display
  init7Segments();

  // initialize the points
  uint8 val = 0;
  uint8 val2 = 0;

  // setting up temp variables for Ball
  uint32 ballLeft = SCREEN_WIDTH / 2;
  uint32 ballTop = SCREEN_HEIGHT / 2;
  uint32 speedX = 0;
  uint32 speedY = 0;

  uint8 score = -1;

  // setting up values for player
  uint32 top = SCREEN_HEIGHT/3;
  uint32 left = 0;

  // setting up values for player2
  uint32 top2 = SCREEN_HEIGHT/3;
  uint32 left2 = 0;

  // color of the players
  uint16 p_color = makeColor(0, 255, 0);
  // color of the ball
  uint16 b_color = makeColor(0, 0, 255);

  // setting up ball and players
  initGame();

  //Setup Text
  setupSymbols(1);

  ball.x = ballLeft;
  ball.y = ballTop;

  while (1) {
    if (val == 5 | val2 == 5) {
      break;
    }

    sync();

    // starting the ball with whoever moves first
    if (val == 0 && val2 == 0 && (speedX == 0 && speedY == 0)) {
      // start
      if ((!(REG_KEY_INPUT & DOWN)) | (!(REG_KEY_INPUT & UP) && top2 != 0)) {
        speedX = 1;
        speedY = 1;
        score = 0;
      }

      if ((!(REG_KEY_INPUT & B) && top != 0) | (!(REG_KEY_INPUT & A))) {
        speedX = -1;
        speedY = 1;
        score = 1;
      }
    }

    // erase prev ball position
    drawRect(prevBall, makeColor(0, 0, 0));
    // erase prev player position
    drawRect(prevPlayer, makeColor(0, 0, 0));
    // erase prev player2 position
    drawRect(prevPlayer2, makeColor(0, 0, 0));

    // goes bottom right in the beginning (direction of ball)
    ballTop += speedY;
    ballLeft += speedX;

    // ask for input to move paddle
    if (!(REG_KEY_INPUT & A)) // when it is zero then run (or when you call down)
      top++;

    if (!(REG_KEY_INPUT & B) && top != 0) // when it is zero then run (or when you call up)
      top--;

    top = clamp(top, 0, SCREEN_HEIGHT - player.h); // sets a border for the paddle not to go off borders

    // ask for input to move paddle
    if (!(REG_KEY_INPUT & DOWN)) // when it is zero then run (or when you call down)
      top2++;

    if (!(REG_KEY_INPUT & UP) && top2 != 0) // when it is zero then run (or when you call up)
      top2--;

    top2 = clamp(top2, 0, SCREEN_HEIGHT - player2.h); // sets a border for the paddle not to go off borders

    // do more stuff
    // if balltop is less than 0 then it starts at top, if greater than
    // the height of screen then it is the height screen (means ball is at
    // bottom) else it just returns the value or position of ball mid way same
    // thing goes for the ball left, this creates a wall so that the ball can
    // never get out of bounds
    ballTop = clamp(ballTop, 0, SCREEN_HEIGHT - ball.h);
    ballLeft = clamp(ballLeft, 0, SCREEN_WIDTH - ball.w);

    // once the ball hits the top and bottom wall it goes in the opposite
    // direction
    if (ballTop == 0 || ballTop == SCREEN_HEIGHT - ball.h)
      speedY = -speedY;

    // ball touches right wall (left player gets a point)
    if (ballLeft == SCREEN_WIDTH - ball.w) {
      val++;
      // resetting the ball
      ballLeft = SCREEN_WIDTH / 2;
      ballTop = SCREEN_HEIGHT / 2;
      speedX = 0;
      speedY = 0;
      score = 0;
    }
    // ball touches left wall (right player gets a point)
    else if (ballLeft == 0) {
      val2++;
      // resetting the ball
      ballLeft = SCREEN_WIDTH / 2;
      ballTop = SCREEN_HEIGHT / 2;
      speedX = 0;
      speedY = 0;
      score = 1;
    }

    // stops the ball and waits for the loser to move first
    if (speedX == 0 && speedY == 0) {
      if (score == 0) {
        if ((!(REG_KEY_INPUT & DOWN)) | (!(REG_KEY_INPUT & UP) && top2 != 0)) {
          speedX = 1;
          speedY = 1;
        }
      }
      else if (score == 1) {
        if ((!(REG_KEY_INPUT & B) && top != 0) | (!(REG_KEY_INPUT & A))) {
          speedX = -1;
          speedY = 1;
        }
      }
    }

    // starts going in the opposite direction
    ball.x = ballLeft;
    ball.y = ballTop;
    prevBall = ball;

    // basically allows the ball to hit the paddle and bounce back
    // check to see if ball is lined up with paddle
    if (ball.x == player.w) {
      // bottom of ball is higher than top of paddle
      if (ball.y < player.y + player.h && ball.y + ball.h > player.y) // above bottom of paddle
      {
        speedX = -speedX;
      }
    }

    // check to see if ball is lined up with paddle
    if ((ball.x + ball.w) == (SCREEN_WIDTH - player2.w)) {
      // bottom of ball is higher than top of paddle
      if (ball.y < player2.y + player2.h && ball.y + ball.h > player2.y) // above bottom of paddle
      {
        speedX = -speedX;
      }
    }

    // prepare to draw players and ball
    player.y = top;
    prevPlayer = player;

    player2.y = top2;
    prevPlayer2 = player2;

    // draw area
    draw7Segments(val, val2);

    drawRect(player, p_color);
    drawRect(player2, p_color);
    drawRect(ball, b_color);
    //Don't go over certain amount of text LOL it dies runs of memory xd after the letter T;
    drawString("Pong?", 1,1, makeColor(255,255,255));
  }
}
