#include <iostream> 
#include <string> 
#include <vector> 
#include <fstream> 
#include <ctype.h> 
#include <cctype> 
using namespace std;

enum TokenType {

    KEYWORD_LIFTOFF,
    KEYWORD_MISSION,
    KEYWORD_TRANSMIT,
    KEYWORD_RECEIVE,
    KEYWORD_CONTROL,
    KEYWORD_ALTERNATIVE,
    KEYWORD_JOURNEY,
    KEYWORD_EXPLORE,
    KEYWORD_LAND,
    KEYWORD_IMPORT,
    KEYWORD_UNIVERSE,

    DATATYPE_MERCURY, // Mercury (int) 
    DATATYPE_VENUS,   // Venus (char) 
    DATATYPE_EARTH,   // Earth (float) 
    DATATYPE_MARS,    // Mars (long long) 
    DATATYPE_JUPITER, // Jupiter (string) 
    DATATYPE_SATURN,  // Saturn (double) 
    DATATYPE_URANUS,  // Uranus (bool) 

    IDENTIFIER,

    INT_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    BOOLEAN_LITERAL,

    ASSIGN,
    PLUS,
    MINUS,
    ASTERISK,
    DIVIDE,
    MODULO,
    GREATER_THAN,
    LESS_THAN,
    NOT,
    IS_EQUAL,
    NOT_EQUAL,
    GTE,             // >= 
    LTE,             // <= 
    LOGICAL_AND,     // && 
    LOGICAL_OR,      // || 
    PLUS_EQUAL,      // += 
    MINUS_EQUAL,     // -= 
    MULTIPLY_EQUAL,  // *= 
    DIVIDE_EQUAL,    // /= 
    INCREMENT,       // ++ 
    DECREMENT,       // -- 

    SEMICOLON,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_SQUARE,
    RIGHT_SQUARE,
    COMMA,

    COMMENT,
    END_OF_FILE,
    UNKNOWN
};


struct Token {
    TokenType type;
    string lexeme;
};

class Scanner {
private:
    ifstream f;

    TokenType check_reserved(string s) {
        if (s == "import") return KEYWORD_IMPORT;
        else if (s == "universe") return KEYWORD_UNIVERSE;
        else if (s == "mission") return KEYWORD_MISSION;
        else if (s == "liftoff") return KEYWORD_LIFTOFF;
        else if (s == "transmit") return KEYWORD_TRANSMIT;
        else if (s == "receive") return KEYWORD_RECEIVE;
        else if (s == "control") return KEYWORD_CONTROL;
        else if (s == "alternative") return KEYWORD_ALTERNATIVE;
        else if (s == "journey") return KEYWORD_JOURNEY;
        else if (s == "explore") return KEYWORD_EXPLORE;
        else if (s == "land") return KEYWORD_LAND;
        else if (s == "Mercury") return DATATYPE_MERCURY;
        else if (s == "Venus") return DATATYPE_VENUS;
        else if (s == "Earth") return DATATYPE_EARTH;
        else if (s == "Mars") return DATATYPE_MARS;
        else if (s == "Jupiter") return DATATYPE_JUPITER;
        else if (s == "Saturn") return DATATYPE_SATURN;
        else if (s == "Uranus") return DATATYPE_URANUS;
        else if (s == "true" || s == "false") return BOOLEAN_LITERAL;
        else return IDENTIFIER;
    }

public:
    Scanner(string filename) {
        f.open(filename);
    }

    bool is_open() {
        return f.is_open();
    }

    ~Scanner() {
        f.close();
    }

