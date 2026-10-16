#include<iostream>
#include<string>
using namespace std;
const int max_food=100;
const int max_item=10;
const int max_order=100;
// Order ki current state ko represent karne ke liye enum use kiya hai.
enum orderstatus{
    PENDING,
    CONFIRMED,
    CANCELLED,
    COMPLETED
};

// Food ki details private rakhi hain taaki unhe getter aur setter se access kar sakein.
class food_item{
private:
int id;
string name;
double price;
bool available;
public:
food_item(){
    id=0;
    name="";
    price=0;
    available=false;
}
food_item(int id,string name,double price,bool available){
    this->id=id;
    this->name=name;
    this->price=price;
    this->available=available;
}

    // getter and setter fucntion for accessing and modifiction of private members
int getid() const{
    return id;
}
string getname() const{
    return name;
}
double getprice() const{
    return price;
}
bool getavailable() const{
return available;
} 
void setname(string name){
    this->name=name;
}
void setprice(double price){
if(price>0){
    this->price=price;
}
else{
    cout<<"enter valid price";
}
}
void setavailability(bool available){
    this->available=available;
}
void display(){
    cout<<"ID : "<<id<<"->"<<" NAME : "<<name<<" : "<<" PRICE : "<<price<<" : "<<" AVAILABLE : "<<(available ? "YES" : "NO")<<endl;
}
};

    // second class:- order_item class
// Ye class food item ki details ke saath order ki quantity bhi store karti hai.
class order_item : public food_item {
private:
    int quantity;
public:
    order_item(){
        quantity=0;
    }

    order_item(const food_item& food, int quantity)
        : food_item(food), quantity(quantity) {}

    // Ek food item ki price aur quantity ko multiply karke total nikalte hain.
double gettotal() const {
    return getprice() * quantity;
}
void display(){
    cout<<"foodname : "<<getname()<<" price : "<<getprice()<<" quantity : "<<quantity<<" total amount : "<<gettotal()<<endl;
}
};

   // thrid class:- order class 
class order{
private:
int orderid;
orderstatus status;
order_item items[max_item];
int itemcount;
public:
order(){
    orderid=0;
    status=PENDING;
    itemcount=0;
}
order(int id){
    this->orderid=id;
    status=PENDING;
    itemcount=0;
}

bool additem(const food_item &food,int quantity){
    if(quantity<=0||itemcount>=max_item){
        return false;
    }
    items[itemcount]=order_item(food, quantity);
    itemcount++;
    return true;
}

    // to check whether order placed or not
bool isEmpty(){
    return itemcount==0;
}
int getorderid(){
    return orderid;
}
orderstatus getstatus(){
    return status;
}
void setstatus(orderstatus s){
    status=s;
}

void display(){
    cout<<"order id : "<<orderid<<endl;
    cout<<"status : ";
    if(status==PENDING){
        cout<<"pending"<<endl;  
    }
    else if(status==CONFIRMED){
        cout<<"confirmed"<<endl;
    }
    else if(status==CANCELLED){
        cout<<"cancelled"<<endl;
    }
    else{
        cout<<"completed"<<endl;
    }
    double total=0;
    for(int i=0;i<itemcount;i++){
        items[i].display();
        total+=items[i].gettotal();
    }
    cout<<"total : Rs. "<<total<<endl;
}
};

    // fourth class :- orderManager class
class orderManager{
    private:
    food_item foods[max_food];
    order orders[max_order];
    int foodcount;
    int ordercount;
    int nextorderid;
  // Food ID ko array mein search karta hai aur matching index return karta hai.
// Agar food nahi milta toh -1 return hota hai.
    int findfood(int id){
        for(int i=0;i<foodcount;i++){
            if(foods[i].getid()==id){
                return i;
            }
        }
return -1;
    }

    public:
    orderManager(){
        foodcount=0;
        ordercount=0;
        nextorderid=1001;
        foods[foodcount++]=food_item(1,"PIZZA",200,true);
        foods[foodcount++]=food_item(2,"BIRYANI",100,true);
    }

    void displayMenu(){
        cout<<"\n__ FOOD MENU__\n";
        for(int i=0;i<foodcount;i++){
            foods[i].display();
        }
    }

    // search food by ID
    void searchfood(){
        int id;
        cout<<"enter food ID: ";
        cin>>id;
        int index=findfood(id);
        if(index==-1){
            cout<<"Food item not found\n";
            return;
        }
        foods[index].display();
    }

    // Update food information
void updatefood(){
    int id;
    cout<<"enter food id to update details"<<endl;
    cin>>id;
    int index=findfood(id);
    if(index==-1){
        cout<<"food item not found"<<endl;
        return;
    }
    int choice;
    cout<<"\n 1 : update name "<<endl;
    cout<<"2 : updtae price "<<endl;
    cout<<"3 : update availabilty "<<endl;
    cout<<" enter choice : ";
    cin>>choice;
    if(choice==1){
        string name;
        cout<<"enter name ";
        cin.ignore(100,'\n');
        getline(cin,name);
        foods[index].setname(name);
        cout<<"name is updated"<<endl;
    }

    else if(choice==2){
        double price;
        cout<<"enter price for updation "<<endl;
        cin>>price;
        if(price<=0){
            cout<<"invalid price inserted"<<endl;
            return;
        }
        foods[index].setprice(price);
        cout<<"price upadted"<<endl;
    }

    else if(choice==3){
        int choiceavailable;
         cout << "Enter 1 for available, 0 for unavailable: ";
            cin >> choiceavailable;

            if (choiceavailable != 0 && choiceavailable != 1) {
                cout << "Invalid availability choice.\n";
                return;
            }

            foods[index].setavailability(choiceavailable == 1);

            cout << "Food availability updated.\n";
        }

        else{
            cout<<"invalid choice\n"<<endl;
        }

    }

