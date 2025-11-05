#include <iostream> 
#include <string> 
#include <vector> 
#include <fstream> 
#include <ctype.h> 
#include <cctype> 

#include <memory>
#include <sstream>
#include <iomanip>

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
            case KEYWORD_IMPORT: cout << "KEYWORD_IMPORT"; break; case KEYWORD_UNIVERSE: cout << "KEYWORD_UNIVERSE"; break;
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


struct ASTNode {
    string type;
    string value;
    vector<unique_ptr<ASTNode>> children;

    ASTNode(string t, string v = "") : type(move(t)), value(move(v)) {}
    void add_child(unique_ptr<ASTNode> c) { children.push_back(move(c)); }
};

class Parser {
private:
    Scanner scanner;
    Token cur;
    int tokenCount = 0;

    void advance() {
        cur = scanner.get_token();
        tokenCount++;
    }

    bool accept(TokenType tt) {
        if (cur.type == tt) { advance(); return true; }
        return false;
    }

    bool expect(TokenType tt, const string& what) {
        if (cur.type == tt) { advance(); return true; }
        error("Expected " + what + " but found '" + cur.lexeme + "'");
        return false;
    }

    [[noreturn]] void error(const string& msg) {
        cerr << "Parse error: " << msg << " (token #" << tokenCount
            << ", lexeme=\"" << cur.lexeme << "\")\n";
        exit(1);
    }

public:
    Parser(const string& filename) : scanner(filename) {
        if (!scanner.is_open()) {
            cerr << "Cannot open input file: " << filename << endl;
            exit(1);
        }
        advance();
    }


    unique_ptr<ASTNode> parseProgram() {
        auto root = make_unique<ASTNode>("Program");
        root->add_child(parseStatementList());
        if (cur.type != END_OF_FILE) {
            error("Extra tokens after program end");
        }
        return root;
    }

private:
    unique_ptr<ASTNode> parseStatementList() {
        auto node = make_unique<ASTNode>("StatementList");
        while (cur.type != END_OF_FILE && cur.type != RIGHT_BRACE) {
            if (cur.type == SEMICOLON) { advance(); continue; }
            node->add_child(parseStatement());
        }
        return node;
    }

    unique_ptr<ASTNode> parseStatement() {
        switch (cur.type) {
        case DATATYPE_MERCURY:
        case DATATYPE_VENUS: 
        case DATATYPE_EARTH:
        case DATATYPE_MARS: 
        case DATATYPE_JUPITER:
        case DATATYPE_SATURN:
        case DATATYPE_URANUS:
            return parseDeclaration();
        case IDENTIFIER:
            return parseAssignment();
        case KEYWORD_CONTROL:
            return parseIfStmt();
        case KEYWORD_JOURNEY:
            return parseWhileStmt();
        case KEYWORD_EXPLORE:
            return parseForStmt();
        case KEYWORD_MISSION:
            return parseFunctionStmt();
        case KEYWORD_TRANSMIT:
            return parseOutputStmt();
        case KEYWORD_RECEIVE:
            return parseInputStmt();
        case KEYWORD_LAND:
            return parseReturnStmt();
        case COMMENT: {
            auto n = make_unique<ASTNode>("Comment", cur.lexeme);
            advance();
            return n;
        }
        default:
            return parseImportStmt();
        }
    }
    unique_ptr<ASTNode> parseImportStmt() {
        expect(KEYWORD_IMPORT, "'import'");
        auto node = make_unique<ASTNode>("ImportStmt");

        if (cur.type != IDENTIFIER && cur.type != KEYWORD_UNIVERSE)
            error("Expected module name after 'import'");

        node->add_child(make_unique<ASTNode>("Module", cur.lexeme));
        advance();

        if (cur.type == SEMICOLON)
            advance();

        return node;
    }
    //int x;
    unique_ptr<ASTNode> parseDeclaration() {
        auto node = make_unique<ASTNode>("Declaration");
        node->add_child(make_unique<ASTNode>("Type", cur.lexeme));
        advance();
        if (cur.type != IDENTIFIER) error("Expected identifier in declaration");
        node->add_child(make_unique<ASTNode>("Identifier", cur.lexeme));
        advance();
        if (accept(ASSIGN)) {
            node->add_child(parseExpr());
        }
        expect(SEMICOLON, "';'");
        return node;
    }

