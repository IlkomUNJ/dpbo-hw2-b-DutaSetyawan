#ifndef BANK_H
#define BANK_H

#include "bank_customer.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Bank {
private:
    string name;
    vector<BankCustomer> accounts;
    int nextCustomerId;

public:
    Bank(const string& name) : name(name), nextCustomerId(1) {}

    BankCustomer& createAccount(const string& customerName, const string& addr, const string& phone, const string& mail) {
        BankCustomer newAccount(nextCustomerId, customerName, 0.0, addr, phone, mail);
        accounts.push_back(newAccount);
        nextCustomerId++;
        return accounts.back();
    }

    
    BankCustomer& createAccountWithId(int id, const string& customerName, const string& addr, const string& phone, const string& mail, double balance) {
        BankCustomer newAccount(id, customerName, balance, addr, phone, mail);
        accounts.push_back(newAccount);
        
        if (id >= nextCustomerId) {
            nextCustomerId = id + 1;
        }
        return accounts.back();
    }
    
    void deleteAccountById(int id) {
        auto it = remove_if(accounts.begin(), accounts.end(), [id](const BankCustomer& acc) {
            return acc.getId() == id;
        });
        if (it != accounts.end()) {
            accounts.erase(it, accounts.end());
        }
    }

    BankCustomer* findAccountById(int id) {
        for (auto& account : accounts) {
            if (account.getId() == id) {
                return &account;
            }
        }
        return nullptr;
    }

    const string& getName() const {
        return name;
    }
};

#endif