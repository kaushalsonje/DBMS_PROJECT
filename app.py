from flask import Flask, render_template, request
import mysql.connector

app = Flask(__name__)

# Database connection details
db = mysql.connector.connect(
    host="localhost",
    user="root",  # Replace with your MySQL username
    password="111307",  # Replace with your MySQL password
    database="library_db"  # Replace with your database name
)

cursor = db.cursor()

# Home route - Landing page
@app.route('/')
def home():
    return render_template('index.html')

# Route to search for a book by title or author
@app.route('/search', methods=['POST'])
def search():
    query = request.form['query']
    cursor.execute("SELECT * FROM books WHERE title LIKE %s OR author LIKE %s", ('%' + query + '%', '%' + query + '%'))
    results = cursor.fetchall()
    return render_template('index.html', books=results, query=query)

# Route to view all books
@app.route('/all_books')
def all_books():
    cursor.execute("SELECT * FROM books")
    results = cursor.fetchall()
    return render_template('index.html', books=results)

# Route to add a new book
@app.route('/add_book', methods=['POST'])
def add_book():
    title = request.form['title']
    author = request.form['author']

    cursor.execute("INSERT INTO books (title, author, is_issued) VALUES (%s, %s, 0)", (title, author))
    db.commit()

    message = "Book added successfully!"
    return render_template('index.html', message=message)

# Route to add a new member
@app.route('/add_member', methods=['POST'])
def add_member():
    name = request.form['name']

    cursor.execute("INSERT INTO members (name) VALUES (%s)", (name,))
    db.commit()

    message = "Member added successfully!"
    return render_template('index.html', message=message)

# Route to update a member's information
@app.route('/update_member', methods=['POST'])
def update_member():
    member_id = request.form['member_id']
    new_name = request.form['new_name']

    cursor.execute("UPDATE members SET name = %s WHERE id = %s", (new_name, member_id))
    db.commit()

    if cursor.rowcount > 0:
        message = "Member information updated successfully!"
    else:
        message = "Member not found."
    
    return render_template('index.html', message=message)

# Route to issue a book to a member
@app.route('/issue_book', methods=['POST'])
def issue_book():
    book_id = request.form['book_id']
    member_name = request.form['member_name']

    cursor.execute(
        "UPDATE books SET is_issued = 1, issued_to = %s, issue_date = CURDATE() WHERE id = %s AND is_issued = 0",
        (member_name, book_id)
    )
    db.commit()

    if cursor.rowcount > 0:
        message = "Book issued successfully!"
    else:
        message = "Book not found or already issued."
    
    return render_template('index.html', message=message)

# Route to return a book
@app.route('/return_book', methods=['POST'])
def return_book():
    book_id = request.form['book_id']

    cursor.execute(
        "UPDATE books SET is_issued = 0, return_date = CURDATE() WHERE id = %s AND is_issued = 1",
        (book_id,)
    )
    db.commit()

    if cursor.rowcount > 0:
        message = "Book returned successfully!"
    else:
        message = "Book not found or not issued."
    
    return render_template('index.html', message=message)

# Route to display issue/return status of books
@app.route('/issue_return_status')
def issue_return_status():
    cursor.execute("SELECT * FROM books")
    results = cursor.fetchall()
    return render_template('index.html', issue_return=results)

# Start the Flask app
if __name__ == '__main__':
    app.run(debug=True)
