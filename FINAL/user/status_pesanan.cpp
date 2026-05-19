#include "user.h"
#include <iomanip>
#include <iostream>
#include <string>
#include "../database_handler.h"

static std::string warnaStatus(const std::string& status) {
    if (status == "Menunggu Konfirmasi")  return "\033[1;33m"; 
    if (status == "Menunggu Pembayaran")  return "\033[1;36m"; 
    if (status == "Ditolak")              return "\033[1;31m"; 
    if (status == "Lunas")                return "\033[1;32m";
    if (status == "Lunas & Dikirim")      return "\033[1;34m"; 
    return "\033[0m";
}

static std::string keteranganStatus(const std::string& status) {
    if (status == "Menunggu Konfirmasi")
        return "  -> Pesanan Anda sedang menunggu disetujui Admin.";
    if (status == "Menunggu Pembayaran")
        return "  -> Silakan lakukan pembayaran di menu Bayar Pesanan.";
    if (status == "Ditolak")
        return "  -> Pesanan Anda ditolak Admin. Silakan hubungi toko.";
    if (status == "Lunas")
        return "  -> Pembayaran berhasil. Terima kasih!";
    if (status == "Lunas & Dikirim")
        return "  -> Barang sedang dalam perjalanan ke alamat Anda.";
    return "";
}

void cekStatusPesanan(std::vector<Pesanan>* trxs) {
    bool berjalan = true;

    while (berjalan) {

        std::cout << "\033[2J\033[1;1H";
        std::cout << "\033[1;35m========================================\033[0m\n";
        std::cout << "\033[1;37m          STATUS PESANAN ANDA           \033[0m\n";
        std::cout << "\033[1;35m========================================\033[0m\n\n";

        if (trxs->empty()) {
            std::cout << "\033[1;31m  Belum ada pesanan yang tercatat.\033[0m\n\n";
            std::cout << "\033[1;36mTekan Enter untuk kembali...\033[0m";
            while (_getch() != '\r');
            return;
        }

        std::cout << std::left
                  << std::setw(12) << "ID Pesanan"
                  << std::setw(22) << "Nama Barang"
                  << std::setw(8)  << "Qty"
                  << std::setw(16) << "Total (Rp)"
                  << "Status\n";
        std::cout << "-----------------------------------------------------------------------\n";

        for (const auto& t : *trxs) {
            std::cout << std::left
                      << std::setw(12) << t.id_pesanan
                      << std::setw(22) << t.nama_barang
                      << std::setw(8)  << t.jumlah
                      << std::setw(16) << (long long)t.total_bayar
                      << warnaStatus(t.status) << t.status << "\033[0m\n";
        }
        std::cout << "-----------------------------------------------------------------------\n";

        std::cout << "\n  \033[1;33m[Menunggu Konfirmasi]\033[0m "
                  << "\033[1;36m[Menunggu Pembayaran]\033[0m "
                  << "\033[1;31m[Ditolak]\033[0m\n  "
                  << "\033[1;32m[Lunas]\033[0m "
                  << "\033[1;34m[Lunas & Dikirim]\033[0m\n";

        std::cout << "\nMasukkan ID Pesanan untuk melihat detail\n";
        std::cout << "(Ketik \033[1;33m'0'\033[0m untuk kembali ke menu): ";

        std::string inputId;
        std::getline(std::cin, inputId);

        while (!inputId.empty() && inputId.front() == ' ') inputId.erase(inputId.begin());
        while (!inputId.empty() && inputId.back()  == ' ') inputId.pop_back();

        if (inputId.empty() || inputId == "0") {
            berjalan = false;
            return;
        }

        Pesanan* ditemukan = nullptr;
        for (auto& t : *trxs) {
            if (t.id_pesanan == inputId) {
                ditemukan = &t;
                break;
            }
        }

        if (ditemukan == nullptr) {
            std::cout << "\n\033[1;31m[!] ID Pesanan \"" << inputId << "\" tidak ditemukan.\033[0m\n";
            std::cout << "\033[1;33mTekan Enter untuk mencoba lagi...\033[0m";
            while (_getch() != '\r');
            continue;
        }

        std::cout << "\033[2J\033[1;1H";
        std::cout << "\033[1;35m========================================\033[0m\n";
        std::cout << "\033[1;37m        DETAIL PESANAN                  \033[0m\n";
        std::cout << "\033[1;35m========================================\033[0m\n\n";

        std::cout << "  ID Pesanan    : \033[1;36m" << ditemukan->id_pesanan   << "\033[0m\n";
        std::cout << "  Nama Barang   : "            << ditemukan->nama_barang  << "\n";
        std::cout << "  Jumlah        : "            << ditemukan->jumlah       << " pcs\n";
        std::cout << "  Total Bayar   : \033[1;33mRp" << (long long)ditemukan->total_bayar << "\033[0m\n";
        std::cout << "  Tipe Beli     : "            << ditemukan->tipe_beli    << "\n";
        std::cout << "  Status        : "
                  << warnaStatus(ditemukan->status)
                  << ditemukan->status << "\033[0m\n";

        std::string ket = keteranganStatus(ditemukan->status);
        if (!ket.empty()) {
            std::cout << "\n\033[1;37m" << ket << "\033[0m\n";
        }

        if (ditemukan->status == "Menunggu Pembayaran") {
            std::cout << "\n\033[1;32m  [->] Pergi ke menu \033[1;33m\"Bayar Pesanan\"\033[1;32m"
                      << " untuk melunasi transaksi ini.\033[0m\n";
        }

        std::cout << "\n\033[1;35m========================================\033[0m\n";
        std::cout << "\n\033[1;36mTekan Enter untuk kembali ke daftar pesanan...\033[0m";
        while (_getch() != '\r');
    }
}