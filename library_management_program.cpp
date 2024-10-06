/*
__________________________________________________________________________________________________________________
PROJECT NAME:- ------------------------LIBRARY MANAGEMENT SYSTEM------------------------------------- 
____________________________________________________________________________________________________________________
 brief description of the code:

Classes:

LibraryItem: An abstract base class representing a generic library item.
It has an item ID and a title.
It also declares a pure virtual function displayInfo(), making it an abstract class.
Book: A derived class from LibraryItem, representing a book.
It adds an additional attribute, author, and implements the displayInfo() function to display book information.
Member: Represents a library member with a member ID and a name.
It has functions to display and update member information.
LibraryManager: Manages the library items (books) and members.
It includes functions to add books, add members, update member information, delete books, delete members, display books, display members, and save library data to a file.

Concepts of OOP:

Inheritance: The Book and Member classes inherit from the LibraryItem class, demonstrating the concept of inheritance.
Polymorphism: The displayInfo() function is declared as a pure virtual function in the LibraryItem class, making it polymorphic. 
The Book class provides its implementation of this function.

Functions:

addBook(): Adds a new book to the library.
addMember(): Adds a new member to the library.
updateMemberInfo(): Updates the information of a library member.
deleteBook(): Deletes a book from the library.
deleteMember(): Deletes a member from the library.
displayBooks(): Displays information about all books in the library.
displayMembers(): Displays information about all library members.
saveToFile(const string& fileName): Saves library data (books and members) to a file.
main(): The main function contains a simple menu-driven program that allows the user to perform various operations on the library system.

Password Protection:

The program begins by prompting the user to enter a password ("aditya").
If the correct password is entered, the user gains access to the library management system menu.
Otherwise, the program displays an error message.
File Handling:

The program can save the library data to a file named "library_data.txt" using the saveToFile() function.
User Interface:

The user interacts with the program through a menu-driven console interface.
                  
*/
//______________________________________________________________________________________________________________________________________





#include <iostream>
#include <fstream>     // INCLUDING HEADERE FILE
#include <vector>
#include <string>
#include <conio.h>
using namespace std;

class LibraryItem {   //IMPLEMENTATION OF CLASS
public:
    int itemId;
    string title;

    LibraryItem(int id, string t) : itemId(id), title(t) {}

    virtual void displayInfo() const = 0; //USE OF VIRTUAL FUNCTION

    virtual ~LibraryItem() {} //VIRTUAL DISTRUCTOR
};

class Book : public LibraryItem { //Inheritance CONCEPT IMPLIMENTATION BOOK CLASS IS DERIVED FROM LIBRARY ITEM
public:
    string author;

    Book(int id, string t, string a) : LibraryItem(id, t), author(a) {}

    void displayInfo() const override {
        cout << "Book ID: " << itemId << ", Title: " << title << ", Author: " << author << endl;
    } //FUNCTION TO DISPLAY  INFORMATION
};

class Member { 
public:
    int memberId;
    string name;

    Member(int id, string n) : memberId(id), name(n) {}

    void displayInfo() const {
        cout << "Member ID: " << memberId << ", Name: " << name << endl;
    } // FUNCTION TO DISPLAY INFORMATION OF MEMBERS

    void updateInfo() {
        cout << "Enter new name for member: ";
        getline(cin, name);
    } //FUNCTION TO UPDATE THE MEMBER NAME
};

class LibraryManager {
public:
    vector<LibraryItem*> libraryItems; // IMPLEMENTATION OF VECTOR CONCEPT
    vector<Member*> members;

    ~LibraryManager() {
        // Clean up allocated memory
        for (auto item : libraryItems) {
            delete item;
        }
        for (auto member : members) {
            delete member;
        }
    }

    void addBook() {      // FUNCTION TO ADD BOOK WITH ITS TITLE,ID,AUTHOR
        int id;
        string title, author;

        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore(); // Clear newline from the input buffer

        cout << "Enter Book Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        Book* newBook = new Book(id, title, author); //DYNAMICAL MEMORY ALLOCATION
        libraryItems.push_back(newBook); //PUSHBACK METHOD OF VECTOR CONCEPT
    }

    void addMember() {   //FUNCTION TO ADD MEMBER WITH ITS ID ,NAME
        int id;
        string name;

        cout << "Enter Member ID: ";
        cin >> id;
        cin.ignore(); // Clear newline from the input buffer

        cout << "Enter Member Name: ";
        getline(cin, name);

        Member* newMember = new Member(id, name);
        members.push_back(newMember);
    }

