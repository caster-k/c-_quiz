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

int main() {
    MYSQL *conn;
    MYSQL *conn_a;
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL_RES *res_a;
    MYSQL_ROW row_a;
    const char *server = "localhost"; // Change to your server address
    const char *user = "root"; // Change to your MySQL username
    const char *password = ".."; // Change to your MySQL password
    const char *database = "QUIZ"; // Change to your database name

    // Initialize MySQL connection
    conn = mysql_init(NULL);
    
    // Connect to the database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cout << "mysql_real_connect() failed\n";
        return EXIT_FAILURE;
    }
    conn_a = conn;
    // Execute SQL query
    srand(time(0));
    int qno[5]={0};
    int c = 0;
     while (c < 5){
        int r = (rand() % 5)+1;
        int flag = 0;
        for (int j = 0; j < 5; j++){
            if (qno[j] == r){
                flag = 1;
                break;  
            }
        }
        if(flag == 0){
            qno[c] = r;
            c++;
        }
    }

    for (int i = 0; i < 5; i++){
        cout<<qno[i]<<" ";
    }
    cout<<endl;

    int j = 0;
    
    for (int i = 0; i < 5; i++) {
    string query = "SELECT * FROM questions WHERE Qno = " + to_string(qno[i]);
    cout << "Executing query: " << query << endl;
    
    if (mysql_query(conn, query.c_str())) {
        cout << "Error executing query: " << mysql_error(conn) << endl;
        return EXIT_FAILURE;
    }

    string query_a = "SELECT * FROM answers WHERE Qno = " + to_string(qno[i]);
    cout << "Executing query: " << query_a << endl;
    if (mysql_query(conn, query_a.c_str())) {
        cout << "Error executing query: " << mysql_error(conn) << endl;
        return EXIT_FAILURE;
    }

    // Store the result set for questions and answers
    res = mysql_store_result(conn);
    res_a = mysql_store_result(conn);

    if (res == NULL || res_a == NULL) {
        cout << "mysql_store_result() failed. Error: " << mysql_error(conn) << endl;
        return EXIT_FAILURE;
    }

    // Fetch the rows for questions and answers
    row = mysql_fetch_row(res);
    row_a = mysql_fetch_row(res_a);
    // cout<<endl<<"Yo chalenaaaaaaaaaa"<<endl;

    int k=19;
    // Print each row of the result set
    (row = mysql_fetch_row(res));
    (row_a = mysql_fetch_row(res_a));
   while (k>=0) {
    
        // system("clear");
        cout << "Question:"<< qno[j]<<" "<< (row[1] ? row[1] : "NULL") << endl; // Assuming column 1 contains the question
        // cout << "Options:" << endl;
        cout << "A. " << (row[2] ? row[2] : "NULL") << endl; // Assuming column 2 contains Option A
        cout << "B. " << (row[3] ? row[3] : "NULL") << endl; // Assuming column 3 contains Option B
        cout << "C. " << (row[4] ? row[4] : "NULL") << endl; // Assuming column 4 contains Option C
        cout << "D. " << (row[5] ? row[5] : "NULL") << endl; // Assuming column 5 contains Option D
        cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t\t\tseconds remaining: "<<k-1 << endl;
        k--;
        if(k == 0){
            j++;
            // cout<<j<<endl;
        }
        if((k) % 20 == 0){
            // cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t\t\tseconds remaining: "<<k+1 << endl;
            char option;
            cout << "If you want to more time press 'N': ";
            cout<<"Enter your choice (A/B/C/D/N)"<<endl;
            cin>>option;
            if(option == 'N' || option == 'n'){
                continue;
            }
            else if (strcmp(row_a[1], string(1, option).c_str()) == 0) {
                cout << "Your answer is correct" << endl;
            }
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout<<"Times UP!\nNEXT QUESTION";
    this_thread::sleep_for(chrono::seconds(3));
    }
    // Cleanup
    mysql_free_result(res);
    mysql_free_result(res_a);
    mysql_close(conn);
    mysql_close(conn_a);

    return EXIT_SUCCESS;
}
