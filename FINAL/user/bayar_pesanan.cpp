#include "user.h"
#include <iomanip>
#include <iostream>
#include "../database_handler.h"


void payOrder(std::vector<Pesanan>* trxs,
              std::vector<Barang>*  db_barang,
              std::vector<Rute>*    db_rute,
              std::vector<Pengguna>* db_user) {

    bool berjalan = true;

    while (berjalan) {

        std::cout << "\033[2J\033[1;1H";
        std::cout << "\033[1;35m========================================\033[0m\n";
        std::cout << "\033[1;37m           BAYAR PESANAN                \033[0m\n";
        std::cout << "\033[1;35m========================================\033[0m\n\n";

        if (trxs->empty()) {
            std::cout << "\033[1;31m  Belum ada riwayat pesanan.\033[0m\n\n";
            std::cout << "\033[1;36mTekan Enter untuk kembali...\033[0m";
            while (_getch() != '\r');
            return;
        }


        std::vector<Pesanan*> bisaBayar;
        for (auto& t : *trxs) {
            if (t.status == "Menunggu Pembayaran") {
                bisaBayar.push_back(&t);
            }
        }


        std::cout << std::left
                  << std::setw(12) << "ID Pesanan"
                  << std::setw(24) << "Nama Barang"
                  << std::setw(8)  << "Qty"
                  << std::setw(18) << "Total (Rp)"
                  << "Status\n";
        std::cout << "--------------------------------------------------------------------------\n";

        for (const auto& t : *trxs) {
            std::string warna = "\033[0m";
            if      (t.status == "Menunggu Konfirmasi")  warna = "\033[1;33m";
            else if (t.status == "Menunggu Pembayaran")  warna = "\033[1;36m";
            else if (t.status == "Lunas")                warna = "\033[1;32m";
            else if (t.status == "Lunas & Dikirim")      warna = "\033[1;34m";

            std::cout << std::left
                      << std::setw(12) << t.id_pesanan
                      << std::setw(24) << t.nama_barang
                      << std::setw(8)  << t.jumlah
                      << std::setw(18) << (long long)t.total_bayar
                      << warna << t.status << "\033[0m\n";
        }
        std::cout << "--------------------------------------------------------------------------\n";
        std::cout << "  \033[1;33m[Menunggu Konfirmasi]\033[0m "
                  << "\033[1;36m[Menunggu Pembayaran]\033[0m "
                  << "\033[1;32m[Lunas]\033[0m "
                  << "\033[1;34m[Lunas & Dikirim]\033[0m\n";


        if (bisaBayar.empty()) {
            std::cout << "\n\033[1;33m  [!] Belum ada pesanan yang siap dibayar.\033[0m\n";
            std::cout << "\033[1;37m      Pesanan berstatus \033[1;33m\"Menunggu Konfirmasi\"\033[1;37m\033[0m\n";
            std::cout << "\033[1;37m      harus disetujui Admin terlebih dahulu.\033[0m\n\n";
            std::cout << "\033[1;36mTekan Enter untuk kembali ke menu...\033[0m";
            while (_getch() != '\r');
            return;
        }

        std::cout << "\nMasukkan ID Pesanan yang ingin dibayar\n";
        std::cout << "(hanya \033[1;36mMenunggu Pembayaran\033[0m yang bisa diproses)\n";
        std::cout << "(Ketik \033[1;33m'0'\033[0m untuk kembali ke menu): ";

        std::string inputId;
        std::getline(std::cin, inputId);

        while (!inputId.empty() && inputId.front() == ' ') inputId.erase(inputId.begin());
        while (!inputId.empty() && inputId.back()  == ' ') inputId.pop_back();

        if (inputId.empty() || inputId == "0") {
            berjalan = false;
            return;
        }


        Pesanan* target = nullptr;
        for (auto* t : bisaBayar) {
            if (t->id_pesanan == inputId) {
                target = t;
                break;
            }
        }


        if (target == nullptr) {
            std::string statusnya = "";
            for (const auto& t : *trxs) {
                if (t.id_pesanan == inputId) {
                    statusnya = t.status;
                    break;
                }
            }

            if (!statusnya.empty()) {
                std::cout << "\n\033[1;31m[!] Pesanan \"" << inputId << "\" tidak bisa dibayar.\033[0m\n";
                if (statusnya == "Menunggu Konfirmasi") {
                    std::cout << "\033[1;33m    Status: Menunggu Konfirmasi Admin.\033[0m\n";
                    std::cout << "\033[1;37m    Silakan tunggu Admin menyetujui pesanan Anda.\033[0m\n";
                } else if (statusnya == "Lunas") {
                    std::cout << "\033[1;32m    Status: Lunas (sudah dibayar).\033[0m\n";
                } else if (statusnya == "Lunas & Dikirim") {
                    std::cout << "\033[1;34m    Status: Lunas & Dikirim (sudah selesai).\033[0m\n";
                }
            } else {
                std::cout << "\n\033[1;31m[!] ID Pesanan \"" << inputId << "\" tidak ditemukan.\033[0m\n";
            }

            std::cout << "\033[1;33mTekan Enter untuk mencoba lagi...\033[0m";
            while (_getch() != '\r');
            continue;
        }


        std::cout << "\033[2J\033[1;1H";
        std::cout << "\033[1;35m========================================\033[0m\n";
        std::cout << "\033[1;37m           RINGKASAN TAGIHAN            \033[0m\n";
        std::cout << "\033[1;35m========================================\033[0m\n\n";

        std::cout << "  ID Pesanan    : \033[1;36m" << target->id_pesanan   << "\033[0m\n";
        std::cout << "  Nama Barang   : "            << target->nama_barang  << "\n";
        std::cout << "  Jumlah        : "            << target->jumlah       << " pcs\n";
        std::cout << "  Tipe Beli     : "            << target->tipe_beli    << "\n";
        std::cout << "\033[1;33m  ─────────────────────────────────\033[0m\n";
        std::cout << "\033[1;32m  TOTAL BAYAR  : Rp"
                  << (long long)target->total_bayar << "\033[0m\n\n";


        long long bayar  = 0;
        bool      bayarOk = false;

        std::cout << "Masukkan nominal uang yang dibayarkan (Rp): ";

        while (!bayarOk) {
            std::string inputBayar;
            std::getline(std::cin, inputBayar);

            while (!inputBayar.empty() && inputBayar.front() == ' ') inputBayar.erase(inputBayar.begin());
            while (!inputBayar.empty() && inputBayar.back()  == ' ') inputBayar.pop_back();

            bool semuaAngka = true;
            for (char c : inputBayar) {
                if (!std::isdigit(c)) { semuaAngka = false; break; }
            }

            if (!semuaAngka || inputBayar.empty()) {
                std::cout << "\033[1;31m[!] Masukkan angka yang valid: \033[0m";
                continue;
            }

            bayar = std::stoll(inputBayar);

            if (bayar < (long long)target->total_bayar) {
                std::cout << "\033[1;31m[!] Uang tidak cukup. Kurang Rp"
                          << ((long long)target->total_bayar - bayar)
                          << ". Masukkan lagi: \033[0m";
                continue;
            }

            bayarOk = true;
        }

        long long kembalian = bayar - (long long)target->total_bayar;

        std::cout << "\n  Uang Diterima : Rp" << bayar << "\n";
        std::cout << "  Kembalian     : \033[1;32mRp" << kembalian << "\033[0m\n\n";


        std::vector<std::string> konfOpts = {"Ya, Konfirmasi Pembayaran", "Batal"};
        int konfirmasi = inquirerMenuUser("Apakah pembayaran sudah benar?", konfOpts);

        if (konfirmasi == 1) {
            std::cout << "\n\033[1;33m[!] Pembayaran dibatalkan.\033[0m\n";
            std::cout << "\033[1;33mTekan Enter untuk kembali...\033[0m";
            while (_getch() != '\r');
            continue;
        }


        target->status = "Lunas";


        simpanData(*db_barang, *trxs, *db_rute, *db_user);

        std::cout << "\033[2J\033[1;1H";
        std::cout << "\033[1;35m========================================\033[0m\n";
        std::cout << "\033[1;32m        PEMBAYARAN BERHASIL!            \033[0m\n";
        std::cout << "\033[1;35m========================================\033[0m\n\n";

        std::cout << "  ID Pesanan    : \033[1;36m" << target->id_pesanan           << "\033[0m\n";
        std::cout << "  Nama Barang   : "            << target->nama_barang           << "\n";
        std::cout << "  Jumlah        : "            << target->jumlah                << " pcs\n";
        std::cout << "  Tipe Beli     : "            << target->tipe_beli             << "\n";
        std::cout << "  Total Dibayar : \033[1;33mRp" << (long long)target->total_bayar << "\033[0m\n";
        std::cout << "  Kembalian     : \033[1;32mRp" << kembalian                    << "\033[0m\n";
        std::cout << "  Status        : \033[1;32m"   << target->status               << "\033[0m\n\n";

        if (target->tipe_beli.find("Diantar") != std::string::npos) {
            std::cout << "\033[1;36m  [i] Pesanan diantar akan diproses oleh Admin.\033[0m\n";
            std::cout << "\033[1;36m      Pantau status di menu \033[1;33m\"Status Pesanan\"\033[1;36m.\033[0m\n\n";
        } else {
            std::cout << "\033[1;32m  [>] Silakan ambil barang Anda di toko. Terima kasih!\033[0m\n\n";
        }

        std::cout << "\033[1;32m[+] Data transaksi disimpan ke JSON secara otomatis.\033[0m\n\n";
        std::cout << "\033[1;36mTekan Enter untuk kembali ke menu...\033[0m";
        while (_getch() != '\r');

        berjalan = false;
    }
}