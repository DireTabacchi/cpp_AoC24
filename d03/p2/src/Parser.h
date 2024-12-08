#include <string>
#include <vector>

struct Symbol {
public:
    const std::string literal;
    const int len;
};

const Symbol SYMBOLTABLE[] = {
    Symbol { "mul", 3 }
};

enum TokenType {
    mul,
    l_paren,
    r_paren,
    comma,
    num,
    do_ins,
    dont_ins
};

const std::string TOKENTABLE[] = {
    "mul",
    "(",
    ")",
    ",",
    "NUM",
    "do",
    "don't"
};

class Instruction {
public:
    Instruction();
    virtual ~Instruction();
    virtual void print() const = 0;
    virtual int execute() const = 0;
};

class Mul : public Instruction {
    int operands[2];
public:
    Mul(int a, int b);
    virtual int execute() const override;
    virtual void print() const override;
};

class DoIns : public Instruction {
public:
    DoIns() {}
    virtual int execute() const override { return 0; }
    virtual void print() const override;
};

class DontIns : public Instruction {
    public:
        DontIns() {}
        virtual int execute() const override { return 0; }
        virtual void print() const override;
};

class Parser {
    std::string src;
    int pos;
    char current;
    bool enabled;

    bool is_letter(char c);
    bool is_number(char c);
    int parse_num();

    void next();
    void next(int count);

public:
    Parser(std::string src);
    std::vector<Instruction*> parse();
};
