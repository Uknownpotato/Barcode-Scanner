#include <Inventory.h>
#include <Time.h>

void setup() {

}

bool scanning = false

bool buttonPressAdd(){
}

bool buttonPressRemove(){
}

unsigned long int scanEAN(){
}

bool stopScan(){
}

void loop() {
  if(buttonPressAdd()){
    Inventory scannedObjects(day(), month(), year());
    scanning = true;
    while(scanning) {
      scannedObjects.addProduct(scanEAN());
      scanning = !stopScan();
    }
    saveProductsToFile();
    saveFileToDB();
  }

}