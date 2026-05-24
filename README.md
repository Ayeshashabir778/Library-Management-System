# Library Management System

A console-based Library Management System built in C++ using Object-Oriented Programming, File Handling, and CRUD operations.

---

## About the Project

This system allows an admin to manage books, members, book issuing, and returns through a menu-driven console interface. All data is saved to text files so it persists between sessions. A fine of Rs 5 per day is charged automatically if a book is returned after 14 days.

---

## Features

- Admin login with username and password authentication
- Add, remove, and search books (by ID or title keyword)
- Add and remove library members
- Issue books to members (maximum 3 books per member)
- Return books with automatic fine calculation (Rs 5 per day after 14 days)
- View all currently issued books
- View full borrow history of any member
- File-based persistent storage — data saved across sessions

---

## Classes

| Class | Type | Description |
|---|---|---|
| Person | Base | Stores ID, name, and contact |
| Member | Derived from Person | Tracks books currently held by a member |
| Book | Standalone | Stores title, author, category, total and available copies |
| IssuedRecord | Standalone | Tracks issue date, return date, and fine calculation |
| Library | Controller | Manages all operations, menus, and file I/O |

---

## OOP Concepts Used

- Inheritance — Member extends Person
- Encapsulation — private data with public getters/setters
- Polymorphism — virtual display() overridden in Member
- File Handling — read/write using ifstream and ofstream

---

## Fine Policy

- Loan period: 14 days
- Fine: Rs 5 per extra day after 14 days
- Fine is calculated automatically on return

---

## File Storage

| File | Data Stored |
|---|---|
| books.txt | All book records |
| members.txt | All member records |
| issued.txt | All issue and return records |

---

## Admin Credentials

| Field | Value |
|---|---|
| Username | admin |
| Password | lib123 |

---

## How to Run

1. Open the project in Visual Studio or VS Code
2. Compile the file:
   ```
   g++ library.cpp -o library
   ```
3. Run the executable:
   ```
   ./library
   ```
4. Login with admin credentials and use the menu

---

## Menu Structure

```
Main Menu
├── 1. Book Management
│     ├── Add Book
│     ├── Remove Book
│     ├── Search Book
│     └── View All Books
├── 2. Member Management
│     ├── Add Member
│     ├── Remove Member
│     └── View All Members
├── 3. Issue a Book
├── 4. Return a Book
├── 5. View Currently Issued Books
└── 6. View Member Borrow History
```

---

## Tech Stack

- Language: C++
- Concepts: OOP, Inheritance, File Handling, CRUD
- Tools: VS Code, Visual Studio, GitHub

---

## Author

Ayesha Shabir — Computer Engineering Student, 2nd Semester
