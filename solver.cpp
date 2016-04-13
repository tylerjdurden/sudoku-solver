#include "solver.h"
#include <iostream>
#include <fstream>

void solver::read_in_values()
{
	string response;
	ifstream ifs;
	cout << "Do you want to read in from a file?" << endl;
	cin >> response;
	bool from_file = (response.at(0) == 'y' || response.at(0) == 'Y');
	if(from_file)
	{
		string filename;
		cout << "Enter the filename with extension: ";
		cin >> filename;
		ifs.open(filename);
		if(!ifs)
		{
			cerr << "Error opening file: " << filename;
			return;
		}
		read_in_values(ifs);
	}
	else
	{
		read_in_values(std::cin);
	}
}

void solver::read_in_values(istream& is)
{
	bool from_file = (&is != &(std::cin));
	if(!from_file)
		cout << "What should the size of each subgrid be? (e.g. for normal sudoku, this is 3): ";
	is >> size;
	for(int i = 0;i < size*size;++i)
	{
		if(!from_file)
			cout << "\nEnter the values for row " << (i+1) << ". Use -1 to represent blank cells. ";
		vector<int> current_row;
		for(int j = 0;j < size*size;++j)
		{
			int current_cell;
			is >> current_cell;
			current_row.push_back(current_cell);
		}
		grid.push_back(current_row);
	}
}

void solver::print_grid()
{
	cout << "====================GRID====================" << endl;
	for(auto row : grid)
	{
		for(int cell : row)
		{
			if(cell == -1)
				cout << " X "; 
			else
				cout << ' ' << cell << ' ';
		}
		cout << endl;
	}
	cout << "============================================" << endl;
}

void solver::print_guesses(const vector<int>& guesses)
{
	if(guesses.empty())
	{
		cout << "guesses: {}" << endl;
		return;
	}
	cout << "guesses: {";
	for(int i = 0;i < guesses.size()-1;++i)
		cout << guesses[i] << ", ";
	cout << guesses[guesses.size()-1] << "}" << endl;
}

Point solver::first_empty_cell()
{
	int i_empty = 0; //row of first empty cell	
	int j_empty = 0; //column of first empty cell	
	bool done = false; //just so we can break twice

	for(int i = 0;i < size*size;++i)
	{
		for(int j = 0;j < size*size;++j)
		{
			if(grid[i][j] == -1)
			{
				done = true;
				i_empty = i;
				j_empty = j;
				break;
			}
		}
		if(done) break;
	}

	//cout << "FEC is (" << i_empty << "," << j_empty << ")" << endl;

	return Point(i_empty,j_empty);
}

vector<int> solver::legal_values(Point p)
{
	vector<int> current_guesses;
	//initially {1,2,...,size*size} are possible
	for(int i = 0;i < size*size;++i)
		current_guesses.push_back(i+1);

	//check rows and columns
	/*
	for(int i = 0;i < size*size;++i)
	{
		for(int j = 0;j < size*size;++j)
		{
			if(i == p.x && j == p.y) continue;
			//else if(j == p.x && i == p.y) continue;
			//cout << "grid[i][j] is " << grid[i][j] << " and grid[j][i] is " << grid[j][i] << endl;

			if(grid[i][j] != -1)
			{
				auto it = std::find(current_guesses.begin(), current_guesses.end(), grid[i][j]);

				if (it != current_guesses.end()) {
					using std::swap;
					swap(*it, current_guesses.back());
					current_guesses.pop_back();
				}
			}

			if(grid[j][i] != -1)
			{
				auto it = std::find(current_guesses.begin(), current_guesses.end(), grid[j][i]);

				if (it != current_guesses.end()) {
					using std::swap;
					swap(*it, current_guesses.back());
					current_guesses.pop_back();
				}
			}
		}
	}
	*/
	//check row
	for(int j = 0;j < size*size;++j)
	{
		if(grid[p.x][j] != -1)	
		{
				auto it = std::find(current_guesses.begin(), current_guesses.end(), grid[p.x][j]);

				if (it != current_guesses.end()) {
					using std::swap;
					swap(*it, current_guesses.back());
					current_guesses.pop_back();
				}
		}
	}

	//check row
	for(int i = 0;i < size*size;++i)
	{
		if(grid[i][p.y] != -1)	
		{
				auto it = std::find(current_guesses.begin(), current_guesses.end(), grid[i][p.y]);

				if (it != current_guesses.end()) {
					using std::swap;
					swap(*it, current_guesses.back());
					current_guesses.pop_back();
				}
		}
	}
	//cout << "POINT 1: ";
	//print_guesses(current_guesses);

	//check size by size grids
	//we first find the upper left corner of the size by size grid
	//    for example, for normal sudoku, point (5,3) would have
	//    upper left corner (4,3)
	Point upper_left((p.x/size)*size,(p.y/size)*size); //exploiting integer division
	for(int i = upper_left.x;i < upper_left.x+size;++i)
	{
		for(int j = upper_left.y;j < upper_left.y+size;++j)
		{
			if(grid[i][j] != -1)
			{
				auto it = std::find(current_guesses.begin(), current_guesses.end(), grid[i][j]);

				if (it != current_guesses.end()) {
					using std::swap;
					swap(*it, current_guesses.back());
					current_guesses.pop_back();
				}
			}
		}
	}
	return current_guesses;
}

bool solver::solve()
{
	/*
	 *  1. Find the first empty cell in the grid.
	 *  2. Find all legal values for the grid, put in vector "guesses"
	 *  3. If this is the final cell, fill it with guesses[0] and return true
	 *  4. Elseif guesses is empty, return false
	 *  5. While guesses is nonempty,
	 *  	5a. Put guesses[0] in the cell
	 *  	5b. If (recursive) solve() returns true, return true
	 *  	5c. Otherwise, remove guesses[0] from guesses
	 */

	//print_grid();
	Point first_empty = first_empty_cell();	
	vector<int> guesses = legal_values(first_empty);
	//print_guesses(guesses);
	if(guesses.empty())
	{
		//cout << "Guesses empty!" << endl;
		return false;
	}
	if(first_empty.x == size*size-1 && first_empty.y == size*size-1)
	{
		//assert(!guesses.empty());
		grid[first_empty.x][first_empty.y] = guesses[0];
		return true;
	}
	while(!guesses.empty())
	{
		grid[first_empty.x][first_empty.y] = guesses[0];
		if(solve())
			return true;
		else
			guesses.erase(guesses.begin());
	}
}
