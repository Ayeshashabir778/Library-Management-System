#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

// ─── Date helper ───────────────────────────────────────────────────────────
int todayDate() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return (now->tm_year + 1900) * 10000 + (now->tm_mon + 1) * 100 + now->tm_mday;
}

int daysDiff(int from, int to) {
    auto toDays = [](int d) -> long long {
        int y = d / 10000, m = (d / 100) % 100, day = d % 100;
        return (long long)(y * 365.25) + (long long)(m * 30.44) + day;
    };
    return (int)(toDays(to) - toDays(from));
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(1000, '\n');
    cin.get();
}

void printLine(char c = '-', int n = 60) { cout << "  " << string(n, c) << endl; }
void printHeader(const string& title) {
    printLine('=');
    cout << "     " << title << endl;
    printLine('=');
}

// ───────────────────────────────────────────────────────────────────────────
//  BASE CLASS: Person
// ───────────────────────────────────────────────────────────────────────────
class Person {
protected:
    int    id;
    string name;
    string contact;
public:
    Person() : id(0) {}
    Person(int id, string name, string contact) : id(id), name(name), contact(contact) {}
    int    getId()      const { return id; }
    string getName()    const { return name; }
    string getContact() const { return contact; }
    virtual ~Person() {}
};

// ───────────────────────────────────────────────────────────────────────────
//  DERIVED CLASS: Member
// ───────────────────────────────────────────────────────────────────────────
class Member : public Person {
private:
    int booksIssued;
public:
    Member() : Person(), booksIssued(0) {}
    Member(int id, string name, string contact, int issued = 0)
        : Person(id, name, contact), booksIssued(issued) {}

    int  getBooksIssued() const { return booksIssued; }
    void incrementIssued()      { booksIssued++; }
    void decrementIssued()      { if (booksIssued > 0) booksIssued--; }

    void display() const {
        cout << "  " << left << setw(5) << id << setw(22) << name
             << setw(18) << contact << "Books Held: " << booksIssued << endl;
    }

    string toFileLine() const {
        return to_string(id)+","+name+","+contact+","+to_string(booksIssued);
    }

    static Member fromFileLine(const string& line) {
        stringstream ss(line); string t; vector<string> p;
        while (getline(ss, t, ',')) p.push_back(t);
        if (p.size() < 4) return Member();
        return Member(stoi(p[0]), p[1], p[2], stoi(p[3]));
    }
};

// ───────────────────────────────────────────────────────────────────────────
//  CLASS: Book
// ───────────────────────────────────────────────────────────────────────────
class Book {
private:
    int    bookId;
    string title;
    string author;
    string category;
    int    totalCopies;
    int    availableCopies;
public:
    Book() : bookId(0), totalCopies(0), availableCopies(0) {}
    Book(int id, string title, string author, string cat, int total, int avail)
        : bookId(id), title(title), author(author), category(cat),
          totalCopies(total), availableCopies(avail) {}

    int    getBookId()          const { return bookId; }
    string getTitle()           const { return title; }
    string getAuthor()          const { return author; }
    string getCategory()        const { return category; }
    int    getTotalCopies()     const { return totalCopies; }
    int    getAvailableCopies() const { return availableCopies; }
    bool   isAvailable()        const { return availableCopies > 0; }

    void issueOne()  { if (availableCopies > 0) availableCopies--; }
    void returnOne() { if (availableCopies < totalCopies) availableCopies++; }

    void display() const {
        cout << "  " << left << setw(5) << bookId << setw(28) << title
             << setw(20) << author << setw(14) << category
             << availableCopies << "/" << totalCopies << endl;
    }

    string toFileLine() const {
        return to_string(bookId)+","+title+","+author+","+category+","+
               to_string(totalCopies)+","+to_string(availableCopies);
    }

    static Book fromFileLine(const string& line) {
        stringstream ss(line); string t; vector<string> p;
        while (getline(ss, t, ',')) p.push_back(t);
        if (p.size() < 6) return Book();
        return Book(stoi(p[0]), p[1], p[2], p[3], stoi(p[4]), stoi(p[5]));
    }
};

