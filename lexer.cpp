
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
        int     pos;
        int     numberValue;
        Token   tok;
        
    public:
                Lexer(string str)
                    :pos(0), calc(str){}
        string  getStringValue()    { return stringValue;       }
        int     getNumberValue()    { return numberValue;       }
        bool    hasNextToken()      { return (pos<calc.size()); }
        Token   getToken()          { return tok;               }
        void    moveToNextToken()   {
            char ch = calc[pos++];
            cout << ch << " "; 
            while (ch == ' ') { ch = calc[pos++]; }
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
                        numberValue = 0;
                        while ((i<calc.size()) && (isdigit(calc[i]))){
                            numberValue = ((numberValue * 10) + calc[i++] - '0');
                        }
                        pos = i;
                        tok = NUM;
                    } else if (isalpha(ch)) {
                        int i = pos - 1;
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
        void parseError(){ assert(false); }
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
                if (!expr2()) { parseError(); }
                return true;
            } else if (accept(VAR)) {
                if (!expr2()) { parseError(); }
                return true;
            } else if (accept(L_PAR)) {
                if (!expr2()) { parseError(); }
                expect(R_PAR);
                return true;
            } else {
                parseError();
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
                if (!expr())  { parseError(); }
                if (!expr2()) { parseError(); }
                return true;
            } else if (accept(SUB)) {
                if (!expr())  { parseError(); }
                if (!expr2()) { parseError(); }
                return true;
            } else if (accept(MUL)) {
                if (!expr())  { parseError(); }
                if (!expr2()) { parseError(); }
                return true;
            } else if (accept(DIV)) {
                if (!expr())  { parseError(); }
                if (!expr2()) { parseError(); }
                return true;
            } else {
                return true;
            }
        }

    public:
        Parser(string calc)
            : _lexer(calc){
                if (parse(calc)) { 
                    cout << "give out calculated value" << endl;
                } else {
                    cout << "this should never be reached?" << endl;
                }
        }
        bool parse(string calc){
            _lexer.moveToNextToken();
            return (expr());
        }
};

int main(){
    string calc;
    getline(cin, calc);
    //Parser _parser(calc);
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
