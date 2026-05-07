#include "shop.h"

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void pauseScreen() {
    std::string dummy;
    std::cout << "\n\033[1;36mTekan Enter untuk kembali...\033[0m";
    std::getline(std::cin, dummy);
}

std::string getValidString() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << "\033[1;31mInput tidak boleh kosong. Masukkan kembali: \033[0m";
            continue;
        }
        bool valid = true;
        for (char c : input) {
            if (!std::isalnum(c) && c != ' ') {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "\033[1;31mKarakter asing tidak diizinkan. Masukkan kembali: \033[0m";
            continue;
        }
        break;
    }
    return input;
}

long long getValidNumber() {
    std::string input;
    long long num = -1;
    while (true) {
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << "\033[1;31mInput tidak boleh kosong. Masukkan kembali: \033[0m";
            continue;
        }
        bool valid = true;
        for (char c : input) {
            if (!std::isdigit(c)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "\033[1;31mHanya angka bulat yang diizinkan. Masukkan kembali: \033[0m";
            continue;
        }
        num = std::stoll(input);
        if (num < 0) {
            std::cout << "\033[1;31mNominal tidak boleh minus. Masukkan kembali: \033[0m";
            continue;
        }
        break;
    }
    return num;
}

int inquirerMenu(std::string title, std::vector<std::string> options) {
    int selected = 0;
    while (true) {
        clearScreen();
        std::cout << "\033[1;35m================================\033[0m\n";
        std::cout << "\033[1;37m   " << title << "   \033[0m\n";
        std::cout << "\033[1;35m================================\033[0m\n";
        std::cout << "\033[1;33mGunakan Panah Atas/Bawah dan Enter\033[0m\n\n";

        for (size_t i = 0; i < options.size(); ++i) {
            if (i == selected) {
                std::cout << "\033[1;36m > " << options[i] << " \033[0m\n";
            } else {
                std::cout << "   " << options[i] << " \n";
            }
        }

        char key = _getch();
        if (key == -32 || key == 224) {
            key = _getch();
            if (key == 72 && selected > 0) {
                selected--;
            } else if (key == 80 && selected < options.size() - 1) {
                selected++;
            }
        } else if (key == 13) {
            return selected;
        }
    }
}

void initData(std::vector<Item>* items, std::vector<Route>* routes, std::vector<Transaction>* trxs) {
    
}

void displayItems(std::vector<Item>* items) {
    if (items->empty()) {
        std::cout << "\n\033[1;31mBelum ada barang yang tersedia. Menunggu Admin.\033[0m\n";
        return;
    }

    tabulate::Table table;
    table.add_row({"Kode Barang", "Nama Barang", "Harga (Rp)", "Stok"});
    table[0].format().font_color(tabulate::Color::yellow).font_style({tabulate::FontStyle::bold}).font_align(tabulate::FontAlign::center);

    for (const auto& it : *items) {
        table.add_row({it.id, it.name, std::to_string(it.price), std::to_string(it.stock)});
    }

    std::cout << "\n\033[1;32m=== DAFTAR BARANG ===\033[0m\n";
    std::cout << table << "\n";
}

void sortItemsAscending(std::vector<Item>* items) {
    if (items->empty()) {
        std::cout << "\n\033[1;31mData barang kosong, tidak ada yang bisa diurutkan.\033[0m\n";
        return;
    }

    for (size_t i = 0; i < items->size() - 1; ++i) {
        for (size_t j = 0; j < items->size() - i - 1; ++j) {
            if ((*items)[j].price > (*items)[j + 1].price) {
                Item temp = (*items)[j];
                (*items)[j] = (*items)[j + 1];
                (*items)[j + 1] = temp;
            }
        }
    }
    std::cout << "\033[1;32mBarang berhasil diurutkan berdasarkan harga termurah.\033[0m\n";
}