    unique_ptr<ASTNode> parseDeclarationNoSemi() {
        auto node = make_unique<ASTNode>("Declaration");
        node->add_child(make_unique<ASTNode>("Type", cur.lexeme));
        advance();
        if (cur.type != IDENTIFIER) error("Expected identifier in declaration (for-loop)");
        node->add_child(make_unique<ASTNode>("Identifier", cur.lexeme));
        advance();
        if (accept(ASSIGN)) node->add_child(parseExpr());
        return node;
    }

    unique_ptr<ASTNode> parseAssignment() {
        auto node = make_unique<ASTNode>("Assignment");

        node->add_child(make_unique<ASTNode>("Identifier", cur.lexeme));
        advance();

        if (cur.type == INCREMENT || cur.type == DECREMENT) {
            node->add_child(make_unique<ASTNode>("Operator", cur.lexeme));
            advance();
            expect(SEMICOLON, "';' after increment/decrement");
            return node;
        }

        if (cur.type == ASSIGN) {
            advance();
            node->add_child(parseExpr());
            expect(SEMICOLON, "';' after assignment");
            return node;
        }

        error("Expected '=', '++', or '--' after identifier");
        return nullptr;
    }

    unique_ptr<ASTNode> parseAssignmentNoSemi() {
        auto node = make_unique<ASTNode>("Assignment");
        if (cur.type != IDENTIFIER) error("Expected identifier in assignment (for-loop)");
        node->add_child(make_unique<ASTNode>("Identifier", cur.lexeme));
        advance();
        expect(ASSIGN, "'='");
        node->add_child(parseExpr());
        return node;
    }

    unique_ptr<ASTNode> parseIfStmt() {
        expect(KEYWORD_CONTROL, "'control'");
        expect(LEFT_PAREN, "'('");
        auto node = make_unique<ASTNode>("IfStmt");
        node->add_child(parseCondition());
        expect(RIGHT_PAREN, "')'");
        expect(LEFT_BRACE, "'{'");
        node->add_child(parseStatementList());
        expect(RIGHT_BRACE, "'}'");

        if (accept(KEYWORD_ALTERNATIVE)) {
            expect(LEFT_BRACE, "'{' after alternative");
            auto alt = make_unique<ASTNode>("Alternative");
            alt->add_child(parseStatementList());
            expect(RIGHT_BRACE, "'}' after alternative");
            node->add_child(move(alt));
        }
        return node;
    }

    unique_ptr<ASTNode> parseWhileStmt() {
        expect(KEYWORD_JOURNEY, "'journey'");
        expect(LEFT_PAREN, "'('");
        auto node = make_unique<ASTNode>("WhileStmt");
        node->add_child(parseCondition());
        expect(RIGHT_PAREN, "')'");
        expect(LEFT_BRACE, "'{'");
        node->add_child(parseStatementList());
        expect(RIGHT_BRACE, "'}'");
        return node;
    }

    unique_ptr<ASTNode> parseForStmt() {
        expect(KEYWORD_EXPLORE, "'explore'");
        expect(LEFT_PAREN, "'('");
        auto node = make_unique<ASTNode>("ForStmt");
        if (!(cur.type == DATATYPE_MERCURY || cur.type == DATATYPE_VENUS || cur.type == DATATYPE_EARTH
            || cur.type == DATATYPE_MARS || cur.type == DATATYPE_JUPITER || cur.type == DATATYPE_SATURN
            || cur.type == DATATYPE_URANUS)) {
            error("Expected declaration type in for-loop init");
        }
        node->add_child(parseDeclarationNoSemi());
        expect(SEMICOLON, "';'");
        node->add_child(parseCondition());
        expect(SEMICOLON, "';'");
        node->add_child(parseAssignmentNoSemi());
        expect(RIGHT_PAREN, "')'");
        expect(LEFT_BRACE, "'{'");
        node->add_child(parseStatementList());
        expect(RIGHT_BRACE, "'}'");
        return node;
    }

