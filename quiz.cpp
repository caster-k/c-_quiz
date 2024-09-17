#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server = "localhost"; // Change to your server address
    const char *user = "root"; // Change to your MySQL username
    const char *password = "Sunta@123..."; // Change to your MySQL password
    const char *database = "QUIZ"; // Change to your database name

    // Initialize MySQL connection
    conn = mysql_init(NULL);
    
    // Connect to the database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cerr << "mysql_real_connect() failed\n";
        return EXIT_FAILURE;
    }

    // Execute SQL query
    if (mysql_query(conn, "SELECT * FROM questions")) { // Change 'your_table' to your table name
        cerr << "SELECT * FROM your_table failed. Error: " << mysql_error(conn) << endl;
        return EXIT_FAILURE;
    }

    // Store the result set
    res = mysql_store_result(conn);
    
    if (res == NULL) {
        cerr << "mysql_store_result() failed. Error: " << mysql_error(conn) << endl;
        return EXIT_FAILURE;
    }

    // Print column names
    int num_fields = mysql_num_fields(res);
    MYSQL_FIELD *fields = mysql_fetch_fields(res);
    
    for (int i = 0; i < num_fields; i++) {
        cout << setw(20) << fields[i].name;
    }
    
    cout << endl;

    // Print each row of the result set
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            cout << setw(20) << (row[i] ? row[i] : "NULL");
        }
        cout << endl;
    }

    // Cleanup
    mysql_free_result(res);
    mysql_close(conn);

    return EXIT_SUCCESS;
}
