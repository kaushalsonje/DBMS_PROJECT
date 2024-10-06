import datetime

class LibraryItem:
    def __init__(self, item_id, title):
        self.item_id = item_id
        self.title = title

    def display_info(self):
        raise NotImplementedError("Subclasses should implement this method!")

class Book(LibraryItem):
    def __init__(self, item_id, title, author):
        super().__init__(item_id, title)
        self.author = author
        self.is_issued = False
        self.issued_to = ""
        self.issue_date = ""
        self.return_date = ""

    def display_info(self):
        print(f"Book ID: {self.item_id}, Title: {self.title}, Author: {self.author}", end="")
        if self.is_issued:
            print(f" [Issued to: {self.issued_to}, Issue Date: {self.issue_date}]", end="")
        print()

class Member:
    def __init__(self, member_id, name):
        self.member_id = member_id
        self.name = name

    def display_info(self):
        print(f"Member ID: {self.member_id}, Name: {self.name}")

    def update_info(self):
        self.name = input("Enter new name for member: ")

class LibraryManager:
    def __init__(self):
        self.library_items = []
        self.members = []

    def add_book(self):
        item_id = int(input("Enter Book ID: "))
        title = input("Enter Book Title: ")
        author = input("Enter Author: ")
        new_book = Book(item_id, title, author)
        self.library_items.append(new_book)

    def add_member(self):
        member_id = int(input("Enter Member ID: "))
        name = input("Enter Member Name: ")
        new_member = Member(member_id, name)
        self.members.append(new_member)

    def update_member_info(self):
        member_id = int(input("Enter Member ID to update information: "))
        for member in self.members:
            if member.member_id == member_id:
                member.update_info()
                print("Member information updated successfully.")
                return
        print(f"Member not found with ID: {member_id}")

    def delete_book(self):
        book_id = int(input("Enter Book ID to delete: "))
        for item in self.library_items:
            if item.item_id == book_id:
                self.library_items.remove(item)
                print("Book deleted successfully.")
                return
        print(f"Book not found with ID: {book_id}")

    def delete_member(self):
        member_id = int(input("Enter Member ID to delete: "))
        for member in self.members:
            if member.member_id == member_id:
                self.members.remove(member)
                print("Member deleted successfully.")
                return
        print(f"Member not found with ID: {member_id}")

    def display_books(self):
        print("=== Library Books ===")
        for item in self.library_items:
            item.display_info()

    def display_members(self):
        print("=== Library Members ===")
        for member in self.members:
            member.display_info()

    def save_to_file(self, file_name):
        try:
            with open(file_name, 'w') as out_file:
                for item in self.library_items:
                    out_file.write(f"B {item.item_id} {item.title}\n")
                for member in self.members:
                    out_file.write(f"M {member.member_id} {member.name}\n")
            print("Library information saved to file.")
        except IOError:
            print("Error opening file for writing.")

    def get_current_date(self):
        return datetime.datetime.now().strftime("%Y-%m-%d")

    def issue_book(self):
        book_id = int(input("Enter Book ID to issue: "))
        member_name = input("Enter Member Name: ")
        for item in self.library_items:
            if isinstance(item, Book) and item.item_id == book_id and not item.is_issued:
                item.is_issued = True
                item.issued_to = member_name
                item.issue_date = self.get_current_date()
                print(f"Book issued successfully to {member_name} on {item.issue_date}.")
                return
        print("Book not found or already issued.")

    def return_book(self):
        book_id = int(input("Enter Book ID to return: "))
        for item in self.library_items:
            if isinstance(item, Book) and item.item_id == book_id and item.is_issued:
                item.is_issued = False
                item.return_date = self.get_current_date()
                print(f"Book returned successfully on {item.return_date}.")
                return
        print("Book not found or not issued.")

    def display_issue_return_status(self):
        print("ID\tTitle\t\tAuthor\t\tIssued To\tIssue Date\tReturn Date")
        for item in self.library_items:
            if isinstance(item, Book):
                if item.is_issued:
                    print(f"{item.item_id}\t{item.title}\t\t{item.author}\t\t"
                          f"{item.issued_to}\t\t{item.issue_date}\t"
                          f"{item.return_date if item.return_date else 'Not returned yet'}")
                else:
                    print(f"{item.item_id}\t{item.title}\t\t{item.author}\t\t[Not issued]\t---\t---")

def main():
    library_manager = LibraryManager()
    password = ""
    
    # Password input logic
    print("\nEnter Program Password:", end="")
    while True:
        ch = input()
        if ch == "kaushal":
            break
        else:
            print("Wrong Password, try again.")

    while True:
        print("\nMenu:\n"
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
              "12. Exit")
        choice = int(input("Enter your choice: "))

        if choice == 1:
            library_manager.add_book()
        elif choice == 2:
            library_manager.add_member()
        elif choice == 3:
            library_manager.update_member_info()
        elif choice == 4:
            library_manager.delete_book()
        elif choice == 5:
            library_manager.delete_member()
        elif choice == 6:
            library_manager.display_books()
        elif choice == 7:
            library_manager.display_members()
        elif choice == 8:
            library_manager.issue_book()
        elif choice == 9:
            library_manager.return_book()
        elif choice == 10:
            library_manager.display_issue_return_status()
        elif choice == 11:
            library_manager.save_to_file("library_data.txt")
        elif choice == 12:
            print("Exiting the program. Goodbye!")
            break
        else:
            print("Invalid choice. Please try again.")

if __name__ == "__main__":
    main()
