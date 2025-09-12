#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

class Ball {
private:
    int x, y;        
    int dx, dy;      
    int radius;      
    int color;       

public:
    Ball() {
        radius = 10;
        x = rand() % (getmaxx() - 2 * radius) + radius;
        y = rand() % (getmaxy() - 2 * radius) + radius;

        int speedX = rand() % 5 + 2;  
        int speedY = rand() % 5 + 2; 

        // tentukan arah dx
        if (rand() % 2 == 0) {
            dx = speedX;
        } else {
            dx = -speedX;
        }

       
        if (rand() % 2 == 0) {
            dy = speedY;
        } else {
            dy = -speedY;
        }

        color = rand() % 15 + 1;
    }

    void draw() {
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        fillellipse(x, y, radius, radius);
    }

    void update() {
        x += dx;
        y += dy;

      
        if (x - radius <= 0 || x + radius >= getmaxx()) {
            dx *= -1;
        }
        if (y - radius <= 0 || y + radius >= getmaxy()) {
            dy *= -1;
        }
    }
};


int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    srand(time(NULL));

    const int BALL_COUNT = 30;
    Ball balls[BALL_COUNT]; 

    int page = 0; 

    while (!kbhit()) {
        setactivepage(page);   
        setvisualpage(1 - page); 
        cleardevice();

        for (int i = 0; i < BALL_COUNT; i++) {
            balls[i].draw();
            balls[i].update();
        }

        delay(20);
        page = 1 - page;
    }

    closegraph();
    return 0;
}
