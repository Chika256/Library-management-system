#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
// Include necessary headers for input/output, data structures, file operations, string manipulation, and date/time functions.

using namespace std;

// Maps usernames to passcodes.
unordered_map<string, string> users;

// Specifies the name of the file to store library records.
string filename = "library_records.csv";

// Initialize library records as a vector of vectors of strings.
// Each inner vector represents a book record with fields: Book Name, Is Borrowed, User Name, Date Borrowed, Return Date.
vector<vector<string> > records = {
    {"The hobbit", "Yes", "John Doe",  "2023-04-01", "2023-04-15"},
    {"1984", "No", "",  "", ""},
    {"Pride and prejudice", "No", "",  "", ""},
    {"The book thief", "No", "",  "", ""},
    {"Midnight library", "No", "",  "", ""},
    {"To kill a mockingbird", "No", "",  "", ""},
    {"The Hunger Games", "No", "", "", ""},
    {"Twilight", "No", "",  "", ""},
    {"Animal farm", "No", "",  "", ""}
};


// Function to write library records to a CSV file.

void LibraryRecordsCSV(const string& filename, const vector<vector<string> >& records) {
    ofstream file(filename);

    // Writes column headers to the file.
    file << "Book Name,Is Borrowed,User Name,User Code,Date Borrowed,Return Date\n";

    // Iterates over each record and writes it to the file, separating fields with commas.
    for (const auto& record : records) {
        for (size_t i = 0; i < record.size(); ++i) {
            file << record[i];
            if (i < record.size() - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
}

// Function to save user data to a file.
void saveUsersToFile(const string& filename) {
    ofstream file(filename); // Opens the file for writing.
    if (!file.is_open()) {
        cerr << "Failed to open file for writing user data." << endl;
        return;
    }
    // Iterates over the users map and writes each username and passcode pair to the file.
    for (const auto& user : users) {
        file << user.first << "," << user.second << "\n";
    }
    file.close(); // Closes the file.
}

// Function to load user data from a file.
void loadUsersFromFile(const string& filename) {
    ifstream file(filename); // Opens the file for reading.
    if (!file.is_open()) {
        cerr << "Failed to open file for reading user data." << endl;
        return;
    }
    string line;
    // Reads each line from the file, extracting username and passcode, and stores them in the users map.
    while (getline(file, line)) {
        stringstream ss(line);
        string username, passcode;
        if (getline(ss, username, ',') && getline(ss, passcode)) {
            users[username] = passcode;
        }
    }
    file.close();
}


// Function to load library records from a CSV file.
void loadLibraryRecordsCSV(const string& filename) {
    ifstream file(filename); // Opens the file for reading.
    if (!file.is_open()) {
        cerr << "Failed to open file for reading library records." << endl;
        return;
    }
    string line, data;
    getline(file, line); // Skips the header line.
    records.clear(); // Clears existing records.
    // Reads each line from the file, splitting it into fields by commas, and adds each record to the records vector.
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> record;
        while (getline(ss, data, ',')) {
            record.push_back(data);
        }
        records.push_back(record);
    }
    file.close(); // Closes the file.
}

// Function to handle user login.
bool loginUser() {
    auto tries = 3 ;
    string username, passcode;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter passcode: ";
    cin >> passcode;

    // Checks if the entered username and passcode match an existing user.
    if (users.find(username) != users.end() && users[username] == passcode) {
        cout << "Login successful!" << endl;
        return true;
    } else {
        cout << "Invalid username or passcode." << endl;
        tries = tries - 1 ;
        if (tries = 0);
        cout << "you have ran out of tries try the system will now restart" << endl ;

        return false;
    }
}

// Function to register a new user.
bool registerUser() {
    string username, passcode;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter 5-digit passcode: ";
    cin >> passcode;

    // Validates the length of the passcode.
    if (passcode.length() != 5) {
        cout << "Passcode must be 5 digits." << endl;
        return false;
    }

    // Checks if the username already exists.
    if (users.find(username) != users.end()) {
        cout << "User already exists." << endl;
        return false;
    }

    // Adds the new user to the users map and saves the updated map to the file.
    users[username] = passcode;
    cout << "Registration successful!" << endl;
    saveUsersToFile("users.csv");
    return true;
}


// Function to allow a user to borrow a book.
void borrowBook(const string& username) {
    // Check if the user exists.
    if (users.find(username) == users.end()) {
        cout << "User does not exist. Please register first." << endl;
        return;
    }

    string bookName;
    cout << "Enter the name of the book you wish to borrow: ";
    cin.ignore(); // Ignore newline left in the input stream.
    getline(cin, bookName); // Read the book name.

    bool bookFound = false;
    for (auto& record : records) {
        if (record[0] == bookName) { // Check if the book name matches.
            bookFound = true;
            if (record[1] == "No") { // Check if the book is available.
                // Update the record to indicate the book is borrowed.
                record[1] = "Yes";
                record[2] = username;
                record[3] = users[username]; // Use the user's passcode as the user code.

                // Get the current date as the borrow date.
                auto now = chrono::system_clock::now();
                time_t borrow_time = chrono::system_clock::to_time_t(now);
                tm *ltm = localtime(&borrow_time);

                // Format the borrow date as YYYY-MM-DD.
                stringstream borrowDate;
                borrowDate << 1900 + ltm->tm_year << "-"
                           << setfill('0') << setw(2) << 1 + ltm->tm_mon << "-"
                           << setfill('0') << setw(2) << ltm->tm_mday;

                // Calculate the return date (borrow date + 7 days).
                auto return_time = now + chrono::hours(24 * 7); // Adding 7 days.
                time_t return_time_t = chrono::system_clock::to_time_t(return_time);
                tm *return_ltm = localtime(&return_time_t);

                // Format the return date as YYYY-MM-DD.
                stringstream returnDate;
                returnDate << 1900 + return_ltm->tm_year << "-"
                           << setfill('0') << setw(2) << 1 + return_ltm->tm_mon << "-"
                           << setfill('0') << setw(2) << return_ltm->tm_mday;

                // Update the record with borrow and return dates.
                record[4] = borrowDate.str();
                record[5] = returnDate.str();

                cout << "You have successfully borrowed the book: " << bookName << endl;
                cout << "Borrow Date: " << borrowDate.str() << ", Return Date: " << returnDate.str() << endl;
                LibraryRecordsCSV(filename, records); // Update the CSV file with the new book status.
                return;
            } else {
                cout << "This book is currently borrowed by someone else." << endl;
                return;
            }
        }
    }

    if (!bookFound) {
        cout << "Book not available for borrowing or does not exist." << endl;
    }
}

// Defines a function to read data from a CSV file and return a boolean value.
// Takes a filename as a string argument.
bool readFromCSV(const string& filename) {
    // Opens the file using ifstream.
    ifstream file(filename);
    // Declares variables for line reading and data storage.
    string line;
    vector<size_t> maxWidths; // Tracks maximum column widths.
    vector<vector<string> > tableData; // Holds the table data.

    // First pass: Reads the file line by line to determine column widths and store data.
    while (getline(file, line)) {
        stringstream ss(line); // Splits the line into comma-separated values.
        string data;
        vector<string> rowData; // Stores row data.
        size_t columnIndex = 0; // Tracks the current column index.

        // Extracts data separated by commas.
        while (getline(ss, data, ',')) {
            rowData.push_back(data); // Adds data to the row.
            // Updates maxWidths for each column based on the data width.
            if (maxWidths.size() <= columnIndex) {
                maxWidths.push_back(data.length());
            } else {
                if (data.length() > maxWidths[columnIndex]) {
                    maxWidths[columnIndex] = data.length();
                }
            }
            ++columnIndex; // Moves to the next column.
        }
        tableData.push_back(rowData); // Adds the row to the table data.
    }
    file.close(); // Closes the file.

    // Second pass: Prints the data with columns aligned according to maxWidths.
    for (size_t row = 0; row < tableData.size(); ++row) {
        for (size_t i = 0; i < tableData[row].size(); ++i) {
            // Prints each column data left-aligned with padding for readability.
            cout << left << setw(maxWidths[i] + 3) << tableData[row][i];
        }
        cout << "\n"; // Moves to the next line after printing a row.
    }

    return true; // Indicates success.
};

// Function to allow a user to return a borrowed book.
void returnBook(const string& username) {
    // Checks if the user exists.
    if (users.find(username) == users.end()) {
        cout << "User does not exist. Please register first." << endl;
        return;
    }

    string bookName;
    cout << "Enter the name of the book you wish to return: ";
    cin.ignore(); // Ignores newline left in the input stream.
    getline(cin, bookName); // Reads the book name.

    for (auto& record : records) {
        if (record[0] == bookName && record[2] == username) { // Checks if the book is borrowed by the user.
            // Updates the record to indicate the book is returned.
            record[1] = "No";
            record[2] = "";
            record[3] = "";
            record[4] = "";
            record[5] = "";
            cout << "You have successfully returned the book: " << bookName << endl;
            LibraryRecordsCSV(filename, records); // Updates the CSV file with the new book status.
            return;
        }
    }

    cout << "This book was not borrowed by you or does not exist." << endl;
}

// Function to display library records.
void displayLibraryRecords(const string& filename) {
    ifstream file(filename); // Open the file for reading.
    if (!file.is_open()) {
        cerr << "Failed to open file for reading library records." << endl;
        return;
    }

    string line;
    vector<size_t> maxWidths; // Store the maximum width of each column for formatting.
    vector<vector<string> > tableData; // Store the table data.

    // Determine the maximum width of each column.
    while (getline(file, line)) {
        stringstream ss(line);
        string data;
        vector<string> rowData;
        size_t columnIndex = 0;

        while (getline(ss, data, ',')) { // Split the line by commas.
            rowData.push_back(data); // Add each piece of data to the row.
            if (maxWidths.size() <= columnIndex) {
                maxWidths.push_back(data.length()); // Initialize column width.
            } else {
                if (data.length() > maxWidths[columnIndex]) {
                    maxWidths[columnIndex] = data.length(); // Update column width if necessary.
                }
            }
            ++columnIndex;
        }
        tableData.push_back(rowData); // Add the row to the table data.
    }
    file.close(); // Close the file.

    // Print the data with alignment.
    for (const auto& row : tableData) {
        for (size_t i = 0; i < row.size(); ++i) {
            cout << left << setw(maxWidths[i] + 3) << row[i]; // Print each field with padding for alignment.
        }
        cout << "\n";
    }
}


// Main function to drive the library system.
int main() {

    loadUsersFromFile("users.csv");// Load user data at program start.
    loadLibraryRecordsCSV(filename); // Load library records at program start

    bool accessGranted = false;
    int choice;

    // Main loop for the library system menu.
    while (true) {
        cout << "====================================\n";
        cout << "Library System Main Menu\n";
        cout << "====================================\n";
        cout << "\n1. Login\n";
        cout << "2. Register\n";
        cout << "3. View Books (Login Required)\n";
        cout << "4. Borrow Books (Login Required)\n";
        cout << "5. Return Books (Login Required)\n";
        cout << "6. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                accessGranted = loginUser();
                break;
            case 2:
                accessGranted = registerUser();
                break;
            case 3:
                if (accessGranted) {
                    cout << "Displaying available books:\n";
                    displayLibraryRecords(filename);
                } else {
                    cout << "Please login first." << endl;
                }
                break;
            case 4:
                if (accessGranted) {
                    string username;
                    cout << "Enter your username: ";
                    cin >> username;
                    borrowBook(username);
                } else {
                    cout << "Please login first." << endl;
                }
                break;
            case 5:
                if (accessGranted) {
                    string username;
                    cout << "Enter your username: ";
                    cin >> username;
                    returnBook(username);
                } else {
                    cout << "Please login first." << endl;
                }
                break;
            case 6:
                cout << "Exiting program\n";
                return 0;
            default:
                cout << "\nInvalid option." << endl;

            if (accessGranted) {

            LibraryRecordsCSV(filename, records);
            readFromCSV(filename);

                cout << "====================================\n";
                cout << "Access granted. Welcome!" << endl;
                cout << "====================================\n";



                }else{

                cout << "Access denied, try again." << endl;
                main();


                }
        }
    }

    return 0;
}
