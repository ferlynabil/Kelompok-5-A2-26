#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "admin.h"
#include "../database_handler.h"

using namespace std;


void layananPembayaranDanPengiriman(vector<Pesanan>& daftar_pesanan, vector<Barang>& db_barang, vector<Rute>& db_rute, vector<Pengguna>& db_user) {
    if (daftar_pesanan.empty()) {
        system("cls");
        cout << "\n=== LAYANAN PEMBAYARAN & PENGIRIMAN ===\n";
        cout << "Belum ada pesanan masuk.\n";
        cout << "\nTekan Enter untuk kembali...";
        string dummy;
        getline(cin >> ws, dummy);
        return;
    }

    system("cls");
    cout << "\n=== DAFTAR PESANAN MASUK ===\n";
    cout << "-------------------------------------------------------------------------------\n";
    cout << setw(10) << left << "ID" 
         << setw(20) << "Barang" 
         << setw(10) << "Qty" 
         << setw(15) << "Total" 
         << setw(15) << "Status\n";
    cout << "-------------------------------------------------------------------------------\n";

    for (const auto& p : daftar_pesanan) {
        cout << setw(10) << left << p.id_pesanan 
             << setw(20) << p.nama_barang 
             << setw(10) << p.jumlah 
             << setw(15) << fixed << setprecision(0) << p.total_bayar 
             << setw(15) << p.status << "\n";
    }
    cout << "-------------------------------------------------------------------------------\n";

    string id_cari;
    cout << "\nMasukkan ID Pesanan untuk update status (Ketik '0' untuk batal): ";
    cin >> ws;
    getline(cin, id_cari);

    if (id_cari == "0") return;

    auto it = daftar_pesanan.begin();
    bool ditemukan = false;
    for (; it != daftar_pesanan.end(); ++it) {
        if (it->id_pesanan == id_cari) {
            ditemukan = true;
            break;
        }
    }

    if (!ditemukan) {
        cout << "\n[-] ID Pesanan tidak ditemukan.\n";
        system("pause");
        return;
    }

    cout << "\nDetail Pesanan: " << it->nama_barang << " (" << it->jumlah << ")\n";
    cout << "Tipe Beli: " << it->tipe_beli << "\n";
    cout << "Status Saat Ini: " << it->status << "\n";

    cout << "\nUpdate status menjadi 'Lunas & Dikirim'? (y/n): ";
    char konfirm;
    cin >> konfirm;

    if (konfirm == 'y' || konfirm == 'Y') {

        it->status = "Lunas & Dikirim";
        

        simpanData(db_barang, daftar_pesanan, db_rute, db_user);

        cout << "\n[+] Status pesanan berhasil diperbarui!\n";
        cout << "[+] Data Pesanan di JSON otomatis diperbarui!\n";
    } else {
        cout << "\n[~] Pembaruan dibatalkan.\n";
    }

    system("pause");
}