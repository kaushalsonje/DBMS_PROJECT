from flask import Flask, render_template, request, redirect, url_for, flash
import mysql.connector

app = Flask(__name__)
app.secret_key = '1112131415161718'  # Needed for flashing messages

# Database connection function
def get_db_connection():
    conn = mysql.connector.connect(
        host="localhost", 
        user="root",
        password="111307",
        database="library_db"
    )
    return conn

@app.route('/')
def home():
    return render_template('ksindex.html')

# Route to add a new book
@app.route('/add_book', methods=['GET', 'POST'])
def add_book():
    if request.method == 'POST':
        id = request.form['id']
        title = request.form['title']
        author = request.form['author']

        # Insert into database
        conn = get_db_connection()
        cursor = conn.cursor()
        query = "INSERT INTO books (id,title, author) VALUES (%s,%s, %s)"
        cursor.execute(query, (id, title, author))
        conn.commit()
        cursor.close()
        conn.close()

        # Flash success message
        flash('Book added successfully!')
        return redirect(url_for('add_book'))
    
    return render_template('add_book.html')

# Route to issue a book
@app.route('/issue_book', methods=['GET', 'POST'])
def issue_book():
    if request.method == 'POST':
        book_id = request.form['book_id']
        issued_to = request.form['issued_to']
        
        # Update database
        conn = get_db_connection()
        cursor = conn.cursor()
        query = "UPDATE books SET is_issued = TRUE, issued_to = %s, issue_date = CURDATE() WHERE id = %s AND is_issued = FALSE"
        cursor.execute(query, (issued_to, book_id))
        conn.commit()

        if cursor.rowcount > 0:
            flash('Book issued successfully!')
        else:
            flash('Book is already issued or does not exist.')
        
        cursor.close()
        conn.close()
        return redirect(url_for('issue_book'))

    return render_template('issue_book.html')

# Route to return a book
@app.route('/return_book', methods=['GET', 'POST'])
def return_book():
    if request.method == 'POST':
        book_id = request.form['book_id']
        
        # Update database
        conn = get_db_connection()
        cursor = conn.cursor()
        query = "UPDATE books SET is_issued = FALSE, issued_to = NULL, issue_date = NULL, return_date = CURDATE() WHERE id = %s AND is_issued = TRUE"
        cursor.execute(query, (book_id,))
        conn.commit()

        if cursor.rowcount > 0:
            flash('Book returned successfully!')
        else:
            flash('Book is not currently issued or does not exist.')
        
        cursor.close()
        conn.close()
        return redirect(url_for('return_book'))

    return render_template('return_book.html')

# Route to search for books by title or author
@app.route('/search', methods=['GET', 'POST'])
def search():
    if request.method == 'POST':
        search_query = request.form['search_query']
        
        conn = get_db_connection()
        cursor = conn.cursor(dictionary=True)
        query = "SELECT * FROM books WHERE title LIKE %s OR author LIKE %s"
        search_pattern = "%" + search_query + "%"
        cursor.execute(query, (search_pattern, search_pattern))
        results = cursor.fetchall()
        cursor.close()
        conn.close()
        
        return render_template('search_results.html', results=results)

    return render_template('search.html')

# Route to add a new member
@app.route('/add_member', methods=['GET', 'POST'])
def add_member():
    if request.method == 'POST':
        member_id = request.form['member_id']
        name = request.form['name']

        # Insert into database
        conn = get_db_connection()
        cursor = conn.cursor()
        query = "INSERT INTO members (id, name) VALUES (%s, %s)"
        cursor.execute(query, (member_id, name))
        conn.commit()
        cursor.close()
        conn.close()

        flash('Member added successfully!')
        return redirect(url_for('add_member'))
    
    return render_template('add_member.html')

# Route to update a member's name by ID
@app.route('/update_member', methods=['GET', 'POST'])
def update_member():
    if request.method == 'POST':
        member_id = request.form['member_id']
        name = request.form['name']

        # Update database
        conn = get_db_connection()
        cursor = conn.cursor()
        query = "UPDATE members SET name = %s WHERE id = %s"
        cursor.execute(query, (name, member_id))
        conn.commit()
        cursor.close()
        conn.close()

        flash('Member updated successfully!')
        return redirect(url_for('update_member'))

    # Fetch all members to populate the update form
    conn = get_db_connection()
    cursor = conn.cursor(dictionary=True)
    cursor.execute("SELECT * FROM members")
    members = cursor.fetchall()
    cursor.close()
    conn.close()

    return render_template('update_member.html', members=members)

# Route to list all members
@app.route('/members')
def members():
    conn = get_db_connection()
    cursor = conn.cursor(dictionary=True)
    cursor.execute("SELECT * FROM members")
    members = cursor.fetchall()
    cursor.close()
    conn.close()

    return render_template('members.html', members=members)

if __name__ == '__main__':
    app.run(debug=True)