    Token get_token() {
        char ch;
        string s;
        Token t;

        f.get(ch);
        while (isspace(ch) && !f.eof()) f.get(ch);
        if (f.eof()) { t.type = END_OF_FILE; t.lexeme = ""; return t; }


        if (ch == '=') {
            f.get(ch);
            if (ch == '=') { t.type = IS_EQUAL; t.lexeme = "=="; }
            else { f.putback(ch); t.type = ASSIGN; t.lexeme = "="; }
            return t;
        }


        else if (ch == '+') {
            f.get(ch);
            if (ch == '+') { t.type = INCREMENT; t.lexeme = "++"; }
            else if (ch == '=') { t.type = PLUS_EQUAL; t.lexeme = "+="; }
            else { f.putback(ch); t.type = PLUS; t.lexeme = "+"; }
            return t;
        }


        else if (ch == '-') {
            f.get(ch);
            if (ch == '-') { t.type = DECREMENT; t.lexeme = "--"; }
            else if (ch == '=') { t.type = MINUS_EQUAL; t.lexeme = "-="; }
            else { f.putback(ch); t.type = MINUS; t.lexeme = "-"; }
            return t;
        }

        else if (ch == '*') {
            char next;
            f.get(next);


            if (next == '*') {
                char peek;
                f.get(peek);

                if (peek == '*') {
                    std::string comment;
                    char prev1 = 0, prev2 = 0;
                    while (f.get(ch)) {
                        if (prev2 == '*' && prev1 == '*' && ch == '*') break;
                        comment += ch;
                        prev2 = prev1;
                        prev1 = ch;
                    }
                    t.type = COMMENT;
                    t.lexeme = "***" + comment + "***";
                }
                else {
                    std::string comment;
                    if (peek != '\n') {
                        comment += peek;
                        std::getline(f, comment, '\n');
                    }
                    t.type = COMMENT;
                    t.lexeme = "**" + comment;
                }
            }


            else if (next == '=') {
                t.type = MULTIPLY_EQUAL;
                t.lexeme = "*=";
            }


            else {
                f.putback(next);
                t.type = ASTERISK;
                t.lexeme = "*";
            }

            return t;
        }



        else if (ch == '/') {
            f.get(ch);
            if (ch == '=') { t.type = DIVIDE_EQUAL; t.lexeme = "/="; }
            else { f.putback(ch); t.type = DIVIDE; t.lexeme = "/"; }
            return t;
        }

        else if (ch == '%') { t.type = MODULO; t.lexeme = "%"; return t; }

        else if (ch == '>') {
            f.get(ch);
            if (ch == '=') { t.type = GTE; t.lexeme = ">="; }
            else { f.putback(ch); t.type = GREATER_THAN; t.lexeme = ">"; }
            return t;
        }

        else if (ch == '<') {
            f.get(ch);
            if (ch == '=') { t.type = LTE; t.lexeme = "<="; }
            else { f.putback(ch); t.type = LESS_THAN; t.lexeme = "<"; }
            return t;
        }

        else if (ch == '!') {
            f.get(ch);
            if (ch == '=') { t.type = NOT_EQUAL; t.lexeme = "!="; }
            else { f.putback(ch); t.type = NOT; t.lexeme = "!"; }
            return t;
        }


        else if (ch == '&') {
            f.get(ch);
            if (ch == '&') { t.type = LOGICAL_AND; t.lexeme = "&&"; }
            else { f.putback(ch); t.type = UNKNOWN; t.lexeme = "&"; }
            return t;
        }


        else if (ch == '|') {
            f.get(ch);
            if (ch == '|') { t.type = LOGICAL_OR; t.lexeme = "||"; }
            else { f.putback(ch); t.type = UNKNOWN; t.lexeme = "|"; }
            return t;
        }


        else if (ch == '(') { t.type = LEFT_PAREN; t.lexeme = "("; return t; }
        else if (ch == ')') { t.type = RIGHT_PAREN; t.lexeme = ")"; return t; }
        else if (ch == '{') { t.type = LEFT_BRACE; t.lexeme = "{"; return t; }
        else if (ch == '}') { t.type = RIGHT_BRACE; t.lexeme = "}"; return t; }
        else if (ch == '[') { t.type = LEFT_SQUARE; t.lexeme = "["; return t; }
        else if (ch == ']') { t.type = RIGHT_SQUARE; t.lexeme = "]"; return t; }
        else if (ch == ';') { t.type = SEMICOLON; t.lexeme = ";"; return t; }
        else if (ch == ',') { t.type = COMMA; t.lexeme = ","; return t; }

        else if (ch == '"') {
            s = "";
            f.get(ch);
            while (ch != '"' && !f.eof()) { s += ch; f.get(ch); }
            t.type = STRING_LITERAL;
            t.lexeme = s;
            return t;
        }

        else if (isdigit(ch)) {
            s = ch;
            f.get(ch);
            bool hasDot = false;
            while (isdigit(ch) || ch == '.') {
                if (ch == '.') {
                    if (hasDot) break;
                    hasDot = true;
                }
                s += ch;
                f.get(ch);
            }
            f.putback(ch);
            t.type = hasDot ? FLOAT_LITERAL : INT_LITERAL;
            t.lexeme = s;
            return t;
        }


        else if (isalpha(ch) || ch == '_') {
            s = ch;
            f.get(ch);
            while (isalnum(ch) || ch == '_') {
                s += ch;
                f.get(ch);
            }
            f.putback(ch);
            t.type = check_reserved(s);
            t.lexeme = s;
            return t;
        }


        else {
            t.type = UNKNOWN;
            t.lexeme = string(1, ch);
            return t;
        }
    }

