#include "user.h"
#include <iomanip>
#include <iostream>
#include <string>
#include "../database_handler.h"

// fungsi pembantu buat ngintip isi gudang supaya user bisa langsung milih
static void tampilDaftarBarang(std::vector<Barang>* items) {
    std::cout << "\033[1;32m=== DAFTAR BARANG TERSEDIA ===\033[0m\n";
    std::cout << std::left
              << std::setw(14) << "Kode Barang"
              << std::setw(26) << "Nama Barang"
              << std::setw(16) << "Harga (Rp)"
              << "Stok\n";
    std::cout << "--------------------------------------------------------------\n";
    for (const auto& it : *items) {
        std::cout << std::left
                  << std::setw(14) << it.id_barang
                  << std::setw(26) << it.nama_barang
                  << std::setw(16) << (long long)it.harga
                  << it.stok << "\n";
    }
    std::cout << "--------------------------------------------------------------\n";
}

// nampilin jalur ekspedisi yang bisa dipakai sama pelanggan
static void tampilDaftarRute(std::vector<Rute>* routes) {
    std::cout << "\033[1;32m=== DAFTAR RUTE PENGIRIMAN (DARI SAMARINDA) ===\033[0m\n";
    std::cout << std::left
              << std::setw(12) << "ID Rute"
              << std::setw(20) << "Kota Tujuan"
              << std::setw(15) << "Reguler (Rp)"
              << std::setw(15) << "Standar (Rp)"
              << "Premium (Rp)\n";
    std::cout << "--------------------------------------------------------------------------\n";
    for (const auto& r : *routes) {
        std::cout << std::left
                  << std::setw(12) << r.id_rute
                  << std::setw(20) << r.tujuan
                  << std::setw(15) << (long long)r.biaya_reguler
                  << std::setw(15) << (long long)r.biaya_standar
                  << (long long)r.biaya_premium << "\n";
    }
    std::cout << "--------------------------------------------------------------------------\n";
}

// tempat nyatukan logika dari awal milih sampai jadi orderan masuk
void orderItem(std::vector<Barang>* items, std::vector<Rute>* routes, std::vector<Pesanan>* trxs, std::vector<Pengguna>* users) {
    if (items->empty()) {
        std::cout << "\033[1;31mGudang kosong! Belum ada barang untuk dipesan.\033[0m\n";
        return;
    }

    tampilDaftarBarang(items);
    std::cout << "\nMasukkan Kode Barang yang ingin dipesan (atau 0 untuk batal): ";
    std::string id_barang;
    std::getline(std::cin, id_barang);

    if (id_barang == "0") return;

    // pastiin dulu barangnya memang terdaftar di dalam gudang
    Barang* selectedItem = nullptr;
    for (auto& it : *items) {
        if (it.id_barang == id_barang) {
            selectedItem = &it;
            break;
        }
    }

    if (!selectedItem) {
        std::cout << "\033[1;31mBarang tidak ditemukan.\033[0m\n";
        return;
    }

    std::cout << "Masukkan jumlah yang ingin dibeli: ";
    long long qty = getValidNumberUser();

    if (qty <= 0 || qty > selectedItem->stok) {
        std::cout << "\033[1;31mJumlah tidak valid atau melebihi stok yang ada (" << selectedItem->stok << ").\033[0m\n";
        return;
    }

    long long totalHargaBarang = (long long)selectedItem->harga * qty;
    std::cout << "Total harga barang: Rp" << totalHargaBarang << "\n";

    std::vector<std::string> opsiBeli = {"Ambil Sendiri di Toko", "Kirim via Ekspedisi", "Batal"};
    int pilihan = inquirerMenuUser("PILIH METODE PEMBELIAN", opsiBeli);

    std::string tipeBeli;
    long long grandTotal = totalHargaBarang;
    
    if (pilihan == 2) {
        std::cout << "\n\033[1;33m[!] Pesanan dibatalkan. Kembali ke menu.\033[0m\n";
        return;
    } else if (pilihan == 0) {
        tipeBeli = "Ambil di Toko";
    } else if (pilihan == 1) {
        tipeBeli = "Kirim Ekspedisi";
        clearScreenUser();
        
        if (routes->empty()) {
             std::cout << "\033[1;31mMaaf, belum ada rute pengiriman yang tersedia.\033[0m\n";
             return;
        }
        
        tampilDaftarRute(routes);
        std::cout << "\nMasukkan ID Rute: ";
        std::string id_rute;
        std::getline(std::cin, id_rute);
        
        Rute* selectedRute = nullptr;
        for (auto& r : *routes) {
            if (r.id_rute == id_rute) {
                selectedRute = &r;
                break;
            }
        }
        
        if (!selectedRute) {
            std::cout << "\033[1;31mRute tidak valid.\033[0m\n";
            return;
        }
        
        std::vector<std::string> opsiOngkir = {"Reguler (Rp" + std::to_string((long long)selectedRute->biaya_reguler) + ")", 
                                               "Standar (Rp" + std::to_string((long long)selectedRute->biaya_standar) + ")", 
                                               "Premium (Rp" + std::to_string((long long)selectedRute->biaya_premium) + ")"};
        int pilOngkir = inquirerMenuUser("PILIH LAYANAN PENGIRIMAN", opsiOngkir);
        
        long long biayaOngkir = 0;
        if (pilOngkir == 0) biayaOngkir = selectedRute->biaya_reguler;
        else if (pilOngkir == 1) biayaOngkir = selectedRute->biaya_standar;
        else if (pilOngkir == 2) biayaOngkir = selectedRute->biaya_premium;
        
        grandTotal += biayaOngkir;
        std::cout << "\n\033[1;32mOngkos kirim ditambahkan. Total Bayar Baru: Rp" << grandTotal << "\033[0m\n";
    }

    std::cout << "\nApakah Anda yakin ingin memesan ini? (y/n): ";
    std::string konfirm;
    std::getline(std::cin, konfirm);
    
    if (konfirm != "y" && konfirm != "Y") {
        std::cout << "\n\033[1;33m[!] Pesanan dibatalkan. Kembali ke menu.\033[0m\n";
        return;
    }

    // stok di gudang berkurang seketika pas pesanan fix dibuat
    selectedItem->stok -= qty;

    std::string newTrxId = "TRX00" + std::to_string(trxs->size() + 1);
    Pesanan newTrx = {
        newTrxId,
        selectedItem->nama_barang,
        static_cast<int>(qty),
        static_cast<double>(grandTotal),
        tipeBeli,
        "Menunggu Konfirmasi"
    };
    trxs->push_back(newTrx);

    // auto backup datanya pakai JSON biar gak raib pas dilogout
    simpanData(*items, *trxs, *routes, *users);

    std::cout << "\033[2J\033[1;1H";
    std::cout << "\033[1;35m========================================\033[0m\n";
    std::cout << "\033[1;32m         PESANAN BERHASIL DIBUAT!       \033[0m\n";
    std::cout << "\033[1;35m========================================\033[0m\n\n";
    std::cout << "  ID Pesanan    : \033[1;36m" << newTrxId << "\033[0m\n";
    std::cout << "  Barang        : " << selectedItem->nama_barang << " (" << qty << " pcs)\n";
    std::cout << "  Metode        : " << tipeBeli << "\n";
    std::cout << "  Total Tagihan : \033[1;33mRp" << grandTotal << "\033[0m\n";
    std::cout << "\n[+] Data pesanan berhasil disimpan ke sistem.\n";
}