#include <iostream>
#include <iomanip>
#include <conio.h>
#include <vector>
#include <string>
#include <cctype>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

void lihatBarang(const vector<Barang>& daftar_barang) {
    if (daftar_barang.empty()) {
        system("cls");
        cout << "\n=== DAFTAR BARANG TOKO ===\n";
        cout << "Gudang masih kosong!\n";
        cout << "\nTekan Enter untuk kembali...";
        string dummy;
        getline(cin, dummy);
        return;
    }

    vector<string> menu_sort = {
        "Tanpa Pengurutan (Sesuai Input)", 
        "Nama Barang (A - Z)", 
        "Harga (Termurah - Termahal)", 
        "Harga (Termahal - Termurah)", 
        "Stok (Paling Sedikit)",
        "Kembali"
    };
    
    int posisi_sort = 0;
    bool memilih_sort = true;
    vector<Barang> data_tampil = daftar_barang;

    while (memilih_sort) {
        system("cls");
        cout << "\n=== OPSI TAMPILAN DAFTAR BARANG ===\n";
        cout << "Pilih metode pengurutan (Gunakan Panah Atas/Bawah & Enter):\n\n";

        for (int i = 0; i < menu_sort.size(); i++) {
            if (i == posisi_sort) {
                cout << "  > \033[1;32m" << menu_sort[i] << "\033[0m <\n";
            } else {
                cout << "    " << menu_sort[i] << "\n";
            }
        }

        char tombol = _getch();

        if (tombol == 72 && posisi_sort > 0) {
            posisi_sort--;
        } else if (tombol == 80 && posisi_sort < menu_sort.size() - 1) {
            posisi_sort++;
        } else if (tombol == '\r') {
            memilih_sort = false;
        }
    }

    if (posisi_sort == 5) {
        return;
    }

    if (posisi_sort == 1) {
        for (int i = 1; i < data_tampil.size(); i++) {
            Barang kunci = data_tampil[i];
            int j = i - 1;
            
            string kunci_nama = kunci.nama_barang;
            for (char& c : kunci_nama) c = tolower(c);
            
            while (j >= 0) {
                string banding_nama = data_tampil[j].nama_barang;
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
        for (int i = 1; i < data_tampil.size(); i++) {
            Barang kunci = data_tampil[i];
            int j = i - 1;
            
            while (j >= 0 && data_tampil[j].harga > kunci.harga) {
                data_tampil[j + 1] = data_tampil[j];
                j--;
            }
            data_tampil[j + 1] = kunci;
        }
    } 
    else if (posisi_sort == 3) {
        for (int i = 1; i < data_tampil.size(); i++) {
            Barang kunci = data_tampil[i];
            int j = i - 1;
            
            while (j >= 0 && data_tampil[j].harga < kunci.harga) {
                data_tampil[j + 1] = data_tampil[j];
                j--;
            }
            data_tampil[j + 1] = kunci;
        }
    } 
    else if (posisi_sort == 4) {
        for (int i = 1; i < data_tampil.size(); i++) {
            Barang kunci = data_tampil[i];
            int j = i - 1;
            
            while (j >= 0 && data_tampil[j].stok > kunci.stok) {
                data_tampil[j + 1] = data_tampil[j];
                j--;
            }
            data_tampil[j + 1] = kunci;
        }
    }

    system("cls");
    cout << "\n=== DAFTAR BARANG TOKO ===\n";
    cout << "Metode: " << menu_sort[posisi_sort] << "\n";
    cout << "-------------------------------------------------------------------------------\n";
    cout << setw(15) << left << "ID" 
         << setw(20) << "Nama Barang" 
         << setw(12) << "Harga" 
         << setw(10) << "Berat" 
         << setw(15) << "Satuan" 
         << setw(5) << "Stok\n";
    cout << "-------------------------------------------------------------------------------\n";
    
    for (const auto& brg : data_tampil) {
        cout << setw(15) << left << brg.id_barang 
             << setw(20) << brg.nama_barang 
             << setw(12) << brg.harga 
             << setw(10) << brg.berat 
             << setw(15) << brg.satuan 
             << setw(5) << brg.stok << "\n";
    }
    cout << "-------------------------------------------------------------------------------\n";
    cout << "\nTekan Enter untuk kembali...";
    string dummy;
    getline(cin, dummy);
}