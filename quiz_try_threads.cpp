#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>
#include<time.h>
#include<cstdlib>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std;
atomic<char> option(' ');  // Shared atomic variable for input
atomic<bool> time_extended(false); // To keep track of whether time extension was requested

void inputHandler() {
    while (true) {
        char input[2];
        cin >> input[0];
        option = input[0];
    }
}

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
        cout << "mysql_real_connect() failed\n";
        return EXIT_FAILURE;
    }
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
    
    for (int i = 0; i < 5; i++){
        string query = "SELECT * FROM questions WHERE Qno = " + to_string(qno[i]);
        cout << "Executing query: " << query << endl;
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

    cout << endl;

    int k=19;
    // Print each row of the result set
    thread inputThread(inputHandler);
    (row = mysql_fetch_row(res));
   while (k>=0) {
    
        system("clear");
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
        
        this_thread::sleep_for(chrono::seconds(1));
        if (k == 0 || option != ' ') {
            j++;  // Move to the next question
            cout << "You chose: " << option << endl;
            break;  // Break the loop (you can handle it differently)
        }
    }
    inputThread.detach();
    }
    // Cleanup
    mysql_free_result(res);
    mysql_close(conn);

    return EXIT_SUCCESS;
}
