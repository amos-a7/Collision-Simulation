#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 

class Ball {
private:
    int x, y;      
    int dx, dy;     
    int radius;     
    int color;     

public:

    Ball() {
        radius = 10;
        // Posisi awal acak, memastikan bola tidak keluar batas
        x = rand() % (getmaxx() - 2 * radius) + radius;
        y = rand() % (getmaxy() - 2 * radius) + radius;

        int speedX = rand() % 5 + 2;
        int speedY = rand() % 5 + 2;

        // Tentukan arah acak
        dx = (rand() % 2 == 0) ? speedX : -speedX;
        dy = (rand() % 2 == 0) ? speedY : -speedY;

        color = rand() % 15 + 1;
    }

   
    void draw() {
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        // Menggunakan fillellipse (fillellipse(x, y, xradius, yradius))
        fillellipse(x, y, radius, radius); 
    }

    // Metode untuk memperbarui posisi dan menangani tabrakan dinding
    void update() {
        x += dx;
        y += dy;

        // Tabrakan dengan sumbu X (kiri dan kanan)
        if (x - radius <= 0 || x + radius >= getmaxx()) {
            dx *= -1; 
            // ngecek bola tidak terjebak di dinding
            if (x - radius < 0) x = radius;
            if (x + radius > getmaxx()) x = getmaxx() - radius;
        }

        // Tabrakan dengan sumbu Y (atas dan bawah)
        if (y - radius <= 0 || y + radius >= getmaxy()) {
            dy *= -1; 
            // ngecek bola tidak terjebak di dinding
            if (y - radius < 0) y = radius;
            if (y + radius > getmaxy()) y = getmaxy() - radius;
        }
    }

    // ALGORITMA BRUTE FORCE (Respons Tabrakan Antar Bola) 
    void resolveCollision(Ball& other) {
        // Hitung perbedaan posisi
        int dx_dist = x - other.x;
        int dy_dist = y - other.y;
        
        //Hitung jarak kuadrat antar pusat bola
        long long dist_sq = (long long)dx_dist * dx_dist + (long long)dy_dist * dy_dist;
        
        // Jarak minimum agar tidak bertabrakan
        int min_dist = radius + other.radius;

        // Cek tabrakan
        if (dist_sq <= (long long)min_dist * min_dist) {
            
            // 1. Tanggapan (Respons) Tabrakan Sederhana: Tukar Kecepatan
            int temp_dx = dx;
            int temp_dy = dy;

            dx = other.dx;
            dy = other.dy;

            other.dx = temp_dx;
            other.dy = temp_dy;

            // 2. Pemisahan Posisi (Mencegah Bola Terjebak)
            // Hitung seberapa jauh bola berjarak (overlap)
            float dist = sqrt((float)dist_sq);
            float overlap = (float)min_dist - dist; 

            // Vektor normalisasi arah pemisahan
            float nx = (float)dx_dist / dist;
            float ny = (float)dy_dist / dist;
            
            // Pindahkan kedua bola menjauh satu sama lain (setengah dari overlap)
            x += (int)(nx * overlap * 0.5f);
            y += (int)(ny * overlap * 0.5f);
            other.x -= (int)(nx * overlap * 0.5f);
            other.y -= (int)(ny * overlap * 0.5f);
        }
    }
    
    // Diperlukan untuk menghindari error saat membandingkan jarak dengan sqrt
    friend float getDistance(const Ball& b1, const Ball& b2);
};


int main() {
    int gd = DETECT, gm;
    
    initgraph(&gd, &gm, ""); 

    srand(time(NULL));

    const int BALL_COUNT = 30;
    Ball balls[BALL_COUNT];

    // Double Buffering: Menggunakan dua halaman memori (page 0 dan page 1)
    
    int page = 0;

    // Loop utama
    while (!kbhit()) {
        // Mengatur halaman aktif dan halaman visual
        setactivepage(page);
        setvisualpage(1 - page);
        cleardevice(); // Bersihkan halaman aktif

        // ALGORITMA BRUTE FORCE (Deteksi Tabrakan Antar Bola) 
        // Cek setiap pasangan bola (i, j)
        for (int i = 0; i < BALL_COUNT; i++) {
            for (int j = i + 1; j < BALL_COUNT; j++) {
                
                balls[i].resolveCollision(balls[j]); 
            }
        }
        
        // Update dan gambar semua bola
        for (int i = 0; i < BALL_COUNT; i++) {
            balls[i].draw();
            balls[i].update(); // update menangani tabrakan dinding
        }

        delay(20); 
        page = 1 - page; 
    }

    closegraph();
    return 0;
}
