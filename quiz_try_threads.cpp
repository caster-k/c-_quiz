#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <ctime>

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
    srand(time(0));
    int qno[5] = {0};
    int c = 0;
    while (c < 5) {
        int r = (rand() % 5) + 1;
        bool flag = false;
        for (int j = 0; j < c; j++) {
            if (qno[j] == r) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            qno[c] = r;
            c++;
        }
    }

    // Display the random question numbers for reference
    for (int i = 0; i < 5; i++) {
        cout << qno[i] << " ";
    }
    cout << endl;

    // Start input thread
    thread inputThread(inputHandler);

    // Main loop to handle each question
    for (int i = 0; i < 5; i++) {
        string query = "SELECT * FROM questions WHERE Qno = " + to_string(qno[i]);
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

        // Fetch the row (only expecting one row per question)
        row = mysql_fetch_row(res);

        int timer = 20;
        while (timer > 0 && option == ' ') {
            system("clear");
            cout << "Question " << qno[i] << ": " << (row[1] ? row[1] : "NULL") << endl; // Assuming column 1 contains the question
            cout << "A. " << (row[2] ? row[2] : "NULL") << endl; // Assuming column 2 contains Option A
            cout << "B. " << (row[3] ? row[3] : "NULL") << endl; // Assuming column 3 contains Option B
            cout << "C. " << (row[4] ? row[4] : "NULL") << endl; // Assuming column 4 contains Option C
            cout << "D. " << (row[5] ? row[5] : "NULL") << endl; // Assuming column 5 contains Option D
            cout << "\t\t\t\t\t\tSeconds remaining: " << timer << endl;

            this_thread::sleep_for(chrono::seconds(1));
            timer--;

            // If the user has given input, break the loop
            if (option != ' ') {
                cout << "You chose: " << option << endl;
                this_thread::sleep_for(chrono::seconds(5));
                break;
            }
        }

        // If time runs out or user selects an option, proceed to next question
        if (timer == 0) {
            cout << "Time's up for this question!" << endl;
        }

        // Reset option for the next question
        option = ' ';

        // Free the result set for the current question
        mysql_free_result(res);
    }

    // Cleanup MySQL connection
    mysql_close(conn);

    // Join input thread before exiting
    inputThread.detach();

    return EXIT_SUCCESS;
}
