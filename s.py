import mysql.connector
from datetime import datetime

class LibraryManager:
    def __init__(self):
        # Establish a connection to the MySQL database
        self.conn = mysql.connector.connect(
            host='localhost',
            user='root',  # Replace with your MySQL username
            password='111307',  # Replace with your MySQL password
            database='library_db'
        )
        self.cursor = self.conn.cursor()

    def add_book(self):
        # Get user input for book details
        book_id = int(input("Enter Book ID: "))
        title = input("Enter Book Title: ")
        author = input("Enter Book Author: ")

        # Insert new book record into the database
        query = """
        INSERT INTO books (id, title, author, is_issued, issued_to, issue_date, return_date) 
        VALUES (%s, %s, %s, %s, %s, %s, %s)
        """
        self.cursor.execute(query, (book_id, title, author, False, None, None, None))
        self.conn.commit()
        print("Book added successfully.")

    def add_member(self):
        # Get user input for member details
        member_id = int(input("Enter Member ID: "))
        name = input("Enter Member Name: ")

        # Insert new member record into the database
        query = "INSERT INTO members (id, name) VALUES (%s, %s)"
        self.cursor.execute(query, (member_id, name))
        self.conn.commit()
        print("Member added successfully.")

    def issue_book(self):
        # Get the book ID and member name for issuing
        book_id = int(input("Enter Book ID to issue: "))
        member_name = input("Enter Member Name: ")

        # Check if the book is already issued
        self.cursor.execute("SELECT is_issued FROM books WHERE id = %s", (book_id,))
        result = self.cursor.fetchone()

        if result and result[0]:  # Book is already issued
            print("The book is already issued.")
        else:
            issue_date = datetime.now().strftime('%Y-%m-%d')

            # Update book record to mark it as issued
            query = """
            UPDATE books 
            SET is_issued = %s, issued_to = %s, issue_date = %s 
            WHERE id = %s
            """
            self.cursor.execute(query, (True, member_name, issue_date, book_id))
            self.conn.commit()
            print(f"Book issued to {member_name} on {issue_date}.")

    def return_book(self):
        # Get the book ID to return
        book_id = int(input("Enter Book ID to return: "))

        # Check if the book is issued
        self.cursor.execute("SELECT is_issued FROM books WHERE id = %s", (book_id,))
        result = self.cursor.fetchone()

        if result and result[0]:  # Book is issued
            return_date = datetime.now().strftime('%Y-%m-%d')

            # Update book record to mark it as returned
            query = "UPDATE books SET is_issued = %s, return_date = %s WHERE id = %s"
            self.cursor.execute(query, (False, return_date, book_id))
            self.conn.commit()
            print(f"Book returned on {return_date}.")
        else:
            print("The book is not issued yet.")

    def display_books(self):
        # Fetch and display all books from the database
        self.cursor.execute("SELECT * FROM books")
        books = self.cursor.fetchall()

        print("\n=== List of Books ===")
        print("ID | Title | Author | Is Issued | Issued To | Issue Date | Return Date")
        for book in books:
            print(f"{book[0]} | {book[1]} | {book[2]} | {'Yes' if book[3] else 'No'} | {book[4]} | {book[5]} | {book[6]}")

    def display_members(self):
        # Fetch and display all members from the database
        self.cursor.execute("SELECT * FROM members")
        members = self.cursor.fetchall()

        print("\n=== List of Members ===")
        print("ID | Name")
        for member in members:
            print(f"{member[0]} | {member[1]}")

    def display_issue_return_status(self):
        # Fetch and display the status of issued books
        self.cursor.execute("SELECT * FROM books WHERE is_issued = TRUE")
        books = self.cursor.fetchall()

        print("\n=== Issue/Return Status ===")
        print("ID | Title | Author | Issued To | Issue Date | Return Date")
        for book in books:
            print(f"{book[0]} | {book[1]} | {book[2]} | {book[4]} | {book[5]} | {book[6]}")

    def save_to_file(self, file_name):
        # Save book data to a CSV file
        self.cursor.execute("SELECT * FROM books")
        books = self.cursor.fetchall()

        with open(file_name, 'w') as file:
            file.write("ID,Title,Author,Is Issued,Issued To,Issue Date,Return Date\n")
            for book in books:
                file.write(f"{book[0]},{book[1]},{book[2]},{book[3]},{book[4]},{book[5]},{book[6]}\n")

        print(f"Library data saved to {file_name}")

    def search_book(self):
        # Search for books by title or author
        search_term = input("Enter book title or author to search: ").strip()

        query = "SELECT * FROM books WHERE title LIKE %s OR author LIKE %s"
        self.cursor.execute(query, (f"%{search_term}%", f"%{search_term}%"))
        books = self.cursor.fetchall()

        if books:
            print("\n=== Search Results ===")
            print("ID | Title | Author | Is Issued | Issued To | Issue Date | Return Date")
            for book in books:
                print(f"{book[0]} | {book[1]} | {book[2]} | {'Yes' if book[3] else 'No'} | {book[4]} | {book[5]} | {book[6]}")
        else:
            print("No matching books found.")

    def __del__(self):
        # Close the database connection
        self.conn.close()

def main():
    library_manager = LibraryManager()

    while True:
        print("\n--- Library Menu ---")
        print("1. Add Book")
        print("2. Add Member")
        print("3. Display Books")
        print("4. Display Members")
        print("5. Issue Book")
        print("6. Return Book")
        print("7. Display Issue/Return Status")
        print("8. Save Library Data to File")
        print("9. Search for Book by Title or Author")
        print("10. Exit")

        try:
            choice = int(input("\nEnter your choice: "))
        except ValueError:
            print("Invalid input! Please enter a number.")
            continue

        if choice == 1:
            library_manager.add_book()
        elif choice == 2:
            library_manager.add_member()
        elif choice == 3:
            library_manager.display_books()
        elif choice == 4:
            library_manager.display_members()
        elif choice == 5:
            library_manager.issue_book()
        elif choice == 6:
            library_manager.return_book()
        elif choice == 7:
            library_manager.display_issue_return_status()
        elif choice == 8:
            file_name = input("Enter file name to save data (e.g., library_data.csv): ")
            library_manager.save_to_file(file_name)
        elif choice == 9:
            library_manager.search_book()
        elif choice == 10:
            print("Exiting the program.")
            break
        else:
            print("Invalid choice, please try again.")

if __name__ == "__main__":
    main()
