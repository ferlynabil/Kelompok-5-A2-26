#include <iostream>
#include <iomanip>
#include <conio.h>
#include <vector>
#include <string>
#include <cctype>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

void lihatRute(const vector<Rute>& daftar_rute) {
    if (daftar_rute.empty()) {
        system("cls");
        cout << "\n=== DAFTAR RUTE PENGIRIMAN (DARI SAMARINDA) ===\n";
        cout << "Belum ada rute yang terdaftar.\n";
        
        // Tombol Kembali ala Inquirer
        cout << "\n  > \033[1;32mKembali\033[0m <\n";
        while (true) {
            char tombol = _getch();
            if (tombol == '\r') break; 
        }
        return;
    }

    // Pilihan Menu Sorting
    vector<string> menu_sort = {
        "Tanpa Pengurutan (Sesuai Input)", 
        "Kota Tujuan (A - Z)", 
        "Jarak (Terdekat - Terjauh)", 
        "Jarak (Terjauh - Terdekat)", 
        "Kembali"
    };
    
    int posisi_sort = 0;
    bool memilih_sort = true;
    vector<Rute> data_tampil = daftar_rute; // Copy data untuk disortir

    // Menampilkan Menu Sorting
    while (memilih_sort) {
        system("cls");
        cout << "\n=== OPSI TAMPILAN DAFTAR RUTE ===\n";
        cout << "Pilih metode pengurutan (Gunakan Panah Atas/Bawah & Enter):\n\n";

        for (int i = 0; i < menu_sort.size(); i++) {
            if (i == posisi_sort) {
                cout << "  > \033[1;32m" << menu_sort[i] << "\033[0m <\n";
            } else {
                cout << "    " << menu_sort[i] << "\n";
            }
        }

        char tombol = _getch();

        if (tombol == 72 && posisi_sort > 0) { // Panah Atas
            posisi_sort--;
        } else if (tombol == 80 && posisi_sort < menu_sort.size() - 1) { // Panah Bawah
            posisi_sort++;
        } else if (tombol == '\r') { // Enter
            memilih_sort = false;
        }
    }

    if (posisi_sort == 4) {
        return; // Batal & Kembali ke menu
    }

    // ---------------------------------------------
    // LOGIKA PENGURUTAN (INSERTION SORT)
    // ---------------------------------------------
    if (posisi_sort == 1) {
        // Sortir Kota Tujuan (A - Z)
        for (int i = 1; i < data_tampil.size(); i++) {
            Rute kunci = data_tampil[i];
            int j = i - 1;
            
            string kunci_nama = kunci.tujuan;
            for (char& c : kunci_nama) c = tolower(c); // Ubah ke huruf kecil untuk perbandingan
            
            while (j >= 0) {
                string banding_nama = data_tampil[j].tujuan;
                for (char& c : banding_nama) c = tolower(c);
                
                if (banding_nama > kunci_nama) {
                    data_tampil[j + 1] = data_tampil[j];
                    j--;
                } else {
                    break;
                }
            }
            data_tampil[j + 1] = kunci;
        }
    } 
    else if (posisi_sort == 2) {
        // Sortir Jarak (Ascending / Terdekat)
        for (int i = 1; i < data_tampil.size(); i++) {
            Rute kunci = data_tampil[i];
            int j = i - 1;
            
            while (j >= 0 && data_tampil[j].jarak > kunci.jarak) {
                data_tampil[j + 1] = data_tampil[j];
                j--;
            }
            data_tampil[j + 1] = kunci;
        }
    } 
    else if (posisi_sort == 3) {
        // Sortir Jarak (Descending / Terjauh)
        for (int i = 1; i < data_tampil.size(); i++) {
            Rute kunci = data_tampil[i];
            int j = i - 1;
            
            while (j >= 0 && data_tampil[j].jarak < kunci.jarak) {
                data_tampil[j + 1] = data_tampil[j];
                j--;
            }
            data_tampil[j + 1] = kunci;
        }
    }

    // ---------------------------------------------
    // MENAMPILKAN TABEL HASIL
    // ---------------------------------------------
    system("cls");
    cout << "\n=== DAFTAR RUTE PENGIRIMAN (DARI SAMARINDA) ===\n";
    cout << "Metode: " << menu_sort[posisi_sort] << "\n";
    cout << "----------------------------------------------------------------------------------------\n";
    cout << setw(10) << left << "ID" 
         << setw(20) << "Kota Tujuan" 
         << setw(10) << "Jarak" 
         << setw(15) << "Reguler (Rp)"
         << setw(15) << "Standar (Rp)"
         << setw(15) << "Premium (Rp)\n";
    cout << "----------------------------------------------------------------------------------------\n";
    
    for (const auto& r : data_tampil) {
        cout << setw(10) << left << r.id_rute 
             << setw(20) << r.tujuan 
             << setw(10) << r.jarak 
             << setw(15) << fixed << setprecision(0) << r.biaya_reguler
             << setw(15) << fixed << setprecision(0) << r.biaya_standar
             << setw(15) << fixed << setprecision(0) << r.biaya_premium << "\n";
    }
    
    cout << "----------------------------------------------------------------------------------------\n";
    cout << "*Catatan: Jika harga Rp 0, berarti layanan tersebut tidak tersedia untuk rute ini.\n";
    
    // Tombol Kembali ala Inquirer di akhir tabel
    cout << "\n  > \033[1;32mKembali\033[0m <\n";
    
    while (true) {
        char tombol = _getch();
        if (tombol == '\r') break;
    }
}