    void display_tokens() {
        Token t;
        while (true) {
            t = get_token();
            if (t.type == END_OF_FILE) {
                cout << "End of file reached.\n";
                break;
            }

            cout << t.lexeme << " : ";

            switch (t.type) {
            case KEYWORD_IMPORT: cout << "KEYWORD_IMPORT"; break;case KEYWORD_UNIVERSE: cout << "KEYWORD_UNIVERSE"; break;
            case KEYWORD_MISSION: cout << "KEYWORD_MISSION"; break;
            case KEYWORD_LIFTOFF: cout << "KEYWORD_LIFTOFF"; break;
            case KEYWORD_TRANSMIT: cout << "KEYWORD_TRANSMIT"; break;
            case KEYWORD_RECEIVE: cout << "KEYWORD_RECEIVE"; break;
            case KEYWORD_CONTROL: cout << "KEYWORD_CONTROL"; break;
            case KEYWORD_ALTERNATIVE: cout << "KEYWORD_ALTERNATIVE"; break;
            case KEYWORD_JOURNEY: cout << "KEYWORD_JOURNEY"; break;
            case KEYWORD_EXPLORE: cout << "KEYWORD_EXPLORE"; break;
            case KEYWORD_LAND: cout << "KEYWORD_LAND"; break;
            case DATATYPE_MERCURY: cout << "DATATYPE_MERCURY"; break;
            case DATATYPE_VENUS: cout << "DATATYPE_VENUS"; break;
            case DATATYPE_EARTH: cout << "DATATYPE_EARTH"; break;
            case DATATYPE_MARS: cout << "DATATYPE_MARS"; break;
            case DATATYPE_JUPITER: cout << "DATATYPE_JUPITER"; break;
            case DATATYPE_SATURN: cout << "DATATYPE_SATURN"; break;
            case DATATYPE_URANUS: cout << "DATATYPE_URANUS"; break;
            case IDENTIFIER: cout << "IDENTIFIER"; break;
            case INT_LITERAL: cout << "INT_LITERAL"; break;
            case FLOAT_LITERAL: cout << "FLOAT_LITERAL"; break;
            case STRING_LITERAL: cout << "STRING_LITERAL"; break;
            case BOOLEAN_LITERAL: cout << "BOOLEAN_LITERAL"; break;
            case ASSIGN: cout << "ASSIGN"; break;
            case PLUS: cout << "PLUS"; break;
            case MINUS: cout << "MINUS"; break;
            case ASTERISK: cout << "ASTERISK"; break;
            case DIVIDE: cout << "DIVIDE"; break;
            case MODULO: cout << "MODULO"; break;
            case GREATER_THAN: cout << "GREATER_THAN"; break;
            case LESS_THAN: cout << "LESS_THAN"; break;
            case GTE: cout << "GTE"; break;
            case LTE: cout << "LTE"; break;
            case NOT: cout << "NOT"; break;
            case IS_EQUAL: cout << "IS_EQUAL"; break;
            case NOT_EQUAL: cout << "NOT_EQUAL"; break;
            case LOGICAL_AND: cout << "LOGICAL_AND"; break;
            case LOGICAL_OR: cout << "LOGICAL_OR"; break;
            case PLUS_EQUAL: cout << "PLUS_EQUAL"; break;
            case MINUS_EQUAL: cout << "MINUS_EQUAL"; break;
            case MULTIPLY_EQUAL: cout << "MULTIPLY_EQUAL"; break;
            case DIVIDE_EQUAL: cout << "DIVIDE_EQUAL"; break;
            case INCREMENT: cout << "INCREMENT"; break;
            case DECREMENT: cout << "DECREMENT"; break;
            case SEMICOLON: cout << "SEMICOLON"; break;
            case LEFT_PAREN: cout << "LEFT_PAREN"; break;
            case RIGHT_PAREN: cout << "RIGHT_PAREN"; break;
            case LEFT_BRACE: cout << "LEFT_BRACE"; break;
            case RIGHT_BRACE: cout << "RIGHT_BRACE"; break;
            case LEFT_SQUARE: cout << "LEFT_SQUARE"; break;
            case RIGHT_SQUARE: cout << "RIGHT_SQUARE"; break;
            case COMMA: cout << "COMMA"; break;
            case COMMENT: cout << "COMMENT"; break;
            default: cout << "UNKNOWN"; break;
            }

            cout << endl;
        }
    }
};


int main() {
    string Filename;
    cout << "Enter Name of input File: ";
    cin >> Filename;

    if (Filename.size() < 4 || Filename.substr(Filename.size() - 4) != ".uni") {
        cout << "Error: Invalid file type. Please use a .uni file only.\n";
        return 1;
    }

    Scanner SC(Filename);

    if (!SC.is_open()) {
        cout << "Error: Cannot open file '" << Filename << "'" << endl;
        return 1;
    }

    SC.display_tokens();
    return 0;
}