#include "bank_customer.h"
#include <iostream>

BankCustomer::BankCustomer(int id, const string& name, double balance, const string& addr, const string& phone, const string& mail)
    : id(id), name(name), balance(balance), address(addr), phoneNumber(phone), email(mail) {}

string BankCustomer::getName() const {
    return this->name;
}

int BankCustomer::getId() const {
    return this->id;
}

double BankCustomer::getBalance() const {
    return this->balance;
}

void BankCustomer::setBalance(double amount) {
    this->balance = amount;
}

void BankCustomer::addBalance(double amount) {
    this->balance += amount;
    // Catat transaksi
    transactionHistory.push_back("Deposit: +$" + to_string(amount));
}

bool BankCustomer::withdrawBalance(double amount){
    if (amount > this->balance) {
        std::cout << "Rejected: Insufficient funds!" << std::endl;
        return false;
    }
    this->balance -= amount;
    // Catat transaksi
    transactionHistory.push_back("Withdrawal: -$" + to_string(amount));
    return true;
}

void BankCustomer::printInfo() const {
    std::cout << "Customer Name: " << this->name << std::endl;
    std::cout << "Customer ID: " << this->id << std::endl;
    std::cout << "Address: " << this->address << std::endl;
    std::cout << "Phone Number: " << this->phoneNumber << std::endl;
    std::cout << "Email: " << this->email << std::endl;
    std::cout << "Balance: $" << this->balance << std::endl;
}

void BankCustomer::printTransactionHistory() const {
    cout << "\n--- Transaction History for " << this->name << " ---" << endl;
    if (transactionHistory.empty()) {
        cout << "No transactions found." << endl;
    } else {
        for (const auto& transaction : transactionHistory) {
            cout << "- " << transaction << endl;
        }
    }
}