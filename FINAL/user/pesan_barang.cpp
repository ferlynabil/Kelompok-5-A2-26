#include "user.h"
#include <iomanip>
#include <iostream>
#include <string>
#include "../database_handler.h"

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

static void tampilDaftarRute(std::vector<Rute>* routes) {
    std::cout << "\033[1;32m=== DAFTAR RUTE PENGIRIMAN (DARI SAMARINDA) ===\033[0m\n";
    std::cout << std::left
              << std::setw(14) << "Kode Rute"
              << std::setw(26) << "Tujuan Kota"
              << "Jarak\n";
    std::cout << "---------------------------------------------------\n";
    for (const auto& r : *routes) {
        std::cout << std::left
                  << std::setw(14) << r.id_rute
                  << std::setw(26) << r.tujuan
                  << r.jarak << " km\n";
    }
    std::cout << "---------------------------------------------------\n";
}

void orderItem(std::vector<Barang>* items, std::vector<Rute>* routes,
               std::vector<Pesanan>* trxs, std::vector<Pengguna>* users) {

    if (items->empty()) {
        std::cout << "\n\033[1;31mBelum ada barang untuk dipesan. Menunggu Admin.\033[0m\n";
        return;
    }

    Barang* selectedItem = nullptr;

    while (selectedItem == nullptr) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "\033[1;35m========================================\033[0m\n";
        std::cout << "\033[1;37m             PESAN BARANG               \033[0m\n";
        std::cout << "\033[1;35m========================================\033[0m\n\n";

        tampilDaftarBarang(items);

        std::cout << "\nMasukkan Kode Barang yang ingin dibeli\n";
        std::cout << "(Ketik \033[1;33m'0'\033[0m untuk kembali ke menu): ";

        std::string kode;
        std::getline(std::cin, kode);

        while (!kode.empty() && kode.front() == ' ') kode.erase(kode.begin());
        while (!kode.empty() && kode.back()  == ' ') kode.pop_back();

        if (kode.empty() || kode == "0") {
            return;
        }

        for (auto& it : *items) {
            if (it.id_barang == kode) {
                selectedItem = &it;
                break;
            }
        }

        if (selectedItem == nullptr) {
            std::cout << "\n\033[1;31m[!] Kode barang \"" << kode << "\" tidak ditemukan. Silakan coba lagi.\033[0m\n";
            std::cout << "\033[1;33mTekan Enter untuk mencoba lagi...\033[0m";
            while (_getch() != '\r');
        }
    }

    long long qty = 0;
    bool qtyOk = false;

    while (!qtyOk) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "\033[1;35m========================================\033[0m\n";
        std::cout << "\033[1;37m        LANGKAH 2: JUMLAH BELI          \033[0m\n";
        std::cout << "\033[1;35m========================================\033[0m\n\n";

        std::cout << "  Barang dipilih : \033[1;36m" << selectedItem->nama_barang << "\033[0m\n";
        std::cout << "  Harga          : \033[1;33mRp" << (long long)selectedItem->harga << "\033[0m\n";
        std::cout << "  Stok tersedia  : \033[1;32m" << selectedItem->stok << " pcs\033[0m\n\n";

        std::cout << "Masukkan jumlah beli\n";
        std::cout << "(Ketik \033[1;33m'0'\033[0m untuk kembali ke pilih barang): ";

        std::string inputQty;
        std::getline(std::cin, inputQty);

        while (!inputQty.empty() && inputQty.front() == ' ') inputQty.erase(inputQty.begin());
        while (!inputQty.empty() && inputQty.back()  == ' ') inputQty.pop_back();

        if (inputQty.empty() || inputQty == "0") {
            selectedItem = nullptr;
            break;
        }

        bool semuaAngka = true;
        for (char c : inputQty) {
            if (!std::isdigit(c)) { semuaAngka = false; break; }
        }
        if (!semuaAngka || inputQty.empty()) {
            std::cout << "\n\033[1;31m[!] Masukkan angka bulat yang valid.\033[0m\n";
            std::cout << "\033[1;33mTekan Enter untuk mencoba lagi...\033[0m";
            while (_getch() != '\r');
            continue;
        }

        qty = std::stoll(inputQty);

        if (qty <= 0) {
            std::cout << "\n\033[1;31m[!] Jumlah harus lebih dari 0.\033[0m\n";
            std::cout << "\033[1;33mTekan Enter untuk mencoba lagi...\033[0m";
            while (_getch() != '\r');
            continue;
        }
        if (qty > selectedItem->stok) {
            std::cout << "\n\033[1;31m[!] Stok tidak mencukupi. Stok saat ini: "
                      << selectedItem->stok << " pcs.\033[0m\n";
            std::cout << "\033[1;33mTekan Enter untuk mencoba lagi...\033[0m";
            while (_getch() != '\r');
            continue;
        }

        qtyOk = true;
    }

    if (selectedItem == nullptr) {
        orderItem(items, routes, trxs, users);
        return;
    }

    long long subtotal = (long long)selectedItem->harga * qty;
    long long ongkir   = 0;
    std::string tipeBeli = "Ditempat";

    std::vector<std::string> tipeOptions = {
        "Beli Ditempat",
        "Diantar (Kirim ke Alamat)",
        "Kembali"
    };
    int tipePilihan = inquirerMenuUser("LANGKAH 3: TIPE PEMBELIAN", tipeOptions);

    if (tipePilihan == 2) {
        orderItem(items, routes, trxs, users);
        return;
    }

    Rute* selectedRoute = nullptr;

    if (tipePilihan == 1) {
        if (routes->empty()) {
            std::cout << "\033[2J\033[1;1H";
            std::cout << "\n\033[1;31m[!] Belum ada rute pengiriman dari Admin.\033[0m\n";
            std::cout << "\033[1;31m    Transaksi pengiriman tidak dapat dilanjutkan.\033[0m\n";
            std::cout << "\n\033[1;36mTekan Enter untuk kembali...\033[0m";
            while (_getch() != '\r');
            orderItem(items, routes, trxs, users);
            return;
        }

        while (selectedRoute == nullptr) {
            std::cout << "\033[2J\033[1;1H";
            std::cout << "\033[1;35m========================================\033[0m\n";
            std::cout << "\033[1;37m      LANGKAH 4: PILIH RUTE             \033[0m\n";
            std::cout << "\033[1;35m========================================\033[0m\n\n";

            tampilDaftarRute(routes);

            std::cout << "\nMasukkan Kode Rute Pengiriman\n";
            std::cout << "(Ketik \033[1;33m'0'\033[0m untuk kembali ke tipe pembelian): ";

            std::string ruteKode;
            std::getline(std::cin, ruteKode);

            while (!ruteKode.empty() && ruteKode.front() == ' ') ruteKode.erase(ruteKode.begin());
            while (!ruteKode.empty() && ruteKode.back()  == ' ') ruteKode.pop_back();

            if (ruteKode.empty() || ruteKode == "0") {
                orderItem(items, routes, trxs, users);
                return;
            }

            for (auto& r : *routes) {
                if (r.id_rute == ruteKode) {
                    selectedRoute = &r;
                    break;
                }
            }

            if (selectedRoute == nullptr) {
                std::cout << "\n\033[1;31m[!] Kode rute \"" << ruteKode << "\" tidak valid. Silakan coba lagi.\033[0m\n";
                std::cout << "\033[1;33mTekan Enter untuk mencoba lagi...\033[0m";
                while (_getch() != '\r');
            }
        }

        std::vector<std::string> serviceOptions;
        std::vector<long long>   serviceCosts;
        std::vector<std::string> serviceNames;

        if (selectedRoute->biaya_reguler > 0) {
            serviceOptions.push_back("Reguler  (Rp" + std::to_string((long long)selectedRoute->biaya_reguler)
                                     + ") - " + selectedRoute->estimasi_reguler);
            serviceCosts.push_back((long long)selectedRoute->biaya_reguler);
            serviceNames.push_back("Reguler");
        }
        if (selectedRoute->biaya_standar > 0) {
            serviceOptions.push_back("Standar  (Rp" + std::to_string((long long)selectedRoute->biaya_standar)
                                     + ") - " + selectedRoute->estimasi_standar);
            serviceCosts.push_back((long long)selectedRoute->biaya_standar);
            serviceNames.push_back("Standar");
        }
        if (selectedRoute->biaya_premium > 0) {
            serviceOptions.push_back("Premium  (Rp" + std::to_string((long long)selectedRoute->biaya_premium)
                                     + ") - " + selectedRoute->estimasi_premium);
            serviceCosts.push_back((long long)selectedRoute->biaya_premium);
            serviceNames.push_back("Premium");
        }

        if (serviceOptions.empty()) {
            std::cout << "\n\033[1;31m[!] Rute ini belum memiliki harga layanan yang diatur Admin.\033[0m\n";
            std::cout << "\033[1;33mTekan Enter untuk kembali...\033[0m";
            while (_getch() != '\r');
            orderItem(items, routes, trxs, users);
            return;
        }

        serviceOptions.push_back("Kembali");

        int servicePick = inquirerMenuUser(
            "LANGKAH 5: PILIH LAYANAN (" + selectedRoute->kota_asal + " -> " + selectedRoute->tujuan + ")",
            serviceOptions
        );

        if (servicePick == (int)serviceOptions.size() - 1) {
            orderItem(items, routes, trxs, users);
            return;
        }

        ongkir   = serviceCosts[servicePick];
        tipeBeli = "Diantar (" + serviceNames[servicePick] + ")";
    }

    long long grandTotal = subtotal + ongkir;

    std::cout << "\033[2J\033[1;1H";
    std::cout << "\033[1;35m========================================\033[0m\n";
    std::cout << "\033[1;37m    LANGKAH 6: KONFIRMASI PESANAN       \033[0m\n";
    std::cout << "\033[1;35m========================================\033[0m\n\n";

    std::cout << "  Barang        : \033[1;36m" << selectedItem->nama_barang << "\033[0m\n";
    std::cout << "  Jumlah        : " << qty << " pcs\n";
    std::cout << "  Harga satuan  : Rp" << (long long)selectedItem->harga << "\n";
    std::cout << "  Subtotal      : Rp" << subtotal << "\n";
    std::cout << "  Tipe Beli     : " << tipeBeli << "\n";
    if (ongkir > 0) {
        std::cout << "  Ongkos Kirim  : \033[1;33mRp" << ongkir << "\033[0m\n";
    }
    std::cout << "\033[1;33m  ─────────────────────────────────\033[0m\n";
    std::cout << "\033[1;32m  TOTAL BAYAR  : Rp" << grandTotal << "\033[0m\n\n";

    std::vector<std::string> konfirmasiOptions = {"Ya, Konfirmasi Pesanan", "Batal / Kembali"};
    int konfirmasi = inquirerMenuUser("Apakah Anda yakin ingin memesan?", konfirmasiOptions);

    if (konfirmasi == 1) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "\n\033[1;33m[!] Pesanan dibatalkan. Kembali ke menu.\033[0m\n";
        return;
    }

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

    simpanData(*items, *trxs, *routes, *users);

    std::cout << "\033[2J\033[1;1H";
    std::cout << "\033[1;35m========================================\033[0m\n";
    std::cout << "\033[1;32m         PESANAN BERHASIL DIBUAT!       \033[0m\n";
    std::cout << "\033[1;35m========================================\033[0m\n\n";
    std::cout << "  ID Pesanan    : \033[1;36m" << newTrxId << "\033[0m\n";
    std::cout << "  Barang        : " << newTrx.nama_barang << "\n";
    std::cout << "  Jumlah        : " << newTrx.jumlah << " pcs\n";
    std::cout << "  Tipe Beli     : " << newTrx.tipe_beli << "\n";
    std::cout << "  Total Bayar   : \033[1;33mRp" << (long long)newTrx.total_bayar << "\033[0m\n";
    std::cout << "  Status        : \033[1;33m" << newTrx.status << "\033[0m\n\n";
    std::cout << "\033[1;36m  Silakan tunggu konfirmasi Admin, lalu\033[0m\n";
    std::cout << "\033[1;36m  lakukan pembayaran di menu Bayar Pesanan.\033[0m\n\n";
    std::cout << "\033[1;32m[+] Data disimpan ke JSON secara otomatis.\033[0m\n";
}