#include "user.h"
#include <iomanip>
#include "../database_handler.h"

// 1. UPDATE PARAMETER: Tambahkan pointer ke Barang, Rute, dan Pengguna
void payOrder(std::vector<Pesanan>* trxs, std::vector<Barang>* db_barang, std::vector<Rute>* db_rute, std::vector<Pengguna>* db_user) {
    if (trxs->empty()) {
        std::cout << "\033[1;31mBelum ada riwayat pesanan.\033[0m\n";
        return;
    }

    std::cout << "\033[1;32m=== DAFTAR PESANAN ===\033[0m\n";
    std::cout << std::left << std::setw(15) << "ID Transaksi" << std::setw(25) << "Nama Barang" << std::setw(15) << "Total (Rp)" << "Status\n";
    std::cout << "--------------------------------------------------------------------------\n";

    for (const auto& t : *trxs) {
        std::cout << std::left << std::setw(15) << t.id_pesanan << std::setw(25) << t.nama_barang << std::setw(15) << (long long)t.total_bayar << t.status << "\n";
    }

    std::cout << "\nMasukkan ID Transaksi yang ingin dibayar: ";
    std::string id = getValidStringUser();

    bool found = false;
    for (auto& t : *trxs) {
        if (t.id_pesanan == id) {
            found = true;
            if (t.status == "Menunggu Konfirmasi") {
                std::cout << "\n\033[1;33mPesanan ini masih menunggu konfirmasi admin.\033[0m\n";
                std::cout << "Simulasikan konfirmasi admin untuk testing? (y/n): ";
                std::string ans;
                std::getline(std::cin, ans);
                if (ans == "y" || ans == "Y") {
                    t.status = "Menunggu Pembayaran";
                    // AUTO UPDATE JSON MESKIPUN CUMA SIMULASI
                    simpanData(*db_barang, *trxs, *db_rute, *db_user);
                    std::cout << "\033[1;32mStatus pesanan berhasil diubah menjadi Menunggu Pembayaran.\033[0m\n";
                } else {
                    return;
                }
            }
            
            if (t.status == "Menunggu Pembayaran") {
                std::cout << "\n\033[1;33mTotal yang harus dibayar: Rp" << (long long)t.total_bayar << "\033[0m\n";
                std::cout << "Masukkan nominal uang: ";
                long long bayar = getValidNumberUser();

                while (bayar < t.total_bayar) {
                    std::cout << "\033[1;31mUang tidak cukup. Masukkan nominal yang sesuai: \033[0m";
                    bayar = getValidNumberUser();
                }

                // Ubah status
                t.status = "Lunas";
                
                // 2. AUTO-UPDATE JSON SAAT LUNAS (pakai * karena dia pointer)
                simpanData(*db_barang, *trxs, *db_rute, *db_user);
                
                std::cout << "\n\033[1;32mPembayaran berhasil dilakukan. Kembalian: Rp" << (bayar - (long long)t.total_bayar) << "\033[0m\n";
                std::cout << "[+] Data Transaksi JSON otomatis diperbarui!\n";
            } else if (t.status == "Lunas") {
                std::cout << "\n\033[1;36mPesanan ini sudah lunas.\033[0m\n";
            } else if (t.status == "Lunas & Dikirim") {
                std::cout << "\n\033[1;36mPesanan ini sudah Lunas dan Sedang Dikirim.\033[0m\n";
            }
            break;
        }
    }

    if (!found) {
        std::cout << "\n\033[1;31mID Transaksi tidak ditemukan.\033[0m\n";
    }
}