void searchItem(std::vector<Item>* items) {
    if (items->empty()) {
        std::cout << "\n\033[1;31mBelum ada barang yang tersedia. Menunggu Admin.\033[0m\n";
        return;
    }

    std::cout << "Masukkan Nama Barang yang dicari: ";
    std::string query = getValidString();
    
    tabulate::Table table;
    table.add_row({"Kode Barang", "Nama Barang", "Harga (Rp)", "Stok"});
    table[0].format().font_color(tabulate::Color::cyan).font_style({tabulate::FontStyle::bold});

    bool found = false;
    for (const auto& it : *items) {
        if (it.name.find(query) != std::string::npos) {
            table.add_row({it.id, it.name, std::to_string(it.price), std::to_string(it.stock)});
            found = true;
        }
    }

    if (found) {
        std::cout << "\n\033[1;32mHasil Pencarian:\033[0m\n" << table << "\n";
    } else {
        std::cout << "\n\033[1;31mBarang tidak ditemukan!\033[0m\n";
    }
}

void orderItem(std::vector<Item>* items, std::vector<Route>* routes, std::vector<Transaction>* trxs) {
    if (items->empty()) {
        std::cout << "\n\033[1;31mBelum ada barang untuk dipesan. Menunggu Admin.\033[0m\n";
        return;
    }

    displayItems(items);
    std::cout << "\nMasukkan Kode Barang yang ingin dibeli: ";
    std::string kode = getValidString();

    Item* selectedItem = nullptr;
    for (auto& it : *items) {
        if (it.id == kode) {
            selectedItem = &it;
            break;
        }
    }

    if (selectedItem == nullptr) {
        std::cout << "\033[1;31mBarang tidak ditemukan.\033[0m\n";
        return;
    }

    std::cout << "Masukkan jumlah beli: ";
    long long qty = getValidNumber();

    if (qty > selectedItem->stock) {
        std::cout << "\033[1;31mStok tidak mencukupi.\033[0m\n";
        return;
    }

    long long subtotal = selectedItem->price * qty;
    long long ongkir = 0;
    std::string tipeBeli = "Ditempat";

    std::vector<std::string> tipeOptions = {"Beli Ditempat", "Diantar"};
    int tipePilihan = inquirerMenu("PILIH TIPE PEMBELIAN", tipeOptions);

    clearScreen();
    std::cout << "\033[1;36mMelanjutkan pesanan: " << selectedItem->name << " (x" << qty << ")\033[0m\n\n";

    if (tipePilihan == 1) {
        if (routes->empty()) {
            std::cout << "\033[1;31mBelum ada rute pengiriman dari Admin. Transaksi dibatalkan.\033[0m\n";
            return;
        }

        tipeBeli = "Diantar";
        tabulate::Table rTable;
        rTable.add_row({"Kode Rute", "Tujuan", "Biaya (Rp)"});
        rTable[0].format().font_color(tabulate::Color::magenta).font_style({tabulate::FontStyle::bold});

        for (const auto& r : *routes) {
            rTable.add_row({r.id, r.destination, std::to_string(r.cost)});
        }
        std::cout << "\033[1;32m=== DAFTAR RUTE PENGIRIMAN ===\033[0m\n";
        std::cout << rTable << "\n\n";
        
        std::cout << "Masukkan Kode Rute Pengiriman: ";
        std::string ruteKode = getValidString();

        bool ruteValid = false;
        for (const auto& r : *routes) {
            if (r.id == ruteKode) {
                ongkir = r.cost;
                ruteValid = true;
                break;
            }
        }

        if (!ruteValid) {
            std::cout << "\033[1;31mRute tidak valid. Transaksi dibatalkan.\033[0m\n";
            return;
        }
    }

    long long grandTotal = subtotal + ongkir;
    selectedItem->stock -= qty;

    std::string newTrxId = "TRX00" + std::to_string(trxs->size() + 1);
    Transaction newTrx = {newTrxId, selectedItem->name, static_cast<int>(qty), grandTotal, tipeBeli, "Menunggu Konfirmasi"};
    trxs->push_back(newTrx);

    std::cout << "\n\033[1;33mTotal Pesanan: Rp" << grandTotal << "\033[0m\n";
    std::cout << "\033[1;32mPesanan berhasil dibuat dengan ID: " << newTrxId << "\033[0m\n";
    std::cout << "\033[1;36mSilakan tunggu konfirmasi Admin sebelum melakukan pembayaran di menu Bayar Pesanan.\033[0m\n";
}

