#ifndef INVENTORY_H
#define INVENTORY_H

#include <SPI.h>
#include <SdFat.h>

// Creates an instance of SdFat library
SdFat sd;

// Function declaration for resetting inacitivty timer
extern void resetInactivityTime();

// Cache file name
const char* cacheFileName = "cache.txt";

// Length of EAN code (13 digits + null terminator)
const int EAN_LENGTH = 14;    

// Inventory class to manage product inventory
class Inventory {
  const char* inventoryFileName = "inventory.txt";      // Inventory file name

  // Add item to inventory
  void addProduct(const char* eanCode) {
    // Open the inventory file in write, create and append mode
    File inventoryFile;
    if(!inventoryFile.open(inventoryFileName, O_WRITE | O_CREAT | O_APPEND)) {
      Serial.println("Failed to open inventory file!");
      return;
    }

    char line[20];

    // Check if the EAN already exists in the inventory
    if(eanExist(eanCode)) {
      inventoryFile.seekSet(0);                                     // Move to beginning of the file
      while(inventoryFile.available()) {
        inventoryFile.readBytesUntil('\n', line, sizeof(line));     // Read until new line
        
        // Compare scanned EAN with EAN from inventory.txt
        if(strncmp(line, eanCode, EAN_LENGTH) == 0) {
          // If it matches, increment the quantity 
          int currQty = atoi(&line[EAN_LENGTH + 1]);
          currQty++;
          sprintf(line, "%s %d", eanCode, currQty);
          break;
        }
      }
    } else {
      // Scanned EAN was not found in inventory, create new line and initialize the quantity with 1
      Serial.println("Adding new product...");
      sprintf(line, "%s 1", eanCode);
    }

    if(inventoryFile.println(line)) {
      // Print message if added successfully to inventory
      Serial.println("Item added successfully.");
    } else {
      // Print error if failed to add item
      Serial.println("Failed to add item!");
    }

    inventoryFile.close();
  }

  // Remove item from inventory
  void removeProduct(const char* eanCode) {
    // Check if EAN exists in the inventory
    if(!eanExist(eanCode)) {
      Serial.println("Product not found in inventory.");
      return;
    }

    File inventoryFile;
    // Open the inventory file in read mode
    if(!inventoryFile.open(inventoryFileName, O_READ)) {
      // Print error if failed to open inventory file
      Serial.println("Failed to open inventory file!");
      return;
    }

    File tempFile;
    // Open a temporary file in write and create mode
    if(!tempFile.open("temp.txt", O_WRITE | O_CREAT)) {
      // Print error if failed to open temporary file
      Serial.println("Failed to open temporary file!");
      return;
    }

    bool found = false;                                         // Indicate if EAN was found

    // Read through the inventory file
    while(inventoryFile.available()) {
      char line[20];                                            // Save edited and non-edited line
      inventoryFile.readBytesUntil('\n', line, sizeof(line));   // Read line until new line

      // Check if scanned EAN matches EAN from inventory
      if(strncmp(line, eanCode, EAN_LENGTH) == 0) {
        // If found, check the quantity
        found = true;

        int currQty = atoi(&line[EAN_LENGTH + 1]);

        if(currQty > 1) {
          // If more than 1 in quantity, decrement the quantity and format the line with updated data
          currQty--;
          sprintf(line, "%s %d", eanCode, currQty);
          tempFile.println(line);
        } else {
          // Else skip line = there is only 1 of this item, we skip and it will be "removed"
        }
      } else {
        // If scanned EAN was not found in the line, copy this line and print it in the temporary file
        tempFile.println(line);
      }
    }

    inventoryFile.close();
    tempFile.close();

  
    sd.remove(inventoryFileName);                                 // Remove old file
    sd.rename("temp.txt", inventoryFileName);                     // Rename new file with old file name


    // Used for debugging purpose
    if(found) {
      Serial.println("Product removed successfully.");
    } else {
      Serial.println("Product not found in inventory.");
    }

  }

  // Fetch EAN from serial
  void scanEAN(char* eanCode, int maxLen) {
    int index = 0;
    while(Serial.available() == 0) {};                            // Wait for serial input
    while(Serial.available() > 0 && index < maxLen - 1) {        
      // Fill eanCode array while serial input is read
      eanCode[index] = Serial.read();
      index++;
      resetInactivityTime();                                      // Reset inactivty timer
    }
    eanCode[index] = '\0';                                        // Null-terminate the EAN
  }

  // Check if EAN already exists
  bool eanExist(const char* eanCode) {
    File inventoryFile;

    // Openinventory file in read mode
    if(!inventoryFile.open(inventoryFileName, O_READ)) {
      Serial.println("Failed to open inventory file!");
      return false;
    }

    bool exist = false;
    
    // Read through the inventory file
    while(inventoryFile.available()) {
      String line = inventoryFile.readStringUntil('\n');
      
      // If scanned EAN is found, set to true
      if(line.startsWith(eanCode)) {
        exist = true;
        break;
      }
    }

    inventoryFile.close();
    return exist;
  }

public:
    // Constructor    
    Inventory() {}

    // Adds product by scanning EAN
    void addProductFromScan() {
      char eanCode[EAN_LENGTH];
      scanEAN(eanCode, EAN_LENGTH);
      addProduct(eanCode);
    }

    // Removes product by scanning EAN
    void removeProductFromScan() {
      char eanCode[EAN_LENGTH];
      scanEAN(eanCode, EAN_LENGTH);
      removeProduct(eanCode);
    }

};

#endif