// ───────────────────────────────────────────────────────────────────────────
//  CLASS: IssuedRecord
// ───────────────────────────────────────────────────────────────────────────
class IssuedRecord {
private:
    int recordId, memberId, bookId, issueDate, returnDate;
public:
    IssuedRecord() : recordId(0), memberId(0), bookId(0), issueDate(0), returnDate(0) {}
    IssuedRecord(int rid, int mid, int bid, int idate, int rdate = 0)
        : recordId(rid), memberId(mid), bookId(bid), issueDate(idate), returnDate(rdate) {}

    int  getRecordId()   const { return recordId; }
    int  getMemberId()   const { return memberId; }
    int  getBookId()     const { return bookId; }
    int  getIssueDate()  const { return issueDate; }
    int  getReturnDate() const { return returnDate; }
    bool isReturned()    const { return returnDate != 0; }

    void markReturned(int date) { returnDate = date; }

    int calculateFine() const {
        int checkDate = isReturned() ? returnDate : todayDate();
        int overdue = daysDiff(issueDate, checkDate) - 14;
        return (overdue > 0) ? overdue * 5 : 0;
    }

    void display() const {
        cout << "  " << left << setw(7) << recordId << setw(10) << memberId
             << setw(8) << bookId << setw(12) << issueDate
             << setw(12) << (returnDate == 0 ? "Pending" : to_string(returnDate))
             << "Rs " << calculateFine() << endl;
    }

    string toFileLine() const {
        return to_string(recordId)+","+to_string(memberId)+","+to_string(bookId)+
               ","+to_string(issueDate)+","+to_string(returnDate);
    }

    static IssuedRecord fromFileLine(const string& line) {
        stringstream ss(line); string t; vector<string> p;
        while (getline(ss, t, ',')) p.push_back(t);
        if (p.size() < 5) return IssuedRecord();
        return IssuedRecord(stoi(p[0]), stoi(p[1]), stoi(p[2]), stoi(p[3]), stoi(p[4]));
    }
};

// ───────────────────────────────────────────────────────────────────────────
//  CLASS: Library
// ───────────────────────────────────────────────────────────────────────────
class Library {
private:
    vector<Book>         books;
    vector<Member>       members;
    vector<IssuedRecord> records;

    const string BOOKS_FILE   = "books.txt";
    const string MEMBERS_FILE = "members.txt";
    const string ISSUED_FILE  = "issued.txt";
    const string ADMIN_USER   = "admin";
    const string ADMIN_PASS   = "lib123";

    int nextBookId = 1, nextMemberId = 1, nextRecordId = 1;

    // ── File helpers ──────────────────────────────────────────────────────
    void loadAll() {
        // Books
        books.clear();
        ifstream fb(BOOKS_FILE); string line;
        while (getline(fb, line))
            if (!line.empty()) {
                Book b = Book::fromFileLine(line); books.push_back(b);
                if (b.getBookId() >= nextBookId) nextBookId = b.getBookId() + 1;
            }
        // Members
        members.clear();
        ifstream fm(MEMBERS_FILE);
        while (getline(fm, line))
            if (!line.empty()) {
                Member m = Member::fromFileLine(line); members.push_back(m);
                if (m.getId() >= nextMemberId) nextMemberId = m.getId() + 1;
            }
        // Records
        records.clear();
        ifstream fr(ISSUED_FILE);
        while (getline(fr, line))
            if (!line.empty()) {
                IssuedRecord r = IssuedRecord::fromFileLine(line); records.push_back(r);
                if (r.getRecordId() >= nextRecordId) nextRecordId = r.getRecordId() + 1;
            }
    }

    void saveBooks()   { ofstream f(BOOKS_FILE);   for (auto& b : books)   f << b.toFileLine() << "\n"; }
    void saveMembers() { ofstream f(MEMBERS_FILE);  for (auto& m : members) f << m.toFileLine() << "\n"; }
    void saveRecords() { ofstream f(ISSUED_FILE);   for (auto& r : records) f << r.toFileLine() << "\n"; }

