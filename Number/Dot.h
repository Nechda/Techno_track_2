#pragma once
#include <string>
#include "Tools/Types.h"

class Number;
namespace dot
{
    using namespace std;

    void generateGraph(string filename);

    void createHistory (const Number& num);
    void createNode    (const Number& num, const string& func = "");
    void createLine    (const Number& src, const Number& dst, string label = "", string style = "dashed");
    void createLine    (const string& src, const Number& dst, string label = "", string style = "dashed");
    void createOperator(const Number& lhs, const Number& rhs, string operatorStr);
};