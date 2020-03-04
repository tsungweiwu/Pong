//main.c

#include "gba.h"
#include <stdbool.h>

void sync()
{
    //only modify VRAM if in V_BUFFER
    //typically we would put logic, physics, ai
    while(REG_DISPLAY_VCOUNT >= 160);
    while(REG_DISPLAY_VCOUNT < 160);
}

// changes color
uint16 makeColor(uint8 r, uint8 g, uint8 b)
{
    return (r & 0x1f) | ((g & 0x1f) << 5) | ((b & 0x1f) << 10); 
}

// if value of ball is less than 0, then return 0 or else value is greater
// than max you return the max else the value of ball
uint32 clamp(uint32 value, uint32 min, uint32 max)
{
    return (value < min ? min : (value > max ? max : value));
}

// draws out the shape of a rectangle
void drawRect(struct Rect rect, uint16 color)
{
    for(int y = 0; y < rect.h; y++)
    {
        for(int x = 0; x < rect.w; x++)
        {
            SCREENBUFFER[(rect.y + y) * SCREEN_WIDTH + rect.x + x] = color;
        }
    }
}

// number sequence for count down
void init7seg()
{
    a.w = 16;
    a.h = 4;
    f.w = 4;
    f.h = 16;
    
    b = c = e = f;
    d = g = a;
    
    a.x = d.x = e.x = f.x = g.x = SCREEN_WIDTH/4;
    b.x = c.x = a.x + a.w;
    
    a.y = b.y = f.y = 0;
    c.y = e.y = g.y = b.y + b.h - a.h;
    d.y = g.y + b.h - a.h;
}

// erases or just sets all to white
void clear7seg()
{
    drawRect(a, makeColor(0,0,0));
    drawRect(b, makeColor(0,0,0));
    drawRect(c, makeColor(0,0,0));
    drawRect(d, makeColor(0,0,0));
    drawRect(e, makeColor(0,0,0));
    drawRect(f, makeColor(0,0,0));
    drawRect(g, makeColor(0,0,0));
}

// draws the number count down
void draw7seg(uint8 num)
{
    clear7seg();
    bool w,x,y,z;
        
    w = (num >= 8 ? ((num-=8) ? 1 : 1) : 0);
    x = (num >= 4 ? ((num-=4) ? 1 : 1) : 0);
    y = (num >= 2 ? ((num-=2) ? 1 : 1) : 0);
    z = (num >= 1 ? ((num-=1) ? 1 : 1) : 0);
    
    //this is where the crazy if statements go
    if(w || y || (!x && !z) || (x && z))
        drawRect(a, makeColor(0x1f, 0x1f, 0x1f));
    if((y && z) || (!y && !z) || !x)
        drawRect(b, makeColor(0x1f, 0x1f, 0x1f));
    if(x || !y || z)
        drawRect(c, makeColor(0x1f, 0x1f, 0x1f));
    if(w || (y && !z) || (!x && !z) || (!x && y) || (x && !y && z))
        drawRect(d, makeColor(0x1f, 0x1f, 0x1f));
    if((y && !z) || (!x && !z))
        drawRect(e, makeColor(0x1f, 0x1f, 0x1f));
    if(w || (x && !y) || (x && !z) ||(!y && !z))
        drawRect(f, makeColor(0x1f, 0x1f, 0x1f));
    if(w || (!y && x) || (y && !z) || (y && !x))
        drawRect(g, makeColor(0x1f, 0x1f, 0x1f));
}

int main()
{
    REG_DISPLAY = VIDEOMODE | BGMODE;
    //init a for testing
    init7seg();
    uint8 val = 0;
    uint8 counter = 0;
    
    //setting up temp variables for Ball
    uint32 ballLeft = 120;
    uint32 ballTop = 80;
    uint32 speedX = 1;
    uint32 speedY = 1;

    // setting up values for player
    uint32 top = 0;
    uint32 left = 0;
    
    //player is 32 pixel height, 8 pixels wide
    player.h = 32;
    player.w = 8;
    player.x = 0;
    player.y = 0;
    uint16 p_color = makeColor(0, 0x1f, 0);
    
    ball.h = 8;
    ball.w = 8;
    ball.x = ballLeft;
    ball.y = ballTop;
    prevBall = ball;
    uint16 b_color = makeColor(0, 0, 0x1f);
    
    while(1)
    {
        sync();
        
        //erase prev ball position
        drawRect(prevBall, makeColor(0,0,0));
        //erase prev player position
        drawRect(prevPlayer, makeColor(0,0,0));

        // goes bottom right in the beginning (direction of ball)
        ballTop += speedY;
        ballLeft += speedX;

        // ask for input to move paddle
        if(!(REG_KEY_INPUT & DOWN)) // when it is zero then run (or when you call down)
			top++;

		if(!(REG_KEY_INPUT & UP) && top != 0) // when it is zero then run (or when you call up)
			top--;

		top = clamp(top, 0, SCREEN_HEIGHT - player.h); // sets a border for the paddle not to go off borders
        
        //do more stuff
        // if balltop is less than 0 then it starts at top, if greater than
        // the height of screen then it is the height screen (means ball is at bottom)
        // else it just returns the value or position of ball mid way
        // same thing goes for the ball left, this creates a wall so that the ball can never get out of bounds
        ballTop = clamp(ballTop, 0, SCREEN_HEIGHT - ball.h);
        ballLeft = clamp(ballLeft, 0, SCREEN_WIDTH - ball.w);

        // once the ball hits the wall it goes in the opposite direction
        if(ballTop == 0 || ballTop == SCREEN_HEIGHT - ball.h)
            speedY = -speedY;
        if(ballLeft == 0 || ballLeft == SCREEN_WIDTH - ball.w)
            speedX = -speedX;

        // starts going in the opposite direction
        ball.x = ballLeft;
        ball.y = ballTop;
        prevBall = ball;


        // check to see if ball is lined up with paddle
        if(ball.x == player.w)
        {
        	// bottom of ball is higher than top of paddle
        	if(ball.y < player.y + player.h && ball.y + ball.h > player.y) // above bottom of paddle
        	{
        		speedX = -speedX;
        	}
        }

        player.y = top;
        prevPlayer = player;
        
        //draw area
        draw7seg(val);
        drawRect(player, p_color);
        drawRect(ball, b_color);
    }
}















