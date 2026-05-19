#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <conio.h>
#include "admin.h"
#include "../database_handler.h"

using namespace std;

void editRute(vector<Rute>& daftar_rute, vector<Barang>& db_barang, vector<Pesanan>& db_pesanan, vector<Pengguna>& db_user) {
    if (daftar_rute.empty()) {
        system("cls");
        cout << "\n=== EDIT DATA RUTE ===\n";
        cout << "Data rute masih kosong!\n";
        cout << "\nTekan Enter untuk kembali...";
        string dummy;
        getline(cin >> ws, dummy);
        return;
    }

    char edit_lagi;
    static bool first_run_edit_rute = true;

    do {
        system("cls");
        cout << "\n=== DAFTAR RUTE (REFERENSI EDIT) ===\n";
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

        string id_cari;
        cout << "\nMasukkan ID Rute yang ingin diedit (Ketik '0' untuk batal): "; 
        
        if (first_run_edit_rute) {
            cin >> ws;
            first_run_edit_rute = false;
        }
        getline(cin, id_cari);

        if (id_cari == "0") return;

        if (id_cari.empty()) {
            cout << "\n[-] Error: ID Rute tidak boleh kosong!\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans;
            getline(cin, ans);
            edit_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }

        auto it = daftar_rute.begin();
        bool ditemukan = false;
        for (; it != daftar_rute.end(); ++it) {
            if (it->id_rute == id_cari) {
                ditemukan = true;
                break;
            }
        }

        if (!ditemukan) {
            cout << "\n[-] Error: ID Rute tidak ditemukan.\n";
            cout << "\nIngin coba lagi? (y/n): ";
            string ans;
            getline(cin, ans);
            edit_lagi = ans.empty() ? 'n' : ans[0];
            continue;
        }

        bool sedang_edit = true;
        vector<string> menu_edit = {
            "Kota Tujuan", 
            "Jarak (km)", 
            "Layanan Reguler (Harga & Estimasi)", 
            "Layanan Standar (Harga & Estimasi)", 
            "Layanan Premium (Harga & Estimasi)", 
            "Selesai Edit & Simpan"
        };
        int posisi_edit = 0;

        while (sedang_edit) {
            system("cls");
            cout << "\n=== EDIT DATA RUTE ===\n";
            cout << "ID Rute     : " << it->id_rute << "\n";
            cout << "Kota Asal   : " << it->kota_asal << " (Fixed Base)\n";
            cout << "Kota Tujuan : " << it->tujuan << "\n";
            cout << "Jarak       : " << it->jarak << " km\n";
            cout << "Reguler     : Rp " << fixed << setprecision(0) << it->biaya_reguler << " (" << it->estimasi_reguler << ")\n";
            cout << "Standar     : Rp " << fixed << setprecision(0) << it->biaya_standar << " (" << it->estimasi_standar << ")\n";
            cout << "Premium     : Rp " << fixed << setprecision(0) << it->biaya_premium << " (" << it->estimasi_premium << ")\n";
            cout << "\nPilih bagian yang ingin diubah:\n\n";

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
                    cout << "Ubah Kota Tujuan Baru : ";
                    string input_tujuan;
                    getline(cin, input_tujuan);
                    if (!input_tujuan.empty()) {
                        it->tujuan = input_tujuan;
                        cout << "\n[+] Kota tujuan berhasil diubah!\n";
                    }
                    system("pause");
                } 
                else if (posisi_edit == 1) {
                    cout << "Ubah Jarak Baru (km) : ";
                    string input_jarak;
                    getline(cin, input_jarak);
                    if (!input_jarak.empty()) {
                        try {
                            it->jarak = stod(input_jarak);
                            cout << "\n[+] Jarak berhasil diubah!\n";
                        } catch(...) {
                            cout << "\n[-] Error: Input harus berupa angka!\n";
                        }
                    }
                    system("pause");
                }
                else if (posisi_edit == 2) {
                    cout << "Ubah Harga Reguler Baru (Rp) : "; 
                    string in_h; getline(cin, in_h);
                    if (!in_h.empty()) {
                        try {
                            double h = stod(in_h);
                            it->biaya_reguler = h;
                            if (h > 0) {
                                cout << "Ubah Estimasi Reguler Baru (misal: 3-5 Hari) : ";
                                string in_e; getline(cin, in_e);
                                if (!in_e.empty()) it->estimasi_reguler = in_e;
                            } else {
                                it->estimasi_reguler = "-";
                            }
                            cout << "\n[+] Layanan Reguler berhasil diubah!\n";
                        } catch(...) {
                            cout << "\n[-] Error: Harga harus berupa angka!\n";
                        }
                    }
                    system("pause");
                }
                else if (posisi_edit == 3) {
                    cout << "Ubah Harga Standar Baru (Rp) : "; 
                    string in_h; getline(cin, in_h);
                    if (!in_h.empty()) {
                        try {
                            double h = stod(in_h);
                            it->biaya_standar = h;
                            if (h > 0) {
                                cout << "Ubah Estimasi Standar Baru (misal: 2-3 Hari) : ";
                                string in_e; getline(cin, in_e);
                                if (!in_e.empty()) it->estimasi_standar = in_e;
                            } else {
                                it->estimasi_standar = "-";
                            }
                            cout << "\n[+] Layanan Standar berhasil diubah!\n";
                        } catch(...) {
                            cout << "\n[-] Error: Harga harus berupa angka!\n";
                        }
                    }
                    system("pause");
                }
                else if (posisi_edit == 4) {
                    cout << "Ubah Harga Premium Baru (Rp) : "; 
                    string in_h; getline(cin, in_h);
                    if (!in_h.empty()) {
                        try {
                            double h = stod(in_h);
                            it->biaya_premium = h;
                            if (h > 0) {
                                cout << "Ubah Estimasi Premium Baru (misal: 1 Hari) : ";
                                string in_e; getline(cin, in_e);
                                if (!in_e.empty()) it->estimasi_premium = in_e;
                            } else {
                                it->estimasi_premium = "-";
                            }
                            cout << "\n[+] Layanan Premium berhasil diubah!\n";
                        } catch(...) {
                            cout << "\n[-] Error: Harga harus berupa angka!\n";
                        }
                    }
                    system("pause");
                }
                else if (posisi_edit == 5) {
                    if (it->biaya_reguler <= 0 && it->biaya_standar <= 0 && it->biaya_premium <= 0) {
                        cout << "\n[-] Peringatan: Anda mengosongkan semua harga pengiriman.\n";
                        cout << "Rute ini tidak akan bisa dipilih oleh pelanggan jika tidak ada layanan yang aktif.\n";
                        system("pause");
                    }
                    simpanData(db_barang, db_pesanan, daftar_rute, db_user);
                    sedang_edit = false;
                }
            }
        }

        system("cls");
        cout << "\n[~] Selesai mengedit rute ID: " << it->id_rute << "\n";
        cout << "[+] Data JSON otomatis diperbarui!\n";
        cout << "\nIngin edit rute lain? (y/n): ";
        string ans;
        getline(cin, ans);
        edit_lagi = ans.empty() ? 'n' : ans[0];

    } while (edit_lagi == 'y' || edit_lagi == 'Y');
}