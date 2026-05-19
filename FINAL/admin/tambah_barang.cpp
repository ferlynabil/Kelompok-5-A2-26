#include <iostream>
#include <string>
#include <cctype>
#include <conio.h>
#include <vector>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

void tambahBarang(vector<Barang>& daftar_barang, vector<Pesanan>& db_pesanan,
                  vector<Rute>& db_rute, vector<Pengguna>& db_user) {

    char tambah_lagi;
    static bool first_run = true; // flag untuk bersihkan buffer cin di pertama kali masuk

    do {
        Barang barang_baru;
        system("cls");
        cout << "\n=== TAMBAH BARANG ===\n";
        cout << "[Ketik '0' pada Nama Barang untuk Batal dan Kembali]\n\n";

        cout << "Nama Barang : ";

        // Bersihkan sisa newline di buffer hanya saat pertama masuk fungsi
        if (first_run) {
            string dummy;
            if (cin.peek() == '\n') getline(cin, dummy);
            first_run = false;
        }

        getline(cin, barang_baru.nama_barang);

        if (barang_baru.nama_barang == "0") {
            cout << "\n\033[1;33mDibatalkan. Kembali ke menu admin...\033[0m\n";
            break;
        }

        if (barang_baru.nama_barang.empty()) {
            cout << "\n[-] Error: Nama Barang tidak boleh kosong!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans; getline(cin, ans);
            tambah_lagi = ans.empty() ? 'n' : ans[0];
            continue; 
        }


        vector<string> list_kategori = {"Ringan (RGN)", "Sedang (SDG)", "Berat (BRT)"};
        vector<string> prefix_kategori = {"RGN", "SDG", "BRT"};
        int posisi_kategori = 0;
        bool memilih_kat = true;

        while (memilih_kat) {
            system("cls");
            cout << "\n=== TAMBAH BARANG ===\n";
            cout << "Nama Barang : " << barang_baru.nama_barang << "\n";
            cout << "\nPilih Kategori Berat (Gunakan Panah Atas/Bawah & Enter):\n\n";

            for (int i = 0; i < list_kategori.size(); i++) {
                if (i == posisi_kategori)
                    cout << "  > \033[1;36m" << list_kategori[i] << "\033[0m <\n";
                else
                    cout << "    " << list_kategori[i] << "\n";
            }

            char tombol_kat = _getch();
            if (tombol_kat == 72 && posisi_kategori > 0) posisi_kategori--;
            else if (tombol_kat == 80 && posisi_kategori < list_kategori.size() - 1) posisi_kategori++;
            else if (tombol_kat == '\r') memilih_kat = false; 
        }

        // Buat ID otomatis berdasarkan prefix kategori + urutan
        string prefix_pilihan = prefix_kategori[posisi_kategori];
        int urutan = 1;
        for (const auto& b : daftar_barang) {
            if (b.id_barang.length() >= 3 && b.id_barang.substr(0, 3) == prefix_pilihan)
                urutan++; // hitung sudah berapa barang dengan prefix yang sama
        }
        string str_urutan = to_string(urutan);
        while (str_urutan.length() < 3) str_urutan = "0" + str_urutan; // padding jadi 3 digit
        barang_baru.id_barang = prefix_pilihan + "-" + str_urutan;

        // Input harga
        system("cls");
        cout << "\n=== TAMBAH BARANG ===\n";
        cout << "Nama Barang : " << barang_baru.nama_barang << "\n";
        cout << "Kategori    : " << list_kategori[posisi_kategori] << "\n";
        cout << "ID Otomatis : \033[1;32m" << barang_baru.id_barang << "\033[0m\n\n";

        string input_harga;
        bool harga_valid = true;
        cout << "Harga (Rp) (hanya angka) : ";
        getline(cin, input_harga);

        if (input_harga.empty()) {
            cout << "\n[-] Error: Harga tidak boleh kosong!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans; getline(cin, ans);
            tambah_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }

        for (char c : input_harga) {
            if (!isdigit(c)) { harga_valid = false; break; }
        }

        if (!harga_valid) {
            cout << "\n[-] Error: Harga hanya boleh berisi angka!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans; getline(cin, ans);
            tambah_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }
        barang_baru.harga = stod(input_harga); 

     
        // Pilihan satuan
        vector<string> list_satuan = {"Kilogram (kg)", "Gram (g)", "Liter (L)", "Buah/Pcs", "Sak", "Meter (m)"};
        int posisi_satuan = 0;
        bool memilih = true;

        while (memilih) {
            system("cls");
            cout << "\n=== TAMBAH BARANG ===\n";
            cout << "ID Barang   : " << barang_baru.id_barang << "\n";
            cout << "Nama Barang : " << barang_baru.nama_barang << "\n";
            cout << "Harga (Rp)  : " << input_harga << "\n";
            cout << "\nPilih Satuan (Gunakan Panah Atas/Bawah & Enter):\n\n";

            for (int i = 0; i < list_satuan.size(); i++) {
                if (i == posisi_satuan)
                    cout << "  > \033[1;32m" << list_satuan[i] << "\033[0m <\n";
                else
                    cout << "    " << list_satuan[i] << "\n";
            }

            char tombol = _getch();
            if (tombol == 72 && posisi_satuan > 0) posisi_satuan--;
            else if (tombol == 80 && posisi_satuan < list_satuan.size() - 1) posisi_satuan++;
            else if (tombol == '\r') {
                barang_baru.satuan = list_satuan[posisi_satuan];
                memilih = false; 
            }
        }

        // Input berat per satuan
        system("cls");
        cout << "\n=== TAMBAH BARANG ===\n";
        cout << "ID Barang   : " << barang_baru.id_barang << "\n";
        cout << "Nama Barang : " << barang_baru.nama_barang << "\n";
        cout << "Harga (Rp)  : " << input_harga << "\n";
        cout << "Satuan      : " << barang_baru.satuan << "\n";

        string input_berat;
        bool berat_valid = true;
        cout << "Berat/Jumlah per harga (hanya angka) : ";
        getline(cin, input_berat);

        if (input_berat.empty()) {
            cout << "\n[-] Error: Berat/Jumlah tidak boleh kosong!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans; getline(cin, ans);
            tambah_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }

        for (char c : input_berat) { 
            if (!isdigit(c)) { berat_valid = false; break; }
        }

        if (!berat_valid) {
            cout << "\n[-] Error: Berat/Jumlah hanya boleh berisi angka!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans; getline(cin, ans);
            tambah_lagi = ans.empty() ? 'n' : ans[0];
            continue; 
        }
        barang_baru.berat = stod(input_berat);

        // Input stok awal 
        string input_stok;
        bool stok_valid = true;
        cout << "Stok Awal  (hanya angka) : ";
        getline(cin, input_stok);

        if (input_stok.empty()) {
            cout << "\n[-] Error: Stok tidak boleh kosong!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans; getline(cin, ans);
            tambah_lagi = ans.empty() ? 'n' : ans[0]; 
            continue;
        }

        for (char c : input_stok) {
            if (!isdigit(c)) { stok_valid = false; break; } 
        }

        if (!stok_valid) {
            cout << "\n[-] Error: Stok hanya boleh berisi angka!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans; getline(cin, ans);
            tambah_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }
        barang_baru.stok = stoi(input_stok); 

        daftar_barang.push_back(barang_baru);
        simpanData(daftar_barang, db_pesanan, db_rute, db_user);

        cout << "\n[+] Berhasil menambahkan " << barang_baru.nama_barang
             << " dengan ID " << barang_baru.id_barang << " ke gudang!\n";
        cout << "[+] Data Gudang JSON otomatis diperbarui!\n";

        cout << "\nIngin tambah barang lagi? (y/n): ";
        string ans; getline(cin, ans);
        tambah_lagi = ans.empty() ? 'n' : ans[0];

    } while (tambah_lagi == 'y' || tambah_lagi == 'Y'); // ulang jika user ingin tambah barang lagi
}