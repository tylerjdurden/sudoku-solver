#include <vector>
#include <algorithm>
using namespace std;

struct Point
{
	int x, y;
	Point(int xx, int yy): x(xx), y(yy) {}
};

class solver
{
public:
	void read_in_values();
	void read_in_values(istream&);
	void print_grid();
	void print_guesses(const vector<int>&);
	Point first_empty_cell();
	vector<int> legal_values(Point);
	bool solve();
private:
	vector<vector<int>> grid;
	int size;
};
