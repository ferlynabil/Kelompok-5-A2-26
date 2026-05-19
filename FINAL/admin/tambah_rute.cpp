#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

void tambahRute(vector<Rute>& daftar_rute, vector<Barang>& db_barang,
                vector<Pesanan>& db_pesanan, vector<Pengguna>& db_user) { 

    char tambah_lagi;

    // Lambda: input nama kota, hanya huruf dan spasi, ketik '0' untuk batal
    auto inputTujuanValid = []() -> string {
        string in;
        while (true) {
            cout << "Tujuan Kota   : ";
            getline(cin, in);
            if (in == "0") return in;
            if (in.empty() || in.find_first_not_of(' ') == string::npos) {
                cout << "  \033[1;31m[-] Error: Tujuan kota tidak boleh kosong!\033[0m\n";
                continue;
            }
            bool valid = true;
            for (char c : in) {
                if (isdigit(c) || (!isalpha(c) && c != ' ' && c != '-' && c != '\'')) {
                    valid = false; break;
                }
            }
            if (!valid) {
                cout << "  \033[1;31m[-] Error: Nama kota hanya boleh berisi huruf dan spasi!\033[0m\n";
                continue;
            }
            return in; // valid, kembalikan nama kota tujuan
        }
    };

    // Lambda: input jarak dalam km, harus angka positif, boleh desimal
    auto inputJarakValid = []() -> double {
        string in;
        while (true) {
            cout << "Jarak (km)    : ";
            getline(cin, in);
            if (in.empty() || in.find_first_not_of(' ') == string::npos) {
                cout << "  \033[1;31m[-] Error: Jarak tidak boleh kosong!\033[0m\n";
                continue;
            }
            bool valid_angka = true;
            int jumlah_titik = 0;
            for (char c : in) {
                if (c == '.') jumlah_titik++;
                else if (!isdigit(c)) valid_angka = false;
            }
            if (!valid_angka || jumlah_titik > 1) {
                cout << "  \033[1;31m[-] Error: Jarak harus angka valid!\033[0m\n"; 
                continue; 
            }
            try {
                double jarak = stod(in);
                if (jarak <= 0) throw 1;
                return jarak;
            } catch (...) {
                cout << "  \033[1;31m[-] Error: Jarak harus lebih dari 0!\033[0m\n";
            }
        }
    };

    // Lambda: input harga layanan, return 0 jika layanan tidak tersedia
    auto inputHargaValid = [](const string& nama_layanan) -> double {
        string in;
        while (true) {
            cout << "> Harga " << nama_layanan << " : Rp ";
            getline(cin, in);
            if (in.empty() || in == "0") return 0; // 0 = layanan tidak tersedia
            bool valid = true;
            for (char c : in) {
                if (!isdigit(c)) { valid = false; break; }
            }
            if (!valid) {
                cout << "  \033[1;31m[-] Error: Harga hanya boleh berisi angka!\033[0m\n";
                continue;
            }
            return stod(in); // valid, kembalikan harga sebagai double
        }
    };

    // Lambda: input estimasi hari pengiriman, hanya angka (misal: 3-5)
    auto inputEstimasiValid = [](const string& prompt) -> string {
        string in;
        while (true) {
            cout << prompt;
            getline(cin, in);
            if (in.empty() || in.find_first_not_of(' ') == string::npos) {
                cout << "  \033[1;31m[-] Error: Estimasi tidak boleh kosong!\033[0m\n";
                continue;
            }
            bool adaHuruf = false, adaAngka = false;
            for (char c : in) {
                if (isalpha(c)) adaHuruf = true;
                if (isdigit(c)) adaAngka = true;
            }
            if (adaHuruf || !adaAngka) {
                cout << "  \033[1;31m[-] Error: Masukkan rentang angkanya saja (misal: 3-5)!\033[0m\n";
                continue;
            }
            return in + " Hari"; // tambahkan kata "Hari" di belakang otomatis
        }
    };

   
    do {
        system("cls");
        cout << "\n=== TAMBAH RUTE PENGIRIMAN (DARI SAMARINDA) ===\n";
        cout << "Ketik '0' pada Tujuan Kota untuk batal dan kembali ke menu.\n\n";

        // ID rute dibuat otomatis berdasarkan jumlah rute yang sudah ada
        int next_id = daftar_rute.size() + 1;
        string id_otomatis = "R" + string(next_id < 10 ? "0" : "") + to_string(next_id);
        cout << "ID Rute       : " << id_otomatis << " (Otomatis)\n";

        string input_tujuan = inputTujuanValid();
        if (input_tujuan == "0") return; // batal, keluar fungsi lalu kembali ke menuAdmin

        double jarak_km = inputJarakValid();

        // Input harga dan estimasi untuk tiap layanan
        double h_reguler = 0, h_standar = 0, h_premium = 0;
        string est_reguler = "-", est_standar = "-", est_premium = "-";

        while (true) {
            cout << "\n--- Input Harga & Estimasi Pengiriman ---\n";
            cout << "(Tekan Enter langsung atau ketik '0' jika layanan TIDAK TERSEDIA)\n\n";

            h_reguler = inputHargaValid("Reguler");
            if (h_reguler > 0) est_reguler = inputEstimasiValid("  Estimasi (hari, misal: 3-5) : ");

            h_standar = inputHargaValid("Standar");
            if (h_standar > 0) est_standar = inputEstimasiValid("  Estimasi (hari, misal: 2-3) : ");

            h_premium = inputHargaValid("Premium");
            if (h_premium > 0) est_premium = inputEstimasiValid("  Estimasi (hari, misal: 1)   : ");

            // Minimal 1 layanan harus aktif, tidak boleh semua 0
            if (h_reguler <= 0 && h_standar <= 0 && h_premium <= 0) {
                cout << "\n\033[1;31m[-] Error: Rute ini tidak punya satupun layanan pengiriman yang aktif!\033[0m\n";
                cout << "\033[1;33m[!] Silakan isi minimal 1 harga layanan.\033[0m\n";
                continue;
            }
            break; // keluar loop input harga, lanjut ke simpan data
        }

        // Isi data rute baru dan simpan ke list + file JSON
        Rute rute_baru;
        rute_baru.id_rute = id_otomatis;
        rute_baru.kota_asal = "Samarinda";
        rute_baru.tujuan = input_tujuan;
        rute_baru.jarak = jarak_km;
        rute_baru.biaya_reguler = h_reguler;
        rute_baru.estimasi_reguler = est_reguler;
        rute_baru.biaya_standar = h_standar;
        rute_baru.estimasi_standar = est_standar;
        rute_baru.biaya_premium = h_premium;
        rute_baru.estimasi_premium = est_premium;

        daftar_rute.push_back(rute_baru);
        simpanData(db_barang, db_pesanan, daftar_rute, db_user);

        cout << "\n\033[1;32m[+] Rute Samarinda -> " << rute_baru.tujuan << " berhasil disimpan!\033[0m\n";
        cout << "[+] Data JSON otomatis diperbarui!\n";

        // Tanya mau tambah rute lagi atau tidak
        while (true) {
            cout << "\nTambah rute lain? (y/n): ";
            string ans; getline(cin, ans);
            if (ans.empty() || ans.find_first_not_of(' ') == string::npos) continue;
            tambah_lagi = ans[0];
            if (tambah_lagi == 'y' || tambah_lagi == 'Y' || tambah_lagi == 'n' || tambah_lagi == 'N') break;
            cout << "  \033[1;31m[-] Masukkan 'y' atau 'n'.\033[0m";
        }

    } while (tambah_lagi == 'y' || tambah_lagi == 'Y'); // ulang jika user ingin tambah rute lagi
}