    Book*   findBook(int id)   { for (auto& b : books)   if (b.getBookId() == id) return &b; return nullptr; }
    Member* findMember(int id) { for (auto& m : members) if (m.getId()     == id) return &m; return nullptr; }

    // ── Safe input helpers ────────────────────────────────────────────────
    string inputString(const string& prompt) {
        string val;
        cout << prompt;
        getline(cin, val);
        // trim
        while (!val.empty() && (val.front()==' '||val.front()=='\t')) val.erase(val.begin());
        while (!val.empty() && (val.back() ==' '||val.back() =='\t')) val.pop_back();
        return val;
    }

    int inputInt(const string& prompt) {
        string val; int result = -1;
        while (true) {
            cout << prompt;
            getline(cin, val);
            try { result = stoi(val); break; }
            catch (...) { cout << "  [!] Please enter a valid number.\n"; }
        }
        return result;
    }

public:
    Library() { loadAll(); }

    // ═════════════════════════════════════════════════════════════════════
    //  LOGIN
    // ═════════════════════════════════════════════════════════════════════
    bool adminLogin() {
        printHeader("ADMIN LOGIN");
        string user = inputString("  Username : ");
        string pass = inputString("  Password : ");
        return (user == ADMIN_USER && pass == ADMIN_PASS);
    }

    // ═════════════════════════════════════════════════════════════════════
    //  BOOK MANAGEMENT
    // ═════════════════════════════════════════════════════════════════════
    void addBook() {
        printHeader("ADD NEW BOOK");
        string title    = inputString("  Title    : ");
        string author   = inputString("  Author   : ");
        string category = inputString("  Category : ");
        int copies      = inputInt   ("  Copies   : ");
        if (copies <= 0) { cout << "\n  [!] Copies must be at least 1.\n"; pause(); return; }
        books.emplace_back(nextBookId, title, author, category, copies, copies);
        saveBooks();
        cout << "\n  [OK] Book added! Book ID = " << nextBookId++ << endl;
        pause();
    }

    void removeBook() {
        printHeader("REMOVE BOOK");
        int id = inputInt("  Enter Book ID to remove: ");
        for (auto& r : records)
            if (r.getBookId() == id && !r.isReturned()) {
                cout << "\n  [!] Cannot remove — this book has copies still issued out.\n";
                pause(); return;
            }
        auto it = remove_if(books.begin(), books.end(), [id](const Book& b){ return b.getBookId()==id; });
        if (it == books.end()) { cout << "\n  [!] Book ID not found.\n"; pause(); return; }
        books.erase(it, books.end());
        saveBooks();
        cout << "\n  [OK] Book removed.\n";
        pause();
    }

    void searchBook() {
        printHeader("SEARCH BOOK");
        cout << "  1. Search by ID\n  2. Search by Title\n";
        int ch = inputInt("  Choice: ");
        bool found = false;
        cout << "\n  " << left << setw(5)<<"ID" << setw(28)<<"Title" << setw(20)<<"Author" << setw(14)<<"Category" << "Copies\n";
        printLine();
        if (ch == 1) {
            int id = inputInt("  Book ID: ");
            Book* b = findBook(id);
            if (b) { b->display(); found = true; }
        } else {
            string kw = inputString("  Keyword: ");
            transform(kw.begin(), kw.end(), kw.begin(), ::tolower);
            for (auto& b : books) {
                string t = b.getTitle();
                transform(t.begin(), t.end(), t.begin(), ::tolower);
                if (t.find(kw) != string::npos) { b.display(); found = true; }
            }
        }
        if (!found) cout << "\n  [!] No books found.\n";
        pause();
    }

    void viewAllBooks() {
        printHeader("ALL BOOKS");
        if (books.empty()) { cout << "  No books in library.\n"; pause(); return; }
        cout << "  " << left << setw(5)<<"ID" << setw(28)<<"Title" << setw(20)<<"Author" << setw(14)<<"Category" << "Copies\n";
        printLine();
        for (auto& b : books) b.display();
        pause();
    }

