// Clean and minimal driver
#include <iostream>
#include "persistence.h"
#include "globals.h"
#include "ui.h"

using namespace std;

Bank eshopBank("E-Shop Bank");
vector<Buyer> buyers;
vector<seller> sellers;
vector<Order> orders;

int main() {
    loadData();
    int choice;
    do {
        cout << "\n=====================" << endl;
        cout << "  WELCOME TO E-SHOP" << endl;
        cout << "=====================" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit Program" << endl;
        cout << "9. Reset All Data (DEV)" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            clearInputBuffer();
            choice = 0;
        } else {
            clearInputBuffer();
        }
        switch (choice) {
            case 1: loginUser(); break;
            case 2: registerUser(); break;
            case 3: cout << "Exiting program. Thank you!" << endl; break;
            case 9: {
                char y; cout << "This will DELETE all saved data (buyers, sellers, inventory, orders). Continue? (y/n): "; cin >> y; clearInputBuffer();
                if(y=='y'||y=='Y'){
                    buyers.clear(); sellers.clear(); orders.clear();
                    saveData();
                    cout << "All data cleared." << endl;
                }
                break;
            }
            default: cout << "Invalid option. Please try again." << endl; break;
        }
    } while (choice != 3);
    saveData();
    return 0;
}