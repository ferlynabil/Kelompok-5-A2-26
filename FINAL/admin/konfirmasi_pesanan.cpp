#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <conio.h>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

void konfirmasiPesanan(vector<Pesanan>& daftar_pesanan,
                       vector<Barang>&  db_barang,
                       vector<Rute>&    db_rute,
                       vector<Pengguna>& db_user) {

    system("cls");
    cout << "\n=== KONFIRMASI PESANAN MASUK ===\n";

    if (daftar_pesanan.empty()) {
        cout << "Tidak ada pesanan sama sekali.\n";
        cout << "\nTekan sembarang tombol untuk kembali...";
        _getch();
        return;
    }


    cout << "-------------------------------------------------------------------------------\n";
    cout << setw(12) << left << "ID"
         << setw(22) << "Nama Barang"
         << setw(8)  << "Qty"
         << setw(16) << "Total (Rp)"
         << "Status\n";
    cout << "-------------------------------------------------------------------------------\n";

    bool ada = false;
    for (const auto& p : daftar_pesanan) {
        if (p.status == "Menunggu Konfirmasi") {
            cout << setw(12) << left << p.id_pesanan
                 << setw(22) << p.nama_barang
                 << setw(8)  << p.jumlah
                 << setw(16) << fixed << setprecision(0) << p.total_bayar
                 << p.status << "\n";
            ada = true;
        }
    }

    if (!ada) {
        cout << "\n[~] Tidak ada pesanan yang menunggu konfirmasi.\n";
        cout << "\nTekan sembarang tombol untuk kembali...";
        _getch();
        return;
    }
    cout << "-------------------------------------------------------------------------------\n";

    string id_cari;
    cout << "\nMasukkan ID Pesanan yang ingin dikonfirmasi (Ketik '0' untuk batal): ";
    cin >> ws;
    getline(cin, id_cari);

    if (id_cari == "0") return;


    auto it = daftar_pesanan.end();
    for (auto i = daftar_pesanan.begin(); i != daftar_pesanan.end(); ++i) {
        if (i->id_pesanan == id_cari && i->status == "Menunggu Konfirmasi") {
            it = i;
            break;
        }
    }

    if (it == daftar_pesanan.end()) {
        cout << "\n[-] ID Pesanan tidak ditemukan atau bukan status \"Menunggu Konfirmasi\".\n";
        cout << "\nTekan sembarang tombol untuk kembali...";
        _getch();
        return;
    }

    cout << "\nDetail Pesanan:\n";
    cout << "  Nama Barang : " << it->nama_barang << "\n";
    cout << "  Jumlah      : " << it->jumlah << " pcs\n";
    cout << "  Total       : Rp" << (long long)it->total_bayar << "\n";
    cout << "  Tipe Beli   : " << it->tipe_beli << "\n\n";

    // Pilihan tindakan
    cout << "Pilih tindakan:\n";
    cout << "  [1] Terima  -> status menjadi \"Menunggu Pembayaran\"\n";
    cout << "  [2] Tolak   -> status menjadi \"Ditolak\"\n";
    cout << "  [0] Batal\n";
    cout << "Pilihan: ";

    string pilihan;
    getline(cin, pilihan);

    if (pilihan == "1") {
        it->status = "Menunggu Pembayaran";
        simpanData(db_barang, daftar_pesanan, db_rute, db_user);

        system("cls");
        cout << "\n[+] Pesanan diterima!\n";
        cout << "[+] Status berubah -> \"Menunggu Pembayaran\".\n";
        cout << "[+] Pembeli sekarang dapat melakukan pembayaran.\n";
        cout << "[+] Data JSON diperbarui otomatis.\n";

    } else if (pilihan == "2") {
        it->status = "Ditolak";
        simpanData(db_barang, daftar_pesanan, db_rute, db_user);

        system("cls");
        cout << "\n[!] Pesanan ditolak.\n";
        cout << "[!] Status berubah -> \"Ditolak\".\n";
        cout << "[+] Data JSON diperbarui otomatis.\n";

    } else {
        system("cls");
        cout << "\n[~] Dibatalkan, tidak ada perubahan.\n";
    }

    cout << "\nTekan sembarang tombol untuk kembali...";
    _getch();
}