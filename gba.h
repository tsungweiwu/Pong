//gba.h
#include <stdbool.h>
#include <string.h>

#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   160

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

#define MEM_IO          0x04000000
#define VRAM            0x06000000

#define VIDEOMODE       0x0003
#define BGMODE          0x0400

#define REG_DISPLAY         (*(volatile uint32 *) (MEM_IO))
#define REG_DISPLAY_VCOUNT  (*(volatile uint32 *) (MEM_IO + 0x0006))
#define REG_KEY_INPUT       (*(volatile uint32 *) (MEM_IO + 0x0130))

#define A       1
#define B       2
#define SELECT  4
#define START   8
#define RIGHT   16
#define LEFT    32
#define UP      64
#define DOWN    128
#define R       256
#define L       512

#define SCREENBUFFER    ((volatile uint16 *) VRAM)

struct Rect {
    uint32 x, y;
    uint32 w, h;
} a,b,c,d,e,f,g,i,j,k,l,m,n,o,
  player, player2, ball, prevBall, prevPlayer, prevPlayer2;
//my 7 segments




//Text
struct Square{
    struct Rect rect;
    bool enable;
};

struct Symbol{
    uint32 size;
    struct Square pixel[20];
}_A,_B,_C,_D,_E,_F,_G,_H,_I,_J,_K,_L,_M,_N,_O,_P,_Q,_R,_S,_T,_U,_V,_W,_X,_Y,_Z,_EXCLAMATION,_INTERROGATIVE;

struct Symbol initSymbol(uint32 size){
    struct Symbol symbol;
    int count = 0;
    int y = 0;
    for(int i = 0; i < 5; i++){
        int x = 0;
        for (int j = 0; j < 4; ++j) {
            symbol.pixel[count].rect.x = x;
            symbol.pixel[count].rect.y = y;
            symbol.pixel[count].rect.w = size;
            symbol.pixel[count].rect.h = size;
            symbol.pixel[count].enable = 0;
            x += size;
            count++;
        }
        y += size;
    }
    return symbol;
}

struct Symbol enablePixels(int pixels[20], uint32 size){
    struct Symbol symbol;
    symbol = initSymbol(size);
    for (int i = 0; i < 20; ++i) {
        symbol.pixel[pixels[i]-1].enable = 1;
    }
    return symbol;
}

void setupSymbols(uint32 size){
    //Letter A
    int enabledPixelsA[] ={2,3,5,8,9,10,11,12,13,16,17,20};
    _A = enablePixels(enabledPixelsA, size);
    //Letter B
    int enabledPixelsB[] ={1,2,3,5,8,9,10,11,13,16,17,18,19};
    _B = enablePixels(enabledPixelsB, size);
    //Letter C
    int enabledPixelsC[] ={2,3,4,5,9,13,18,19,20};
    _C = enablePixels(enabledPixelsC, size);
    //Letter D
    int enabledPixelsD[] ={1,2,3,5,8,9,12,13,16,17,18,19};
    _D = enablePixels(enabledPixelsD, size);
    //Letter E
    int enabledPixelsE[] ={1,2,3,4,5,9,10,11,13,17,18,19,20};
    _E = enablePixels(enabledPixelsE, size);
    //Letter F
    int enabledPixelsF[] ={1,2,3,4,5,9,10,11,13,17};
    _F = enablePixels(enabledPixelsF, size);
    //Letter G
    int enabledPixelsG[] ={2,3,4,5,9,11,12,13,16,18,19,20};
    _G = enablePixels(enabledPixelsG, size);
    //Letter H
    int enabledPixelsH[] ={1,4,5,8,9,10,11,12,13,16,17,20};
    _H = enablePixels(enabledPixelsH, size);
    //Letter I
    int enabledPixelsI[] ={2,6,10,14,18};
    _I = enablePixels(enabledPixelsI, size);
    //Letter J
    int enabledPixelsJ[] ={4,8,12,13,16,18,19};
    _J = enablePixels(enabledPixelsJ, size);
    //Letter K
    int enabledPixelsK[] ={1,4,5,7,9,10,13,15,17,20};
    _K = enablePixels(enabledPixelsK, size);
    //Letter L
    int enabledPixelsL[] ={1,5,9,13,17,18,19};
    _L = enablePixels(enabledPixelsL, size);
    //Letter M
    int enabledPixelsM[] ={1,4,5,6,7,8,9,10,11,12,13,16,17,20};//Looks Ugly lol
    _M = enablePixels(enabledPixelsM, size);
    //Letter N
    int enabledPixelsN[] ={1,4,5,6,8,9,11,12,13,16,17,20};
    _N = enablePixels(enabledPixelsN, size);
    //Letter O
    int enabledPixelsO[] ={2,3,5,8,9,12,13,16,18,19};
    _O = enablePixels(enabledPixelsO, size);
    //Letter P
    int enabledPixelsP[] ={1,2,3,5,8,9,10,11,13,17};
    _P = enablePixels(enabledPixelsP, size);
    //Letter Q
    int enabledPixelsQ[] ={2,3,5,8,9,12,13,15,16,18,19,20};
    _Q = enablePixels(enabledPixelsQ, size);
    //Letter R
    int enabledPixelsR[] ={1,2,3,5,8,9,10,11,13,15,17,20};
    _R = enablePixels(enabledPixelsR, size);
    //Letter S
    int enabledPixelsS[] ={2,3,4,5,10,11,16,17,18,19};
    _S = enablePixels(enabledPixelsS, size);
    //Letter T
    int enabledPixelsT[] ={1,2,3,6,10,14,18};
    _T = enablePixels(enabledPixelsT, size);//out of memory? xd maybe After T renders weird
    //Letter U
    int enabledPixelsU[] ={1,4,5,8,9,12,13,16,17,18,19,20};
    _U = enablePixels(enabledPixelsU, size);
    //Letter V
    int enabledPixelsV[] ={1,3,5,7,9,11,13,15,18};//Surprisingly this works and W as well xd I can't math?
    _V = enablePixels(enabledPixelsV, size);
    //Letter W
    int enabledPixelsW[] ={1,4,5,8,9,10,11,12,13,14,15,16,17,20};// also potato v:
    _W = enablePixels(enabledPixelsW, size);
    //Letter X
    int enabledPixelsX[] ={1,3,5,7,10,13,15,17,19};
    _X = enablePixels(enabledPixelsX, size);
    //Letter Y
    int enabledPixelsY[] ={1,3,5,7,9,10,11,14,18};
    _Y = enablePixels(enabledPixelsY, size);
    //Letter Z
    int enabledPixelsZ[] ={1,2,3,4,8,11,14,17,18,19,20};
    _Z = enablePixels(enabledPixelsZ, size);
    // !
    int enabledPixelsExclamation[] ={2,6,10,18};
    _EXCLAMATION = enablePixels(enabledPixelsExclamation, size);
    // ?
    int enabledPixelsInterrogative[] ={2,3,8,10,11,18};
    _INTERROGATIVE = enablePixels(enabledPixelsInterrogative, size);

}

