#ifndef INVENTORY_H
#define INVENTORY_H

#include <Arduino.h>

const int MAX_PRODUCTS = 75; // Maximum number of unique products
const int EAN_LENGTH = 14;    // Length of EAN code (13 digits + null terminator)

class Inventory {
    char inventory[MAX_PRODUCTS][EAN_LENGTH];   // Array to store EAN codes
    int inventoryQuantity[MAX_PRODUCTS];        // Array to store quantities
    int uniqueProducts;                         // Number of unique products

public:
    // Constructor    
    Inventory() : uniqueProducts{ 0 } {}

    // Adds product
    void addProduct(const char* obj) {
      // Check if product already exist
      int index = searchInventory(obj);
        // If product exists, increment by 1
        if (-1 != index) {
            inventoryQuantity[index]++;
        }
        else {
            // If product does not exist and amount of unique products is less than maximum amount,
            // add to inventory with the quantity of 1
            if(uniqueProducts < MAX_PRODUCTS) {
              strncpy(inventory[uniqueProducts], obj, EAN_LENGTH);
              inventory[uniqueProducts][EAN_LENGTH - 1] = '\0';
              inventoryQuantity[uniqueProducts] = 1;
              uniqueProducts++;
            } else {
              Serial.println("Inventory is full!");
            }
        }
    }

    // Removes product
    void removeProduct(const char* obj) {
      // Check if product exists
      int index = searchInventory(obj);
    
      if(-1 != index) {
        // If the quantity is greater than 1, decrement the amount
        if(inventoryQuantity[index] > 1) {
          inventoryQuantity[index]--;
        } else {
          // If the quantity is 1, remove the product
          for(int i = index; i < uniqueProducts - 1; i++) {
            strncpy(inventory[i], inventory[i + 1], EAN_LENGTH);
            inventoryQuantity[i] = inventoryQuantity[i + 1];
          }
          uniqueProducts--;
        }  
      } else {
        Serial.println("Product not  found!");
      }
      
    }

    // Searches for a product in the inventory
    int searchInventory(const char* obj) {
        for(int i = 0; i < uniqueProducts; i++) {
          if(strncmp(inventory[i], obj, EAN_LENGTH) == 0)
            return i;
        }
        return -1;      // Product not found
    }

    void displayInventory() {
      Serial.println("Inventory:");
      for(int i = 0; i < uniqueProducts; i++) {
        Serial.print("EAN: ");
        Serial.print(inventory[i]);
        Serial.print(", Quantity: ");
        Serial.println(inventoryQuantity[i]);
      }
    }
};

#endif
