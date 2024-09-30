#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <ctime>
#include <cstring> 

using namespace std;

// Shared atomic variables for user input and time extension tracking
atomic<char> option(' ');
atomic<bool> time_extended(false);

void inputHandler() {
    while (true) {
        char input;
        cin >> input;
        option = input;
    }
}

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char option;
    const char *server = "localhost";  // Your server address
    const char *user = "root";           // Your MySQL username
    const char *password = "Sunta@123...";       // Your MySQL password
    const char *database = "QUIZ";     // Your database name

    // Initialize MySQL connection
    conn = mysql_init(NULL);

    // Connect to the database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cout << "mysql_real_connect() failed\n";
        return EXIT_FAILURE;
    }

    // Generate 5 random question numbers without repetition
    
        string query = "SELECT * FROM answers WHERE Qno = 1";
        cout << "Executing query: " << query << endl;
        
        // Execute the query
        if (mysql_query(conn, query.c_str())) {
            cout << "Error executing query: " << mysql_error(conn) << endl;
            return EXIT_FAILURE;
        }

        // Store the result set
        res = mysql_store_result(conn);
        if (res == NULL) {
            cout << "mysql_store_result() failed. Error: " << mysql_error(conn) << endl;
            return EXIT_FAILURE;
        }
        
        row = mysql_fetch_row(res);
        cout<<"Enter the option";
        cin>>option;
        // cout<<"row[2] =";
        // cout<<row[1]<<endl<<option<<endl;

        if (strcmp(row[1], string(1, option).c_str()) == 0) {
    cout << "Your answer is correct" << endl;
        }
        

        // Free the result set for the current question
        mysql_free_result(res);
    

    // Cleanup MySQL connection
    mysql_close(conn);

    // Join input thread before exiting
   

    return EXIT_SUCCESS;
}
