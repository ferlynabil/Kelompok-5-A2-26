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
        
        int pil;
        if (!(cin >> pil)) { cin.clear(); string d; getline(cin, d); continue; }
        cin.ignore();

        if (pil == 1) {
            // menuLogin parameternya nggak berubah, tetap 3
            if (menuLogin(database_user, user_aktif, role_aktif)) {
                if (role_aktif == "Admin") {
                    // SUDAH DIUPDATE: Tambah database_user
                    menuAdmin(database_barang, database_pesanan, database_rute, database_user);
                } else {
                    // SUDAH DIUPDATE: Tambah &database_user
                    userMenu(&database_barang, &database_rute, &database_pesanan, &database_user);
                }
                // 2. AUTO-SAVE SETELAH LOGOUT (Safety Net)
                simpanData(database_barang, database_pesanan, database_rute, database_user);
            }
        } else if (pil == 2) {
            // SUDAH DIUPDATE: Tambah 3 database lainnya sesuai urutan di login.h
            menuRegister(database_user, database_barang, database_pesanan, database_rute);
            
            // 3. AUTO-SAVE SETELAH REGISTRASI (Safety Net tambahan)
            simpanData(database_barang, database_pesanan, database_rute, database_user);
        } else if (pil == 0) {
            // 4. FINAL SAVE SEBELUM MATI
            simpanData(database_barang, database_pesanan, database_rute, database_user);
            cout << "\n[+] Data berhasil disimpan. Mematikan program...\n";
            berjalan = false;
        }
    }
    return 0;
}