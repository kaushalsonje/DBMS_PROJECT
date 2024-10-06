#include <stdint.h>   // Include before MySQL headers to avoid redefinition warnings
#include "D:/mysql9.0.0-64/include/jdbc/mysql_driver.h"  // Use forward slashes
#include "D:\mysql9.0.0-64\include\jdbc\cppconn\driver.h"  // Use forward slashes
#include "D:/mysql9.0.0-64/include/jdbc/mysql_connection.h"
#include "D:/mysql9.0.0-64/include/jdbc/mysql_error.h"
#include "D:/mysql9.0.0-64/include/jdbc/cppconn/statement.h"
#include "D:/mysql9.0.0-64/include/jdbc/cppconn/resultset.h"
#include "D:/mysql9.0.0-64/include/jdbc/cppconn/exception.h"

#include <iostream>
using namespace std;

int main() {
    try {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        // Connect to the MySQL database
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "111307");

        // Select the database
        con->setSchema("test_db");

        // Execute query
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM test_table");

        // Fetch and display result
        while (res->next()) {
            cout << res->getString("column_name") << endl;
        }

        // Cleanup
        delete res;
        delete stmt;
        delete con;
    } catch (sql::SQLException &e) {
        cout << "SQLException: " << e.what() << endl;
    }
    return 0;
}
