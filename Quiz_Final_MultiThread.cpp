#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <atomic>

// g++ Quiz_Final_MultiThread.cpp -o quiz -lmysqlclient && ./quiz
using namespace std;

// Shared atomic variable for user input
atomic<char> option(' ');

void inputHandler() {
    while (true) {
        char input;
        // this_thread::sleep_for(chrono::seconds(3));
        cin >> input;
        if (input >= 'A' && input <= 'Z') {
            input += 32; // Convert to lowercase
        }
        option = input;
        
    }
}

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL_RES *res_a;
    MYSQL_ROW row_a;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "Sunta@123...";
    const char *database = "QUIZ";

    // Initialize MySQL connection
    conn = mysql_init(NULL);
    
    // Connect to the database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cout << "mysql_real_connect() failed\n";
        return EXIT_FAILURE;
    }

    // Start input thread
    thread inputThread(inputHandler);

    for (int i = 0; i <= 45; i++) {
        // Fetch question
        string query = "SELECT * FROM questions WHERE Qno = " + to_string(i);
        if (mysql_query(conn, query.c_str())) {
            cout << "Error executing query: " << mysql_error(conn) << endl;
            return EXIT_FAILURE;
        }

        res = mysql_store_result(conn);
        if (res == NULL) {
            cout << "mysql_store_result() failed. Error: " << mysql_error(conn) << endl;
            continue; // Move to next question
        }

        row = mysql_fetch_row(res);
        if (row == NULL) {
            cout << "No question found for Qno: " << i << endl;
            mysql_free_result(res);
            continue; // Skip to next question if none found
        }

        // Fetch answers
        string query_a = "SELECT * FROM answers WHERE Qno = " + to_string(i);
        if (mysql_query(conn, query_a.c_str())) {
            cout << "Error executing query: " << mysql_error(conn) << endl;
            mysql_free_result(res);
            return EXIT_FAILURE;
        }

        res_a = mysql_store_result(conn);
        if (res_a == NULL) {
            cout << "mysql_store_result() failed. Error: " << mysql_error(conn) << endl;
            mysql_free_result(res);
            continue; // Move to next question
        }

        row_a = mysql_fetch_row(res_a);
        if (row_a == NULL) {
            cout << "No answers found for Qno: " << i << endl;
            mysql_free_result(res);
            mysql_free_result(res_a);
            continue; // Skip to next question if none found
        }

        // Countdown and input handling
        for (int k = 75; k >= 0; k--) {
            system("clear");
            cout << "Question " << i << ":  " << (row[1] ? row[1] : "NULL") << endl;
            cout << "a. " << (row[2] ? row[2] : "NULL") << endl; 
            cout << "b. " << (row[3] ? row[3] : "NULL") << endl; 
            cout << "c. " << (row[4] ? row[4] : "NULL") << endl; 
            cout << "d. " << (row[5] ? row[5] : "NULL") << endl;
            cout << "Enter your choice (a/b/c/d): ";
            cout << "\t\t\t seconds remaining: " << k << endl;

            this_thread::sleep_for(chrono::seconds(1));

            if (option != ' ') {
                cout << "You chose: " << option << endl;
                if (strcmp(row_a[1], string(1, option).c_str()) == 0) {
                    cout << "Your answer is correct!" << endl;
                    this_thread::sleep_for(chrono::seconds(5));
                } else {
                    cout << "Your answer is incorrect!" << endl;
                    this_thread::sleep_for(chrono::seconds(5));
                }
                option = ' '; // Reset option for next question
                break;
            }

            if (k == 0) {
                for (int kk = 35; kk >= 0; kk--) {
            system("clear");
            cout << "Question: " << i << " " << (row[1] ? row[1] : "NULL") << endl;
            cout << "a. " << (row[2] ? row[2] : "NULL") << endl; 
            cout << "b. " << (row[3] ? row[3] : "NULL") << endl; 
            cout << "c. " << (row[4] ? row[4] : "NULL") << endl; 
            cout << "d. " << (row[5] ? row[5] : "NULL") << endl;
            cout << "Enter your choice (a/b/c/d): ";
            cout << "\t\t\t seconds remaining: " << kk << endl;
            cout << "Time's up for this question for the selected group!" << endl;

            this_thread::sleep_for(chrono::seconds(1));

            if (option != ' ') {
                cout << "You chose: " << option << endl;
                if (strcmp(row_a[1], string(1, option).c_str()) == 0) {
                    cout << "Your answer is correct!" << endl;
                    this_thread::sleep_for(chrono::seconds(5));
                } 
                else {
                    cout << "Your answer is incorrect!" << endl;
                    this_thread::sleep_for(chrono::seconds(5));
                }
                option = ' '; // Reset option for next question
                break;
            }
                
            if(kk == 0){
                cout<<"Time's for the second round!"<<endl;
               
                this_thread::sleep_for(chrono::seconds(660));//rest for 11 minutes for explanation and choose the correct answer
            }
                
            }  

        }}

        

        mysql_free_result(res);
        mysql_free_result(res_a);
    }
    system("clear");
    cout << "Quiz completed!" << endl;

    mysql_close(conn);
    inputThread.join(); // Wait for the input thread to finish

    return EXIT_SUCCESS;
}
