#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 100

typedef struct Item {
    int code;             
    char name[50];        
    double price;         
    int stock;           
    struct Item *next;    
} Item;

Item *warehouse[HASH_SIZE];

int hashFunction(int code) {
        return code % HASH_SIZE;
    }

Item *createItem(int code, const char *name, double price, int stock) {
        Item *newItem = (Item *)malloc(sizeof(Item));
        newItem->code = code;
        strcpy(newItem->name, name);
        newItem->price = price;
        newItem->stock = stock;
        newItem->next = NULL;
        return newItem;
    }

void addItemToWarehouse(int code, const char *name, double price, int stock) {
        int index = hashFunction(code);
        Item *newItem = createItem(code, name, price, stock);

        if (warehouse[index] == NULL) {
            warehouse[index] = newItem;
        } else {
            Item *current = warehouse[index];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newItem;
        }
        printf("Item added to warehouse: %s\n", name);
        printf("\n");
    }

void removeItemFromWarehouse(int code) {
        int index = hashFunction(code);
        Item *current = warehouse[index];
        Item *prev = NULL;

        while (current != NULL && current->code != code) {
            prev = current;
            current = current->next;
        }

        if (current == NULL) {
            printf("Item not found in warehouse.\n");
            return;
        }

        if (prev == NULL) {
            warehouse[index] = current->next;
        } else {
            prev->next = current->next;
        }

        printf("Item removed from warehouse: %s\n", current->name);
        free(current);
        printf("\n");
    }

void updateWarehouseItem(int code, const char *name, double price, int stock) {
        int index = hashFunction(code);
        Item *current = warehouse[index];

        while (current != NULL && current->code != code) {
            current = current->next;
        }

        if (current == NULL) {
            printf("Item not found in warehouse.\n");
            return;
        }

        strcpy(current->name, name);
        current->price = price;
        current->stock = stock;
        printf("Warehouse item updated: %s\n", name);
        printf("\n");
    }

void displayWarehouseItems() {
        printf("\n+-----------------+------------------+----------+-------+\n");
        printf("| Code            | Name             | Price    | Stock |\n");
        printf("+-----------------+------------------+----------+-------+\n");
        
        for (int i = 0; i < HASH_SIZE; i++) {
            Item *current = warehouse[i];
            while (current != NULL) {
                printf("| %-15d | %-16s | %-8.2f | %-5d |\n", current->code, current->name, current->price, current->stock);
                current = current->next;
            }
        }

        printf("+-----------------+------------------+----------+-------+\n");
        printf("\n");
    }

void generateBill() {
        double totalAmount = 0.0;
        int code, quantity;

        typedef struct {
            int code;
            char name[50];
            int quantity;
            double amount;
        } BillItem;

        BillItem billItems[100]; 
        int itemCount = 0;

        printf("\nEnter the item code and quantity for each item (Code should be 3 digits and enter -1 to finish):\n");

        while (1) {
            printf("Item code: ");
            fflush(stdout); 
            scanf("%d", &code);
            if (code == -1) break;

            printf("Quantity: ");
            fflush(stdout); 
            scanf("%d", &quantity);

            int index = hashFunction(code);
            Item *current = warehouse[index];
            while (current != NULL && current->code != code) {
                current = current->next;
            }

            if (current == NULL) {
                printf("Item not found in warehouse.\n");
            } else if (quantity > current->stock) {
                printf("Insufficient stock for item: %s\n", current->name);
            } else {
                double amount = current->price * quantity;
                billItems[itemCount].code = current->code;
                strcpy(billItems[itemCount].name, current->name);
                billItems[itemCount].quantity = quantity;
                billItems[itemCount].amount = amount;
                itemCount++;
                totalAmount += amount;
                current->stock -= quantity;  
            }
        }

        printf("+-----------------+------------------+----------+---------------+\n");
        printf("| Code            | Name             | Quantity | Amount        |\n");
        printf("+-----------------+------------------+----------+---------------+\n");
        for (int i = 0; i < itemCount; i++) {
            printf("| %-15d | %-16s | %-8d | %-13.2f |\n", billItems[i].code, billItems[i].name, billItems[i].quantity, billItems[i].amount);
        }
        printf("+-----------------+------------------+----------+---------------+\n");
        printf("| Total Amount: %-48.2f|\n", totalAmount);
        printf("+---------------------------------------------------------------+\n");
        printf("\n");
    }

void displayFileContent(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("\n");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
}

void displayMenu() {
    printf("+-------------------------------+\n");
    printf("|         Warehouse Menu        |\n");
    printf("+-------------------------------+\n");
    printf("| 1. Add Item to Warehouse      |\n");
    printf("| 2. Remove Item from Warehouse |\n");
    printf("| 3. Update Warehouse Item      |\n");
    printf("| 4. Display Warehouse Items    |\n");
    printf("| 5. Generate Bill              |\n");
    printf("| 6. Exit                       |\n");
    printf("+-------------------------------+\n");
}

int main() {
        int choice, code, stock;
        char name[50];
        double price;

        displayFileContent("ascii.txt");
        
        while (1) {
            displayMenu();
            printf("\nEnter your choice: ");
            fflush(stdout); 
            scanf("%d", &choice);
            printf("\n");

            switch (choice) {
                case 1:
                    printf("Enter item code (3 digits): ");
                    fflush(stdout); 
                    scanf("%d", &code);
                    printf("Enter item name: ");
                    fflush(stdout); 
                    scanf("%s", name);
                    printf("Enter item price: ");
                    fflush(stdout); 
                    scanf("%lf", &price);
                    printf("Enter stock quantity: ");
                    fflush(stdout); 
                    scanf("%d", &stock);
                    addItemToWarehouse(code, name, price, stock);
                    break;
                case 2:
                    printf("Enter item code to remove: ");
                    fflush(stdout); 
                    scanf("%d", &code);
                    removeItemFromWarehouse(code);
                    break;
                case 3:
                    printf("Enter item code to update: ");
                    fflush(stdout); 
                    scanf("%d", &code);
                    printf("Enter new item name: ");
                    fflush(stdout); 
                    scanf("%s", name);
                    printf("Enter new item price: ");
                    fflush(stdout); 
                    scanf("%lf", &price);
                    printf("Enter new stock quantity: ");
                    fflush(stdout); 
                    scanf("%d", &stock);
                    updateWarehouseItem(code, name, price, stock);
                    break;
                case 4:
                    displayWarehouseItems();
                    break;
                case 5:
                    generateBill();
                    break;
                case 6:
                    exit(0);
                default:
                    printf("Invalid choice. Try again.\n");
            }
        }

        return 0;
    }
