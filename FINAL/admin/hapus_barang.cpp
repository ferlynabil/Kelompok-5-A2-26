#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

// Tambahkan db_pesanan, db_rute, dan db_user di parameternya
void hapusBarang(vector<Barang>& daftar_barang, vector<Pesanan>& db_pesanan, vector<Rute>& db_rute, vector<Pengguna>& db_user) {
    if (daftar_barang.empty()) {
        system("cls");
        cout << "\n=== HAPUS BARANG ===\n";
        cout << "Gudang masih kosong!\n";
        cout << "\nTekan Enter untuk kembali...";
        string dummy;
        getline(cin, dummy);
        return;
    }

    char hapus_lagi;
    static bool first_run_hapus = true;

    do {
        system("cls");
        cout << "\n=== DAFTAR BARANG (REFERENSI HAPUS) ===\n";
        cout << "-------------------------------------------------------------------------------\n";
        cout << setw(15) << left << "ID" 
             << setw(20) << "Nama Barang" 
             << setw(12) << "Harga" 
             << setw(10) << "Berat" 
             << setw(15) << "Satuan" 
             << setw(5) << "Stok\n";
        cout << "-------------------------------------------------------------------------------\n";
        for (const auto& brg : daftar_barang) {
            cout << setw(15) << left << brg.id_barang 
                 << setw(20) << brg.nama_barang 
                 << setw(12) << brg.harga 
                 << setw(10) << brg.berat 
                 << setw(15) << brg.satuan 
                 << setw(5) << brg.stok << "\n";
        }
        cout << "-------------------------------------------------------------------------------\n";

        if (daftar_barang.empty()) break;

        string id_hapus;
        cout << "\nMasukkan ID Barang yang ingin dihapus (Ketik '0' untuk batal dan kembali): "; 
        
        getline(cin, id_hapus);
        if (first_run_hapus) {
            if (id_hapus.empty()) {
                getline(cin, id_hapus);
            }
            first_run_hapus = false;
        }

        if (id_hapus.empty()) {
            cout << "\n[-] Error: ID Barang tidak boleh kosong!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans;
            getline(cin, ans);
            hapus_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }

        if (id_hapus == "0") {
            cout << "\n[~] Penghapusan dibatalkan. Kembali ke Dashboard Admin...\n";
            return;
        }

        auto it = daftar_barang.begin();
        for (; it != daftar_barang.end(); ++it) {
            if (it->id_barang == id_hapus) {
                break;
            }
        }

        if (it == daftar_barang.end()) {
            cout << "\n[-] Error: ID Barang tidak ditemukan.\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans;
            getline(cin, ans);
            hapus_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }

        cout << "\nApakah Anda yakin ingin menghapus barang [" << it->nama_barang << "]? (y/n): ";
        string konfirmasi;
        getline(cin, konfirmasi);

        if (!konfirmasi.empty() && (konfirmasi[0] == 'y' || konfirmasi[0] == 'Y')) {
            cout << "\n[-] Barang " << it->nama_barang << " berhasil dihapus!\n";
            daftar_barang.erase(it);
            
            // ---> AUTO UPDATE JSON DETIK INI JUGA <---
            simpanData(daftar_barang, db_pesanan, db_rute, db_user);
            cout << "[+] Data Gudang JSON otomatis diperbarui!\n";

        } else {
            cout << "\n[~] Penghapusan dibatalkan.\n";
        }

        if (daftar_barang.empty()) {
            cout << "\n[!] Gudang sekarang kosong.\n";
            cout << "\nTekan Enter untuk kembali...";
            string dummy;
            getline(cin, dummy);
            break;
        }

        cout << "\nIngin hapus barang lain? (y/n): ";
        string ans;
        getline(cin, ans);
        hapus_lagi = ans.empty() ? 'n' : ans[0];

    } while (hapus_lagi == 'y' || hapus_lagi == 'Y');
}