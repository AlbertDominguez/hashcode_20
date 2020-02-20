#include <iostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>

using namespace std;

struct Book {
    // int id;
    int score;
    set<int> in_libs;
    bool read = false;
};

struct Library {
    int id;
    int capacity;
    int signup;
    int score = 0;
    priority_queue<pair<int,int>> books_here; // Store score, bookid
};

int main() {
    int B, L, D; // number of books, number of libraries, number of days
    cin >> B >> L >> D;
    vector<Book> books(B); // Store books
    vector<Library> libraries(L);
    for (int i = 0; i < B; ++i) {
        cin >> books[i].score;
        //books[i].id = i;
    }
    for (int j = 0; j < L; ++j) {
        int num_books, signup, capacity;
        cin >> num_books >> signup >> capacity;
        libraries[j].id = j;
        libraries[j].capacity = capacity;
        libraries[j].signup = signup;
        for (int k = 0; k < num_books; ++k) {
            int book_to_add_id;
            cin >> book_to_add_id;
            pair<int,int> book_to_add = {books[book_to_add_id].score,book_to_add_id};
            libraries[j].books_here.push(book_to_add);
            books[book_to_add_id].in_libs.insert(j);
        }
    }

    // for (int i = 0; i < B; ++i) {
    //     cout << "Book " << i << endl;
    //     cout << books[i].score << endl;
    //     for (auto read : books[i].in_libs) {
    //         cout << read << ' ';
    //     }
    //     cout << endl;
    // }
    // for (int i = 0; i < L; ++i) {
    //     cout << i << endl;
    //     while (not libraries[i].books_here.empty()) {
    //         cout << libraries[i].books_here.top().second;
    //         cout << ' ' << libraries[i].books_here.top().first << endl;
    //         libraries[i].books_here.pop();
    //     }
    // }
}