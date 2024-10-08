#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>
#include<time.h>
#include<cstdlib>

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
    srand(time(0));
    int qno = rand()%6;

    string query = "SELECT * FROM questions WHERE Qno = " + to_string(qno);
    cout << "Executing query: " << query << endl;

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error executing query: " << mysql_error(conn) << endl;
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
    // int i = 1;
    // Print each row of the result set
   while ((row = mysql_fetch_row(res))) {
        cout << "Question:"<< qno<<" "<< (row[1] ? row[1] : "NULL") << endl; // Assuming column 1 contains the question
        // cout << "Options:" << endl;
        cout << "A. " << (row[2] ? row[2] : "NULL") << endl; // Assuming column 2 contains Option A
        cout << "B. " << (row[3] ? row[3] : "NULL") << endl; // Assuming column 3 contains Option B
        cout << "C. " << (row[4] ? row[4] : "NULL") << endl; // Assuming column 4 contains Option C
        cout << "D. " << (row[5] ? row[5] : "NULL") << endl; // Assuming column 5 contains Option D
    }

    // Cleanup
    mysql_free_result(res);
    mysql_close(conn);

    return EXIT_SUCCESS;
}
