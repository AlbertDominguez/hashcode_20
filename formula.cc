#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <pair>




// Score del temps en fer sign up
int formula_temps(const Library&  library, int D_left) {
	int sign_up = library.sign_up;
	int sign_D_left = sign_up/D_left;

	return sign_D_left;
}

// Score dels llibres que podras llegir
int formula_potencial(const Library& library, int D_left, int sum_total = 0, int l_total = 0){
	int capacity = library.capacity;
	priority_queue <pair<int,int>> cua;
	cua = library.books_here;
	int count= 0;
	int total_sum = 0;
	while (not cua.empty() or count < capacity*(D_left - library.sign_up)){
		pair<int,int> element = cua.top();
		if (not books[element.second].read){
			int appear = books[element.second].in_libs.size();
			total += element.first*1/appear;
			count += 1;
		}
		cua.pop();
		
	}
	return total;
}

// Formula total
int formula(const Library& library, int D_left, int sum_total = 0, int l_total = 0) {
	int potencial = formula_potencial(library, D_left, 0, 0);
	int temps = formula_temps(library, D_left);

	int final = temps + potencial;

	return final;

}

// llibres que processaràs
vector<int> llibres_processats(priority_queue <pair<int,int>>& cua, int D_left, int capacity, int sign_up){
	int count = 0;
	vector<int> selected;
	while (not cua.empty() or count < capacity*(D_left - sign_up)){
		pair<int,int> element = cua.top();
		selected.push_back(element.second);
		cua.pop();
	}
	return selected;
}

// Mira quina llibreria has de retornar i els llibres que retornes de la llibreria
pair <int,vector<int>> sign_up (int D_left){
	pair<int,int> maxim; //primer score, dsp id
	maxim.first = -1;
	maxim.second = -1;
	int score=0;
	for(int i = 0; i<libraries.size(); i++){
		if (not libraries[i].visited){
			score = formula(libraries[i], B, D_left, 0, 0);
		}
		if (score > maxim.first){
			maxim.first = score;
			maxim.second = i;
		}
	}
	libraries[maxim.second].visited = true;
	lib_ret = libraries[maxim.second];
	books_process = llibres_processats(lib_ret.books_here, D_left, lib_ret.capacity, lib_ret.sign_up);
	pair <int,vector<int>> ret;
	ret.first = maxim.second;
	ret.second = books_process;
	return ret;
}