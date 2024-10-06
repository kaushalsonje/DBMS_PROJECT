#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h>
#include <ctime> 
 
using namespace std;

class LibraryItem {
public:
    int itemId;
    string title;

    LibraryItem(int id, string t) : itemId(id), title(t) {}

    virtual void displayInfo() const = 0;
    virtual ~LibraryItem() {}
};

class Book : public LibraryItem {
public:
    string author;
    bool isIssued;
    string issuedTo;
    string issueDate;
    string returnDate;

    Book(int id, string t, string a) : LibraryItem(id, t), author(a), isIssued(false), issuedTo(""), issueDate(""), returnDate("") {}

    void displayInfo() const override {
        cout << "Book ID: " << itemId << ", Title: " << title << ", Author: " << author;
        if (isIssued) {
            cout << " [Issued to: " << issuedTo << ", Issue Date: " << issueDate << "]";
        }
        cout << endl;
    }
};

class Member {
public:
    int memberId;
    string name;

    Member(int id, string n) : memberId(id), name(n) {}

    void displayInfo() const {
        cout << "Member ID: " << memberId << ", Name: " << name << endl;
    }

    void updateInfo() {
        cout << "Enter new name for member: ";
        getline(cin, name);
    }
};

class LibraryManager {
public:
    vector<LibraryItem*> libraryItems;
    vector<Member*> members;

    ~LibraryManager() {
        for (auto item : libraryItems) {
            delete item;
        }
        for (auto member : members) {
            delete member;
        }
    }

    void addBook() {
        int id;
        string title, author;

        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Book Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        Book* newBook = new Book(id, title, author);
        libraryItems.push_back(newBook);
    }

    void addMember() {
        int id;
        string name;

        cout << "Enter Member ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Member Name: ";
        getline(cin, name);

        Member* newMember = new Member(id, name);
        members.push_back(newMember);
    }

    void updateMemberInfo() {
        int memberId;

        cout << "Enter Member ID to update information: ";
        cin >> memberId;
        cin.ignore();

        for (auto member : members) {
            if (member->memberId == memberId) {
                member->updateInfo();
                cout << "Member information updated successfully." << endl;
                return;
            }
        }

        cout << "Member not found with ID: " << memberId << endl;
    }

    void deleteBook() {
        int bookId;

        cout << "Enter Book ID to delete: ";
        cin >> bookId;
        cin.ignore();

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

    void deleteMember() {
        int memberId;

        cout << "Enter Member ID to delete: ";
        cin >> memberId;
        cin.ignore();

        for (auto it = members.begin(); it != members.end(); ++it) {
            if ((*it)->memberId == memberId) {
                delete *it;
                members.erase(it);
                cout << "Member deleted successfully." << endl;
                return;
            }
        }

        cout << "Member not found with ID: " << memberId << endl;
    }

    void displayBooks() const {
        cout << "=== Library Books ===" << endl;
        for (const auto& item : libraryItems) {
            item->displayInfo();
        }
    }

    void displayMembers() const {
        cout << "=== Library Members ===" << endl;
        for (const auto& member : members) {
            member->displayInfo();
        }
    }

    void saveToFile(const string& fileName) const {
        ofstream outFile(fileName);
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

        outFile.close();
        cout << "Library information saved to file." << endl;
    }

    string getCurrentDate() {
        time_t now = time(0);
        tm* localtm = localtime(&now);
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d", localtm);
        return string(buffer);
    }

    void issueBook() {
        int bookId;
        string memberName;

        cout << "Enter Book ID to issue: ";
        cin >> bookId;
        cin.ignore();

        cout << "Enter Member Name: ";
        getline(cin, memberName);

        for (auto& item : libraryItems) {
            Book* book = dynamic_cast<Book*>(item);
            if (book && book->itemId == bookId && !book->isIssued) {
                book->isIssued = true;
                book->issuedTo = memberName;
                book->issueDate = getCurrentDate();
                cout << "Book issued successfully to " << memberName << " on " << book->issueDate << "." << endl;
                return;
            }
        }

        cout << "Book not found or already issued." << endl;
    }

    void returnBook() {
        int bookId;

        cout << "Enter Book ID to return: ";
        cin >> bookId;
        cin.ignore();

        for (auto& item : libraryItems) {
            Book* book = dynamic_cast<Book*>(item);
            if (book && book->itemId == bookId && book->isIssued) {
                book->isIssued = false;
                book->returnDate = getCurrentDate();
                cout << "Book returned successfully on " << book->returnDate << "." << endl;
                return;
            }
        }

        cout << "Book not found or not issued." << endl;
    }

    void displayIssueReturnStatus() const {
    cout << "ID\tTitle\t\tAuthor\t\tIssued To\tIssue Date\tReturn Date" << endl;
    for (const auto& item : libraryItems) {
        const Book* book = dynamic_cast<const Book*>(item);
        if (book) {
            if (book->isIssued) {
                cout << book->itemId << "\t" << book->title << "\t\t" << book->author 
                     << "\t\t" << book->issuedTo 
                     << "\t\t" << book->issueDate 
                     << "\t" << (book->returnDate.empty() ? "Not returned yet" : book->returnDate) 
                     << endl;
            } else {
                cout << book->itemId << "\t" << book->title << "\t\t" << book->author 
                     << "\t\t" << "[Not issued]" 
                     << "\t" << "---" 
                     << "\t" << "---" 
                     << endl;
            }
        }
    }
}

};

int main() {
    LibraryManager libraryManager;
    string pass = "";
    char c = ' ';
    cout << "\nEnter Program Password:";
    while (c != 13) {
        c = _getch();
        if (c != 13) {
            pass += c;
            cout << "*";
        }
    }

    if (pass == "kaushal") {
        cout << "\a";
        int choice;

        do {
            cout << "\nMenu:\n"
                    "1. Add Book\n"
                    "2. Add Member\n"
                    "3. Update Member Information\n"
                    "4. Delete Book\n"
                    "5. Delete Member\n"
                    "6. Display Books\n"
                    "7. Display Members\n"
                    "8. Issue Book\n"
                    "9. Return Book\n"
                    "10. Display Issue/Return Status\n"
                    "11. Save Library Data\n"
                    "12. Exit\n"
                    "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    libraryManager.addBook();
                    break;
                case 2:
                    libraryManager.addMember();
                    break;
                case 3:
                    libraryManager.updateMemberInfo();
                    break;
                case 4:
                    libraryManager.deleteBook();
                    break;
                case 5:
                    libraryManager.deleteMember();
                    break;
                case 6:
                    libraryManager.displayBooks();
                    break;
                case 7:
                    libraryManager.displayMembers();
                    break;
                case 8:
                    libraryManager.issueBook();
                    break;
                case 9:
                    libraryManager.returnBook();
                    break;
                case 10:
                    libraryManager.displayIssueReturnStatus();
                    break;
                case 11:
                    libraryManager.saveToFile("library_data.txt");
                    break;
                case 12:
                    cout << "Exiting the program. Goodbye!" << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }

        } while (choice != 12);
    } else {
        cout << "\nWrong Password\n";
    }

    return 0;
}
