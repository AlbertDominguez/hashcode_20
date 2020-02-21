#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>

using namespace std;

struct Book {
    int score;
    set<int> in_libs;
    bool read = false;
};

struct Library {
    int id;
    int capacity;
    int signup;
    int score = 0;
    bool visited = false;
    priority_queue<pair<int,int>> books_here; // Store score and bookid
};

int B, L, D; // number of books, number of libraries, number of days
vector<Book> books; // Store books
vector<Library> libraries; // Store libraries
vector<pair<int,vector<int>>> processed; // Store processed libraries


// Read data in the given format
void read() {
    for (int i = 0; i < B; ++i) {
        cin >> books[i].score;
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
            libraries[j].score += books[book_to_add_id].score;
            libraries[j].books_here.push(book_to_add);
            books[book_to_add_id].in_libs.insert(j);
        }
    }
}

// Score taking into account signup time and left days
double formula_temps(const Library&  library, int D_left) {
	int signup = library.signup;
	double sign_D_left = double(signup)/double(D_left);
	return sign_D_left;
}

// Score of readable books
double formula_potencial(const Library& library, int D_left, int sum_total = 0, int l_total = 0){
	int capacity = library.capacity;
	priority_queue <pair<int,int>> cua;
	cua = library.books_here;
	int count= 0;
	double total = 0;
	while (not cua.empty() and count < capacity*(D_left - library.signup)){
		pair<int,int> element = cua.top();
		if (not books[element.second].read){
			int appear = books[element.second].in_libs.size();
			total += element.first/(double(appear));
            count += 1;
		}
		cua.pop();
		
	}
	return total;
}

// Total priority formula
double formula(const Library& library, int D_left, int sum_total = 0, int l_total = 0) { 
	double potencial = formula_potencial(library, D_left, 0, 0);
    if (potencial == 0) {
        return -1;
    }
	double temps = formula_temps(library, D_left);

	double fin = pow(1-temps,3)*potencial;
    assert(fin >= 0); // Security
	return fin;

}

// Books to process
vector<int> llibres_processats(priority_queue <pair<int,int>>& cua, int D_left, int capacity, int signup){
	int count = 0;
	vector<int> selected;
	while (not cua.empty() and count < capacity*(D_left - signup)){
		pair<int,int> element = cua.top();
        if (not books[element.second].read) {
            selected.push_back(element.second);
            books[element.second].read = true;
            ++count;
        }
        cua.pop();
	}
	return selected;
}

// Look which library to return and which books in that library will be processed
pair <int,vector<int>> to_sign_up(int D_left){
	pair<int,int> maxim; // First score, then id
	maxim.first = -1;
	maxim.second = -1;
	double score = 0;
	for(int i = 0; i<libraries.size(); i++){
		if (not libraries[i].visited){
			score = formula(libraries[i], D_left, 0, 0);
            if (score > maxim.first){
                if (rand()%100 > 2){
                    maxim.first = score;
                    maxim.second = i;
                }
		    }
		}		
	}
    if (maxim.second != -1 and maxim.first != -1) {
        libraries[maxim.second].visited = true;
        Library lib_ret = libraries[maxim.second];
        vector<int> books_process = llibres_processats(lib_ret.books_here, D_left, lib_ret.capacity, lib_ret.signup);
        pair <int,vector<int>> ret;
        ret.first = maxim.second;
        ret.second = books_process;
        return ret;
    } else {
        vector<int> junk;
        return {-1,junk};
    }
}


// Make days pass
void simulation() {
    int days_left = D;
    while (processed.size() < L and days_left > 0) {
        pair<int,vector<int>> lib_to_be_add = to_sign_up(days_left);
        if (lib_to_be_add.first == -1) {
            days_left = 0;
        } else {
            processed.push_back(lib_to_be_add);
            days_left -= libraries[lib_to_be_add.first].signup;
        }
    }
}


// Write processed libraries in desired format
void output() {
    cout << processed.size() << endl;
    for (int i = 0; i < processed.size(); ++i) {
        cout << processed[i].first << ' ' << processed[i].second.size() << endl;
        for (int j = 0; j < processed[i].second.size(); ++j) {
            if (j == 0) {
                cout << processed[i].second[j];
            } else {
                cout << ' ' << processed[i].second[j];
            }
        }
        cout << endl;
    }
}


int main() {
    cin >> B >> L >> D;
    books = vector<Book>(B);
    libraries = vector<Library>(L);
    read();
    simulation();
    output();
    return 0;
}