    // ═════════════════════════════════════════════════════════════════════
    //  MEMBER MANAGEMENT
    // ═════════════════════════════════════════════════════════════════════
    void addMember() {
        printHeader("ADD MEMBER");
        string name    = inputString("  Name    : ");
        string contact = inputString("  Contact : ");
        members.emplace_back(nextMemberId, name, contact, 0);
        saveMembers();
        cout << "\n  [OK] Member added! Member ID = " << nextMemberId++ << endl;
        pause();
    }

    void removeMember() {
        printHeader("REMOVE MEMBER");
        int id = inputInt("  Member ID: ");
        for (auto& r : records)
            if (r.getMemberId() == id && !r.isReturned()) {
                cout << "\n  [!] Member has unreturned books. Return them first.\n";
                pause(); return;
            }
        auto it = remove_if(members.begin(), members.end(), [id](const Member& m){ return m.getId()==id; });
        if (it == members.end()) { cout << "\n  [!] Member ID not found.\n"; pause(); return; }
        members.erase(it, members.end());
        saveMembers();
        cout << "\n  [OK] Member removed.\n";
        pause();
    }

    void viewAllMembers() {
        printHeader("ALL MEMBERS");
        if (members.empty()) { cout << "  No members registered.\n"; pause(); return; }
        cout << "  " << left << setw(5)<<"ID" << setw(22)<<"Name" << setw(18)<<"Contact" << "Books Held\n";
        printLine();
        for (auto& m : members) m.display();
        pause();
    }

    // ═════════════════════════════════════════════════════════════════════
    //  ISSUE & RETURN
    // ═════════════════════════════════════════════════════════════════════
    void issueBook() {
        printHeader("ISSUE BOOK");
        int mid = inputInt("  Member ID : ");
        int bid = inputInt("  Book ID   : ");

        Member* m = findMember(mid);
        if (!m) { cout << "\n  [!] Member not found.\n"; pause(); return; }

        Book* b = findBook(bid);
        if (!b) { cout << "\n  [!] Book not found.\n"; pause(); return; }

        if (!b->isAvailable()) { cout << "\n  [!] No copies available right now.\n"; pause(); return; }

        if (m->getBooksIssued() >= 3) {
            cout << "\n  [!] Member already holds 3 books (maximum limit).\n"; pause(); return;
        }

        for (auto& r : records)
            if (r.getMemberId()==mid && r.getBookId()==bid && !r.isReturned()) {
                cout << "\n  [!] Member already has this book.\n"; pause(); return;
            }

        b->issueOne();
        m->incrementIssued();
        records.emplace_back(nextRecordId++, mid, bid, todayDate(), 0);
        saveBooks(); saveMembers(); saveRecords();

        cout << "\n  [OK] Book issued to " << m->getName() << "!\n";
        cout << "  Due date : 14 days from today.\n";
        cout << "  Fine     : Rs 5 per day after due date.\n";
        pause();
    }

    void returnBook() {
        printHeader("RETURN BOOK");
        int mid = inputInt("  Member ID : ");
        int bid = inputInt("  Book ID   : ");

        Member* m = findMember(mid);
        if (!m) { cout << "\n  [!] Member not found.\n"; pause(); return; }

        Book* b = findBook(bid);
        if (!b) { cout << "\n  [!] Book not found.\n"; pause(); return; }

        for (auto& r : records) {
            if (r.getMemberId()==mid && r.getBookId()==bid && !r.isReturned()) {
                int fine = r.calculateFine();
                r.markReturned(todayDate());
                b->returnOne();
                m->decrementIssued();
                saveBooks(); saveMembers(); saveRecords();
                cout << "\n  [OK] Book returned by " << m->getName() << ".\n";
                if (fine > 0) cout << "  Fine due : Rs " << fine << " — please pay at counter.\n";
                else          cout << "  No fine. Returned on time!\n";
                pause(); return;
            }
        }
        cout << "\n  [!] No active issue record found for this member and book.\n";
        pause();
    }

    // ═════════════════════════════════════════════════════════════════════
    //  REPORTS
    // ═════════════════════════════════════════════════════════════════════
    void viewIssuedBooks() {
        printHeader("CURRENTLY ISSUED BOOKS");
        bool any = false;
        cout << "  " << left << setw(7)<<"RecID" << setw(10)<<"MembID" << setw(8)<<"BookID"
             << setw(12)<<"Issued" << setw(12)<<"Due/Return" << "Fine\n";
        printLine();
        for (auto& r : records)
            if (!r.isReturned()) { r.display(); any = true; }
        if (!any) cout << "  No books currently issued.\n";
        pause();
    }