    unique_ptr<ASTNode> parseFunctionStmt() {
        expect(KEYWORD_MISSION, "'mission'");
        auto node = make_unique<ASTNode>("FunctionStmt");

        if (cur.type != IDENTIFIER && cur.type != KEYWORD_LIFTOFF)
            error("Expected function name after 'mission'");
        node->add_child(make_unique<ASTNode>("Name", cur.lexeme));
        advance();


        expect(LEFT_PAREN, "'(' after function name");
        node->add_child(parseParameterList());
        expect(RIGHT_PAREN, "')' after parameters");


        expect(LEFT_BRACE, "'{' to start function body");
        node->add_child(parseStatementList());
        expect(RIGHT_BRACE, "'}' to end function body");

        return node;
    }

    unique_ptr<ASTNode> parseParameterList() {
        auto node = make_unique<ASTNode>("ParameterList");
        if (cur.type == RIGHT_PAREN) return node;
        while (cur.type == DATATYPE_MERCURY || cur.type == DATATYPE_VENUS || cur.type == DATATYPE_EARTH
            || cur.type == DATATYPE_MARS || cur.type == DATATYPE_JUPITER || cur.type == DATATYPE_SATURN
            || cur.type == DATATYPE_URANUS) {
            auto p = make_unique<ASTNode>("Parameter");
            p->add_child(make_unique<ASTNode>("Type", cur.lexeme));
            advance();
            if (cur.type != IDENTIFIER) error("Expected identifier in parameter");
            p->add_child(make_unique<ASTNode>("Identifier", cur.lexeme));
            advance();
            node->add_child(move(p));
            if (!accept(COMMA)) break;
        }
        return node;
    }

    unique_ptr<ASTNode> parseReturnStmt() {
        expect(KEYWORD_LAND, "'land'");
        auto node = make_unique<ASTNode>("ReturnStmt");
        if (cur.type != SEMICOLON) {
            node->add_child(parseExpr());
        }
        expect(SEMICOLON, "';'");
        return node;
    }

    unique_ptr<ASTNode> parseCondition() {
        auto node = make_unique<ASTNode>("Condition");
        node->add_child(parseExpr());
        return node;
    }


    unique_ptr<ASTNode> parseExpr() {
        return parseLogicalOr();
    }

    unique_ptr<ASTNode> parseLogicalOr() {
        auto node = parseLogicalAnd();
        while (cur.type == LOGICAL_OR) {
            string op = cur.lexeme;
            advance();
            auto right = parseLogicalAnd();
            auto parent = make_unique<ASTNode>("LogicalOr", op);
            parent->add_child(move(node));
            parent->add_child(move(right));
            node = move(parent);
        }
        return node;
    }

    unique_ptr<ASTNode> parseLogicalAnd() {
        auto node = parseEquality();
        while (cur.type == LOGICAL_AND) {
            string op = cur.lexeme;
            advance();
            auto right = parseEquality();
            auto parent = make_unique<ASTNode>("LogicalAnd", op);
            parent->add_child(move(node));
            parent->add_child(move(right));
            node = move(parent);
        }
        return node;
    }

    unique_ptr<ASTNode> parseEquality() {
        auto node = parseRelational();
        while (cur.type == IS_EQUAL || cur.type == NOT_EQUAL) {
            string op = cur.lexeme;
            advance();
            auto right = parseRelational();
            auto parent = make_unique<ASTNode>("Equality", op);
            parent->add_child(move(node));
            parent->add_child(move(right));
            node = move(parent);
        }
        return node;
    }

    unique_ptr<ASTNode> parseRelational() {
        auto node = parseAdditive();
        while (cur.type == LESS_THAN || cur.type == GREATER_THAN ||
            cur.type == LTE || cur.type == GTE) {
            string op = cur.lexeme;
            advance();
            auto right = parseAdditive();
            auto parent = make_unique<ASTNode>("Relational", op);
            parent->add_child(move(node));
            parent->add_child(move(right));
            node = move(parent);
        }
        return node;
    }

