//Press Alt + Z in VS Code to see comments better

#include <iostream>

using namespace std;


class Item {
    string name;
    int price;
    public:
        Item();
        Item(string s, int p);
        void display();
        string getName();
        int getPrice();
};

class Menu {
    Item menu_array[10] = {Item("Big Mac",500), Item("McFlurry Oreo",150), Item("Chicken McNuggets",200), Item("Happy Meal",400), Item("Strawberry Shake",300), Item("Brownie",650), Item("Plain Cone",120), Item("Choco Cone",160), Item("McCrispy",350), Item("Cappuccino",250)};

    public:
        void display();
        string getName(int i);
        int getPrice(int i);
};

class Order {
    static int current_order_number;
    int order_number;
    bool order_status;
    int ** order_items;
    int num_items;
    int total_bill;

    public:
        Order();
        Order(Menu *menu);
        ~Order(); //destructor deletes the order_items 2D array

        void display(Menu *menu);

        void setStatus(int i);
        int getStatus();
        int getOrderNumber();
        
};


class Node {
    public:
        //Order is a pointer because it will be created Dynamically using the "new" keyword.
        Order *order;
        Node* next;

        Node(); //default constructor
        ~Node(); //destructor deletes the "order"
        Node(Menu *menu); 
        //You may use a parameterized constructor as well if that's your style. It's fine if you don't.
};


class LinkedList {
    Node* head;
    Node* tail;

    public:
        LinkedList();
        ~LinkedList();  //destructor traverses the list from head to tail and deletes every Node

        //this method copies a node from another list to this list
        void add(Node *ptr);

        //this method adds an Order to the head of this list. It takes menu as argument because we must display the Menu to the user so that the user can choose the items for the menu.
        void addToTail(Menu *menu);

        //this function deletes the tail Order of the List. Be sure to use the "delete" keyword to delete the Node from memory as well. You may need to create a helper function to accomplish this task.
        void removeFromHead();

        //this moves the tail node of this list to the collect_queue list.
        //I'm passing the address of the collect_queue list, so I have a pointer in the function argument. You can choose to pass the list in any other way that works for you as long as the changes reflect in the original queue in the OrderSystem Class.
        //Remember, don't use the "delete" keyword in this function to delete the Node from this list. We want the node to remain in memory. We are not permanently deleting the node. We are just moving it from one list to another.

        friend void moveOrderToCollectList(LinkedList&, LinkedList&);

        //you may need a helper function (it's okay if you don't)
        Node* removeHeadWithoutDeleting(Node* ptr);

        // void display();
        
        //You can add parameters to the display() function. It's upto you.
        //One way is this:
        void display(Menu *menu, bool option);
        //I'm passing the menu to the display() function because it may need to display the name and price of items in the order, which are in the Menu
        //The "option" parameter saves you from having to make two different display() functions. If option=0, display only order_number of each order. If option=1, display all information of every order including items, price, etc.
        //You may need a heler function as well (it's upto you)
        void displayHelper(Node *ptr, Menu *menu, bool option);
};

class OrderSystem {
    Menu menu;
    LinkedList preparing_queue;
    LinkedList collect_queue;

    public:
        void addOrderToQueue();
        void setOrderToPrepared();

        void removeOrderFromQueue();
        void displayQueue();

        //I've already implementated this function for you :)
        bool displayOptions();
};

int main() {
    OrderSystem system;

    bool continue_or_not;
    do continue_or_not = system.displayOptions();
    while(continue_or_not);

    return 0;
}

//ITEM CLASS METHODS

Item::Item()  {
    name="";
    price=0;
}

Item::Item(string s, int p) {
    this->name=s;
    this->price=p;
}

void Item::display() {
    cout<<""<<name<<"\t"<<price<<endl;
}

string Item::getName() {
    return name;
}

int Item::getPrice() {
    return price;
}



//MENU CLASS METHODS


void Menu::display() {
    cout<<"\n----Menu----\nIndex\tName\tPrice\n";
    for(int i=0;i<10;i++) {
        cout<<i+1<<"\t";
        menu_array[i].display();
    }
    cout<<endl;
}

string Menu::getName(int i) {
    return menu_array[i].getName();
}

int Menu::getPrice(int i) {
    return menu_array[i].getPrice();
}


//ORDER CLASS METHODS AND ATTRIBUTE

Order::Order() {
    order_number=current_order_number+1;
    order_status = 0;
    order_items=NULL;
    num_items=0;
    total_bill=0;
}

Order::~Order() {
    for(int i=0;i<num_items;i++) {
        delete [] order_items[i];
    }
    delete [] order_items;
}

int Order::current_order_number=0;
//You can use this static variable to generate unique order_numbers. You can use some other way if you don't like this.

Order::Order(Menu *menu) {
    order_status = 0;
    order_number=current_order_number+1;
    current_order_number++;
    total_bill=0;

    cout<<"How many items do you want to order? ";
    cin>>num_items;
    order_items = new int*[num_items];

    for(int i=0;i<num_items;i++) {
        order_items[i]=new int[3];
    }

    menu->display();
    for(int i=0;i<num_items;i++) {
        cout<<"Enter Item "<<i+1<<" Number: ";
        cin>>order_items[i][0];
        order_items[i][0]--;

        cout<<"\tEnter Quantity of " << menu->getName(order_items[i][0]) <<": ";
        cin>>order_items[i][1];

        order_items[i][2] = menu->getPrice(order_items[i][0])*order_items[i][1];
        total_bill+=order_items[i][2];
    }
}

