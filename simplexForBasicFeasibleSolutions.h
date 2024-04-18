#include <cfloat>
#include <iostream>
#include <string>
using namespace std;
int enteringVar(vector<double>& objFunc);
int leavingVar(vector<vector<double>>& constraints, unsigned int enteringVar);
void solveForVar(vector<double>& eqn, double* isolated, unsigned int toIsolate);
void substitute(vector<double> into, vector<double> from, unsigned int replaceInd);
void phaseII(vector<double>& objFunc, vector<vector<double>>& constraints, vector<unsigned int>& basicVar);
