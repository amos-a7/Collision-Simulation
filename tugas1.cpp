#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

bool useQuadtree = false;
int page = 0;

class bola {
public:
    int x, y;
    int dx, dy;
    int r;
    int warna;

    bola() {
        r = 10;

        x = rand() % (getmaxx() - 2 * r) + r;
        y = rand() % (getmaxy() - 2 * r) + r;

        int speedx = rand() % 5 + 2;
        int speedy = rand() % 5 + 2;

        dx = (rand() % 2) ? speedx : -speedx;
        dy = (rand() % 2) ? speedy : -speedy;

        warna = rand() % 15 + 1;
    }

    void draw() {
        setcolor(warna);
        setfillstyle(SOLID_FILL, warna);
        fillellipse(x, y, r, r);
    }

    void update() {
        x += dx;
        y += dy;

        if (x - r <= 0 || x + r >= getmaxx()) dx = -dx;
        if (y - r <= 0 || y + r >= getmaxy()) dy = -dy;
    }

    void cektabrakan(bola& other) {
        int dx_jarakx = x - other.x;
        int dy_jaraky = y - other.y;

        int jarak = dx_jarakx * dx_jarakx + dy_jaraky * dy_jaraky;
        int min_jarak = (r + other.r);

        if (jarak <= min_jarak * min_jarak) {
            int temp_dx = dx;
            int temp_dy = dy;

            dx = other.dx;
            dy = other.dy;

            other.dx = temp_dx;
            other.dy = temp_dy;
        }
    }
};


// QUADTREE
class Quadtree {
public:
    int x, y, w, h;
    int kapasitas;

    bola* object[10];
    int jumlah;
    bool terbagi;

    Quadtree *aki, *ak, *bki, *bk;

    Quadtree(int _x, int _y, int _w, int _h, int kapasit = 4) {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
        kapasitas = kapasit;
        jumlah = 0;
        terbagi = false;

        aki = ak = bki = bk = NULL;
    }

    bool contains(bola* b) {
        return (b->x >= x && b->x <= x + w &&
                b->y >= y && b->y <= y + h);
    }

    void subdivide() {
        aki = new Quadtree(x,        y,        w/2, h/2, kapasitas);
        ak  = new Quadtree(x+w/2,    y,        w/2, h/2, kapasitas);
        bki = new Quadtree(x,        y+h/2,    w/2, h/2, kapasitas);
        bk  = new Quadtree(x+w/2,    y+h/2,    w/2, h/2, kapasitas);
        terbagi = true;
    }

    void insert(bola* b) {
        if (!contains(b)) return;

        if (jumlah < kapasitas) {
            object[jumlah] = b;
            jumlah++;
        } else {
            if (!terbagi) subdivide();

            aki->insert(b);
            ak->insert(b);
            bki->insert(b);
            bk->insert(b);
        }
    }
};


// QUERY QUADTREE
void query(Quadtree* qt, bola* target, bola* hasil[], int &jumlah) {
    if (!qt->contains(target)) return;

    for (int i = 0; i < qt->jumlah; i++) {
        hasil[jumlah] = qt->object[i];
        jumlah++;               
    }

    if (qt->terbagi) {
        query(qt->aki, target, hasil, jumlah);
        query(qt->ak,  target, hasil, jumlah);
        query(qt->bki, target, hasil, jumlah);
        query(qt->bk,  target, hasil, jumlah);
    }
}


//   BRUTE FORCE COLLISION
void bruteForceCollision(bola balls[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            balls[i].cektabrakan(balls[j]);
        }
    }
}


//   QUADTREE COLLISION

void quadtreecolission(bola balls[], int n) {
    Quadtree akar(0, 0, getmaxx(), getmaxy());

    for (int i = 0; i < n; i++) {
        akar.insert(&balls[i]);
    }

    for (int i = 0; i < n; i++) {
        bola* target[50];
        int jumlah = 0;

        query(&akar, &balls[i], target, jumlah);

        for (int k = 0; k < jumlah; k++) {
            if (target[k] != &balls[i]) {
                balls[i].cektabrakan(*target[k]);
            }
        }
    }
}


int main() {
    initwindow(800, 600);
    srand(time(NULL));

    const int JUMLAH_BOLA = 100;
    bola balls[JUMLAH_BOLA];

    while (true) {
        setactivepage(page);
        setvisualpage(1 - page);
        cleardevice();

        // INPUT MODE
        if (kbhit()) {
            char c = getch();
            if (c == 'b') useQuadtree = false;
            else if (c == 'q') useQuadtree = true;
        }

        // TAMPILKAN MODE SETIAP FRAME
        setcolor(WHITE);
        if (useQuadtree)
            outtextxy(10, 10, (char*)"MODE: QUADTREE");
        else
            outtextxy(10, 10, (char*)"MODE: BRUTE FORCE");

        // UPDATE BOLA
        for (int i = 0; i < JUMLAH_BOLA; i++) { 
            balls[i].update();}

        // COLLISION
        if (!useQuadtree)
            bruteForceCollision(balls, JUMLAH_BOLA);
        else
            quadtreecolission(balls, JUMLAH_BOLA);

        // DRAW BOLA
        for (int i = 0; i < JUMLAH_BOLA; i++) balls[i].draw();

        page = 1 - page;
        delay(10);
    }

    return 0;
}
