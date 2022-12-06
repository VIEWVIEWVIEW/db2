#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <exception>
#include <algorithm>

using namespace std;

const int MAX_BLOCK_SIZE = 5;

struct Book
{
    int key; // ... (nun als numerisch anzunehmenden) Schlüssels.
    char author[21] = {0};
    char title[21] = {0};
    char publisher[21] = {0};
    int release_year;
    char place_of_publication[41] = {0};
    char isbn[15] = {0};
};

struct Block
{
    int hashKey;
    Book data[5];
    int len = 0;
    Block *ueberlauf = nullptr;
};

struct Result
{
    int dataIndex;
    int blockIndex;
    Block *block;
};

void print_book(Book b)
{
    cout << "Key: " << b.key << endl;
    cout << "Author: " << b.author << endl;
    cout << "Title: " << b.title << endl;
    cout << "Publisher: " << b.publisher << endl;
    cout << "Release Year: " << b.release_year << endl;
    cout << "Place of Publication: " << b.place_of_publication << endl;
    cout << "ISBN: " << b.isbn << endl;
}

void print_books(Block blockList[], int blockListSize)
{
    for (int i = 0; i < blockListSize; i++)
    {
        Block *block = &blockList[i];
        cout << "==============================" << endl;
        cout << "Block " << i << " Hash" << block->hashKey << endl;

        while (block != nullptr)
        {
            for (int j = 0; j < block->len; j++)
            {
                print_book(block->data[j]);
            }
            block = block->ueberlauf;
        }
    }
}

/**
 *
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
*/

/**
 *
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
*/

int digit_sum(int n)
{
    int sum = 0;
    while (n > 0)
    {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

bool compare_book(Book a, Book b)
{
    return a.key < b.key;
}

bool compare_block(Block a, Block b)
{
    return a.hashKey < b.hashKey;
}

int binary_search(Block *blocklist, int size, int hashKey)
{
    int left = 0;
    int right = size - 1;
    int middle = 0;

    while (left <= right)
    {
        middle = (left + right) / 2;
        if (blocklist[middle].hashKey == hashKey)
        {
            return middle;
        }
        else if (blocklist[middle].hashKey < hashKey)
        {
            left = middle + 1;
        }
        else
        {
            right = middle - 1;
        }
    }
    return -1;
}

void write_db_to_file(Block blockList[], int blockListSize)
{
    // get file handle and write to file
    fstream file;
    file.open("nussplidb", ios::out);

    for (int i = 0; i < blockListSize; i++)
    {
        Block *block = &blockList[i];

        Book book;
        while (block != nullptr)
        {
            for (int j = 0; j < block->len; j++)
            {
                book = block->data[j];
                file.write((char *)&book.author, sizeof(book.author));
                file.write((char *)&book.title, sizeof(book.title));
                file.write((char *)&book.publisher, sizeof(book.publisher));
                file.write((char *)&book.release_year, sizeof(book.release_year));
                file.write((char *)&book.place_of_publication, sizeof(book.place_of_publication));
                file.write((char *)&book.isbn, sizeof(book.isbn));
            }
            block = block->ueberlauf;
        }
        /**
         *
        file.write(book.author, sizeof(book.author));
        file.write(book.title, sizeof(book.title));
        file.write(book.publisher, sizeof(book.publisher));

        file.write((char *)&book.release_year, sizeof(book.release_year));
        file.write(book.place_of_publication, sizeof(book.place_of_publication));
        file.write(book.isbn, sizeof(book.isbn));
        */
    }
    file.close();
};

int binary_search_key(Block *block, int l, int r, int key)
{
    if (l <= r)
    {
        int mid = l + (r - l) / 2;

        // Element in der Mitte
        if (block->data[mid].key == key)
            return mid;

        // Linke Seite
        if (block->data[mid].key > key)
            return binary_search_key(block, 0, mid - 1, key);

        // Rechte Seite
        return binary_search_key(block, mid + 1, r, key);
    }

    // Kein Element gefunden
    return -1;
}

Result search_key(Block blockList[], int blockListSize, int key)
{
    int blockIndex = -1;
    int dataIndex = -1;
    Block *block = nullptr;
    int hashKey = digit_sum(key);

    // HashKey finden
    blockIndex = binary_search(blockList, blockListSize, hashKey);
    if (blockIndex != -1)
    {
        // Block durchsuchen
        Block *block = &blockList[blockIndex];

        // Den Block und alle Ueberlaeufe durchsuchen
        while (block != nullptr)
        {
            dataIndex = binary_search_key(block, 0, block->len - 1, key);

            if (dataIndex != -1)
                return Result{
                    dataIndex,
                    blockIndex,
                    block};

            block = block->ueberlauf;
        }
    }

    return Result{-1, blockIndex, block};
}

void add_book(Block *blockList, int &blockListSize, Book new_book)
{
    /** Check if key is already taken */
    if (search_key(blockList, blockListSize, new_book.key).dataIndex != -1)
    {
        cout << "Key not unique. Try again." << endl;
        return;
    }

    if (new_book.key < 1 || new_book.key > 99999)
    {
        cout << "Key range: 1 .. 99999" << endl;
        return;
    }

    int hashKey = digit_sum(new_book.key);
    int indexBlockList = -1;

    indexBlockList = binary_search(blockList, blockListSize, hashKey);

    if (indexBlockList == -1)
    {
        // create new block
        Block newBlock;
        newBlock.hashKey = hashKey;
        newBlock.data[0] = new_book;
        newBlock.len = 1;
        blockList[blockListSize] = newBlock;
        blockListSize++;
    }
    else
    {
        // add to existing block
        Block *block = &blockList[indexBlockList];
        if (block->len < 5)
        {
            block->data[block->len] = new_book;
            block->len++;
        }
        else
        {
            // add to overflow block
            if (block->ueberlauf == nullptr)
            {
                Block *newBlock = new Block;
                newBlock->data[0] = new_book;
                newBlock->len = 1;
                block->ueberlauf = newBlock;
            }
            else
            {
                Block *ueberlauf = block->ueberlauf;
                while (ueberlauf->ueberlauf != nullptr)
                {
                    ueberlauf = ueberlauf->ueberlauf;
                }
                ueberlauf->data[ueberlauf->len] = new_book;
                ueberlauf->len++;
            }
        }
    }

    // sort block
    std::sort(blockList[indexBlockList].data, blockList[indexBlockList].data + blockList[indexBlockList].len, compare_book);
}

void add_sample_books(Block blockList[], int &blockListSize)
{
    Book book = {
        1,
        "Tolkien",
        "The Lord of the Meow",
        "Allen & Unwin",
        1954,
        "London",
        "1337-1337-0"};
    add_book(blockList, blockListSize, book);

    book = {
        10,
        "Tolkien",
        "The Hobbit",
        "Allen & Unwin",
        1937,
        "London",
        "1337-1337-1"};
    add_book(blockList, blockListSize, book);

    book = {
        333,
        "Tolkien",
        "The Silmarillion",
        "Allen & Unwin",
        1977,
        "London",
        "1337-1337-2"};
    add_book(blockList, blockListSize, book);

    book = {
        9,
        "Tolkien",
        "The Children of WHO",
        "Allen & Unwin",
        2007,
        "London",
        "1337-1337-3"};
    add_book(blockList, blockListSize, book);
}

void add_book_by_hand(Block blockList[], int &blockListSize)
{
    Book book;
    cin.ignore();
    cout << "Enter key: ";
    cin >> book.key;

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

    add_book(blockList, blockListSize, book);
}

void load_db_from_file(Block *blockList, int &blockListSize)
{

    // load entries from file
    ifstream file("nussplidb", ios::binary);
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

            add_book(blockList, blockListSize, book);
            // cout << "Loaded book: " << book.title << endl;
        }
    }
    file.close();
};

