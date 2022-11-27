#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <exception>
#include <algorithm>

using namespace std;

struct Index
{
    char id[21]; // Ordnungsbegriff
    int position;
};

struct Book
{
    char author[21] = {0};
    char title[21] = {0};
    char publisher[21] = {0};
    int release_year;
    char place_of_publication[41] = {0};
    char isbn[15] = {0};
};

void print_books(vector<Book> books)
{
    cout << "Elements in vector: " << books.size() << endl;
    for (auto book : books)
    {
        cout << "Author: " << book.author << endl;
        cout << "Title: " << book.title << endl;
        cout << "Publisher: " << book.publisher << endl;
        cout << "Release year: " << book.release_year << endl;
        cout << "Place of publication: " << book.place_of_publication << endl;
        cout << "ISBN: " << book.isbn << endl;
        cout << "----------------" << endl;
    }
}

void build_index(vector<Book> books, vector<Index> &index)
{
    for (int i = 0; i < books.size(); i++)
    {
        Index idx;
        strcpy(idx.id, books[i].title);
        idx.position = i;
        index.push_back(idx);
    }
}

void print_index(vector<Index> index)
{
    cout << "Elements in index: " << index.size() << endl;
    for (auto idx : index)
    {
        cout << "ID: " << idx.id << endl;
        cout << "Position: " << idx.position << endl;
        cout << "----------------" << endl;
    }
}

void load_db_from_file(vector<Book> &books)
{
    books.clear();

    // load entries from file
    ifstream file("boggadb", ios::binary);
    if (file.is_open())
    {
        while (!file.eof() && file.peek() != EOF)
        {
            Book book;
            // file.read((char *)&book, sizeof(Book));
            file.read((char *)&book.author, sizeof(book.author));
            file.read((char *)&book.title, sizeof(book.title));
            file.read((char *)&book.publisher, sizeof(book.publisher));

            // idk what g++ is doing here, maybe compiler bug?
            // file.read((char *)&book.release_year, sizeof(book.release_year)); <-- doesnt work
            // workaround:
            int release_year;
            file.read((char *)&release_year, sizeof(release_year));
            book.release_year = release_year;

            file.read((char *)&book.place_of_publication, sizeof(book.place_of_publication));
            file.read((char *)&book.isbn, sizeof(book.isbn));

            books.push_back(book);
            // cout << "Loaded book: " << book.title << endl;
        }
    }
    file.close();
    print_books(books);
};

void write_db_to_file(vector<Book> &books)
{
    // get file handle and write to file
    fstream file;
    file.open("boggadb", ios::out);
    for (Book book : books)
    {
        file.write(book.author, sizeof(book.author));
        file.write(book.title, sizeof(book.title));
        file.write(book.publisher, sizeof(book.publisher));

        file.write((char *)&book.release_year, sizeof(book.release_year));
        file.write(book.place_of_publication, sizeof(book.place_of_publication));
        file.write(book.isbn, sizeof(book.isbn));
    }
    file.close();
};

void add_sample_book(vector<Book> &books)
{
    Book book = {
        "Tolkien",
        "The Lord of the Meow",
        "Allen & Unwin",
        1954,
        "London",
        "1337-1337-0"};
    books.push_back(book);
}

void add_book(vector<Book> &books)
{
    Book book;
    cout << "Enter author: " << endl;
    cin.ignore();
    cin.getline(book.author, sizeof(book.author), '\n');

    cout << "Enter title: " << endl;
    cin.getline(book.title, sizeof(book.title), '\n');

    cout << "Enter publisher: " << endl;
    cin.getline(book.publisher, sizeof(book.publisher), '\n');

    cout << "Enter release year: " << endl;
    cin >> book.release_year;
    cin.ignore(); // https://stackoverflow.com/a/25077260

    cout << "Enter place of publication: " << endl;
    cin.getline(book.place_of_publication, sizeof(book.place_of_publication), '\n');

    cout << "Enter ISBN: " << endl;
    cin.getline(book.isbn, sizeof(book.isbn), '\n');

    books.push_back(book);
}

