#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include "login.h"
#include "../validasi.h"
#include "../database_handler.h"

// CATATAN: Kalau temanmu punya file tambahan seperti "data.h" 
// yang berisi fungsi tidur() atau inputYaTidak(), uncomment baris di bawah:
// #include "data.h"

using namespace std;

static void clearScreenLogin() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void tampilHeader() {
    cout << "\n====================================================" << endl;
    cout << "     MOLOTOV - TOKO MATERIAL BANGUNAN ONLINE" << endl;
    cout << "====================================================" << endl;
}

void tampilGaris() {
    cout << "----------------------------------------------------" << endl;
}

void validasiNamaLengkap(const string &nama, const string &field) {
    validasiTidakKosong(nama, field);
    validasiMinPanjang(nama, 3, field);
    validasiMaksPanjang(nama, 50, field);

    bool adaHuruf = false;
    for (int i = 0; i < (int)nama.length(); i++) {
        char c = nama[i];
        if (isalpha(c)) {
            adaHuruf = true;
        } else if (c == ' ') {
            continue;
        } else if (isdigit(c)) {
            throw runtime_error(field + " tidak boleh mengandung angka.");
        } else {
            throw runtime_error(field + " tidak boleh mengandung simbol '" + string(1, c) + "'.");
        }
    }

    if (!adaHuruf)
        throw runtime_error(field + " harus mengandung huruf.");
}

// SUDAH DIUPDATE: Membaca dari vector database_user, bukan array global
bool cekUsernameTerdaftar(const vector<Pengguna>& database_user, const string &username) {
    for (const auto& u : database_user) {
        if (u.username == username)
            return true;
    }
    return false;
}

// SUDAH DIUPDATE: Membaca dari vector database_user, bukan array global
Pengguna* cariPenggunaLogin(vector<Pengguna>& database_user, const string &username, const string &password) {
    for (auto& u : database_user) {
        if (u.username == username && u.password == password)
            return &u;
    }
    return nullptr;
}

// SUDAH DIUPDATE: Menerima 4 database untuk keperluan auto-save JSON
void menuRegister(vector<Pengguna>& database_user, vector<Barang>& db_barang, vector<Pesanan>& db_pesanan, vector<Rute>& db_rute) {
    bool selesai = false;

    while (!selesai) {
        clearScreenLogin();
        tampilHeader();
        cout << "            REGISTRASI AKUN BARU" << endl;
        tampilGaris();

        try {
            string nama, username, password, konfirmasi, telepon, alamat;

            cout << "  Nama Lengkap : ";
            getline(cin, nama);
            validasiNamaLengkap(nama, "Nama Lengkap");

            cout << "  Username     : ";
            getline(cin, username);
            username = trim(username);
            validasiUsername(username, "Username");
            if (cekUsernameTerdaftar(database_user, username))
                throw runtime_error("Username \"" + username + "\" sudah dipakai. Silakan pilih username lain.");

            cout << "  No Telepon   : ";
            getline(cin, telepon);
            telepon = trim(telepon);
            validasiTidakKosong(telepon, "No Telepon");
            for (int i = 0; i < (int)telepon.length(); i++) {
                if (!isdigit(telepon[i]))
                    throw runtime_error("No Telepon hanya boleh berisi angka.");
            }
            if ((int)telepon.length() < 9 || (int)telepon.length() > 13)
                throw runtime_error("No Telepon harus antara 9-13 digit.");

            cout << "  Alamat       : ";
            getline(cin, alamat);
            validasiTidakKosong(alamat, "Alamat");
            validasiMaksPanjang(alamat, 100, "Alamat");

            cout << "  Password     : ";
            getline(cin, password);
            validasiMinPanjang(password, 8, "Password");
            validasiMaksPanjang(password, 30, "Password");

            cout << "  Konfirmasi   : ";
            getline(cin, konfirmasi);
            if (password != konfirmasi)
                throw runtime_error("Konfirmasi password tidak cocok.");

            // 1. TAMBAH USER BARU KE VECTOR
            database_user.push_back({username, password, nama, telepon, alamat, "Customer"});

            // 2. AUTO-SAVE JSON LANGSUNG DI SINI!
            simpanData(db_barang, db_pesanan, db_rute, database_user);

            tampilGaris();
            cout << "  Registrasi berhasil! Silahkan login." << endl;
            cout << "  [+] Data Pengguna di JSON otomatis diperbarui!" << endl;
            
            // Jeda sebentar agar user bisa membaca
            for(int i=0; i<100000000; i++); 
            selesai = true;

        } catch (exception &e) {
            tampilGaris();
            cout << "  [!] " << e.what() << endl;
            tampilGaris();

            cout << "  Coba lagi? (y/n) : ";
            string ans;
            getline(cin, ans);
            if (!ans.empty() && (ans[0] == 'y' || ans[0] == 'Y')) {
                cout << "  Memuat ulang form...\n";
            } else {
                cout << "  Registrasi dibatalkan." << endl;
                selesai = true;
            }
        }
    }
}

// SUDAH DIUPDATE: Menggunakan vector dan setter langsung ke variabel di main.cpp
bool menuLogin(vector<Pengguna>& database_user, string& user_aktif, string& role_aktif) {
    bool selesai = false;

    while (!selesai) {
        clearScreenLogin();
        tampilHeader();
        cout << "                    LOGIN" << endl;
        tampilGaris();

        int percobaan = 3;

        while (percobaan > 0) {
            try {
                string username, password;

                cout << "  Username : ";
                getline(cin, username);
                username = trim(username);

                cout << "  Password : ";
                getline(cin, password);

                validasiTidakKosong(username, "Username");
                validasiTidakKosong(password, "Password");

                Pengguna* p = cariPenggunaLogin(database_user, username, password);
                if (p == nullptr)
                    throw runtime_error("Username atau password salah.");

                // Mengirim data ke main.cpp
                user_aktif = p->username;
                role_aktif = p->role;
                string nama_aktif = p->nama_lengkap;

                tampilGaris();
                cout << "  Login berhasil!" << endl;
                cout << "  Selamat datang, " << nama_aktif << "!" << endl;
                tampilGaris();
                
                // Jeda agar bisa terbaca
                for(int i=0; i<100000000; i++); 
                return true;

            } catch (exception &e) {
                percobaan--;
                cout << "  [!] " << e.what();

                if (percobaan > 0) {
                    cout << " | Sisa percobaan: " << percobaan << endl;
                    tampilGaris();
                } else {
                    cout << endl;
                    tampilGaris();
                    cout << "  Akses ditolak. Terlalu banyak percobaan gagal." << endl;
                    tampilGaris();

                    cout << "  Coba lagi? (y/n) : ";
                    string ans;
                    getline(cin, ans);
                    if (!ans.empty() && (ans[0] == 'y' || ans[0] == 'Y')) {
                        cout << "  Memuat ulang halaman login...\n";
                    } else {
                        cout << "  Kembali ke menu utama." << endl;
                        selesai = true;
                    }
                }
            }
        }
    }
    return false;
}