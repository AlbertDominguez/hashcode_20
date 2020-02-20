#include <algorithm>
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
    bool visited = false;
    priority_queue<pair<int,int>> books_here; // Store score, bookid
};

int B, L, D; // number of books, number of libraries, number of days
vector<Book> books; // Store books
vector<Library> libraries; // Store libraries
//vector<vector<int>> processed;
vector<pair<int,vector<int>>> processed;

// bool sorter(Library const& l1, Library const& l2) {
//     return l1.score > l2.score;
// }

// void greedy() {
//     sort(libraries.begin(),libraries.end(),&sorter);
//     int current_day = 0;
//     int last_idx = -1;

//     while (current_day < D) {
//         last_idx++;
//         Library poss = libraries[last_idx];
//         while (poss.signup > D-current_day) {
//             ++last_idx;
//             poss = libraries[last_idx];

//             if (last_idx == libraries.size()) {
//                 current_day = D;
//                 break;
//             } else {
//                 // processed.push_back({last_idx,D-current_day-poss.signup});
//                 current_day += poss.signup;
//                 processed_libs_ids.push_back(last_idx);
//                 // int temp_capacity = 0;
//                 for (int i = 0; i < D-current_day; ++i) {
//                     priority_queue<pair<int,int>> q = poss.books_here;
//                     vector<int> to_push;
//                     processed.push_back(to_push);
//                     for (int j = 0; j < poss.capacity; ++j) {
//                         if (not q.empty()) {
//                             int curr_id = q.top().second;
//                             q.pop();
//                             processed[processed.size()-1].push_back(curr_id);
//                         } else {
//                             break;
//                         }
//                     }
//                 }
//             }
//         }
//     }
//     cout << processed_libs_ids.size() << endl;
//     for (int i = 0; i < processed_libs_ids.size(); ++i) {
//         cout << processed_libs_ids[i] << ' ' << processed[i].size() << endl;
//         bool first = true;
//         for (int j = 0; j < processed[i].size(); ++j) {
//             if (first) {
//                 cout << processed[i][j];
//             } else {
//                 cout << ' ' << processed[i][j];
//             }
//         }
//         cout << endl;
//     }
// }


// Score del temps en fer sign up
double formula_temps(const Library&  library, int D_left) {
	int signup = library.signup;
	double sign_D_left = double(signup)/double(D_left);

	return sign_D_left;
}

// Score dels llibres que podras llegir
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
			total += element.first/double(appear);
			count += 1;
		}
		cua.pop();
		
	}
	return total;
}

// Formula total
double formula(const Library& library, int D_left, int sum_total = 0, int l_total = 0) { 
	double potencial = formula_potencial(library, D_left, 0, 0);
    // cout << "potencial done" << endl;
    if (potencial == 0) {
        return -1;
    }
	double temps = formula_temps(library, D_left);
    // cout << "temps done" << endl;

	double fin = temps + potencial;

	return fin;

}

// llibres que processaràs
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

// Mira quina llibreria has de retornar i els llibres que retornes de la llibreria
pair <int,vector<int>> to_sign_up(int D_left){
    // cout << "in signup" << endl;
	pair<int,int> maxim; //primer score, dsp id
	maxim.first = -1;
	maxim.second = -1;
	double score = 0;
	for(int i = 0; i<libraries.size(); i++){
		if (not libraries[i].visited){
            // cout << "jeje" << endl;
			score = formula(libraries[i], D_left, 0, 0);
            // cout << "jojo" << endl;
            if (score > maxim.first){
                maxim.first = score;
                maxim.second = i;
		    }
		}
		
	}
    if (maxim.second != -1 and maxim.first != -1) {
        libraries[maxim.second].visited = true;
        Library lib_ret = libraries[maxim.second];
        vector<int> books_process = llibres_processats(lib_ret.books_here, D_left, lib_ret.capacity, lib_ret.signup);
        // cout << "hola" << endl;
        pair <int,vector<int>> ret;
        ret.first = maxim.second;
        ret.second = books_process;
        // cout << "out" << endl;
        return ret;
    } else {
        vector<int> junk;
        return {-1,junk};
    }
}

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

void simulation() {
    // cout << "in simulation()" << endl;
    int days_left = D;
    while (processed.size() < L and days_left > 0) {
        // cout << "in loop" << endl;
        // cout << days_left << endl;
        pair<int,vector<int>> lib_to_be_add = to_sign_up(days_left);
        if (lib_to_be_add.first == -1) {
            days_left = 0;
        } else {
            processed.push_back(lib_to_be_add);
            days_left -= libraries[lib_to_be_add.first].signup;
        }
    }
    output();
}

int main() {
    cin >> B >> L >> D;
    books = vector<Book>(B);
    libraries = vector<Library>(L);

    // READ
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
            libraries[j].score += books[book_to_add_id].score;
            libraries[j].books_here.push(book_to_add);
            books[book_to_add_id].in_libs.insert(j);
        }
    }



    // ------------------------------
    simulation();
    

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