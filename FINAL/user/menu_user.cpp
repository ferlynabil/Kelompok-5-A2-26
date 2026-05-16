#include "user.h"
#include "../database_handler.h"
#include <iostream>

void userMenu(std::vector<Barang>* items, std::vector<Rute>* routes, std::vector<Pesanan>* trxs, std::vector<Pengguna>* users) {
    bool running = true;

    std::vector<std::string> menuOptions = {
        "Lihat Barang",
        "Pesan Barang",
        "Status Pesanan",
        "Bayar Pesanan",
        "Riwayat Transaksi",
        "Keluar"
    };

    while (running) {
        int pilihan = inquirerMenuUser("SISTEM TOKO BANGUNAN", menuOptions);

        switch (pilihan) {
            case 0:
                clearScreenUser();
                displayItems(items);
                break;
            case 1:
                clearScreenUser();
                orderItem(items, routes, trxs, users);
                pauseScreenUser();
                break;
            case 2:
                clearScreenUser();
                cekStatusPesanan(trxs);
                break;
            case 3:
                clearScreenUser();
                payOrder(trxs, items, routes, users);
                pauseScreenUser();
                break;
            case 4:
                clearScreenUser();
                displayHistory(trxs);
                pauseScreenUser();
                break;
            case 5:
                clearScreenUser();
                std::cout << "\n\033[1;32mTerima kasih telah berbelanja!\033[0m\n";
                running = false;
                break;
        }
    }
}