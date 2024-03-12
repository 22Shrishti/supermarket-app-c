#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structure for warehouse items
struct WarehouseItem {
    char hsnCode[20];
    char description[100];
    int quantity;
    float mrp;
};

// Function prototypes
void addNewItem();
void updateItem();
void generateBill();

// Main function
int main() {
    int choice;
    
    // Menu for user to choose option
    do {
        printf("\nSupermarket Management System\n");
        printf("1. Add New Item to Warehouse\n");
        printf("2. Update Item in Warehouse\n");
        printf("3. Generate Customer Bill\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                addNewItem();
                break;
            case 2:
                updateItem();
                break;
            case 3:
                generateBill();
                break;
            case 4:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 4);

    return 0;
}

// Function to add new item to warehouse
void addNewItem() {
    FILE *fp;
    struct WarehouseItem item;
    
    fp = fopen("warehouse_inventory.txt", "a");
    if(fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    printf("\nEnter HSN Code: ");
    scanf("%s", item.hsnCode);
    printf("Enter Product Description: ");
    scanf(" %[^\n]s", item.description);
    printf("Enter Quantity: ");
    scanf("%d", &item.quantity);
    printf("Enter MRP: ");
    scanf("%f", &item.mrp);
    
    fprintf(fp, "%s|%s|%d|%.2f\n", item.hsnCode, item.description, item.quantity, item.mrp);
    
    fclose(fp);
    printf("Item added to warehouse successfully.\n");
}

// Function to update item in warehouse
void updateItem() {
    FILE *fp, *temp;
    struct WarehouseItem item;
    char hsnCode[20];
    float newMRP;
    int newQuantity;
    
    fp = fopen("warehouse_inventory.txt", "r");
    if(fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    temp = fopen("temp.txt", "w");
    if(temp == NULL) {
        printf("Error opening file.\n");
        fclose(fp);
        return;
    }
    
    printf("\nEnter HSN Code of item to update: ");
    scanf("%s", hsnCode);
    printf("Enter new MRP: ");
    scanf("%f", &newMRP);
    printf("Enter new Quantity: ");
    scanf("%d", &newQuantity);
    
    while(fscanf(fp, "%[^|]|%[^|]|%d|%f\n", item.hsnCode, item.description, &item.quantity, &item.mrp) != EOF) {
        if(strcmp(item.hsnCode, hsnCode) == 0) {
            item.mrp = newMRP;
            item.quantity = newQuantity;
        }
        fprintf(temp, "%s|%s|%d|%.2f\n", item.hsnCode, item.description, item.quantity, item.mrp);
    }
    
    fclose(fp);
    fclose(temp);
    
    remove("warehouse_inventory.txt");
    rename("temp.txt", "warehouse_inventory.txt");
    
    printf("Item updated successfully.\n");
}

// Function to generate customer bill
void generateBill() {
    FILE *fp;
    struct WarehouseItem item;
    char hsnCode[20];
    int quantity;
    float totalMRP = 0;
    
    fp = fopen("warehouse_inventory.txt", "r");
    if(fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    printf("\nGenerating Customer Bill\n");
    printf("Enter Customer Name: ");
    char customerName[100];
    scanf(" %[^\n]s", customerName);
    
    char fileName[100];
    printf(fileName, "%s_bill.txt", customerName);
    FILE *bill = fopen(fileName, "w");
    
    if(bill == NULL) {
        printf("Error creating bill file.\n");
        fclose(fp);
        return;
    }
    
    fprintf(bill, "Customer Name: %s\n", customerName);
    fprintf(bill, "HSN Code | Description | Quantity | MRP\n");
    
    printf("Enter '0' to exit.\n");
    while(1) {
        printf("Enter HSN Code of item: ");
        scanf("%s", hsnCode);
        if(strcmp(hsnCode, "0") == 0)
            break;
        
        while(fscanf(fp, "%[^|]|%[^|]|%d|%f\n", item.hsnCode, item.description, &item.quantity, &item.mrp) != EOF) {
            if(strcmp(item.hsnCode, hsnCode) == 0) {
                printf("Product Description: %s\n", item.description);
                printf("Enter Quantity: ");
                scanf("%d", &quantity);
                
                if(quantity <= item.quantity) {
                    totalMRP += (item.mrp * quantity);
                    fprintf(bill, "%s | %s | %d | %.2f\n", item.hsnCode, item.description, quantity, item.mrp);
                    item.quantity -= quantity;
                } else {
                    printf("Not enough stock available.\n");
                }
                break;
            }
        }
        rewind(fp);
    }
    
    fclose(fp);
    fclose(bill);
    
    printf("Bill generated successfully for %s. Total MRP: %.2f\n", customerName, totalMRP);
}