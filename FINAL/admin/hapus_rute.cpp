#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "admin.h"
#include "../database_handler.h"

using namespace std;


void hapusRute(vector<Rute>& daftar_rute, vector<Barang>& db_barang, vector<Pesanan>& db_pesanan, vector<Pengguna>& db_user) {
    if (daftar_rute.empty()) {
        system("cls");
        cout << "\n=== HAPUS RUTE PENGIRIMAN ===\n";
        cout << "Data rute masih kosong!\n";
        cout << "\nTekan Enter untuk kembali...";
        string dummy;
        getline(cin >> ws, dummy);
        return;
    }

    system("cls");
    cout << "\n=== DAFTAR RUTE (REFERENSI HAPUS) ===\n";
    cout << "----------------------------------------------------------------------------------------\n";
    cout << setw(10) << left << "ID" 
         << setw(20) << "Kota Tujuan" 
         << setw(10) << "Jarak" 
         << setw(15) << "Reguler (Rp)"
         << setw(15) << "Standar (Rp)"
         << setw(15) << "Premium (Rp)\n";
    cout << "----------------------------------------------------------------------------------------\n";
    for (const auto& r : daftar_rute) {
        cout << setw(10) << left << r.id_rute 
             << setw(20) << r.tujuan 
             << setw(10) << r.jarak 
             << setw(15) << fixed << setprecision(0) << r.biaya_reguler
             << setw(15) << fixed << setprecision(0) << r.biaya_standar
             << setw(15) << fixed << setprecision(0) << r.biaya_premium << "\n";
    }
    cout << "----------------------------------------------------------------------------------------\n";

    string id_hapus;
    cout << "\nMasukkan ID Rute yang ingin dihapus (Ketik '0' untuk batal): ";
    cin >> ws;
    getline(cin, id_hapus);

    if (id_hapus == "0") return;

    auto it = daftar_rute.begin();
    bool ditemukan = false;
    for (; it != daftar_rute.end(); ++it) {
        if (it->id_rute == id_hapus) {
            ditemukan = true;
            break;
        }
    }

    if (!ditemukan) {
        cout << "\n[-] ID Rute tidak ditemukan.\n";
        system("pause");
        return;
    }

    cout << "\nHapus rute [Samarinda -> " << it->tujuan << "]? (y/n): ";
    char konfirm;
    cin >> konfirm;

    if (konfirm == 'y' || konfirm == 'Y') {
        daftar_rute.erase(it);
        

        simpanData(db_barang, db_pesanan, daftar_rute, db_user);
        
        cout << "\n[+] Rute berhasil dihapus!\n";
        cout << "[+] Data JSON otomatis diperbarui!\n";
    } else {
        cout << "\n[~] Penghapusan dibatalkan.\n";
    }

    system("pause");
}