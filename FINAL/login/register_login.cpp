#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include "login.h"
#include "../validasi.h"
#include "../database_handler.h"

using namespace std;

// Bersihkan layar agar tampilan tidak menumpuk
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

// Validasi nama lengkap: tidak boleh kosong, tidak boleh ada angka/simbol
void validasiNamaLengkap(const string &nama, const string &field) {

    validasiTidakKosong(nama, field);
    validasiMaksPanjang(nama, 50, field);

    bool adaHuruf = false;

    for (int i = 0; i < (int)nama.length(); i++) {

        char c = nama[i];

        if (isalpha(c)) {

            adaHuruf = true; 

        } 
        else if (c == ' ') {

            continue;     

        } 
        else if (isdigit(c)) {

            throw runtime_error(field + " tidak boleh mengandung angka.");

        } 
        else {

            throw runtime_error(
                field + " tidak boleh mengandung simbol '" 
                + string(1, c) + "'."
            );

        }
    }

    if (!adaHuruf) {

        throw runtime_error(field + " harus mengandung huruf.");

    }

    validasiMinPanjang(nama, 3, field);
}


// Cek apakah username sudah dipakai di database
bool cekUsernameTerdaftar(const vector<Pengguna>& database_user, const string &username) {

    for (const auto& u : database_user) {

        if (u.username == username)
            return true;   
    }

    return false; 
}

// Cari pengguna berdasarkan username + password untuk login
Pengguna* cariPenggunaLogin(vector<Pengguna>& database_user,
                            const string &username,
                            const string &password) {

    for (auto& u : database_user) {

        if (u.username == username && u.password == password)
            return &u;  // cocok, kembalikan pointer ke data pengguna
    }

    return nullptr;  // tidak ditemukan
}

// Menu Register untuk buat akun baru, dengan validasi input dan simpan ke database
void menuRegister(vector<Pengguna>& database_user,
                  vector<Barang>& db_barang,
                  vector<Pesanan>& db_pesanan,
                  vector<Rute>& db_rute) {

    bool selesai = false;

    while (!selesai) {

        clearScreenLogin();

        tampilHeader();
        cout << "            REGISTRASI AKUN BARU" << endl;
        tampilGaris();

        try {

            string nama;
            string username;
            string password;
            string konfirmasi;
            string telepon;
            string alamat;

            cout << "  Nama Lengkap : ";
            getline(cin, nama);

            validasiNamaLengkap(nama, "Nama Lengkap");

            cout << "  Username     : ";
            getline(cin, username);

            username = trim(username);

            validasiTidakKosong(username, "Username");
            validasiUsername(username, "Username");


            // Cek username tidak boleh duplikat
            if (cekUsernameTerdaftar(database_user, username)) {

                throw runtime_error(
                    "Username \"" + username + "\" sudah dipakai. Silakan pilih username lain."
                );
            }

            cout << "  No Telepon   : ";
            getline(cin, telepon);

            telepon = trim(telepon); 

            validasiTidakKosong(telepon, "No Telepon");
            
            // Telepon hanya boleh angka 
            for (int i = 0; i < (int)telepon.length(); i++) {

                if (!isdigit(telepon[i])) {

                    throw runtime_error(
                        "No Telepon hanya boleh berisi angka."
                    );
                }
            }
             
             // Panjang nomor telepon harus 9-13 digit
            if ((int)telepon.length() < 9 || (int)telepon.length() > 13) {

                throw runtime_error(
                    "No Telepon harus antara 9-13 digit."
                );
            }

            cout << "  Alamat       : ";
            getline(cin, alamat);

            validasiTidakKosong(alamat, "Alamat");
            validasiMaksPanjang(alamat, 100, "Alamat");

            cout << "  Password     : ";
            getline(cin, password);

            validasiTidakKosong(password, "Password");
            validasiMinPanjang(password, 8, "Password");
            validasiMaksPanjang(password, 30, "Password");

            cout << "  Konfirmasi   : ";
            getline(cin, konfirmasi);

            // Password dan konfirmasi harus sama
            if (password != konfirmasi) {

                throw runtime_error(
                    "Konfirmasi password tidak cocok."
                );
            }
            
            // Semua valid, tambahkan user baru ke database dengan role Customer
            database_user.push_back({
                username,
                password,
                nama,
                telepon,
                alamat,
                "Customer"
            });

            // Simpan perubahan ke file JSON
            simpanData(
                db_barang,
                db_pesanan,
                db_rute,
                database_user
            );

            tampilGaris();

            cout << "  Registrasi berhasil! Silahkan login." << endl;
            cout << "  [+] Data Pengguna di JSON otomatis diperbarui!" << endl;

            for(int i = 0; i < 100000000; i++);  // delay singkat sebelum kembali ke menu utama

            selesai = true;

        } 
        catch (exception &e) {

            // Jika ada error validasi, tampilkan pesan dan tanya mau coba lagi atau tidak
            tampilGaris();

            cout << "  [!] " << e.what() << endl;

            tampilGaris();

            string ans;

            while (true) {

                cout << "  Coba lagi? (y/n) : ";
                getline(cin, ans);

                if (ans == "y" || ans == "Y") {

                    cout << "  Memuat ulang form...\n";
                    break;

                } 
                else if (ans == "n" || ans == "N") {

                    cout << "  Registrasi dibatalkan." << endl;

                    selesai = true;
                    break;

                } 
                else {

                    cout << "  Input hanya boleh y atau n.\n";

                }
            }
        }
    }
}


