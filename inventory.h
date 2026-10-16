#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// One ingredient in the inventory
class Ingredient {
private:
    int id;
    string name;
    string unit;
    float minimumStock;

public:
    // Default constructor
    Ingredient() {
        id = 0;
        name = "";
        unit = "";
        minimumStock = 0;
    }

    // Parameterized constructor
    Ingredient(int i, string n, string u, float minStock) {
        id = i;
        name = n;
        unit = u;
        minimumStock = minStock;
    }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getUnit() const {
        return unit;
    }

    float getMinimumStock() const {
        return minimumStock;
    }
};


// A delivery of an ingredient with its own expiry date
// Har batch mein ingredient ID, expiry date aur available quantity store hoti hai.
class Batch {
private:
    int batchId;
    int ingredientId;
    int expiryDate;       // Format: YYYYMMDD
    float quantity;

public:
    // Default constructor
    Batch() {
        batchId = 0;
        ingredientId = 0;
        expiryDate = 0;
        quantity = 0;
    }

    // Parameterized constructor
    Batch(int b, int i, int expiry, float q) {
        batchId = b;
        ingredientId = i;
        expiryDate = expiry;
        quantity = q;
    }

    int getBatchId() const {
        return batchId;
    }

    int getIngredientId() const {
        return ingredientId;
    }

    int getExpiryDate() const {
        return expiryDate;
    }

    float getQuantity() const {
        return quantity;
    }

    void use(float q) {
        if (q <= 0 || q > quantity) {
            throw "Invalid quantity for this batch.";
        }

        quantity = quantity - q;
    }
};


class Inventory {
private:
    // Arrays replace vectors
    Ingredient ingredients[100];
    Batch batches[100];

    int ingredientCount;
    int batchCount;

    int today;  // Date format: YYYYMMDD


    // Ingredient ID search karke uska array index return karta hai.
// ID nahi milne par -1 return hota hai.
    int findIngredientIndex(int id) const {
        for (int i = 0; i < ingredientCount; i++) {
            if (ingredients[i].getId() == id) {
                return i;
            }
        }

        return -1;
    }

public:
    Inventory(int currentDate) {
        today = currentDate;
        ingredientCount = 0;
        batchCount = 0;
    }

    // Add a new ingredient
    void addIngredient(int id, string name,
                       string unit, float minimumStock) {

        if (findIngredientIndex(id) != -1) {
            throw "Ingredient ID already exists.";
        }

        if (id <= 0 || minimumStock < 0) {
            throw "Invalid ingredient details.";
        }

        if (ingredientCount >= 100) {
            throw "Ingredient storage is full.";
        }

        ingredients[ingredientCount] =
            Ingredient(id, name, unit, minimumStock);

        ingredientCount++;
    }
    // Received stock ko ek naye batch ke form mein inventory mein add karte hain.
    void receiveStock(int ingredientId, int batchId,
                int expiryDate, float quantity) {

        if(findIngredientIndex(ingredientId) == -1) {
            throw "Ingredient not found.";
        }

        if(quantity <= 0) {
            throw "Quantity must be greater than zero.";
        }

        if(batchCount >= 100) {
            throw "Batch storage is full.";
        }

        for(int i = 0; i < batchCount; i++) {
            if (batches[i].getBatchId() == batchId) {
                throw "Batch ID already exists.";
            }
        }

        batches[batchCount] =
            Batch(batchId, ingredientId, expiryDate, quantity);

        batchCount++;
    }

// Sirf un batches ka stock count karte hain jinki expiry aaj ya future mein hai.
    float getUsableStock(int ingredientId) const {
        float total = 0;

        for (int i = 0; i < batchCount; i++) {
            if (batches[i].getIngredientId() == ingredientId &&
                batches[i].getExpiryDate() >= today) {

                total = total + batches[i].getQuantity();
            }
        }

        return total;
    }

    // FEFO: First Expiry, First Out
    void consumeStock(int ingredientId, float quantity) {

        if (findIngredientIndex(ingredientId) == -1) {
            throw "Ingredient not found.";
        }

        if (quantity <= 0) {
            throw "Quantity must be greater than zero.";
        }// Pehle check karte hain ki required quantity ke liye usable stock enough hai.
// Stock kam hone par quantity ko change nahi karte.

        if (getUsableStock(ingredientId) < quantity) {
            throw "Not enough usable stock. Nothing was changed.";
        }

        float remaining = quantity;
// Sabse pehle expire hone wale usable batch se stock consume karte hain.
        while (remaining > 0.001f) {

            int earliestIndex = -1;

            // Find the usable batch with the earliest expiry
            for (int i = 0; i < batchCount; i++) {

                if (batches[i].getIngredientId() == ingredientId &&
                    batches[i].getExpiryDate() >= today &&
                    batches[i].getQuantity() > 0) {

                    if (earliestIndex == -1 ||
                        batches[i].getExpiryDate() <
                        batches[earliestIndex].getExpiryDate()) {

                        earliestIndex = i;
                    }
                }
            }

            if (earliestIndex == -1) {
                throw "No usable batch found.";
            }

            float take = batches[earliestIndex].getQuantity();

            if (take > remaining) {
                take = remaining;
            }

            batches[earliestIndex].use(take);

            remaining = remaining - take;
        }
    }
    // Har ingredient ka usable stock aur minimum stock compare karke status dikhate hain.
    void showStock() const {

        cout << "\n--- INVENTORY STOCK REPORT ---\n";

        cout << left << setw(8) << "ID"
             << setw(18) << "Ingredient"
             << setw(10) << "Unit"
             << setw(14) << "Usable Stock"
             << setw(14) << "Minimum"
             << "Status\n";

        for (int i = 0; i < ingredientCount; i++) {

            int id = ingredients[i].getId();

            float usable = getUsableStock(id);

            cout << left << setw(8) << id
                 << setw(18) << ingredients[i].getName()
                 << setw(10) << ingredients[i].getUnit()
                 << setw(14) << fixed << setprecision(2) << usable
                 << setw(14) << ingredients[i].getMinimumStock();

            if (usable < ingredients[i].getMinimumStock()) {
                cout << "LOW STOCK\n";
            }
            else {
                cout << "OK\n";
            }
        }
    }
};
