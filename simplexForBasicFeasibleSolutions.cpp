int enteringVar(vector<double>& objFunc)
{
    int ind = -1;
    int max = 0;
    for(int i = 0; i < objFunc.size(); i++)
    {
        if(objFunc[i] > max)
        {
            max = objFunc[i];
            ind = i;
        }
    }
    return ind;
}

#include <cfloat>
int leavingVar(vector<vector<double>>& constraints, unsigned int enteringVar, bool maximize = 0)
{
    int ind = -1;
    double min = DBL_MAX;
    double max = 0;
    for(int i = 0; i < constraints.size(); i++)
    {
        double ratio = constraints[i][0] / constraints[i][enteringVar];
        if(ratio < 0)
        {
            if(!maximize && abs(ratio) < min)
            {
                min = abs(ratio);
                ind = i;
            }
            if(maximize && abs(ratio) > max)
            {
                max = abs(ratio);
                ind = i;
            }
        }
    }
    return ind;
}

void solveForVar(vector<double>& eqn, unsigned int isolated, unsigned int toIsolate)
{
    eqn[isolated] *= -1;
    isolated = toIsolate;
    eqn[isolated] *= -1;
    for(auto d : eqn)
        d /= eqn[isolated];
}

void substitute(vector<double> into, vector<double> from, unsigned int replaceInd)
{
    for(unsigned int i = 0; i < into.size(); i++)
    {
        if(i == replaceInd)
            into[i] = 0;
        else
            into[i] += (into[replaceInd] * from[i]);
    }
}

#include <string>
#include <iostream>
void phaseII(vector<double>& objFunc, vector<vector<double>>& constraints, vector<unsigned int>& basicVar, vector<double> auxFunc = {})
{
    int eVar = (auxFunc != {}) ? enteringVar(auxFunc) : enteringVar(objFunc);
    while(eVar != -1)
    {
        int lVar = leavingVar(constraints, eVar);
        if(lVar == -1)
        {
            cout << "unbounded" << endl;
            return;
        }
        solveForVar(constraints[lVar], basicVar[lVar], eVar);
        for(unsigned int i = 0; i < constraints.size(); i++)
        {
            if(i != lVar)
                substitute(constraints[i], constraints[lVar], eVar);
        }
        if(auxFunc != {})
            substitute(auxFunc, constraints[lVar], eVar);
        substitute(objFunc, constraints[lVar], eVar);
        eVar = (auxFunc != {}) ? enteringVar(auxFunc) : enteringVar(objFunc);
    }
}

void phaseI(vector<double>& objFunc, vector<vector<double>>& constraints, vector<unsigned int> basicVar)
{
    vector<double> auxFunc(objFunc, 0);
    auxFunc.push_back(-1);
    objFunc.push_back(0);
    for(auto& v : constraints)
        v.push_back(1);
    int eVar = objFunc.size() - 1;
    int lVar = leavingVar(constraints, eVar, 1);
    if(lVar == -1)
    {
        cout << "unbounded" << endl;
        return;
    }
    solveForVar(constraints[lVar], basicVar[lVar], eVar);
    for(unsigned int i = 0; i < constraints.size(); i++)
    {
        if(i != lVar)
            substitute(constraints[i], constraints[lVar], eVar);
    }
    substitute(auxFunc, constraints[lVar], eVar);
    substitute(objFunc, constraints[lVar], eVar);
    phaseII(objFunc, constraints, basicVar, auxFunc);
    auxFunc.pop_back();
    objFunc.pop_back();
    for(auto& v : constraints)
        v.pop_back();
}