// Aufgabe 2
/**
 *
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
*/

/**
 *
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
*/

void print_menu()
{
    cout << "Welcome to the book database!" << endl;
    cout << "Please select an option:" << endl;
    cout << "1) Load db from file" << endl;
    cout << "2) Print all" << endl;
    cout << "3) Add book" << endl;
    cout << "4) Add sample books" << endl;
    cout << "5) Search by key" << endl;

    cout << "9) Save and exit" << endl;
}

int main()
{

    Block blockList[20];
    int blockListSize = 0;

    int option;

    print_menu();
    Result result;
    while (true)
    {
        cout << "$ ";
        cin >> option;
        switch (option)
        {
        case 1:
            load_db_from_file(blockList, blockListSize);
            cout << "Database loaded!" << endl;
            break;

        case 2:
            print_books(blockList, blockListSize);
            break;

        case 3:
            add_book_by_hand(blockList, blockListSize);
            break;

        case 4:
            add_sample_books(blockList, blockListSize);
            break;

        case 5:
            int key;
            cout << "Enter key: " << endl;
            cin >> key;
            result = search_key(blockList, blockListSize, key);

            if (result.dataIndex != -1)
            {
                cout << "Found book: " << result.block->data[result.dataIndex].title << endl;
            }
            else
            {
                cout << "Book not found!" << endl;
            }

            break;
        case 9:
            cout << "Dumping db to disk..." << endl;
            write_db_to_file(blockList, blockListSize);
            cout << "Exiting..." << endl;
            return 0;
        case 10:
            print_menu();
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
            break;
        }
    }
    return 0;
}