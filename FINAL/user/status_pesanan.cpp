#include "user.h"
#include <iomanip>
#include <iostream>
#include <string>
#include "../database_handler.h"

void cekStatusPesanan(std::vector<Pesanan>* trxs) {
    bool berjalan = true;

    while (berjalan) {
        if (trxs->empty()) {
            std::cout << "\n\033[1;31mBelum ada pesanan yang tercatat.\033[0m\n";
            std::cout << "\n\033[1;36mTekan Enter untuk kembali...\033[0m";
            while (_getch() != '\r');
            return;
        }

        std::cout << "\033[2J\033[1;1H";
        std::cout << "\033[1;35m========================================\033[0m\n";
        std::cout << "\033[1;37m          STATUS PESANAN ANDA           \033[0m\n";
        std::cout << "\033[1;35m========================================\033[0m\n\n";

        std::cout << std::left
                  << std::setw(12) << "ID Pesanan"
                  << std::setw(22) << "Nama Barang"
                  << std::setw(8)  << "Qty"
                  << std::setw(16) << "Total (Rp)"
                  << "Status\n";
        std::cout << "-----------------------------------------------------------------------\n";

        for (const auto& t : *trxs) {
            std::string warna = "\033[0m";
            if (t.status == "Menunggu Konfirmasi") warna = "\033[1;33m";
            else if (t.status == "Menunggu Pembayaran") warna = "\033[1;36m";
            else if (t.status == "Lunas") warna = "\033[1;32m";
            else if (t.status == "Lunas & Dikirim") warna = "\033[1;34m";

            std::cout << std::left
                      << std::setw(12) << t.id_pesanan
                      << std::setw(22) << t.nama_barang
                      << std::setw(8)  << t.jumlah
                      << std::setw(16) << (long long)t.total_bayar
                      << warna << t.status << "\033[0m\n";
        }
        std::cout << "-----------------------------------------------------------------------\n";

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
        std::cout << "  Nama Barang   : " << ditemukan->nama_barang << "\n";
        std::cout << "  Jumlah        : " << ditemukan->jumlah      << " pcs\n";
        std::cout << "  Total Bayar   : \033[1;33mRp" << (long long)ditemukan->total_bayar << "\033[0m\n";
        std::cout << "  Tipe Beli     : " << ditemukan->tipe_beli   << "\n";

        std::string warna = "\033[0m";
        std::string keterangan = "";
        if (ditemukan->status == "Menunggu Konfirmasi") {
            warna = "\033[1;33m";
            keterangan = "  -> Pesanan Anda sedang menunggu disetujui Admin.";
        } else if (ditemukan->status == "Menunggu Pembayaran") {
            warna = "\033[1;36m";
            keterangan = "  -> Silakan lakukan pembayaran di menu Bayar Pesanan.";
        } else if (ditemukan->status == "Lunas") {
            warna = "\033[1;32m";
            keterangan = "  -> Pembayaran berhasil. Terima kasih!";
        } else if (ditemukan->status == "Lunas & Dikirim") {
            warna = "\033[1;34m";
            keterangan = "  -> Barang sedang dalam perjalanan ke alamat Anda.";
        }

        std::cout << "  Status        : " << warna << ditemukan->status << "\033[0m\n";
        if (!keterangan.empty()) {
            std::cout << "\033[1;37m" << keterangan << "\033[0m\n";
        }

        std::cout << "\n\033[1;35m========================================\033[0m\n";
        std::cout << "\n\033[1;36mTekan Enter untuk kembali ke daftar pesanan...\033[0m";
        while (_getch() != '\r');
    }
}