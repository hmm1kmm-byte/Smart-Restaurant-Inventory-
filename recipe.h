#include "order2.h"
const int MAX_RECIPES = 100;
const int MAX_INGREDIENTS_PER_RECIPE = 10;
const int MAX_DEMAND_ITEMS = 100;

// Ek food item ke liye ingredient ID aur per-serving quantity store hoti hai.
class IngredientRequirement{
private:
    int ingredientId;
    float quantityPerServing;

public:
    IngredientRequirement(){
        ingredientId= 0;
        quantityPerServing= 0;
    }

    IngredientRequirement(int id, float quantity){
        ingredientId = id;
        quantityPerServing= quantity;
    }

    int getIngredientId(){
        return ingredientId;
    }

    float getQuantity(){
        return quantityPerServing;
    }

    void setQuantity(float quantity){
        quantityPerServing = quantity;
    }
};

class Recipe{
private:
    int foodId;
    IngredientRequirement requirements[MAX_INGREDIENTS_PER_RECIPE];
    int requirementCount;

public:
    Recipe(){
        foodId = 0;
        requirementCount = 0;
    }

    Recipe(int id){
        foodId = id;
        requirementCount = 0;
    }

    int getFoodId(){
        return foodId;
    }

    int getReqCount(){
        return requirementCount;
    }

    IngredientRequirement getReq(int index){
        return requirements[index];
    }
// Recipe mein ingredient ID search karke uska index return karta hai.
// Ingredient nahi milne par -1 return hota hai.
    int findReq(int ingredientId){
        for (int i = 0; i < requirementCount; i++)
        {
            if (requirements[i].getIngredientId() == ingredientId)
                return i;
        }
        return -1;
    }// Agar ingredient pehle se hai toh quantity update hoti hai,
// warna available space mein naya ingredient add hota hai.

    void setRequirement(int ingredientId, float quantity){
        if (ingredientId <= 0 || quantity <= 0){
            cout << "Ingredient ID and quantity must be positive." << endl;
            return;
        }

        int index = findReq(ingredientId);
        if (index != -1){
            requirements[index].setQuantity(quantity);
        }
        else if (requirementCount < MAX_INGREDIENTS_PER_RECIPE){
            requirements[requirementCount] =
                IngredientRequirement(ingredientId, quantity);
            requirementCount++;
        }
        else{
            cout << "Recipe has reached its ingredient limit." << endl;
        }
    }
};

class RecipeModule{
private:
    Recipe recipes[MAX_RECIPES];
    int recipeCount;

public:
    RecipeModule(){
        recipeCount = 0;
    }
// Food ID ke basis par recipe search karta hai.
    int findRecipe(int foodId){
        for (int i = 0; i < recipeCount; i++){
            if (recipes[i].getFoodId() == foodId)
                return i;
        }
        return -1;
    }

    void setIngredient(int foodId, int ingredientId, float quantity){
        if (foodId <= 0 || ingredientId <= 0 || quantity <= 0){
            cout << "IDs and quantity must be positive." << endl;
            return;
        }// Recipe pehle se nahi hai toh nayi recipe banate hain,
// phir usmein ingredient ki requirement set karte hain.
        int index = findRecipe(foodId);

        if (index == -1){
            if (recipeCount >= MAX_RECIPES)
            {
                cout << "Recipe storage is full." << endl;
                return;
            }

            recipes[recipeCount] = Recipe(foodId);
            index = recipeCount;
            recipeCount++;
        }

        recipes[index].setRequirement(ingredientId, quantity);
    }

    int hasRecipe(int foodId){
        return findRecipe(foodId) != -1;
    }

    Recipe getRecipe(int foodId){
        int index = findRecipe(foodId);
        if (index == -1)
        {
            cout << "No recipe found for food ID " << foodId << endl;
            return Recipe();
        }
        return recipes[index];
    }

    void showRecipe(int foodId){
        int index = findRecipe(foodId);

        if (index == -1)
        {
            cout << "No recipe found for this food." << endl;
            return;
        }

        cout << "Recipe for food ID " << foodId
             << " (ingredients per serving):" << endl;

        for (int i = 0; i < recipes[index].getReqCount(); i++){
            IngredientRequirement item = recipes[index].getReq(i);
            cout << "Ingredient ID: " << item.getIngredientId()
                 << ", Quantity: " << item.getQuantity() << endl;
        }
    }
};

struct DemandEntry{
    int ingredientId;
    float quantity;
};

class DemandCalculator{
public:
    // Per-serving ingredient quantity ko servings se multiply karke
// total required quantity calculate karte hain.
    int calculate(int foodId, int servings, RecipeModule &recipeModule,DemandEntry result[]){
        if (servings <= 0){
            cout << "Servings must be greater than zero." << endl;
            return 0;
        }

        if (!recipeModule.hasRecipe(foodId)){
            cout << "No recipe available for food ID " << foodId << endl;
            return 0;
        }

        Recipe recipe = recipeModule.getRecipe(foodId);
        int resultCount = 0;

        for (int i = 0; i < recipe.getReqCount(); i++){
            IngredientRequirement item = recipe.getReq(i);
            result[resultCount].ingredientId = item.getIngredientId();
            result[resultCount].quantity = item.getQuantity() * servings;
            resultCount++;
        }
        // Calculate hui ingredient demand ko screen par display karte hain.
        return resultCount;
    }

    void showDemand(DemandEntry result[], int count){
        if (count == 0){
            cout << "No ingredient demand to display." << endl;
            return;
        }
        cout << "\nRequired ingredients:" << endl;
        for (int i = 0; i < count; i++){
            cout << "Ingredient ID: " << result[i].ingredientId
                 << ", Required quantity: " << result[i].quantity << endl;
        }
    }
};

struct PopularEntry{
    int foodId;
    int servings;
};
class PopularityAnalyzer{
public:
    // Sorts the supplied food totals from highest servings to lowest.
    void sortPopular(PopularEntry items[], int count){
        for (int i = 1; i < count; i++){
            PopularEntry current = items[i];
            int j = i- 1;

            while (j>=0 && items[j].servings < current.servings)
            {
                items[j+1] = items[j];
                j--;
            }

            items[j + 1] = current;
        }
    }
    void showPopular(PopularEntry items[], int count){
        sortPopular(items, count);
        cout << "\nPopular food items:" << endl;
        for (int i = 0; i < count; i++){
            cout << "Food ID: " << items[i].foodId
                 << ", Servings ordered: " << items[i].servings << endl;
        }
    }
};