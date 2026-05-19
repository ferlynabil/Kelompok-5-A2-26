#include "user.h"
#include <iomanip>
#include "../database_handler.h"

// nampilin cetakan histori semua keranjang yang pernah terjadi
void displayHistory(std::vector<Pesanan>* trxs) {
    if (trxs->empty()) {
        std::cout << "\033[1;31mBelum ada riwayat transaksi.\033[0m\n";
        return;
    }

    std::cout << "\033[1;32m=== RIWAYAT TRANSAKSI ===\033[0m\n";
    std::cout << std::left << std::setw(15) << "ID Transaksi" << std::setw(20) << "Nama Barang" << std::setw(10) << "Qty" << std::setw(15) << "Total (Rp)" << std::setw(15) << "Tipe" << "Status\n";
    std::cout << "--------------------------------------------------------------------------------------------\n";

    // bongkar isi memori transaksi satu per satu
    for (const auto& t : *trxs) {
        std::cout << std::left << std::setw(15) << t.id_pesanan 
                  << std::setw(20) << t.nama_barang 
                  << std::setw(10) << t.jumlah 
                  << std::setw(15) << (long long)t.total_bayar 
                  << std::setw(15) << t.tipe_beli 
                  << t.status << "\n";
    }
}