#include "user.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <conio.h>

// ubah ke huruf kecil semua biar pencarian barang jadi lebih santai gak kaku
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
    
    // sengaja copy datanya di sini biar database aslinya gak rusak urutannya
    std::vector<Barang> dataTampil = *items; 

    while (milih) {
        system("cls");
        std::cout << "\n=== OPSI TAMPILAN BARANG ===\n";
        for (int i = 0; i < menuSort.size(); i++) {
            if (i == posisi) std::cout << "  > \033[1;32m" << menuSort[i] << "\033[0m <\n";
            else std::cout << "    " << menuSort[i] << "\n";
        }
        
        int tombol = _getch();
        if (tombol == 224) {
            tombol = _getch();
            if (tombol == 72 && posisi > 0) posisi--;
            else if (tombol == 80 && posisi < menuSort.size() - 1) posisi++;
        } else if (tombol == '\r') {
            if (posisi == 5) return;
            
            if (posisi == 4) {
                 std::cout << "\nMasukkan kata kunci pencarian: ";
                 std::string keyword;
                 std::getline(std::cin, keyword);
                 keyword = toLower(keyword);
                 
                 std::vector<Barang> hasilCari;
                 for (const auto& brg : *items) {
                     if (toLower(brg.nama_barang).find(keyword) != std::string::npos) {
                         hasilCari.push_back(brg);
                     }
                 }
                 dataTampil = hasilCari;
                 milih = false;
                 continue;
            }
            
            if (posisi == 1) { 
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

    // langsung cetak hasil susunan datanya ke layar pembeli
    system("cls");
    if (dataTampil.empty()) {
        std::cout << "\n\033[1;31mBarang tidak ditemukan.\033[0m\n";
    } else {
        std::cout << "\n\033[1;32m=== DAFTAR BARANG TOKO ===\033[0m\n";
        std::cout << std::left << std::setw(15) << "Kode" << std::setw(25) << "Nama Barang" << std::setw(15) << "Harga (Rp)" << "Stok\n";
        std::cout << "--------------------------------------------------------------\n";

        for (const auto& it : dataTampil) {
             std::cout << std::left 
                       << std::setw(15) << it.id_barang 
                       << std::setw(25) << it.nama_barang 
                       << std::setw(15) << (long long)it.harga 
                       << it.stok << "\n";
        }
        std::cout << "--------------------------------------------------------------\n";
    }
    std::cout << "\n\033[1;36mTekan Enter untuk kembali...\033[0m";
    std::string dumm;
    std::getline(std::cin, dumm);
}