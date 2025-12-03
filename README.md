# Simulasi Deteksi Tabrakan: Brute Force vs Quadtree

Proyek ini adalah simulasi grafis menggunakan C++ (Graphics.h) untuk mendemonstrasikan dan membandingkan kinerja dua algoritma deteksi tabrakan (*collision detection*) pada objek bola yang bergerak: **Brute Force** dan **Quadtree**.

## 📋 Daftar Isi
- [Gambaran Umum](#gambaran-umum)
- [Algoritma yang Digunakan](#algoritma-yang-digunakan)
  - [Brute Force](#1-brute-force)
  - [Quadtree](#2-quadtree)
- [Struktur Data dalam Kode](#struktur-data-dalam-kode)
- [Implementasi Kode](#implementasi-kode)
- [Cara Kerja Program](#cara-kerja-program)
- [Kontrol](#kontrol)

---

## 🔍 Gambaran Umum
Program ini merender sejumlah besar partikel (bola) yang memantul di layar. Tantangan utama dalam simulasi ini adalah mengecek apakah satu bola bertabrakan dengan bola lainnya. Dengan jumlah bola yang banyak (misal: 5000), pengecekan ini menjadi sangat berat jika tidak dioptimalkan.

---

## 🧠 Algoritma yang Digunakan

### 1. Brute Force
Metode ini adalah pendekatan paling sederhana dan naif.
- **Konsep:** Setiap bola akan dicek terhadap **SEMUA** bola lain yang ada di layar untuk melihat apakah mereka bersentuhan.
- **Kompleksitas:** $O(N^2)$. Jika ada 5.000 bola, maka akan terjadi sekitar 25.000.000 pengecekan per frame. Ini sangat lambat dan menyebabkan *lag* jika jumlah objek banyak.

### 2. Quadtree
Metode ini menggunakan teknik *Spatial Partitioning* (Partisi Ruang).
- **Konsep:** Layar dibagi menjadi 4 kuadran (kotak). Jika sebuah kuadran terlalu penuh (melebihi kapasitas), kuadran tersebut dibagi lagi menjadi 4 sub-kuadran, dan seterusnya.
- **Keuntungan:** Kita hanya perlu mengecek tabrakan antar bola yang berada di dalam kuadran (kotak) yang sama. Bola di pojok kiri atas tidak perlu dicek dengan bola di pojok kanan bawah.
- **Kompleksitas:** Rata-rata mendekati $O(N \log N)$. Jauh lebih efisien dibanding Brute Force.

---

## 🏗️ Struktur Data dalam Kode

Berdasarkan kode C++ yang dibuat, berikut adalah struktur data intinya:

### A. Class `bola`
Ini adalah objek entitas dasar.
* **Properti:** `x`, `y` (posisi), `dx`, `dy` (kecepatan), `r` (jari-jari), `warna`.
* **Fungsi:** `update()` untuk gerak, `cektabrakan()` untuk menghitung jarak Euclidean antar dua bola.

### B. Class `Quadtree`
Ini adalah struktur data pohon (Tree) untuk membagi area.
* **Boundary (`x, y, w, h`):** Menentukan batas wilayah kotak persegi panjang node ini.
* **`kapasitas`:** Jumlah maksimal bola yang boleh ada dalam satu node sebelum node itu membelah diri (di kode ini `kapasitas = 4`).
* **`bola* object[10]`:** Array pointer untuk menyimpan data bola yang ada di wilayah ini.
* **Child Pointers:** Pointer ke 4 anak sub-node:
    * `aki` (Atas Kiri / North West)
    * `ak` (Atas Kanan / North East)
    * `bki` (Bawah Kiri / South West)
    * `bk` (Bawah Kanan / South East)

---

## 💻 Implementasi Kode

Berikut adalah potongan kode (Screenshot logic) untuk masing-masing algoritma dalam file `main.cpp`.

### Bagian Brute Force
Bagian ini melakukan *nested loop* (perulangan bersarang).

```cpp
// 🟢 FUNGSI BRUTE FORCE
void bruteForceCollision(bola balls[], int n) {
    for (int i = 0; i < n; i++) {
        // Cek terhadap semua bola lain (j)
        for (int j = i + 1; j < n; j++) {
            balls[i].cektabrakan(balls[j]);
        }
    }
}
// 🌳 FUNGSI INSERT (Membangun Tree)
void insert(bola* b) {
    if (!contains(b)) return; // Jika bola tidak ada di area ini, abaikan

    if (jumlah < kapasitas) {
        object[jumlah] = b; // Masukkan bola jika masih muat
        jumlah++;
    } else {
        if (!terbagi) subdivide(); // Belah area jadi 4 jika penuh

        // Masukkan bola ke salah satu anak (sub-kuadran)
        aki->insert(b); ak->insert(b);
        bki->insert(b); bk->insert(b);
    }
}

// 🌳 FUNGSI UTAMA QUADTREE COLLISION
void quadtreecolission(bola balls[], int n) {
    // 1. Buat Root Node (Akar) seukuran layar
    Quadtree akar(0, 0, getmaxx(), getmaxy());

    // 2. Masukkan semua bola ke dalam Tree
    for (int i = 0; i < n; i++) {
        akar.insert(&balls[i]);
    }

    // 3. Cek tabrakan HANYA dengan tetangga terdekat
    for (int i = 0; i < n; i++) {
        bola* target[50]; // Penampung kandidat tabrakan
        int jumlah = 0;

        // Ambil bola-bola yang satu kotak dengan bola[i]
        query(&akar, &balls[i], target, jumlah);

        // Cek tabrakan hanya dengan hasil query
        for (int k = 0; k < jumlah; k++) {
            if (target[k] != &balls[i]) {
                balls[i].cektabrakan(*target[k]);
            }
        }
    }
}
