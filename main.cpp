#include "recipe.h"
#include "inventory.h"

int main(){
    orderManager manager;
    RecipeModule recipes;
    DemandCalculator calculator;
    Inventory inventory(20261016);
    int choice;

    do{
        cout<< "\n===== SMART RESTAURANT INVENTORY SYSTEM =====\n";
        cout<< "1. Order management\n";
        cout<< "2. Add ingredient\n";
        cout<< "3. Receive ingredient stock\n";
        cout<< "4. Show inventory stock\n";
        cout<< "5. Add or update recipe ingredient\n";
        cout<< "6. Show recipe\n";
        cout<< "7. Calculate ingredient demand\n";
        cout<< "0. Exit\n";
        cout<< "Enter choice: ";
        if(!(cin >> choice)){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input\n";
            continue;
        }

        try{
            if(choice == 1){
                manager.menu();
            }
            // Naya ingredient inventory mein add kare hain.
            else if(choice == 2){
                int id;
                string name, unit;
                float minimum;
                cout<<"Ingredient ID: ";
                cin>>id;
                cout<<"Ingredient name (one word): ";
                cin>>name;
                cout<<"Unit: ";
                cin>>unit;
                cout<<"Minimum stock: ";
                cin>>minimum;
                inventory.addIngredient(id, name, unit, minimum);
                cout<<"Ingredient added\n";
            }
            // Received quantity ko batch ke form mein record karte hain.

            else if(choice == 3){
                int ingredientId, batchId, expiry;
                float quantity;
                cout<<"Ingredient ID: ";
                cin>>ingredientId;
                cout<<"Batch ID: ";
                cin>>batchId;
                cout<<"Expiry date (YYYYMMDD): ";
                cin>>expiry;
                cout<<"Quantity: ";
                cin>>quantity;
                inventory.receiveStock(ingredientId, batchId, expiry, quantity);
                cout<< "Stock received\n";
            }
            else if(choice == 4){
                inventory.showStock();
            }
            else if(choice == 5){
                int foodId, ingredientId;
                float quantity;
                cout << "Food ID: ";
                cin >> foodId;
                cout << "Ingredient ID: ";
                cin >> ingredientId;
                cout << "Quantity per serving: ";
                cin >> quantity;
                recipes.setIngredient(foodId, ingredientId, quantity);
            }
            // Selected food ki recipe display karte hain.
            else if(choice == 6){
                int foodId;
                cout << "Food ID: ";
                cin >> foodId;
                recipes.showRecipe(foodId);
            }
            // Servings ke basis par required ingredients calculate karte hain
            else if(choice == 7){
                int foodId, servings;
                DemandEntry demand[MAX_DEMAND_ITEMS];
                cout << "Food ID: ";
                cin >> foodId;
                cout << "Number of servings: ";
                cin >> servings;
                int count = calculator.calculate(foodId, servings, recipes, demand);
                calculator.showDemand(demand, count);
            }
            else if(choice == 0){
                cout << "Exiting program\n";
            }
            else{
                cout << "Invalid choice\n";
            }
        }
        catch(const char* error){
            cout << "Error: " << error << endl;
        }
    }while(choice != 0);

    return 0;
}