    // place order
    void placeorder(){
        if(ordercount>=max_order){
            cout<<"order storage is full"<<endl;
            return;
        }
        // Naya order banate hain aur user se food ID aur quantity lete hain.

        order neworder(nextorderid);
        cout<<"\nenter food id and qauntity"<<endl;
        cout<<"\nenter 0 to finish"<<endl;
        while(true){
            int id;
            cout<<"enter food id"<<endl;
            cin>>id;
            if(cin.fail()){
                cin.clear();
                cin.ignore(1000,'\n');
                cout<<"invalid input"<<endl;
                continue;
            }

                        if(id==0){
                cout<<"exit";
                break;
            }

            int index=findfood(id);
            if(index==-1){
                cout<<"food item not found "<<endl;
                continue;
            }

            if(!foods[index].getavailable()){
                cout<<"this food is unavailable";
                continue;
            }

            int quantity;
            cout<<"enter quantity ";
            cin>>quantity;
              if(cin.fail()){
                cin.clear();
                cin.ignore(1000,'\n');
                cout<<"invalid input"<<endl;
                continue;
            }

            if(quantity<=0){
                cout<<"enter valid quantity "<<endl;
                continue;
            }

            if(!neworder.additem(foods[index],quantity)){
            cout<<"order has reached its item limit "<<endl;
            break;
            }

            cout<<"item added to order "<<endl;
        }
// Agar koi item add nahi hua toh empty order save nahi karte.
if(neworder.isEmpty()){
    cout<<"no item ordered"<<endl;
    return;
}

orders[ordercount]=neworder;
ordercount++;
nextorderid++;
cout<<" order placed succesfully"<<endl;
neworder.display();
    }

    // order history
    void orderhistory(){
        if(ordercount==0){
            cout<<" no ordert placed yet"<<endl;
            return;
        }

        for(int i=0;i<ordercount;i++){
            orders[i].display();
        }
    }

    //search order by order ID
    void searchorder(){
        int id;
        cout<<"enter order id to be searched"<<endl;
        cin>>id;
        for(int i=0;i<ordercount;i++){
            if(orders[i].getorderid()==id){
                orders[i].display();
                return;
            }
        }
        cout<<"order not found"<<endl;
    }

    // update order status 
    void updateOrderStatus(){
        int id;
        cout<<"enter order id"<<endl;
        cin>>id;
        for(int i=0;i<ordercount;i++){
            if(orders[i].getorderid()==id){
                int choice;
                cout<<" 1: confirm orders "<<endl;
                cout<<" 2: complete order "<<endl;
                cout<<" 3: cancel order "<<endl;// Order ka status current state ke according hi update karte hain.
                cout<<"enter choice ";
                cin>>choice;
                if(choice==1 && orders[i].getstatus()==PENDING){
                    orders[i].setstatus(CONFIRMED);
                }
                else if(choice==2 && orders[i].getstatus()==CONFIRMED){
                    orders[i].setstatus(COMPLETED);
                }
                else if(choice==3 && (orders[i].getstatus()==PENDING||orders[i].getstatus()==CONFIRMED)){
                    orders[i].setstatus(CANCELLED);
                }
                else{
                    cout<<"invalid choice"<<endl;
                    return;
                }
                cout<<"order status updated"<<endl;
                orders[i].display();
                return;
            }
        }
        cout<<"order not found "<<endl;
    }

    // Main Menu
    void menu(){
        int choice;
        do{
            cout<<"\n_____ORDER MANAGEMENT______\n";
            cout<<" 1 : Display Food Menu "<<endl;
            cout<<" 2 : Search Food "<<endl;
            cout<<" 3 : Place Order "<<endl;
            cout<<" 4 : Order History "<<endl;
            cout<<" 5 : Search Order by ID "<<endl;
            cout<<" 6 : Update Food information "<<endl;
            cout<<" 7 : Update Order Status "<<endl;
            cout<<" 0 : exit"<<endl;
            cout<<"enter choice ";
            if(!(cin>>choice)){
                cout<<"invalid choice "<<endl;
                break;
            }
            switch (choice)
            {
            case 1:
                displayMenu();
                    break;
            
            case 2:
                searchfood();
                    break;

            case 3:
                placeorder();
                    break;

            case 4:
                orderhistory();
                    break;

            case 5:
                searchorder();
                    break;
            case 6:
                updatefood();
                    break;

            case 7:
                updateOrderStatus();
                    break;

            case 0:
                cout<<" Exit "<<endl;
                break;
            default:
                cout<<"invalid choice"<<endl;        
            }
        }while(choice!=0);
    }
};
