#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

atomic<char> option(' ');  // Shared atomic variable for input
atomic<bool> time_extended(false); // To keep track of whether time extension was requested

void inputHandler() {
    while (true) {
        char input;
        cin >> input;

        if (input == 'N' || input == 'n') {
            time_extended = true;
        } else {
            option = input;  // Update the option selected by the user
        }
    }
}

int main() {
    int k = 19;
    int j = 0;
    // Example question/answer setup (replace with your MySQL result fetching logic)
    string row[6] = {"Question", "What is 2+2?", "4", "3", "2", "1"};
    string qno[] = {"Q1"};  // Just an example for the question number

    // Start the input handler thread
    thread inputThread(inputHandler);

    while (k >= 0) {
        system("clear");
        cout << "Question:" << qno[j] << " " << (row[1].empty() ? "NULL" : row[1]) << endl;
        cout << "A. " << (row[2].empty() ? "NULL" : row[2]) << endl;
        cout << "B. " << (row[3].empty() ? "NULL" : row[3]) << endl;
        cout << "C. " << (row[4].empty() ? "NULL" : row[4]) << endl;
        cout << "D. " << (row[5].empty() ? "NULL" : row[5]) << endl;
        cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t\t\tseconds remaining: " << k - 1 << endl;
        
        // Sleep for 1 second
        this_thread::sleep_for(chrono::seconds(1));

        // Decrement the timer
        k--;

        // Check if time extension is requested
        if (time_extended) {
            k += 20;  // Add 20 seconds
            time_extended = false;  // Reset the extension flag
        }

        // Check if the timer has expired or the user answered
        if (k == 0 || option != ' ') {
            j++;  // Move to the next question
            cout << "You chose: " << option << endl;
            break;  // Break the loop (you can handle it differently)
        }

        // Prompt for more time
        // if ((k) % 20 == 0) {
        //     cout << "If you want more time, press 'N'. Enter your choice (A/B/C/D/N): ";
        // }
    }

    // Join the input thread (optional, depending on your program flow)
    inputThread.detach();  // Detach the thread if you want it to run independently

    return 0;
}