    void updateMemberInfo() { //FUNCTION TO UPDATE MEMBER INFORMATION
        int memberId;

        cout << "Enter Member ID to update information: ";
        cin >> memberId;
        cin.ignore(); // Clear newline from the input buffer

        for (auto member : members) {
            if (member->memberId == memberId) { //SEARCHING FOR MEMBER WHOSE INFORMATION WE WANT TO UPDATE 
                member->updateInfo();
                cout << "Member information updated successfully." << endl;
                return;
            }
        }

        cout << "Member not found with ID: " << memberId << endl;
    }

    void deleteBook() { //DELETING THE BOOK
        int bookId;

        cout << "Enter Book ID to delete: ";
        cin >> bookId;
        cin.ignore(); // Clear newline from the input buffer

        for (auto it = libraryItems.begin(); it != libraryItems.end(); ++it) {
            if ((*it)->itemId == bookId) {
                delete *it;
                libraryItems.erase(it); 
                cout << "Book deleted successfully." << endl;
                return;
            }
        }

        cout << "Book not found with ID: " << bookId << endl;
    }

    void deleteMember() { // FUNCTION TO DELETE THE MEMBER
        int memberId;

        cout << "Enter Member ID to delete: ";
        cin >> memberId;
        cin.ignore(); // Clear newline from the input buffer

        for (auto it = members.begin(); it != members.end(); ++it) {
            if ((*it)->memberId == memberId) { //ITERATING USING ITERATOR TO SEARCH THE MEMBER ID TO DELETE
                delete *it;
                members.erase(it);
                cout << "Member deleted successfully." << endl;
                return;
            }
        }

        cout << "Member not found with ID: " << memberId << endl;
    }

    void displayBooks() const {  // DISPLAYING BOOKS
        cout << "=== Library Books ===" << endl;
        for (const auto& item : libraryItems) {
            item->displayInfo();
        }
    }

    void displayMembers() const {   //DISPLAYING MEMBERS
        cout << "=== Library Members ===" << endl;
        for (const auto& member : members) {
            member->displayInfo();
        }
    }

    void saveToFile(const string& fileName) const { //SAVING ALL INFORMATION IN FILE 
        ofstream outFile(fileName); //OUTPUT MODE 
        if (!outFile.is_open()) {
            cerr << "Error opening file for writing." << endl;
            return;
        }

        for (const auto& item : libraryItems) {
            outFile << "B " << item->itemId << " " << item->title << endl;
        }

        for (const auto& member : members) {
            outFile << "M " << member->memberId << " " << member->name << endl;
        }

        outFile.close(); //CLOSING THE FLE
        cout << "Library information saved to file." << endl;
    }
};

int main() {
    LibraryManager libraryManager;
    string pass = ""; //EMPTY STRING DECLARATION 
    //x :
     char c = ' ';
    cout << "\nEnter Program Password:";
    while (c != 13)
    {
        c = _getch(); //CLEARING  
        if (c != 13)
        {
            pass += c;
            cout << "*"; //DISPLAYING * TO HIDE THE PASSWORD
        }
    }
    if (pass == "aditya") //PASSWORD IS "aditya"
    {
        cout << "\a";
        int choice;
    
   

        do {                         // displaying menu 
            cout << "\nMenu:\n"
                    "1. Add Book\n"
                    "2. Add Member\n"
                    "3. Update Member Information\n"
                    "4. Delete Book\n"
                    "5. Delete Member\n"
                    "6. Display Books\n"
                    "7. Display Members\n"
                    "8. Save Library Data\n"
                    "9. Exit\n"
                    "Enter your choice: ";
            cin >> choice;
            cin.ignore(); // Clear newline from the input buffer

            switch (choice) {  //switch case 
                case 1:
                    libraryManager.addBook(); //add books
                    break;
                case 2:
                    libraryManager.addMember();//add members
                    break;
                case 3:
                    libraryManager.updateMemberInfo(); //update members
                    break;
                case 4:
                    libraryManager.deleteBook(); //delete books
                    break;
                case 5:
                    libraryManager.deleteMember();//delete member
                    break;
                case 6:
                    libraryManager.displayBooks(); //display books 
                    break;
                case 7:
                    libraryManager.displayMembers(); //display members
                    break;
                case 8:
                    libraryManager.saveToFile("library_data.txt"); // saving all info in file 
                    break;
                case 9:
                    cout << "Exiting the program. Goodbye!" << endl; //exit of program
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl; 
            }

        } while (choice != 9); // while loop condition
    }
    else
    {
        cout << "\n....Invalid Password....";
        //pass = _getch();
        //goto x;
    }

    return 0; //return statement
} // closing bracket of main function
//_______________________________________end of the program byeeeeeeeeeeeeeeeeeee____________________________________