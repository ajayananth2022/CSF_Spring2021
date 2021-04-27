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


/*
 * a struct that contains a dictionary that maps variables to their values
 */
struct Calc {

    private:
        std::map<std::string, int> variables; 
        pthread_mutex_t lock; 
    public:
        Calc() {
            pthread_mutex_init(&lock, NULL);
        }
        ~Calc() {
            pthread_mutex_destroy(&lock);
        }

        /*
         * a public function used by calc_eval that evaluates the expr and
         * stores the result of evaluation in result
         * @param expr the expression to be evaluated
         * @param result stores the evaluation result
         * returns 1 if evaluation succeeded, 0 therwise
         */
        int evalExpr(const string &expr, int &result);

    private:
        /*
         * reads elements of a string expression and stores into a vector
         * @param expr the expression to be read and tokenized
         * returns a vector of strings that contain elements of the expr
         */
        vector<string> tokenize(const string &expr);

        /*
         * checks if a string is a valid number
         * @param operand string to be checked
         * returns true if the string is a number, 0 otherwise
         */
        bool isNum(const string &operand);
        
        /*
         * performs the operation specified by op (+, -, *, /, =)
         * @param op1 the first operand
         * @param op2 the second operand
         * @param op specifies the operation to be carried out
         * returns the integer result of the operation
         */
        int operation(string op1, string op2, string op);

        /*
         * checks if a string is an existing variable in the dictionary
         * @param var string to be checked
         * returns true if the string exists in dictionary, 0 otherwise
         */
        bool existsInDict(string &var);

        /*
         * checks if a string is a valid variable
         * @param operand string to be checked
         * returns true if the string is a variable, 0 otherwise
         */
        bool isVar(const string &operand);

        /*
         * checks if a string is a valid operator 
         * @param op string to be checked
         * returns true if the string is an operator, 0 otherwise
         */
        bool isOperator(const string &op);

        /*
         * checks if an expression is a valid operation
         * @param op1 the first operand
         * @param op2 the second operand
         * @param op the operator
         * @param vec the tokenized expression to be evaluated
         * returns true if the operation is valid, 0 otherwise
         */
        bool isValidOperation(string &op1, string &op2, string &op, vector<string> vec);

};

vector<string> Calc::tokenize(const string &expr) {
    vector<string> vec;
    stringstream s(expr); //create a stringstream of expr

    string tok;
    //read strings from expr, separated by space
    while (s >> tok) { 
        vec.push_back(tok);
    }

    return vec;
}

bool Calc::isNum(const string &operand) {
    int length = operand.length();

    //check if each char of operand is a digit
    for (int i = 0; i < length; i++) {

        //check if negative num
        if (i == 0 && operand.at(0) == '-') {
            continue; 
        }
        if (!isdigit(operand.at(i))) return false;
    }
    return true;
}

bool Calc::existsInDict(string &var) {
    map<string, int>::iterator it;

    //find the variable in dictionary
    it = variables.find(var);
    if (it != variables.end()) return true;
    return false;
}

bool Calc::isVar(const string &operand) {
    int length = operand.length();

    //loop through each char to check if its a valid letter
    for (int i = 0; i < length; i++) {
        char cur = operand.at(i);
        if (cur < 65 || cur > 122) return false;
        if (cur > 90 && cur < 97) return false;
    }
    return true;
}

bool Calc::isOperator(const string &op) {
    return (op != "+" || op != "-" || op != "*" 
        || op != "/" || op != "=");
}

int Calc::operation(string op1, string op2, string op) {
    pthread_mutex_lock(&lock); 
    int result;

    if (op == "=") {
        //stores variable into dictionary
        variables[op1] = stoi(op2);
        //stores the assigned value into result
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
    pthread_mutex_unlock(&lock); 
    return result;
}

bool Calc::isValidOperation(string &op1, string &op2, string &op, vector<string> vec) {
    //if the second operand is not a number
    if (!isNum(op2)) { 

        //invalid if it's not in the dictionary
        if (!isVar(op2) || !existsInDict(op2)) return false;
        //if op2 is a variable in dictionary, stores its value in op2
        op2 = to_string(variables[op2]);
    }

    //if the first operand is not a number
    if (!isNum(op1)) {
        //invalid if it's not a valid variable
        if (!isVar(op1)) return false;
        if (!existsInDict(op1)) {
            //invalid if op1 doesn't exist in dictionary and operation 
            //is not assignment or if assignment is in the middle of expression
            if (op != "=" || !vec.empty()) return false;
        } else {
            //if op1 is a variable in dictionary, stores its value in op1
            if (op != "=") op1 = to_string(variables[op1]);
        }
    }
    //invalid if trying to assign to a number
    if (isNum(op1) && op == "=") return false;
    //invalid for division by 0
    if (op == "/" && stoi(op2) == 0) return false;
    return true;
}

int Calc::evalExpr(const string &expr, int &result) {

    pthread_mutex_lock(&lock); 

    vector<string> vec = tokenize(expr);

    int returnVal = 1; 
    
    //read in three strings from vector until it's empty
    while (!vec.empty()) {
        int size = vec.size();
        if (size == 1) { //only 1 operand exists
            string cur = vec.back();
            if (!isNum(cur)) {
                //invalid if the variable doesn't exist in dictionary
                if (!existsInDict(cur)) {
                    returnVal = 0; 
                    break; 
                } 
                result = variables[cur];
            } else {
                result = stoi(cur);
            }
            returnVal = 1; 
            break; 
        } else if (size >= 3) {
            string operand2 = vec.back();
            vec.pop_back();
            string op = vec.back();
            //invalid if operator is not valid
            if (!isOperator(op)) {
                returnVal = 0; 
                break; 
            }
            vec.pop_back();
            string operand1 = vec.back();
            vec.pop_back();
            if (!isValidOperation(operand1, operand2, op, vec)) {
                returnVal = 0; 
                break; 
            }
            vec.push_back(to_string(operation(operand1, operand2, op)));
        } else {
            returnVal = 0; 
            break; 
        }
    }

    pthread_mutex_unlock(&lock); 
    return returnVal;
}


/*
 * creates an instance of the struct Calc data type
 * returns a pointer to the Calc struct
 */
extern "C" struct Calc *calc_create(void) {
    return new Calc();
}

/*
 * destroys an instance of the struct Calc data type by 
 * deallocating its memory
 * @param calc the Calc struct to be destroyed
 */
//TODO: make sure mutex is destroyed. We probably need to figure out what to do on a shutdown. 
extern "C" void calc_destroy(struct Calc *calc) {
    delete calc;
}

/*
 * evaluates an expression stored as a C character string in expr, saving 
 * the result of evaluating the expression in the variable pointed to by result
 * @param calc contains the variable information
 * @param expr expression to be evaluated
 * @param result stores the result of the expression
 * returns 1 if evaluation succeeded, 0 otherwise
 */
extern "C" int calc_eval(struct Calc *calc, const char *expr, int *result) {
    return calc->evalExpr(expr, *result);
}