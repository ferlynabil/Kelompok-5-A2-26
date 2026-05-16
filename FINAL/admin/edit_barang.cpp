#include <iostream>
#include <string>
#include <cctype>
#include <conio.h>
#include <vector>
#include <iomanip>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

// 1. TAMBAH 3 PARAMETER VECTOR LAINNYA DI SINI
void editBarang(vector<Barang>& daftar_barang, vector<Pesanan>& db_pesanan, vector<Rute>& db_rute, vector<Pengguna>& db_user) {
    if (daftar_barang.empty()) {
        system("cls");
        cout << "\n=== EDIT DATA BARANG ===\n";
        cout << "Gudang masih kosong!\n";
        cout << "\nTekan Enter untuk kembali...";
        string dummy;
        getline(cin, dummy);
        return;
    }

    char edit_lagi;
    static bool first_run_edit = true;

    do {
        system("cls");
        cout << "\n=== DAFTAR BARANG (REFERENSI EDIT) ===\n";
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

        string id_cari;
        cout << "\nMasukkan ID Barang yang ingin diedit (Ketik '0' untuk batal dan kembali): "; 
        
        getline(cin, id_cari);
        if (first_run_edit) {
            if (id_cari.empty()) {
                getline(cin, id_cari);
            }
            first_run_edit = false;
        }

        if (id_cari.empty()) {
            cout << "\n[-] Error: ID Barang tidak boleh kosong!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans;
            getline(cin, ans);
            edit_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }

        if (id_cari == "0") {
            cout << "\n[~] Proses edit dibatalkan. Kembali ke Dashboard Admin...\n";
            return;
        }

        auto it = daftar_barang.begin();
        for (; it != daftar_barang.end(); ++it) {
            if (it->id_barang == id_cari) {
                break;
            }
        }

        if (it == daftar_barang.end()) {
            cout << "\n[-] Error: ID Barang tidak ditemukan.\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans;
            getline(cin, ans);
            edit_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }

        bool sedang_edit = true;
        vector<string> menu_edit = {"Nama Barang", "Harga", "Satuan", "Berat", "Stok", "Selesai Edit"};
        int posisi_edit = 0;

        while (sedang_edit) {
            system("cls");
            cout << "\n=== EDIT DATA BARANG ===\n";
            cout << "ID Barang   : " << it->id_barang << "\n";
            cout << "Nama Barang : " << it->nama_barang << "\n";
            cout << "Harga (Rp)  : " << it->harga << "\n";
            cout << "Satuan      : " << it->satuan << "\n";
            cout << "Berat       : " << it->berat << "\n";
            cout << "Stok        : " << it->stok << "\n";
            cout << "\nPilih data yang ingin diubah:\n\n";

            for (int i = 0; i < menu_edit.size(); i++) {
                if (i == posisi_edit) {
                    cout << "  > \033[1;32m" << menu_edit[i] << "\033[0m <\n";
                } else {
                    cout << "    " << menu_edit[i] << "\n";
                }
            }

            char tombol = _getch();

            if (tombol == 72 && posisi_edit > 0) {
                posisi_edit--;
            } else if (tombol == 80 && posisi_edit < menu_edit.size() - 1) {
                posisi_edit++;
            } else if (tombol == '\r') {
                system("cls");
                if (posisi_edit == 0) {
                    cout << "Ubah Nama Barang : ";
                    string input_nama;
                    getline(cin, input_nama);
                    
                    if (input_nama.empty()) {
                        cout << "\n[-] Error: Nama Barang tidak boleh kosong!\n";
                        cout << "\nTekan Enter untuk kembali...";
                        getline(cin, input_nama);
                        continue;
                    }
                    it->nama_barang = input_nama;
                    cout << "\n[+] Nama Barang berhasil diubah!\n";
                    cout << "Tekan Enter untuk kembali...";
                    getline(cin, input_nama);
                } 
                else if (posisi_edit == 1) {
                    string input_harga;
                    bool harga_valid = true;
                    cout << "Ubah Harga Baru (Rp) (hanya angka) : "; 
                    getline(cin, input_harga);
                    
                    if (input_harga.empty()) {
                        cout << "\n[-] Error: Harga tidak boleh kosong!\n";
                        cout << "\nTekan Enter untuk kembali...";
                        getline(cin, input_harga);
                        continue;
                    }
                    
                    for (char c : input_harga) {
                        if (!isdigit(c)) {
                            harga_valid = false;
                            break;
                        }
                    }
                    
                    if (!harga_valid) {
                        cout << "\n[-] Error: Harga hanya boleh berisi angka tanpa titik atau karakter lain!\n";
                        cout << "\nTekan Enter untuk kembali...";
                        getline(cin, input_harga);
                        continue;
                    }
                    it->harga = stod(input_harga);
                    cout << "\n[+] Harga berhasil diubah!\n";
                    cout << "Tekan Enter untuk kembali...";
                    getline(cin, input_harga);
                }
                else if (posisi_edit == 2) {
                    vector<string> list_satuan = {"Kilogram (kg)", "Gram (g)", "Liter (L)", "Buah/Pcs", "Sak", "Meter (m)"};
                    int posisi_satuan = 0;
                    bool memilih_satuan = true;

                    while (memilih_satuan) {
                        system("cls");
                        cout << "\n=== UBAH SATUAN BARANG ===\n";
                        cout << "Satuan saat ini: " << it->satuan << "\n";
                        cout << "\nPilih Satuan Baru:\n\n";

                        for (int i = 0; i < list_satuan.size(); i++) {
                            if (i == posisi_satuan) {
                                cout << "  > \033[1;32m" << list_satuan[i] << "\033[0m <\n";
                            } else {
                                cout << "    " << list_satuan[i] << "\n";
                            }
                        }

                        char tombol_satuan = _getch();
                        if (tombol_satuan == 72 && posisi_satuan > 0) {
                            posisi_satuan--;
                        } else if (tombol_satuan == 80 && posisi_satuan < list_satuan.size() - 1) {
                            posisi_satuan++;
                        } else if (tombol_satuan == '\r') {
                            it->satuan = list_satuan[posisi_satuan];
                            memilih_satuan = false;
                        }
                    }
                    cout << "\n[+] Satuan berhasil diubah menjadi " << it->satuan << "!\n";
                    cout << "Tekan Enter untuk kembali...";
                    string dummy;
                    getline(cin, dummy);
                }
                else if (posisi_edit == 3) {
                    string input_berat;
                    bool berat_valid = true;
                    cout << "Ubah Berat/Jumlah (hanya angka) : "; 
                    getline(cin, input_berat);
                    
                    if (input_berat.empty()) {
                        cout << "\n[-] Error: Berat/Jumlah tidak boleh kosong!\n";
                        cout << "\nTekan Enter untuk kembali...";
                        getline(cin, input_berat);
                        continue;
                    }
                    
                    for (char c : input_berat) {
                        if (!isdigit(c)) {
                            berat_valid = false;
                            break;
                        }
                    }
                    
                    if (!berat_valid) {
                        cout << "\n[-] Error: Berat/Jumlah hanya boleh berisi angka!\n";
                        cout << "\nTekan Enter untuk kembali...";
                        getline(cin, input_berat);
                        continue;
                    }
                    it->berat = stod(input_berat);
                    cout << "\n[+] Berat berhasil diubah!\n";
                    cout << "Tekan Enter untuk kembali...";
                    getline(cin, input_berat);
                }
                else if (posisi_edit == 4) {
                    string input_stok;
                    bool stok_valid = true;
                    cout << "Ubah Stok Baru (hanya angka) : "; 
                    getline(cin, input_stok);
                    
                    if (input_stok.empty()) {
                        cout << "\n[-] Error: Stok tidak boleh kosong!\n";
                        cout << "\nTekan Enter untuk kembali...";
                        getline(cin, input_stok);
                        continue;
                    }
                    
                    for (char c : input_stok) {
                        if (!isdigit(c)) {
                            stok_valid = false;
                            break;
                        }
                    }
                    
                    if (!stok_valid) {
                        cout << "\n[-] Error: Stok hanya boleh berisi angka!\n";
                        cout << "\nTekan Enter untuk kembali...";
                        getline(cin, input_stok);
                        continue;
                    }
                    it->stok = stoi(input_stok);
                    cout << "\n[+] Stok berhasil diubah!\n";
                    cout << "Tekan Enter untuk kembali...";
                    getline(cin, input_stok);
                }
                else if (posisi_edit == 5) {
                    // 2. SAAT KLIK "SELESAI EDIT", LANGSUNG SAVE KE JSON
                    simpanData(daftar_barang, db_pesanan, db_rute, db_user);
                    sedang_edit = false;
                }
            }
        }

        system("cls");
        cout << "\n[~] Selesai mengedit barang: " << it->nama_barang << "\n";
        cout << "[+] Data Gudang JSON otomatis diperbarui!\n";
        cout << "\nIngin edit barang lain? (y/n): ";
        string ans;
        getline(cin, ans);
        edit_lagi = ans.empty() ? 'n' : ans[0];

    } while (edit_lagi == 'y' || edit_lagi == 'Y');
}