#include "user.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <conio.h>


std::string toLower(std::string s) {
    for(char &c : s) c = tolower(c);
    return s;
}

void displayItems(std::vector<Barang>* items) {
    if (items->empty()) {
        std::cout << "\n\033[1;31mBelum ada barang yang tersedia. Menunggu Admin.\033[0m\n";
        return;
    }

    std::vector<std::string> menuSort = {
        "Tanpa Pengurutan (Default)",
        "Nama Barang (A - Z)",
        "Harga (Termurah)",
        "Harga (Termahal)",
        "Cari Nama Barang",
        "Kembali"
    };

    int posisi = 0;
    bool milih = true;
    std::vector<Barang> dataTampil = *items; 

    while (milih) {
        system("cls");
        std::cout << "\n=== OPSI TAMPILAN BARANG ===\n";
        for (int i = 0; i < menuSort.size(); i++) {
            if (i == posisi) std::cout << "  > \033[1;32m" << menuSort[i] << "\033[0m <\n";
            else std::cout << "    " << menuSort[i] << "\n";
        }

        char tombol = _getch();
        if (tombol == 72 && posisi > 0) posisi--;
        else if (tombol == 80 && posisi < menuSort.size() - 1) posisi++;
        else if (tombol == '\r') {
            if (posisi == 5) return; 
            
            if (posisi == 4) {
                system("cls");
                std::cout << "\nMasukkan Nama Barang yang dicari: ";
                std::string keyword;
                std::cin >> keyword;
                std::vector<Barang> hasilCari;
                for (const auto& b : *items) {
                    if (toLower(b.nama_barang).find(toLower(keyword)) != std::string::npos) {
                        hasilCari.push_back(b);
                    }
                }
                dataTampil = hasilCari;
            } else if (posisi == 1) { 
                std::sort(dataTampil.begin(), dataTampil.end(), [](const Barang& a, const Barang& b) {
                    return a.nama_barang < b.nama_barang;
                });
            } else if (posisi == 2) {
                std::sort(dataTampil.begin(), dataTampil.end(), [](const Barang& a, const Barang& b) {
                    return a.harga < b.harga;
                });
            } else if (posisi == 3) { 
                std::sort(dataTampil.begin(), dataTampil.end(), [](const Barang& a, const Barang& b) {
                    return a.harga > b.harga;
                });
            }
            milih = false;
        }
    }


    system("cls");
    if (dataTampil.empty()) {
        std::cout << "\n\033[1;31mBarang tidak ditemukan.\033[0m\n";
    } else {
        std::cout << "\n\033[1;32m=== DAFTAR BARANG TOKO ===\033[0m\n";
        std::cout << std::left << std::setw(15) << "Kode" << std::setw(25) << "Nama Barang" << std::setw(15) << "Harga (Rp)" << "Stok\n";
        std::cout << "--------------------------------------------------------------\n";

        for (const auto& it : dataTampil) {
            std::cout << std::left << std::setw(15) << it.id_barang 
                      << std::setw(25) << it.nama_barang 
                      << std::setw(15) << (long long)it.harga 
                      << it.stok << "\n";
        }
    }

    std::cout << "\n\033[1;32m> Kembali <\033[0m (Tekan Enter)";
    while (_getch() != '\r'); 
}