//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu
#include "calc.h"
#include <map>
#include <vector>
#include <sstream>

using std::map;
using std::string;
using std::vector;
using std::stringstream;
using std::to_string;

struct Calc {
    private:
        std::map<std::string, int> variables;

    public:
        Calc() {}
        ~Calc() {}
        int evalExpr(const string &expr, int &result);

    private:
        vector<string> tokenize(const string &expr);
        bool isNum(const string &operand);
        int operation(string op1, string op2, string op);
        bool existsInDict(string &var);
        bool isVar(const string &operand);
        bool isOperator(const string &op);
        bool isValidOperation(string &operand1, string &operand2, string &op, vector<string> vec);

};

vector<string> Calc::tokenize(const string &expr) {
    vector<string> vec;
    stringstream s(expr);

    string tok;
    while (s >> tok) {
        vec.push_back(tok);
    }

    return vec;
}

bool Calc::isNum(const string &operand) {
    int length = operand.length();
    for (int i = 0; i < length; i++) {
        if (!isdigit(operand.at(i))) return false;
    }
    return true;
}

bool Calc::existsInDict(string &var) {
    map<string, int>::iterator it;
    it = variables.find(var);
    if (it != variables.end()) return true;
    return false;
}

bool Calc::isVar(const string &operand) {
    int length = operand.length();
    for (int i = 0; i < length; i++) {
        char cur = operand.at(i);
        if (cur < 65 || cur > 122) return false;
        if (cur > 90 && cur < 97) return false;
    }
    return true;
}

bool Calc::isOperator(const string &op) {
    return (op != "+" || op != "-" || op != "*" || op != "/" || op != "=");
}

int Calc::operation(string op1, string op2, string op) {
    int result;
    if (op == "=") {
        variables[op1] = stoi(op2);
        result = stoi(op2);
    } else if (op == "+") {
        result = stoi(op1) + stoi(op2);
    } else if (op == "-") {
        result = stoi(op1) - stoi(op2);
    } else if (op == "*") {
        result = stoi(op1) * stoi(op2);
    } else {
        result = stoi(op1) / stoi(op2);
    }
    return result;
}

bool Calc::isValidOperation(string &operand1, string &operand2, string &op, vector<string> vec) {
    if (!isNum(operand2)) { 
        if (!isVar(operand2) || !existsInDict(operand2)) return false;
        operand2 = to_string(variables[operand2]);
    }
    if (!isNum(operand1)) {
        if (!isVar(operand1)) return false;
        if (!existsInDict(operand1)) {
            if (op != "=" || !vec.empty()) return false;
        } else {
            operand1 = to_string(variables[operand1]);
        }
    }
    if (isNum(operand1) && op == "=") return false;
    if (op == "/" && stoi(operand2) == 0) return false;
    return true;
}

int Calc::evalExpr(const string &expr, int &result) {
    vector<string> vec = tokenize(expr);
    
    while (!vec.empty()) {
        int size = vec.size();
        if (size == 1) {
            string cur = vec.back();
            if (!isNum(cur)) {
                if (!existsInDict(cur)) return 0;
                result = variables[cur];
            } else {
                result = stoi(cur);
            }
            return 1;
        } else if (size >= 3) {
            string operand2 = vec.back();
            vec.pop_back();
            string op = vec.back();
            if (!isOperator(op)) return 0;
            vec.pop_back();
            string operand1 = vec.back();
            vec.pop_back();
            if (!isValidOperation(operand1, operand2, op, vec)) return 0;
            vec.push_back(to_string(operation(operand1, operand2, op)));
        } else {
            return 0;
        }
    }
    return 1;
}



extern "C" struct Calc *calc_create(void) {
    return new Calc();
}

extern "C" void calc_destroy(struct Calc *calc) {
    delete calc;
}

extern "C" int calc_eval(struct Calc *calc, const char *expr, int *result) {
    return calc->evalExpr(expr, *result);

}