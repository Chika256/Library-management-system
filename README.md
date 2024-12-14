
# Library Management System

This is a simple Library Management System written in C++ that allows users to register, log in, view books, borrow and return books. It stores user data and library records in CSV files, making it easy to maintain the data.

## Features

- **User Registration**: Allows users to register with a username and a 5-digit passcode.
- **User Login**: Authenticated users can log in using their username and passcode.
- **View Available Books**: After login, users can view the list of books in the library.
- **Borrow Books**: Users can borrow available books. The system updates the borrow date and return date.
- **Return Books**: Users can return borrowed books. The system updates the book status and clears user-related data.
- **CSV File Operations**: User data and library records are stored in CSV files (`users.csv` and `library_records.csv`), making it easy to persist and load data.

## Requirements

- C++11 or later
- A C++ compiler (e.g., GCC, Clang)
- Basic understanding of file I/O and C++ standard library

## Files

- **library_records.csv**: Stores information about books (book name, borrow status, username, dates).
- **users.csv**: Stores registered user information (username, passcode).

## How to Use

### Compilation and Execution

1. Clone the repository or copy the code to a file named `LibrarySystem.cpp`.
2. Compile the code using a C++ compiler:
   ```bash
   g++ -o LibrarySystem LibrarySystem.cpp

   run the executable

   ./LibrarySystem

## Program Flow

Upon starting the program, the following menu will appear:


1. **Login**: Enter your username and passcode. After three unsuccessful attempts, the program will restart.
2. **Register**: Register a new username and passcode (5-digit passcode is required).
3. **View Books**: Once logged in, you can view available books.
4. **Borrow Books**: After logging in, enter the book's name to borrow it.
5. **Return Books**: After logging in, return any borrowed books by entering the book name.
6. **Exit**: Exit the system.

## Notes

- Make sure that the users.csv and library_records.csv files exist in the same directory as the executable.
- The program does not handle multiple users simultaneously. It is designed for single-user access at a time.