void payOrder(std::vector<Transaction>* trxs) {
    if (trxs->empty()) {
        std::cout << "\033[1;31mBelum ada riwayat pesanan.\033[0m\n";
        return;
    }

    tabulate::Table table;
    table.add_row({"ID Transaksi", "Nama Barang", "Total (Rp)", "Status"});
    table[0].format().font_color(tabulate::Color::blue).font_style({tabulate::FontStyle::bold});

    for (const auto& t : *trxs) {
        table.add_row({t.trxId, t.itemName, std::to_string(t.total), t.status});
    }

    std::cout << "\033[1;32m=== DAFTAR PESANAN ===\033[0m\n";
    std::cout << table << "\n\n";

    std::cout << "Masukkan ID Transaksi yang ingin dibayar: ";
    std::string id = getValidString();

    bool found = false;
    for (auto& t : *trxs) {
        if (t.trxId == id) {
            found = true;
            if (t.status == "Menunggu Konfirmasi") {
                std::cout << "\n\033[1;33mPesanan ini masih menunggu konfirmasi admin.\033[0m\n";
                std::cout << "Simulasikan konfirmasi admin untuk testing? (y/n): ";
                std::string ans;
                std::getline(std::cin, ans);
                if (ans == "y" || ans == "Y") {
                    t.status = "Menunggu Pembayaran";
                    std::cout << "\033[1;32mStatus pesanan berhasil diubah menjadi Menunggu Pembayaran.\033[0m\n";
                } else {
                    return;
                }
            }
            
            if (t.status == "Menunggu Pembayaran") {
                std::cout << "\n\033[1;33mTotal yang harus dibayar: Rp" << t.total << "\033[0m\n";
                std::cout << "Masukkan nominal uang: ";
                long long bayar = getValidNumber();

                while (bayar < t.total) {
                    std::cout << "\033[1;31mUang tidak cukup. Masukkan nominal yang sesuai: \033[0m";
                    bayar = getValidNumber();
                }

                t.status = "Lunas";
                std::cout << "\n\033[1;32mPembayaran berhasil dilakukan. Kembalian: Rp" << (bayar - t.total) << "\033[0m\n";
            } else if (t.status == "Lunas") {
                std::cout << "\n\033[1;36mPesanan ini sudah lunas.\033[0m\n";
            }
            break;
        }
    }

    if (!found) {
        std::cout << "\n\033[1;31mID Transaksi tidak ditemukan.\033[0m\n";
    }
}

void displayHistory(std::vector<Transaction>* trxs) {
    if (trxs->empty()) {
        std::cout << "\033[1;31mBelum ada riwayat transaksi.\033[0m\n";
        return;
    }

    tabulate::Table table;
    table.add_row({"ID Transaksi", "Nama Barang", "Qty", "Total (Rp)", "Tipe", "Status"});
    table[0].format().font_color(tabulate::Color::blue).font_style({tabulate::FontStyle::bold});

    for (const auto& t : *trxs) {
        table.add_row({t.trxId, t.itemName, std::to_string(t.qty), std::to_string(t.total), t.type, t.status});
    }

    std::cout << "\033[1;32m=== RIWAYAT TRANSAKSI ===\033[0m\n";
    std::cout << table << "\n";
}

void userMenu(std::vector<Item>* items, std::vector<Route>* routes, std::vector<Transaction>* trxs) {
    bool running = true;
    std::vector<std::string> menuOptions = {
        "Lihat Barang",
        "Cari Barang",
        "Urutkan Barang (Harga Termurah)",
        "Pesan Barang",
        "Bayar Pesanan",
        "Riwayat Transaksi",
        "Keluar"
    };

    while (running) {
        int pilihan = inquirerMenu("SISTEM TOKO BANGUNAN ZAKI", menuOptions);

        switch (pilihan) {
            case 0:
                clearScreen();
                displayItems(items);
                pauseScreen();
                break;
            case 1:
                clearScreen();
                searchItem(items);
                pauseScreen();
                break;
            case 2:
                clearScreen();
                sortItemsAscending(items);
                displayItems(items);
                pauseScreen();
                break;
            case 3:
                clearScreen();
                orderItem(items, routes, trxs);
                pauseScreen();
                break;
            case 4:
                clearScreen();
                payOrder(trxs);
                pauseScreen();
                break;
            case 5:
                clearScreen();
                displayHistory(trxs);
                pauseScreen();
                break;
            case 6:
                clearScreen();
                std::cout << "\n\033[1;32mTerima kasih telah berbelanja.\033[0m\n";
                running = false;
                break;
        }
    }
}