
#include <string>
#include <stack>
#include <ctype.h>
#include <assert.h>
#include <iostream>

using namespace std;

enum Token{ ADD, SUB, MUL, DIV, L_PAR, R_PAR, NUM, VAR };

class Lexer{
    private:
        string  calc;
        string  stringValue;
        string  oldStringValue;
        int     pos;
        int     numberValue;
        int     oldNumberValue;
        Token   tok;
        
    public:
                Lexer(string str)
                    :pos(0), calc(str){}
        string  getStringValue()    { return stringValue;       }
        string  getOldStringValue() { return oldStringValue;    }
        int     getNumberValue()    { return numberValue;       }
        int     getOldNumberValue() { return oldNumberValue;    }
        bool    hasNextToken()      { return (pos<calc.size()); }
        Token   getToken()          { return tok;               }
        void    moveToNextToken()   {
            char ch = calc[pos++];
            while (ch == ' ') { ch = calc[pos++]; }
            //cout << ch;
            switch (ch){
                case '+': tok = ADD; break;
                case '-': tok = SUB; break;
                case '*': tok = MUL; break;
                case ':':
                case '/': tok = DIV; break;
                case '(': tok = L_PAR; break;
                case ')': tok = R_PAR; break;
                default:
                    if (isdigit(ch)) {
                        int i = pos - 1;
                        oldNumberValue = numberValue;
                        numberValue = 0;
                        while ((i<calc.size()) && (isdigit(calc[i]))){
                            numberValue = ((numberValue * 10) + calc[i++] - '0');
                        }
                        pos = i;
                        tok = NUM;
                    } else if (isalpha(ch)) {
                        int i = pos - 1;
                        oldStringValue = stringValue;
                        stringValue = "";
                        while ((i<calc.size()) && (isalpha(calc[i]))){
                            stringValue += calc[i++];
                        }
                        tok = VAR;
                    }
                    break;
            }
        }

/*
        Token getNextToken(){
            char ch = calc[pos++];
            while (ch == ' ') { ch = calc[pos++]; }
            cout << "CHAR: " << ch << endl;
            switch (ch){
                case '+': return ADD;
                case '-': return SUB;
                case '*':
                case 'x': return MUL;
                case ':':
                case '/': return DIV;
                case '(': return L_PAR;
                case ')': return R_PAR;
                default:
                    if (isdigit(ch)) {
                        int i = --pos;
                        numberValue = 0;
                        while((i<calc.size()) && (isdigit(calc[i]))){
                            numberValue = ((numberValue * 10) + calc[i++] - '0');
                        }
                        pos = i;
                        return NUM;
                    } else if (isalpha(ch)) {
                        int i = --pos;
                        stringValue = "";
                        while((i<calc.size()) && (isalpha(calc[i]))){
                            stringValue += calc[i++];
                        }
                        return VAR;
                    } else {
                        if (--pos >= calc.size()){
                            return false;
                        }
                    }
            }
        }
*/
};

