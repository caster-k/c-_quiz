#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <cstring> 

// g++ Final_Quiz_Without_MultiThread.cpp -o quiz -lmysqlclient && ./quiz
using namespace std;

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL_RES *res_a;
    MYSQL_ROW row_a;
    const char *server = "localhost"; // Change to your server address
    const char *user = "root"; // Change to your MySQL username
    const char *password = "Sunta@123..."; // Change to your MySQL password
    const char *database = "QUIZ"; // Change to your database name

    // Initialize MySQL connection
    conn = mysql_init(NULL);
    
    // Connect to the database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cout << "mysql_real_connect() failed\n";
        return EXIT_FAILURE;
    }

    // Execute SQL query
    srand(time(0));
    int qno[5] = {0};
    int c = 0;
    while (c < 5) {
        int r = (rand() % 5) + 1;
        int flag = 0;
        for (int j = 0; j < 5; j++) {
            if (qno[j] == r) {
                flag = 1;
                break;  
            }
        }
        if (flag == 0) {
            qno[c] = r;
            c++;
        }
    }

    for (int i = 0; i < 5; i++) {
        cout << qno[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < 5; i++) {
        // Fetch question
        string query = "SELECT * FROM questions WHERE Qno = " + to_string(qno[i]);
        cout << "Executing query: " << query << endl;
        
        if (mysql_query(conn, query.c_str())) {
            cout << "Error executing query: " << mysql_error(conn) << endl;
            return EXIT_FAILURE;
        }

        // Store result for questions
        res = mysql_store_result(conn);
        if (res == NULL) {
            cout << "mysql_store_result() failed. Error: " << mysql_error(conn) << endl;
            return EXIT_FAILURE;
        }

        // Fetch question row
        row = mysql_fetch_row(res);
        if (row == NULL) {
            cout << "No question found for Qno: " << qno[i] << endl;
            mysql_free_result(res);
            continue; // Skip to next question if none found
        }

        // Fetch answers
        string query_a = "SELECT * FROM answers WHERE Qno = " + to_string(qno[i]);
        cout << "Executing query: " << query_a << endl;
        if (mysql_query(conn, query_a.c_str())) {
            cout << "Error executing query: " << mysql_error(conn) << endl;
            mysql_free_result(res);
            return EXIT_FAILURE;
        }

        // Store result for answers
        res_a = mysql_store_result(conn);
        if (res_a == NULL) {
            cout << "mysql_store_result() failed. Error: " << mysql_error(conn) << endl;
            mysql_free_result(res);
            return EXIT_FAILURE;
        }

        // Fetch answer row
        row_a = mysql_fetch_row(res_a);
        if (row_a == NULL) {
            cout << "No answers found for Qno: " << qno[i] << endl;
            mysql_free_result(res);
            mysql_free_result(res_a);
            continue; // Skip to next question if none found
        }

        // Countdown and input handling
        int k = 59;
        while (k >= 0) {
             // Display question and options
            system("clear");
            cout << "Question: " << qno[i] << " " << (row[1] ? row[1] : "NULL") << endl;
            cout << "A. " << (row[2] ? row[2] : "NULL") << endl; 
            cout << "B. " << (row[3] ? row[3] : "NULL") << endl; 
            cout << "C. " << (row[4] ? row[4] : "NULL") << endl; 
            cout << "D. " << (row[5] ? row[5] : "NULL") << endl;
            cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t Seconds remaining: "<<k << endl;
            this_thread::sleep_for(chrono::seconds(1));

            // Handle user input every 20 seconds
            if (k % 20 == 0) {
                char option;
                cout << "If you want more time press 'N': ";
                cout << "Enter your choice (A/B/C/D/N): ";
                cin >> option;
                if(option >='a' && option <='z'){
                    option -= 32;
                }
                if (option == 'N') {
                    continue;
                } 
                else if (strcmp(row_a[1], string(1, option).c_str()) == 0) {
                    cout <<endl<< "Your answer is correct!" << endl;
                    break;
                }
                else{
                    cout <<endl<< "Your answer is incorrect!" << endl;
                    break;
                }
            }
            k--;
        }
        cout << "NEXT QUESTION" << endl;
        this_thread::sleep_for(chrono::seconds(3));

        // Cleanup for each question and answers
        mysql_free_result(res);
        mysql_free_result(res_a);
    }

    // Final cleanup
    mysql_close(conn);
    return EXIT_SUCCESS;
}