    unique_ptr<ASTNode> parseAdditive() {
        auto node = parseTerm();
        while (cur.type == PLUS || cur.type == MINUS) {
            string op = cur.lexeme;
            advance();
            auto right = parseTerm();
            auto parent = make_unique<ASTNode>("Additive", op);
            parent->add_child(move(node));
            parent->add_child(move(right));
            node = move(parent);
        }
        return node;
    }

    unique_ptr<ASTNode> parseTerm() {
        auto node = make_unique<ASTNode>("Term");
        node->add_child(parseFactor());
        while (cur.type == ASTERISK || cur.type == DIVIDE || cur.type == MODULO) {
            node->add_child(make_unique<ASTNode>("BinOp", cur.lexeme));
            advance();
            node->add_child(parseFactor());
        }
        return node;
    }

    unique_ptr<ASTNode> parseFactor() {
        auto node = make_unique<ASTNode>("Factor");

        if (cur.type == INT_LITERAL || cur.type == FLOAT_LITERAL) {
            node->add_child(make_unique<ASTNode>("Number", cur.lexeme));
            advance();
        }
        else if (cur.type == STRING_LITERAL) {
            node->add_child(make_unique<ASTNode>("String", cur.lexeme));
            advance();
        }
        else if (cur.type == IDENTIFIER) {
            string id = cur.lexeme;
            advance();

            if (cur.type == LEFT_PAREN) {
                advance();
                auto callNode = make_unique<ASTNode>("FunctionCall", id);
                if (cur.type != RIGHT_PAREN) {
                    callNode->add_child(parseExpr());
                    while (cur.type == COMMA) {
                        advance();
                        callNode->add_child(parseExpr());
                    }
                }
                expect(RIGHT_PAREN, "')' after function call");
                node = move(callNode);
            }
            else {
                node->add_child(make_unique<ASTNode>("Identifier", id));
            }
        }
        else if (cur.type == LEFT_PAREN) {
            advance();
            node->add_child(parseExpr());
            expect(RIGHT_PAREN, "')'");
        }
        else {
            error("Expected number, string, identifier, or '(' in factor");
        }
        return node;
    }

    unique_ptr<ASTNode> parseOutputStmt() {
        expect(KEYWORD_TRANSMIT, "'transmit'");
        auto node = make_unique<ASTNode>("OutputStmt");
        node->add_child(parseExpr());
        expect(SEMICOLON, "';'");
        return node;
    }

    unique_ptr<ASTNode> parseInputStmt() {
        expect(KEYWORD_RECEIVE, "'receive'");
        auto node = make_unique<ASTNode>("InputStmt");
        if (cur.type != IDENTIFIER) error("Expected identifier after 'receive'");
        node->add_child(make_unique<ASTNode>("Identifier", cur.lexeme));
        advance();
        expect(SEMICOLON, "';'");
        return node;
    }

public:
    static void printAST(const ASTNode* node, int indent = 0) {
        string pad(indent * 2, ' ');
        cout << pad << node->type;
        if (!node->value.empty()) cout << " : " << node->value;
        cout << "\n";
        for (auto& c : node->children) printAST(c.get(), indent + 1);
    }
};

int main(int argc, char** argv) {
    string filename;
    if (argc >= 2) filename = argv[1];
    else {
        cout << "Enter Name of input File: ";
        cin >> filename;
    }

    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".uni") {
        cout << "Error: Invalid file type. Please use a .uni file only.\n";
        return 1;
    }

    Parser P(filename);
    auto ast = P.parseProgram();
    cout << "Parse successful. AST:\n";
    Parser::printAST(ast.get());
    return 0;
}

// this is main for testing only the Scanner

//int main() { 
//    string Filename; 
//    cout << "Enter Name of input File: "; 
//    cin >> Filename; 
// 
//    if (Filename.size() < 4 || Filename.substr(Filename.size() - 4) != ".uni") { 
//        cout << "Error: Invalid file type. Please use a .uni file only.\n"; 
//        return 1; 
//    } 
// 
//    Scanner SC(Filename); 
// 
//    if (!SC.is_open()) { 
//        cout << "Error: Cannot open file '" << Filename << "'" << endl; 
//        return 1; 
//    } 
// 
//    SC.display_tokens(); 
//    return 0; 
//}