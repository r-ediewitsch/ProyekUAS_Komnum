# Analisis Profit dalam Model Biaya-Pendapatan Non-Linear

**Nama:** Muhammad Riyan Satrio Wibowo  
**NPM:** 2306229323

---

## Deskripsi Program

Program ini dirancang untuk melakukan analisis profit secara komprehensif pada model bisnis di mana fungsi pendapatan dan biaya bersifat non-linear. Tujuan utamanya adalah untuk menentukan kuantitas produksi yang optimal guna mencapai profit maksimum, serta menghitung total akumulasi profit pada rentang produksi tertentu.

Karena bentuk fungsi yang kompleks, analisis dilakukan menggunakan pendekatan numerik yang diimplementasikan dalam bahasa C.

### Model Matematis

Program ini didasarkan pada model ekonomi berikut:

1.  **Fungsi Pendapatan (Revenue)**  
    Model ini mengasumsikan pendapatan awal yang linear namun akan menurun pada volume produksi tinggi akibat kejenuhan pasar.
    $$ R(x) = ax - bx^3 $$

2.  **Fungsi Biaya (Cost)**  
    Model ini mencakup biaya tetap serta biaya variabel yang meningkat secara kuadratik dan eksponensial, merepresentasikan adanya batasan produksi.
    $$ C(x) = cx^2 + de^{0.01x} + e $$

3.  **Fungsi Profit (Profit)**  
    Merupakan selisih antara pendapatan dan biaya, fungsi ini menjadi target yang dioptimalkan.
    $$ P(x) = R(x) - C(x) $$

### Metode Numerik yang Digunakan

1.  **Optimisasi Profit (Metode Newton-Raphson)**  
    Untuk menemukan kuantitas profit maksimum, program mencari akar dari turunan pertama fungsi profit (`P'(x) = 0`). Metode Newton-Raphson dipilih karena tingkat konvergensinya yang cepat. Persamaannya adalah:
    $$ x_{k+1} = x_k - \frac{P'(x_k)}{P''(x_k)} $$

2.  **Turunan Numerik**  
    Turunan pertama dan kedua dihitung menggunakan metode _centered finite difference_:
    $$ P'(x) \approx \frac{P(x+h) - P(x-h)}{2h} $$
    $$ P''(x) \approx \frac{P(x+h) - 2P(x) + P(x-h)}{h^2} $$

3.  **Integrasi Profit (Simpson's 1/3 Rule)**  
    Total profit diakumulasikan menggunakan Simpson's 1/3 Rule:
    $$ \int_{a}^{b} P(x) dx \approx \frac{h}{3} \left[ P(x_0) + 4\sum_{i=1,3,5}^{n-1} P(x_i) + 2\sum_{i=2,4,6}^{n-2} P(x_i) + P(x_n) \right] $$


### Output Program

Setelah dijalankan, program akan:
1.  **Menampilkan Hasil di Konsol:** Mencetak ringkasan analisis untuk setiap dataset, termasuk kuantitas optimal, profit maksimum, dan total profit.
2.  **Menghasilkan File Data:** Membuat 5 file data bernama `results_1.txt` hingga `results_5.txt`. Setiap file berisi data mentah (kuantitas, pendapatan, biaya, profit, dan turunan profit) serta ringkasan analisis untuk dataset tersebut.
3.  **Menghasilkan Skrip Plot:** Membuat dua skrip Gnuplot untuk visualisasi komparatif:
    * `plot_all_profits.gnu`: Untuk memplot perbandingan fungsi profit dari kelima dataset.
    * `plot_all_derivatives.gnu`: Untuk memplot perbandingan turunan fungsi profit.
