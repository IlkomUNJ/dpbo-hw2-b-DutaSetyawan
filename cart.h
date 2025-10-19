#ifndef CART_H
#define CART_H

#include "item.h"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Cart {
private:
    vector<pair<Item, int>> items;

public:
    void addItem(const Item& item, int sellerId) {
        items.push_back({item, sellerId});
        cout << "Item '" << item.getName() << "' has been added to your cart." << endl;
    }

    void removeItem(int itemId) {
        auto it = remove_if(items.begin(), items.end(), [itemId](const pair<Item, int>& pair) {
            return pair.first.getId() == itemId;
        });

        if (it != items.end()) {
            items.erase(it, items.end());
            cout << "Item with ID " << itemId << " has been removed from your cart." << endl;
        } else {
            cout << "Item with ID " << itemId << " not found in your cart." << endl;
        }
    }

    void viewCart() const {
        if (items.empty()) {
            cout << "Your cart is empty." << endl;
            return;
        }
        cout << "\n--- Your Shopping Cart ---" << endl;
        double total = 0;
        for (const auto& pair : items) {
            const Item& item = pair.first;
            cout << "ID: " << item.getId() << ", Name: " << item.getName()
                 << ", Price: $" << item.getPrice() << endl;
            total += item.getPrice();
        }
        cout << "------------------------" << endl;
        cout << "Total: $" << total << endl;
    }

    bool isEmpty() const {
        return items.empty();
    }
    
    const vector<pair<Item, int>>& getItems() const {
        return items;
    }

    void clearCart() {
        items.clear();
    }
};

#endif