    void viewMemberHistory() {
        printHeader("MEMBER BORROW HISTORY");
        int mid = inputInt("  Member ID: ");
        Member* m = findMember(mid);
        if (!m) { cout << "\n  [!] Member not found.\n"; pause(); return; }
        cout << "\n  Member : " << m->getName() << " | Contact : " << m->getContact() << "\n";
        printLine();
        cout << "  " << left << setw(7)<<"RecID" << setw(10)<<"MembID" << setw(8)<<"BookID"
             << setw(12)<<"Issued" << setw(12)<<"Due/Return" << "Fine\n";
        printLine();
        bool any = false;
        for (auto& r : records)
            if (r.getMemberId()==mid) { r.display(); any = true; }
        if (!any) cout << "  No history found.\n";
        pause();
    }

    // ═════════════════════════════════════════════════════════════════════
    //  MENUS
    // ═════════════════════════════════════════════════════════════════════
    void bookMenu() {
        int ch;
        do {
            clearScreen();
            printHeader("BOOK MANAGEMENT");
            cout << "  1. Add Book\n";
            cout << "  2. Remove Book\n";
            cout << "  3. Search Book\n";
            cout << "  4. View All Books\n";
            cout << "  0. Back to Main Menu\n";
            ch = inputInt("\n  Choice: ");
            clearScreen();
            switch(ch) {
                case 1: addBook(); break;
                case 2: removeBook(); break;
                case 3: searchBook(); break;
                case 4: viewAllBooks(); break;
                case 0: break;
                default: cout << "\n  [!] Invalid choice.\n"; pause();
            }
        } while (ch != 0);
    }

    void memberMenu() {
        int ch;
        do {
            clearScreen();
            printHeader("MEMBER MANAGEMENT");
            cout << "  1. Add Member\n";
            cout << "  2. Remove Member\n";
            cout << "  3. View All Members\n";
            cout << "  0. Back to Main Menu\n";
            ch = inputInt("\n  Choice: ");
            clearScreen();
            switch(ch) {
                case 1: addMember(); break;
                case 2: removeMember(); break;
                case 3: viewAllMembers(); break;
                case 0: break;
                default: cout << "\n  [!] Invalid choice.\n"; pause();
            }
        } while (ch != 0);
    }

    void mainMenu() {
        clearScreen();
        printLine('=');
        cout << "\n       LIBRARY MANAGEMENT SYSTEM\n";
        cout << "          Built with C++ & OOP\n\n";
        printLine('=');

        if (!adminLogin()) {
            cout << "\n  [!] Wrong credentials. Exiting.\n"; pause(); return;
        }
        cout << "\n  [OK] Welcome, Admin!\n"; pause();

        int ch;
        do {
            clearScreen();
            printLine('=');
            cout << "          LIBRARY MANAGEMENT SYSTEM\n";
            printLine('=');
            cout << "\n  1. Book Management\n";
            cout << "  2. Member Management\n";
            cout << "  3. Issue a Book\n";
            cout << "  4. Return a Book\n";
            cout << "  5. View All Issued Books\n";
            cout << "  6. Member Borrow History\n";
            cout << "  0. Exit\n";
            ch = inputInt("\n  Choice: ");
            clearScreen();
            switch(ch) {
                case 1: bookMenu(); break;
                case 2: memberMenu(); break;
                case 3: issueBook(); break;
                case 4: returnBook(); break;
                case 5: viewIssuedBooks(); break;
                case 6: viewMemberHistory(); break;
                case 0: cout << "\n  Goodbye!\n\n"; break;
                default: cout << "\n  [!] Invalid choice.\n"; pause();
            }
        } while (ch != 0);
    }
};

// ───────────────────────────────────────────────────────────────────────────
int main() {
    Library lib;
    lib.mainMenu();
    return 0;
}