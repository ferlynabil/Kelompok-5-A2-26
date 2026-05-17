#include <iostream>
#include <vector>
#include <string>
#include "struktur_data.h"

// --- INI BAGIAN YANG DIUBAH (TAMBAH NAMA FOLDER) ---
#include "admin/admin.h"
#include "user/user.h"
#include "login/login.h"
// ---------------------------------------------------

#include "database_handler.h" // Pakai handler baru

using namespace std;

int main() {
    vector<Barang> database_barang;
    vector<Pesanan> database_pesanan;
    vector<Rute> database_rute;
    vector<Pengguna> database_user;

    // 1. MUAT DATA SAAT START
    muatData(database_barang, database_pesanan, database_rute, database_user);

    // Default user jika JSON kosong
    if (database_user.empty()) {
        database_user.push_back({"Admin", "123", "Admin Gudang", "0811", "Samarinda", "Admin"});
        database_user.push_back({"Nuron", "037", "Nuron", "0812", "Samarinda", "Customer"});
    }

    string user_aktif, role_aktif;
    bool berjalan = true;

    while (berjalan) {

        system("cls");

        cout << "========================================" << endl;
        cout << "     SISTEM CARGO & TOKO BANGUNAN       " << endl;
        cout << "========================================" << endl;
        cout << "1. Login\n2. Register\n0. Matikan Program\nPilih: ";

        // VALIDASI INPUT MENU
        string input;
        getline(cin, input);

        // Hanya menerima 0,1,2
        if (input != "0" && input != "1" && input != "2") {
            cout << "\n[!] Pilihan tidak valid!" << endl;
            system("pause");
            continue;
        }

        int pil = stoi(input);

        if (pil == 1) {

            // LOGIN
            if (menuLogin(database_user, user_aktif, role_aktif)) {

                if (role_aktif == "Admin") {

                    // MENU ADMIN
                    menuAdmin(database_barang,
                              database_pesanan,
                              database_rute,
                              database_user);

                } else {

                    // MENU USER
                    userMenu(&database_barang,
                             &database_rute,
                             &database_pesanan,
                             &database_user);
                }

                // AUTO SAVE SETELAH LOGOUT
                simpanData(database_barang,
                           database_pesanan,
                           database_rute,
                           database_user);
            }

        } else if (pil == 2) {

            // REGISTER
            menuRegister(database_user,
                         database_barang,
                         database_pesanan,
                         database_rute);

            // AUTO SAVE SETELAH REGISTER
            simpanData(database_barang,
                       database_pesanan,
                       database_rute,
                       database_user);

        } else if (pil == 0) {

            // SAVE SEBELUM EXIT
            simpanData(database_barang,
                       database_pesanan,
                       database_rute,
                       database_user);

            cout << "\n[+] Data berhasil disimpan. Mematikan program...\n";

            berjalan = false;
        }
    }

    return 0;
}