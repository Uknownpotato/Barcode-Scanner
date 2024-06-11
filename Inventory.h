#ifndef INVENTORY_H
#define INVENTORY_H

#include <SPI.h>
#include <SdFat.h>

SdFat sd;

extern void resetInactivityTime();

const char* cacheFileName = "cache.txt";

const int EAN_LENGTH = 14;    // Length of EAN code (13 digits + null terminator)

class Inventory {
  const char* inventoryFileName = "inventory.txt";

  // Add item to inventory
  void addProduct(const char* eanCode) {
    File inventoryFile;
    if(!inventoryFile.open(inventoryFileName, O_WRITE | O_CREAT | O_APPEND)) {
      Serial.println("Failed to open inventory file!");
      return;
    }

    char line[20];

    if(eanExist(eanCode)) {
      inventoryFile.seekSet(0);
      while(inventoryFile.available()) {
        inventoryFile.readBytesUntil('\n', line, sizeof(line));
        if(strncmp(line, eanCode, EAN_LENGTH) == 0) {
          int currQty = atoi(&line[EAN_LENGTH + 1]);
          currQty++;
          sprintf(line, "%s %d", eanCode, currQty);
          break;
        }
      }
    } else {
      Serial.println("Adding new product...");
      sprintf(line, "%s 1", eanCode);
    }

    if(inventoryFile.println(line)) {
      Serial.println("Item added successfully.");
    } else {
      Serial.println("Failed to add item!");
    }

    inventoryFile.close();
  }

  // Remove item from inventory
  void removeProduct(const char* eanCode) {
    if(!eanExist(eanCode)) {
      Serial.println("Product not found in inventory.");
      return;
    }

    File inventoryFile;
    if(!inventoryFile.open(inventoryFileName, O_READ)) {
      Serial.println("Failed to open inventory file!");
      return;
    }

    File tempFile;
    if(!tempFile.open("temp.txt", O_WRITE | O_CREAT | O_TRUNC)) {
      Serial.println("Failed to open temporary file!");
      return;
    }

    bool found = false;

    while(inventoryFile.available()) {
      char line[20];
      inventoryFile.readBytesUntil('\n', line, sizeof(line));

      if(strncmp(line, eanCode, EAN_LENGTH) == 0) {
        found = true;

        int currQty = atoi(&line[EAN_LENGTH + 1]);

        if(currQty > 1) {
          currQty--;
          sprintf(line, "%s %d", eanCode, currQty);
          tempFile.println(line);
        } else {

        }
      } else {
        tempFile.println(line);
      }
    }

    inventoryFile.close();
    tempFile.close();

    sd.remove(inventoryFileName);
    sd.rename("temp.txt", inventoryFileName);

    if(found) {
      Serial.println("Product removed successfully.");
    } else {
      Serial.println("Product not found in inventory.");
    }

  }

  // Fetch EAN from serial
  void scanEAN(char* eanCode, int maxLen) {
    int index = 0;
    while(Serial.available() == 0) {};
    while(Serial.available() > 0 && index < maxLen - 1) {
      eanCode[index] = Serial.read();
      index++;
      resetInactivityTime();
    }
    eanCode[index] = '\0';
  }

  // Check if ean has been scanned previously
  bool eanExist(const char* eanCode) {
    File inventoryFile;
    if(!inventoryFile.open(inventoryFileName, O_READ)) {
      Serial.println("Failed to open inventory file!");
      return false;
    }

    bool exist = false;
    while(inventoryFile.available()) {
      String line = inventoryFile.readStringUntil('\n');
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

    // Adds product
    void addProductFromScan() {
      char eanCode[EAN_LENGTH];
      scanEAN(eanCode, EAN_LENGTH);
      addProduct(eanCode);
    }

    // Removes product
    void removeProductFromScan() {
      char eanCode[EAN_LENGTH];
      scanEAN(eanCode, EAN_LENGTH);
      removeProduct(eanCode);
    }

};

#endif
