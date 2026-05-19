#include <iostream>
#include <vector>
#include <string>
#include "struktur_data.h"

#include "admin/admin.h"
#include "user/user.h"
#include "login/login.h"


#include "database_handler.h" 

using namespace std;

int main() {
    vector<Barang> database_barang;
    vector<Pesanan> database_pesanan;
    vector<Rute> database_rute;
    vector<Pengguna> database_user;

  
    muatData(database_barang, database_pesanan, database_rute, database_user);

    
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

    
        string input;
        getline(cin, input);

  
        if (input != "0" && input != "1" && input != "2") {
            cout << "\n[!] Pilihan tidak valid!" << endl;
            system("pause");
            continue;
        }

        int pil = stoi(input);

        if (pil == 1) {


            if (menuLogin(database_user, user_aktif, role_aktif)) {

                if (role_aktif == "Admin") {


                    menuAdmin(database_barang,
                              database_pesanan,
                              database_rute,
                              database_user);

                } else {


                    userMenu(&database_barang,
                             &database_rute,
                             &database_pesanan,
                             &database_user);
                }


                simpanData(database_barang,
                           database_pesanan,
                           database_rute,
                           database_user);
            }

        } else if (pil == 2) {

            menuRegister(database_user,
                         database_barang,
                         database_pesanan,
                         database_rute);


            simpanData(database_barang,
                       database_pesanan,
                       database_rute,
                       database_user);

        } else if (pil == 0) {


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