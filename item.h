#ifndef ITEM_H
#define ITEM_H

#include <string>

using namespace std;

class Item {
private:
    int id;
    std::string name;
    int quantity;
    double price;

public:
    Item(int id, const std::string& name, int quantity, double price)
        : id(id), name(name), quantity(quantity), price(price) {}

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }

    void setName(const std::string& newName) { name = newName; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setPrice(double newPrice) { price = newPrice; }
};

#endif