void Order::display(Menu *menu) {
    cout<<"\nOrder Number: "<<order_number<<endl;
    for(int i=0;i<num_items;i++) {
        cout<<menu->getName(order_items[i][0])<<"\tx"<<order_items[i][1]<<'\t'<<order_items[i][2]<<endl;
    }
    cout<<"Total Bill:"<<total_bill<<endl;
}

void Order::setStatus(int i) {
    this->order_status=i;
}

int Order::getStatus() {
    return this->order_status;
}
int Order::getOrderNumber() {
    return this->order_number;
}


//NODE CLASS METHODS

Node::Node() {
    next=NULL;
}

Node::~Node() {
    delete order;
}

Node::Node(Menu *menu) {
    order=new Order(menu);
    next=NULL;
}


//LINKED LIST CLASS METHODS

LinkedList::LinkedList() {
    head=NULL;
    tail=NULL;
}

LinkedList::~LinkedList() {
    Node *ptr = head;
    while(ptr!=NULL) {
        delete ptr;
        ptr = ptr->next;
    }
}

//this copies a node from another list to this list's tail
void LinkedList::add(Node *ptr) {
    //ptr is the node of the other list
    if (head==NULL) {
        head=ptr;
        tail=ptr;
        ptr->next=NULL;
    } else {
        tail->next=ptr;
        tail=tail->next;
        tail->next=NULL;
    }
    
}

void LinkedList::addToTail(Menu *menu) {
    if (head==NULL) {
        head=new Node(menu);
        tail=head;
    } else {
        tail->next = new Node(menu);
        tail=tail->next;
    }
    
}

void LinkedList::removeFromHead() {
    if (head!=NULL) {
        if (head==tail) {
            head=NULL;
            tail=NULL;
        } else {
            Node *temp = head;
            head=head->next;
            delete temp;
        }
    }

}


//if option=0, display only order_number of each order. If option=1, display all information of every order including items, price, etc.
void LinkedList::display(Menu *menu, bool option) {
    //callling the recursive function
    displayHelper(head, menu, option);
}

void LinkedList::displayHelper(Node *ptr, Menu *menu, bool option) {
    if(ptr!=NULL) {
        if(option==0)
            cout<<'\t'<<ptr->order->getOrderNumber()<<endl;
        else
            ptr->order->display(menu);
        
        displayHelper(ptr->next, menu, option);
    } else {}
}

void moveOrderToCollectList(LinkedList& preparing_queue, LinkedList& collect_queue) {
    if(preparing_queue.head!=NULL) {
        
        Node* temp;
        if(preparing_queue.head==preparing_queue.tail) {
            temp = preparing_queue.head;
            preparing_queue.head=NULL;
            preparing_queue.tail=NULL;
        } else {
            //remove the head Node from this list but don't delete it from memory
            temp = preparing_queue.removeHeadWithoutDeleting(preparing_queue.head);
            //I could have used the removeFromTail() function but that deletes the tail node from memory
        }

        //add the head of this list (prepared_queue) to the collect_queue list
        collect_queue.add(temp);
    }
}

//this function will remove the head node from the list
Node* LinkedList::removeHeadWithoutDeleting(Node* ptr) {
    Node *temp;
    if(head!=tail) {
        temp=head;
        head = head->next;
    } else {
        head=NULL;
        tail=NULL;
    }
    return temp;
}

//ORDER SYSTEM CLASS METHODS


void OrderSystem::addOrderToQueue() {
    preparing_queue.addToTail(&menu);
    cout<<"\nDisplaying Queue:\n---Preparing:---\n";
    preparing_queue.display(&menu,1);
}
void OrderSystem::setOrderToPrepared() {
    //preparing_queue.setTailToPrepared(&collect_queue);
    moveOrderToCollectList(preparing_queue, collect_queue);
    cout<<"\nDisplaying Queue:\n---Preparing:---\n";
    preparing_queue.display(&menu,0);
    cout<<"\n---Collect:---\n";
    collect_queue.display(&menu, 0);
}

void OrderSystem::removeOrderFromQueue() {
    collect_queue.removeFromHead();
    cout<<"---Preparing:---\n";
    preparing_queue.display(&menu, 0);
    cout<<"\n---Collect:---\n";
    collect_queue.display(&menu,0);
}
void OrderSystem::displayQueue() {
    cout<<"\n---Preparing---\n";
    preparing_queue.display(&menu,1);
    cout<<"\n---Collect:---\n";
    collect_queue.display(&menu,1);
}



bool OrderSystem::displayOptions() {
    cout<<"\nPress 1 to Place New Order\nPress 2 to Set Order to Prepared\nPress 3 to Collect Order\nPress 4 to View Queue\nPress 5 to Quit.\nYour Input: ";
    int option;
    cin>>option;

    switch (option) {
        case 1:
            addOrderToQueue();
            return 1;
            break;
        case 2:
            setOrderToPrepared();
            return 1;
            break;
        case 3:
            removeOrderFromQueue();
            return 1;
            break;
        case 4:
            displayQueue();
            return 1;
            break;
        default:
            return 0;
    }
}

//END