// Menu login user dengan validasi input dan cek ke database
bool menuLogin(vector<Pengguna>& database_user,
               string& user_aktif,
               string& role_aktif) {

    bool selesai = false;

    while (!selesai) {

        clearScreenLogin();

        tampilHeader();

        cout << "                    LOGIN" << endl;

        tampilGaris();

        int percobaan = 3;  // batas maksimal salah input

        while (percobaan > 0) {

            try {

                string username;
                string password;

                cout << "  Username : ";
                getline(cin, username);

                username = trim(username);

                cout << "  Password : ";
                getline(cin, password);

                validasiTidakKosong(username, "Username");
                validasiTidakKosong(password, "Password");

                // Cari user yang cocok di database
                Pengguna* p = cariPenggunaLogin(
                    database_user,
                    username,
                    password
                );

                if (p == nullptr) { // tidak ditemukan, login gagal

                    throw runtime_error(
                        "Username atau password salah."
                    );
                }

                // Login berhasil, simpan info user yang aktif
                user_aktif = p->username;
                role_aktif = p->role;

                string nama_aktif = p->nama_lengkap;

                tampilGaris();

                cout << "  Login berhasil!" << endl;
                cout << "  Selamat datang, " << nama_aktif << "!" << endl;

                tampilGaris();

                for(int i = 0; i < 100000000; i++);  // delay singkat sebelum masuk ke menu utama

                return true;

            } 
            catch (exception &e) {

                percobaan--;

                cout << "  [!] " << e.what();

                if (percobaan > 0) {

                    // Masih ada sisa percobaan, langsung ulang
                    cout << " | Sisa percobaan: "
                         << percobaan << endl;

                    tampilGaris();

                } 
                else {

                    // Percobaan habis, tanya mau coba lagi dari awal atau keluar
                    cout << endl;

                    tampilGaris();

                    cout << "  Akses ditolak. Terlalu banyak percobaan gagal." << endl;

                    tampilGaris();

                    string ans;

                    while (true) {

                        cout << "  Coba lagi? (y/n) : ";
                        getline(cin, ans);

                        if (ans == "y" || ans == "Y") {

                            cout << "  Memuat ulang halaman login...\n";
                            break; // reset percobaan dari awal

                        } 
                        else if (ans == "n" || ans == "N") {

                            cout << "  Kembali ke menu utama." << endl;

                            selesai = true;
                            break; // keluar loop, kembali ke menu utama

                        } 
                        else {

                            cout << "  Input hanya boleh y atau n.\n";

                        }
                    }
                }
            }
        }
    }

    return false;  // login tidak berhasil
}