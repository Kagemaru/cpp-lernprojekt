
#include <string>
#include <stack>
#include <ctype.h>

using namespace std;


class Lexer{
    private:
        enum Token{ ADD, SUB, MUL, DIV, L_PAR, R_PAR, NUM, VAR };
        string calc;
        int pos=0;
        int numberValue;
        string stringValue;
        Token tok;
        
    public:
        Lexer(string str){ calc = str;  };
        int getNumberValue(){ return numberValue;}
        string getStringValue(){ return stringValue;}
        bool hasNextToken(){
            if(pos<calc.size()) { return true;  }
            else                { return false; }
        }

        Token getToken(){ return tok; }

        Token moveToNextToken(){
            char ch = calc[pos];
            while(ch == ' ') { ch = calc[pos++]; }
            switch(ch){
                case '+': tok = ADD;
                case '-': tok = SUB;
                case '*':
                case 'x': tok = MUL;
                case ':':
                case '/': tok = DIV;
                case '(': tok = L_PAR;
                case ')': tok = R_PAR;
                default:
                    if(isdigit(ch)) {
                        int i = --pos;
                        numberValue = 0;
                        while((i<calc.size()) && (isdigit(calc[i]))){
                            numberValue = ((numberValue * 10) + calc[i++] - '0');
                        }
                        pos = i;
                        tok = NUM;
                    } else if(isalpha(ch)) {
                        int i = --pos;
                        stringValue = "";
                        while((i<calc.size()) && (isalpha(calc[i]))){
                            stringValue += calc[i++];
                        }
                        tok = VAR;
                    }
            }
        }

        Token getNextToken(){
            char ch = calc[pos++];
            while(ch == ' ') { ch = calc[pos++]; }
            cout << "CHAR: " << ch << endl;
            switch(ch){
                case '+': return ADD;
                case '-': return SUB;
                case '*':
                case 'x': return MUL;
                case ':':
                case '/': return DIV;
                case '(': return L_PAR;
                case ')': return R_PAR;
                default:
                    if(isdigit(ch)) {
                        int i = --pos;
                        numberValue = 0;
                        while((i<calc.size()) && (isdigit(calc[i]))){
                            numberValue = ((numberValue * 10) + calc[i++] - '0');
                        }
                        pos = i;
                        return NUM;
                    } else if(isalpha(ch)) {
                        int i = --pos;
                        stringValue = "";
                        while((i<calc.size()) && (isalpha(calc[i]))){
                            stringValue += calc[i++];
                        }
                        return VAR;
                    }
                    
            }
        }
}

int main(){
    getline(cin, calc);
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
