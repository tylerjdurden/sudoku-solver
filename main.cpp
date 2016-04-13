#include "solver.h"

int main()
{
	solver s; 
	s.read_in_values();
	s.print_grid();
	s.solve();
	s.print_grid();
}