class Parser {
    private:
        Lexer _lexer;
        string calc;
        stack<int> numbers;
        int num1, num2, total;
        void parseError(const char* func){ cout << "assert in " << func << endl; assert(false); }
        bool accept(Token tok){ 
            if (_lexer.getToken() == tok){
                _lexer.moveToNextToken();
                return true;
            }
            return false;
        }
        bool expect(Token tok){
            if (!accept(tok)) { assert(false); }
        }
        bool expr(){
            /*
            expr  = NUM expr2
                  | VAR expr2
                  | L_PAR expr PAR_R
            */
            if (accept(NUM)) {
                numbers.push(_lexer.getNumberValue());
                if (!expr2()) { parseError(__FUNCTION__); }
                return true;
            } else if (accept(VAR)) {
                if (!expr2()) { parseError(__FUNCTION__); }
                return true;
            } else if (accept(L_PAR)) {
                if (!expr()) { parseError(__FUNCTION__); }
                expect(R_PAR);
                return true;
            } else {
                parseError(__FUNCTION__);
            }
        }
        bool expr2(){
            /*
            expr2 = ADD expr expr2
                  | SUB expr expr2
                  | MUL expr expr2
                  | DIV expr expr2
                  | epsilon
            */
            if (accept(ADD)) {
                if (!expr())  { parseError(__FUNCTION__); }
                if (!expr2()) { parseError(__FUNCTION__); }
                assert(!numbers.empty());
                assert(numbers.size() >= 2);
                num2 = numbers.top();
                numbers.pop();
                num1 = numbers.top();
                numbers.pop();
                numbers.push(num1+num2);
                return true;
            } else if (accept(SUB)) {
                if (!expr())  { parseError(__FUNCTION__); }
                if (!expr2()) { parseError(__FUNCTION__); }
                assert(!numbers.empty());
                assert(numbers.size() >= 2);
                num2 = numbers.top();
                numbers.pop();
                num1 = numbers.top();
                numbers.pop();
                numbers.push(num1-num2);
                return true;
            } else if (accept(MUL)) {
                if (!expr())  { parseError(__FUNCTION__); }
                if (!expr2()) { parseError(__FUNCTION__); }
                assert(!numbers.empty());
                assert(numbers.size() >= 2);
                num2 = numbers.top();
                numbers.pop();
                num1 = numbers.top();
                numbers.pop();
                numbers.push(num1*num2);
                return true;
            } else if (accept(DIV)) {
                if (!expr())  { parseError(__FUNCTION__); }
                if (!expr2()) { parseError(__FUNCTION__); }
                assert(!numbers.empty());
                assert(numbers.size() >= 2);
                num2 = numbers.top();
                numbers.pop();
                num1 = numbers.top();
                numbers.pop();
                numbers.push(num1/num2);
                return true;
            } else {
                return true;
            }
        }

    public:
        Parser(string calc)
            : _lexer(calc){
                parse(calc); 
        }
        void parse(string calc){
            _lexer.moveToNextToken();
            bool success = expr();
            if ((success) && (!_lexer.hasNextToken())){
                if (!numbers.empty()){
                    cout << "Total: " << numbers.top() << endl;
                } else {
                    cout << "Empty Stack!" << endl;
                    assert(false);
                }
            } else {
                cout << "string is wrong!" << endl;
            }
        }
};

int main(){
    string calc;
    getline(cin, calc);
    Parser _parser(calc);



/*
    Lexer _lexer(calc);
    Token tok;
    while (_lexer.hasNextToken()){
       _lexer.moveToNextToken();
       switch(tok = _lexer.getToken()) {
            case ADD: cout << "ADD " << endl; break;
            case SUB: cout << "SUB " << endl; break;
            case MUL: cout << "MUL " << endl; break;
            case DIV: cout << "DIV " << endl; break;
            case NUM: cout << "NUM(" << _lexer.getNumberValue() << ") " << endl; break;
            case VAR: cout << "VAR(" << _lexer.getStringValue() << ") " << endl; break;
            case L_PAR: cout << "L_PAR " << endl; break;
            case R_PAR: cout << "R_PAR " << endl; break;
        }
    }
*/
/*
    getline(cin, calc);
    cout << "lexing \"" << calc << "\":" << endl;
    while (hasNextToken()){
        switch(tok = getNextToken()) {
            case ADD: cout << "ADD "; break;
            case SUB: cout << "SUB "; break;
            case MUL: cout << "MUL "; break;
            case DIV: cout << "DIV "; break;
            case NUM: cout << "NUM(" << getNumberValue() << ") "; break;
            case VAR: cout << "VAR(" << getStringValue() << ") "; break;
            case L_PAR: cout << "L_PAR "; break;
            case R_PAR: cout << "R_PAR "; break;
        }
    }
*/

}
