#ifndef BUYER_H
#define BUYER_H

#include <string>
#include "bank_customer.h"
#include "cart.h"

using namespace std;

class Buyer {
private:
    int id;
    string name;
    BankCustomer *account; 
    Cart cart;

public:
    Buyer(int id, const string& name, BankCustomer *account0) 
        : id(id), name(name), account(account0) {}

    int getId() const { return id; }
    string getName() const { return name; }
    BankCustomer* getAccount() const { return account; } 
    Cart& getCart() { return cart; }

    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
};

#endif