void drawSymbol(struct Symbol symbol, uint32 x, uint32 y, uint16 color){
    for(int i = 0; i < 20; i++){
        for(int h = 0; h < symbol.pixel[i].rect.h; h++){
            for(int w = 0; w < symbol.pixel[i].rect.w; w++){
                if(symbol.pixel[i].enable){
                    SCREENBUFFER[(symbol.pixel[i].rect.y + h + y) * SCREEN_WIDTH + symbol.pixel[i].rect.x + w + x] = color;
                }
            }
        }
    }
}

bool chrcmp(char currentValue, char compareTo){
    return currentValue == compareTo || currentValue == compareTo+32;
}

void drawString(char *text, int x, int y, uint16 color){
    int size = 5;//This is hardcoded
    for(int i = 0; i < strlen(text); i++){
        if(chrcmp(text[i], 'A')){
            drawSymbol(_A, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'B')){
            drawSymbol(_B, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'C')){
            drawSymbol(_C, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'D')){
            drawSymbol(_D, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'E')){
            drawSymbol(_E, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'F')){
            drawSymbol(_F, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'G')){
            drawSymbol(_G, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'H')){
            drawSymbol(_H, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'I')){
            drawSymbol(_I, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'J')){
            drawSymbol(_J, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'K')){
            drawSymbol(_K, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'L')){
            drawSymbol(_L, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'M')){
            drawSymbol(_M, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'N')){
            drawSymbol(_N, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'O')){
            drawSymbol(_O, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'P')){
            drawSymbol(_P, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'Q')){
            drawSymbol(_Q, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'R')){
            drawSymbol(_R, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'S')){
            drawSymbol(_S, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'T')){
            drawSymbol(_T, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'U')){
            drawSymbol(_U, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'V')){
            drawSymbol(_V, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'W')){
            drawSymbol(_W, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'X')){
            drawSymbol(_X, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'Y')){
            drawSymbol(_Y, x + (size*i), y, color);
        }else if(chrcmp(text[i], 'Z')){
            drawSymbol(_Z, x + (size*i), y, color);
        }else if(chrcmp(text[i], '?')){
            drawSymbol(_INTERROGATIVE, x + (size*i), y, color);
        }else if(chrcmp(text[i], '!')){
            drawSymbol(_EXCLAMATION, x + (size*i), y, color);
        }
    }
}








