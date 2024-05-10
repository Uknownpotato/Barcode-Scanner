#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Inventory {
private:
    unordered_map<unsigned long, int> inventory;
    int date[3];
    // quantity par 

public:    
    Inventory(int dd, int mm, int yy) : date{ dd, mm, yy } {}

    // Adds product
    void addProduct(unsigned long obj) {

        // Check if product already exist
        if (searchInventory(obj)) {
            // If it exists, increment by one
            inventory[obj]++;
        }
        else {
            // If it does not exist, add to inventory with the quantity of 1
            inventory[obj] = 1;
        }
    }

    // Removes product
    void removeProduct(unsigned long obj) {

        // Check if product exists
        bool productExists = searchInventory(obj);

        // If the product exists and quantity is greater than 1, decrement te quantity
        if (productExists && inventory[obj] > 1) {
            inventory[obj]--;
        }
        else if (productExists && inventory[obj] == 1) {
            // If the product exists and quantity is equal to 1, remove the product
            inventory.erase(obj);
        }
        else {
            //In this case there is no product with this EAN, what to do? Error message?
        }
    }

    // Looks for a product in the inventory
    bool searchInventory(unsigned long obj) {
        return inventory.find(obj) != inventory.end();
    }

    void displayProduct(unsigned long obj) {

    }
};
