#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <string>
#include <vector> 

using namespace std;

class BankCustomer {
private:
    int id;
    string name;
    double balance;
    string address;
    string phoneNumber;
    string email;
    vector<string> transactionHistory; 

public:
    BankCustomer(int id, const string& name, double balance, const string& addr, const string& phone, const string& mail);

    int getId() const;
    string getName() const;
    double getBalance() const;
    // getters for persisted fields
    const string& getAddress() const { return address; }
    const string& getPhoneNumber() const { return phoneNumber; }
    const string& getEmail() const { return email; }

    void printInfo() const;
    void printTransactionHistory() const; 
    void setName(const string& name);
    void setBalance(double balance);
    void addBalance(double amount);
    bool withdrawBalance(double amount);
};

#endif