void print_menu()
{
    cout << "Welcome to the book database!" << endl;
    cout << "Please select an option:" << endl;
    cout << "1) Load db from file" << endl;
    cout << "2) Build index" << endl;
    cout << "3) Search by ordnungsbegriff" << endl;
    cout << "4) Print db to cout" << endl;
    cout << "5) Add book" << endl;
    cout << "6) Add sample book" << endl;
    cout << "7) Show index" << endl;
    cout << "8) Find book by id (ordnungsbegriff) via index" << endl;
    cout << "9) Save and exit" << endl;
    cout << "10) Print menu" << endl;
    cout << "11) Sort by Ordnungsbegriff" << endl;
}

// Aufgabe 2
Book get_book_by_id(char *id, vector<Index> index, vector<Book> books)
{
    // iterate through index and find position
    for (auto idx : index)
    {
        cout << "Comparing " << idx.id << " with " << id << endl;
        if (strcmp(idx.id, id) == 0)
        {
            Book result = books[idx.position];
            cout << "Found book: " << result.title << endl;

            cout << result.author << endl;
            cout << result.release_year << endl;
            cout << result.place_of_publication << endl;
            cout << result.isbn << endl;

            return result;
        }
    }
    throw out_of_range("Book not found");
}

// Aufgabe 3
void sort_by_ordnungsbegriff_descending(char *ordnungsbegriff, vector<Index> &index, vector<Book> &books)
{

    // cout << strcmp(a.id, b.id) << endl;
    // sort index by ordnungsbegriff

    sort(index.begin(), index.end(), [ordnungsbegriff](Index a, Index b) {
        return strcmp(a.id, ordnungsbegriff) > strcmp(b.id, ordnungsbegriff);
    });

    print_index(index);
}

/**
 * 
void sort_by_ordnungsbegriff(char *ordnungsbegriff, vector<Index> index, vector<Book> books)
{
    cout << "Sorting by " << ordnungsbegriff << endl;
    // sort index by ordnungsbegriff
    sort(index.begin(), index.end(), [ordnungsbegriff](Index a, Index b)
         { return strcmp(a.id, ordnungsbegriff) < strcmp(b.id, ordnungsbegriff); });

    cout << "==== Result =====" << endl;
    // print index
    for (auto idx : index)
    {
        cout << idx.id << endl;

        Book book = books[idx.position];
        cout << book.author << endl;
        cout << book.title << endl;
        cout << book.publisher << endl;
        cout << book.release_year << endl;
        cout << book.place_of_publication << endl;
        cout << book.isbn << endl;
        cout << "====================" << endl;
    }
}
*/

int main()
{
    // interactive menu where user selects between 1) build datanase, 2) build index, 3) search by ordnungsbegriff, 4) exit
    vector<Book> books;
    vector<Index> index;

    int option;

    print_menu();

    while (true)
    {
        cout << "$ ";
        cin >> option;
        switch (option)
        {
        case 1:
            load_db_from_file(books);
            cout << "Database loaded!" << endl;
            break;
        case 2:
            cout << "Building index..." << endl;
            build_index(books, index);
            break;
        case 3:
            cout << "Searching by ordnungsbegriff..." << endl;
            break;
        case 4:
            print_books(books);
            break;
        case 5:
            add_book(books);
            cout << "Book added!" << endl;
            break;
        case 6:
            add_sample_book(books);
            cout << "Sample book added!" << endl;
            break;
        case 7:
            print_index(index);
            break;
        case 8:
            cin.clear();
            cout << "Enter ordnungsbegriff: " << endl;
            char id[21];
            cin.ignore();
            cin.getline(id, sizeof(id), '\n');

            try
            {
                get_book_by_id(id, index, books);
            }
            catch (out_of_range e)
            {
                cout << e.what() << endl;
            }

            cin.clear();
            break;

        case 9:
            cout << "Dumping db to disk..." << endl;
            write_db_to_file(books);
            cout << "Exiting..." << endl;
            return 0;
        case 10:
            print_menu();
            break;
        case 11:
            cout << "Enter ordnungsbegriff: " << endl;
            char idd[21];
            cin.ignore();
            cin.getline(idd, sizeof(idd), '\n');
            sort_by_ordnungsbegriff_descending(idd, index, books);
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
            break;
        }
    }
    return 0;
}