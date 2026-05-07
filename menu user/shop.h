#ifndef SHOP_H
#define SHOP_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <conio.h>
#include "tabulate/table.hpp"

struct Item {
    std::string id;
    std::string name;
    long long price;
    int stock;
};

struct Route {
    std::string id;
    std::string destination;
    long long cost;
};

struct Transaction {
    std::string trxId;
    std::string itemName;
    int qty;
    long long total;
    std::string type;
    std::string status;
};

void clearScreen();
void pauseScreen();
std::string getValidString();
long long getValidNumber();
int inquirerMenu(std::string title, std::vector<std::string> options);

void initData(std::vector<Item>* items, std::vector<Route>* routes, std::vector<Transaction>* trxs);
void displayItems(std::vector<Item>* items);
void sortItemsAscending(std::vector<Item>* items);
void searchItem(std::vector<Item>* items);
void orderItem(std::vector<Item>* items, std::vector<Route>* routes, std::vector<Transaction>* trxs);
void payOrder(std::vector<Transaction>* trxs);
void displayHistory(std::vector<Transaction>* trxs);
void userMenu(std::vector<Item>* items, std::vector<Route>* routes, std::vector<Transaction>* trxs);

#endif