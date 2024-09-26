#include <mysql/mysql.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <set>
#include <vector>
#include <string>

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

    // Randomly select 5 unique questions
    srand(static_cast<unsigned int>(time(0)));
    set<int> qno;

    while (qno.size() < 5) {
        int r = rand() % 5 + 1; // Generate numbers from 1 to 5
        qno.insert(r);
    }

    vector<int> questions(qno.begin(), qno.end());

    // Timer for displaying questions
    for (int i = 0; i < 5; ++i) {
        int question_no = questions[i];
        string query = "SELECT * FROM questions WHERE Qno = " + to_string(question_no);
        cout << "Executing query: " << query << endl;

        if (mysql_query(conn, query.c_str())) {
            cerr << "Error executing query: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return EXIT_FAILURE;
        }

        // Store the result set
        res = mysql_store_result(conn);
        if (res == NULL) {
            cerr << "mysql_store_result() failed. Error: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return EXIT_FAILURE;
        }

        // Fetch the row for the current question
        if ((row = mysql_fetch_row(res))) {
            // Print the question and options
            cout << "Question " << question_no << ": " << (row[1] ? row[1] : "NULL") << endl; // Assuming column 1 contains the question
            cout << "A. " << (row[2] ? row[2] : "NULL") << endl; // Assuming column 2 contains Option A
            cout << "B. " << (row[3] ? row[3] : "NULL") << endl; // Assuming column 3 contains Option B
            cout << "C. " << (row[4] ? row[4] : "NULL") << endl; // Assuming column 4 contains Option C
            cout << "D. " << (row[5] ? row[5] : "NULL") << endl; // Assuming column 5 contains Option D

            // Timer for the current question
            for (int k = 9; k >= 0; --k) {
                cout << "\033[2J\033[1;1H"; // Clear screen (works in most terminal emulators)
                cout << "Question " << question_no << ": " << (row[1] ? row[1] : "NULL") << endl; // Print question again
                cout << "A. " << (row[2] ? row[2] : "NULL") << endl; // Print options again
                cout << "B. " << (row[3] ? row[3] : "NULL") << endl;
                cout << "C. " << (row[4] ? row[4] : "NULL") << endl;
                cout << "D. " << (row[5] ? row[5] : "NULL") << endl;
                cout << "\t\t\t\t\t\tseconds remaining: " << k + 1 << endl;
                this_thread::sleep_for(chrono::seconds(1));
            }
        }

        // Free the result set after each query
        mysql_free_result(res);
    }

    // Close the connection
    mysql_close(conn);
    return EXIT_SUCCESS;
}
