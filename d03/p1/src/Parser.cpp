#include <iostream>
#include "Parser.h"

Parser::Parser(std::string src): src{src}
{
    pos = 0;
    current = src[pos];
}

bool Parser::is_letter(char c)
{
    return ((c > 'a' && c < 'z') || (c > 'A' && c < 'Z'));
}

bool Parser::is_number(char c)
{
    return (c >= '0' && c <= '9');
}

int Parser::parse_num()
{
    int r_pos = pos;
    while (is_number(src[r_pos])) {
        r_pos++;
    }

    int pos_diff = r_pos - pos;

    if (pos_diff > 3) return 0;

    std::string s_num = src.substr(pos, pos_diff);
    this->next(pos_diff);

    return std::stoi(s_num, nullptr, 10);
}

void Parser::next()
{
    pos++;
    current = src[pos];
}

void Parser::next(int count)
{
    int temp_pos = pos + count;
    if (temp_pos > src.size()) {
        pos = src.size() - 1;
    } else {
        pos = temp_pos;
    }
    current = src[pos];
}

std::vector<Instruction*> Parser::parse()
{
    std::vector<Instruction*> instructions{};

    while (pos < src.size()) {
        if (is_letter(current)) {
            std::string tok = src.substr(pos, TOKENTABLE[TokenType::mul].size());
            if (tok != TOKENTABLE[TokenType::mul]) {
                this->next();
                continue;
            }
            this->next(TOKENTABLE[TokenType::mul].size());

            if (std::string(1, current) != TOKENTABLE[TokenType::l_paren]) {
                continue;
            }
            this->next();

            int a = this->parse_num();

            if (std::string(1, current) != TOKENTABLE[TokenType::comma]) {
                continue;
            }
            this->next();

            int b = this->parse_num();

            if (std::string(1, current) != TOKENTABLE[TokenType::r_paren]) {
                continue;
            }
            this->next();

            instructions.push_back(new Mul(a, b));
        } else {
            this->next();
        }
    }

    return instructions;
}

Instruction::Instruction() {}
Instruction::~Instruction() {}
Mul::Mul(int a, int b) {
    operands[0] = a;
    operands[1] = b;
}

int Mul::execute() const
{
    return operands[0] * operands[1];
}

void Mul::print() const
{
    std::cout << "mul(" << operands[0] << "," << operands[1] << ")\n";
}
