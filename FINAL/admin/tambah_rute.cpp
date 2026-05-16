#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

// 1. UPDATE PARAMETER: Tambahkan db_barang, db_pesanan, dan db_user
void tambahRute(vector<Rute>& daftar_rute, vector<Barang>& db_barang, vector<Pesanan>& db_pesanan, vector<Pengguna>& db_user) {
    char tambah_lagi;
    
    // ==========================================
    // --- FUNGSI VALIDASI LOKAL SUPER KETAT ---
    // ==========================================
    
    // 1. Validasi Tujuan (Tanpa angka, tanpa simbol aneh)
    auto inputTujuanValid = []() -> string {
        string in;
        while (true) {
            cout << "Tujuan Kota   : "; 
            getline(cin, in);
            
            if (in == "0") return in; // Tombol kembali
            
            if (in.empty() || in.find_first_not_of(' ') == string::npos) {
                cout << "  \033[1;31m[-] Error: Tujuan kota tidak boleh kosong!\033[0m\n";
                continue;
            }
            
            bool valid = true;
            for (char c : in) {
                if (isdigit(c)) { valid = false; break; } // Cegah angka
                if (!isalpha(c) && c != ' ' && c != '-' && c != '\'') { valid = false; break; } // Cegah simbol aneh
            }
            
            if (!valid) {
                cout << "  \033[1;31m[-] Error: Nama kota hanya boleh berisi huruf dan spasi!\033[0m\n";
                continue;
            }
            return in;
        }
    };

    // 2. Validasi Jarak (Angka desimal valid)
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
                cout << "  \033[1;31m[-] Error: Jarak harus angka valid (tanpa huruf, contoh: 12 atau 15.5)!\033[0m\n";
                continue;
            }
            
            try { 
                double jarak = stod(in); 
                if (jarak <= 0) throw 1; 
                return jarak;
            } catch(...) { 
                cout << "  \033[1;31m[-] Error: Jarak harus lebih dari 0!\033[0m\n"; 
            }
        }
    };

    // 3. Validasi Harga (Angka murni)
    auto inputHargaValid = [](const string& nama_layanan) -> double {
        string in;
        while (true) {
            cout << "> Harga " << nama_layanan << " : Rp ";
            getline(cin, in);
            
            if (in.empty() || in == "0") return 0; // Jika dikosongi atau 0, anggap layanan tidak ada
            
            bool valid = true;
            for (char c : in) {
                if (!isdigit(c)) { valid = false; break; }
            }
            
            if (!valid) {
                cout << "  \033[1;31m[-] Error: Harga hanya boleh berisi angka (tanpa titik, spasi, atau huruf)!\033[0m\n";
                continue;
            }
            return stod(in);
        }
    };

    // 4. Validasi Estimasi (Tanpa huruf, otomatis tambah kata "Hari")
    auto inputEstimasiValid = [](const string& prompt) -> string {
        string in;
        while (true) {
            cout << prompt;
            getline(cin, in);
            
            if (in.empty() || in.find_first_not_of(' ') == string::npos) {
                cout << "  \033[1;31m[-] Error: Estimasi tidak boleh kosong!\033[0m\n";
                continue;
            }
            
            bool adaHuruf = false;
            bool adaAngka = false;
            for (char c : in) {
                if (isalpha(c)) adaHuruf = true;
                if (isdigit(c)) adaAngka = true;
            }
            
            if (adaHuruf || !adaAngka) {
                cout << "  \033[1;31m[-] Error: Masukkan rentang angkanya saja (misal: 3-5 atau 1). Tidak boleh ada huruf!\033[0m\n";
                continue;
            }
            
            return in + " Hari"; // Otomatis menambahkan kata "Hari" ke database
        }
    };
    // ==========================================

    do {
        system("cls");
        cout << "\n=== TAMBAH RUTE PENGIRIMAN (DARI SAMARINDA) ===\n";
        cout << "Ketik '0' pada Tujuan Kota untuk batal dan kembali ke menu.\n\n";
        
        // 1. GENERATE ID RUTE OTOMATIS
        int next_id = daftar_rute.size() + 1;
        string id_otomatis = "R" + string(next_id < 10 ? "0" : "") + to_string(next_id);
        cout << "ID Rute       : " << id_otomatis << " (Otomatis)\n";

        // 2. INPUT TUJUAN KOTA 
        string input_tujuan = inputTujuanValid();
        if (input_tujuan == "0") return; // Keluar dari fungsi

        // 3. INPUT JARAK 
        double jarak_km = inputJarakValid();

        // 4. INPUT 3 JENIS HARGA & ESTIMASI WAKTU
        double h_reguler = 0, h_standar = 0, h_premium = 0;
        string est_reguler = "-", est_standar = "-", est_premium = "-";

        while (true) {
            cout << "\n--- Input Harga & Estimasi Pengiriman ---\n";
            cout << "(Tekan Enter langsung atau ketik '0' pada harga jika layanan TIDAK TERSEDIA)\n\n";

            // --- REGULER ---
            h_reguler = inputHargaValid("Reguler");
            if (h_reguler > 0) est_reguler = inputEstimasiValid("  Estimasi (hari, misal: 3-5) : ");

            // --- STANDAR ---
            h_standar = inputHargaValid("Standar");
            if (h_standar > 0) est_standar = inputEstimasiValid("  Estimasi (hari, misal: 2-3) : ");

            // --- PREMIUM ---
            h_premium = inputHargaValid("Premium");
            if (h_premium > 0) est_premium = inputEstimasiValid("  Estimasi (hari, misal: 1)   : ");

            // Validasi Akhir: Minimal harus ada 1 jenis pengiriman
            if (h_reguler <= 0 && h_standar <= 0 && h_premium <= 0) {
                cout << "\n\033[1;31m[-] Error: Rute ini tidak punya satupun layanan pengiriman yang aktif!\033[0m\n";
                cout << "\033[1;33m[!] Silakan isi minimal 1 harga layanan.\033[0m\n";
                continue; 
            }
            break; 
        }
        
        // 5. MASUKKAN KE DALAM STRUCT RUTE
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
        
        // Simpan ke vector
        daftar_rute.push_back(rute_baru);

        // AUTO-SAVE JSON
        simpanData(db_barang, db_pesanan, daftar_rute, db_user);

        cout << "\n\033[1;32m[+] Rute Samarinda -> " << rute_baru.tujuan << " berhasil disimpan!\033[0m\n";
        cout << "[+] Data JSON otomatis diperbarui!\n";
        
        // Looping Tambah Lagi
        while (true) {
            cout << "\nTambah rute lain? (y/n): ";
            string ans; getline(cin, ans);
            if (ans.empty() || ans.find_first_not_of(' ') == string::npos) continue;
            tambah_lagi = ans[0];
            if (tambah_lagi == 'y' || tambah_lagi == 'Y' || tambah_lagi == 'n' || tambah_lagi == 'N') break;
            cout << "  \033[1;31m[-] Masukkan 'y' atau 'n'.\033[0m";
        }
        
    } while (tambah_lagi == 'y' || tambah_lagi == 'Y');
}