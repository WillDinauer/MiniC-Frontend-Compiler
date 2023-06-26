/*
 * AST for CS57 -- C++ version
 *
 * 19 Apr 2022  bjk  Creation.
 * 22 Apr 2022  jpb  v1 release for class.
 */
#include "ast.hpp"

#include <iostream>
#include <string>
#include <vector>

#define INDENT_STR std::string("|   ")

/*
    Some explanations of design decisions:
    Why use iostream over stdio.h?
        Type safety.
    Why use C enums instead of a class?
        Consistency with the C API.
*/

std::string repeat(std::string str, int times) {
    std::string result;
    for (int i = 0; i < times; i++) {
        result += str;
    }
    return result;
}

std::string stringify_r_op_t(r_op_t r_op) {
    switch (r_op) {
    case LT:
        return "LT";
    case GT:
        return "GT";
    case LEQ:
        return "LEQ";
    case GEQ:
        return "GEQ";
    case EQ:
        return "EQ";
    case NEQ:
        return "NEQ";
    }

    // We should never get here.
    exit(1);
}

std::string stringify_b_op_t(b_op_t b_op) {
    switch (b_op) {
    case ADD:
        return "ADD";
    case SUB:
        return "SUB";
    case DIV:
        return "DIV";
    case MUL:
        return "MUL";
    }

    // We should never get here.
    exit(1);
}

std::string stringify_u_op_t(u_op_t u_op) {
    switch (u_op) {
    case POS:
        return "POS";
    case NEG:
        return "NEG";
    }

    // We should never get here.
    exit(1);
}

std::string stringify_data_type_t(data_type_t data_type) {
    switch (data_type) {
    case FLOAT_T:
        return "FLOAT_T";
    case INT_T:
        return "INT_T";
    case VOID_T:
        return "VOID_T";
    case BOOL_T:
        return "BOOL_T";
    }

    // We should never get here.
    exit(1);
}

ASTAsgnNode::ASTAsgnNode(std::string *lhs, ASTExprNode *rhs) : lhs{lhs}, rhs{rhs} {}

ASTAsgnNode::~ASTAsgnNode() {
    delete lhs;
    delete rhs;
}

void ASTAsgnNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "Asgn. " << *lhs << " =" << std::endl;
    rhs->print(indentation_level + 1);
}

ASTBlockNode::ASTBlockNode(std::vector<ASTStmtNode *> *stmt_list) : stmt_list{stmt_list} {
    if (stmt_list == nullptr) {
        this->stmt_list = new std::vector<ASTStmtNode *>();
    }
}

ASTBlockNode::~ASTBlockNode() {
    for (ASTStmtNode *stmt : *stmt_list) {
        delete stmt;
    }
    delete stmt_list;
}

void ASTBlockNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "Block." << std::endl;
    if (stmt_list != nullptr) {
        for (ASTStmtNode *stmt : *stmt_list) {
            stmt->print(indentation_level + 1);
        }
    }
}

ASTFuncDefNode::ASTFuncDefNode(data_type_t return_type, std::string *name, data_type_t param_type,
                               std::string *param_name, ASTBlockNode *body)
    : return_type{return_type}, name{name}, param_type{param_type},
      param_name{param_name}, body{body} {
    if (param_name == nullptr) {
        this->param_name = new std::string();
    }
}

ASTFuncDefNode::~ASTFuncDefNode() {
    delete body;
    delete name;
    delete param_name;
}

void ASTFuncDefNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "FuncDef. "
              << stringify_data_type_t(return_type) << ' ' << *name << '('
              << stringify_data_type_t(param_type) << ' ' << *param_name << ')' << std::endl;
    body->print(indentation_level + 1);
}

ASTRootNode::ASTRootNode(std::vector<ASTDeclNode *> *decls, std::vector<ASTAsgnNode *> *asgns,
                         std::vector<ASTFuncDefNode *> *funcs)
    : decls{decls}, asgns{asgns}, funcs{funcs} {
    if (decls == nullptr) {
        this->decls = new std::vector<ASTDeclNode *>();
    }
    if (asgns == nullptr) {
        this->asgns = new std::vector<ASTAsgnNode *>();
    }
    if (funcs == nullptr) {
        this->funcs = new std::vector<ASTFuncDefNode *>();
    }
}

ASTRootNode::~ASTRootNode() {
    for (ASTDeclNode *decl : *decls) {
        delete decl;
    }
    delete decls;

    for (ASTAsgnNode *asgn : *asgns) {
        delete asgn;
    }
    delete asgns;

    for (ASTFuncDefNode *func : *funcs) {
        delete func;
    }
    delete funcs;
}

void ASTRootNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "Root." << std::endl;

    if (!decls->empty()) {
        std::cout << repeat(INDENT_STR, indentation_level) << "Declarations:" << std::endl;
        for (ASTDeclNode *decl : *decls) {
            decl->print(indentation_level + 1);
        }
    }

    if (!asgns->empty()) {
        std::cout << repeat(INDENT_STR, indentation_level) << "Assignments:" << std::endl;
        for (ASTAsgnNode *asgn : *asgns) {
            asgn->print(indentation_level + 1);
        }
    }

    if (!funcs->empty()) {
        std::cout << repeat(INDENT_STR, indentation_level) << "Functions:" << std::endl;
        for (ASTFuncDefNode *func : *funcs) {
            func->print(indentation_level + 1);
        }
    }
}

ASTVarDeclNode::ASTVarDeclNode(bool is_extern, data_type_t data_type, std::string *name)
    : is_extern{is_extern}, data_type{data_type}, name{name} {}

ASTVarDeclNode::~ASTVarDeclNode() { delete name; }

void ASTVarDeclNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "VarDecl. "
              << (is_extern ? "extern " : "") << stringify_data_type_t(data_type) << ' ' << *name
              << std::endl;
}

ASTFuncDeclNode::ASTFuncDeclNode(bool is_extern, data_type_t return_type, std::string *name,
                                 data_type_t param_type)
    : is_extern{is_extern}, return_type{return_type}, name{name}, param_type{param_type} {}

ASTFuncDeclNode::~ASTFuncDeclNode() { delete name; }

void ASTFuncDeclNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "FuncDecl. "
              << (is_extern ? "extern " : "") << stringify_data_type_t(return_type) << ' ' << *name
              << '(' << stringify_data_type_t(param_type) << ')' << std::endl;
}

ASTVarNode::ASTVarNode(std::string *name) : name{name} {}

ASTVarNode::~ASTVarNode() { delete name; }

void ASTVarNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "Var. " << *name << std::endl;
}

ASTIntLiteralNode::ASTIntLiteralNode(int value) : value{value} {}

void ASTIntLiteralNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "IntLiteral. " << value << std::endl;
}

ASTFloatLiteralNode::ASTFloatLiteralNode(float value) : value{value} {}

void ASTFloatLiteralNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "FloatLiteral. " << value << std::endl;
}

ASTBoolLiteralNode::ASTBoolLiteralNode(bool value) : value{value} {}

void ASTBoolLiteralNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "BoolLiteral. "
              << (value ? "True" : "False") << std::endl;
}

ASTRExprNode::ASTRExprNode(ASTExprNode *lhs, ASTExprNode *rhs, r_op_t op)
    : lhs{lhs}, rhs{rhs}, op{op} {}

ASTRExprNode::~ASTRExprNode() {
    delete lhs;
    delete rhs;
}

void ASTRExprNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "RExpr. " << stringify_r_op_t(op)
              << std::endl;
    std::cout << repeat(INDENT_STR, indentation_level) << "(RExpr) LHS =" << std::endl;
    lhs->print(indentation_level + 1);
    std::cout << repeat(INDENT_STR, indentation_level) << "(RExpr) RHS =" << std::endl;
    rhs->print(indentation_level + 1);
}

ASTBExprNode::ASTBExprNode(ASTExprNode *lhs, ASTExprNode *rhs, b_op_t op)
    : lhs{lhs}, rhs{rhs}, op{op} {}

ASTBExprNode::~ASTBExprNode() {
    delete lhs;
    delete rhs;
}

void ASTBExprNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "BExpr. " << stringify_b_op_t(op)
              << std::endl;
    std::cout << repeat(INDENT_STR, indentation_level) << "(BExpr) LHS =" << std::endl;
    lhs->print(indentation_level + 1);
    std::cout << repeat(INDENT_STR, indentation_level) << "(BExpr) RHS =" << std::endl;
    rhs->print(indentation_level + 1);
}

ASTUExprNode::ASTUExprNode(ASTExprNode *expr, u_op_t op) : expr{expr}, op{op} {}

ASTUExprNode::~ASTUExprNode() { delete expr; }

void ASTUExprNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "UExpr. " << stringify_u_op_t(op)
              << ", expression =" << std::endl;
    expr->print(indentation_level + 1);
}

ASTCallNode::ASTCallNode(std::string *name, ASTExprNode *param) : name{name}, param{param} {}

ASTCallNode::~ASTCallNode() {
    if (param != nullptr) {
        delete param;
    }
    delete name;
}

void ASTCallNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "Call. " << *name
              << (param != nullptr ? ", parameter =" : "") << std::endl;
    if (param != nullptr) {
        param->print(indentation_level + 1);
    }
}

ASTRetNode::ASTRetNode(ASTExprNode *expr) : expr{expr} {}

ASTRetNode::~ASTRetNode() {
    if (expr != nullptr) {
        delete expr;
    }
}

void ASTRetNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "Ret."
              << (expr != nullptr ? " expression =" : "") << std::endl;
    if (expr != nullptr) {
        expr->print(indentation_level + 1);
    }
}

ASTWhileNode::ASTWhileNode(ASTExprNode *cond, ASTBlockNode *body) : cond{cond}, body{body} {}

ASTWhileNode::~ASTWhileNode() {
    delete cond;
    delete body;
}

void ASTWhileNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "While." << std::endl;
    std::cout << repeat(INDENT_STR, indentation_level) << "(While) condition =" << std::endl;
    cond->print(indentation_level + 1);
    std::cout << repeat(INDENT_STR, indentation_level) << "(While) body =" << std::endl;
    body->print(indentation_level + 1);
}

ASTIfNode::ASTIfNode(ASTExprNode *cond, ASTBlockNode *if_body, ASTBlockNode *else_body)
    : cond{cond}, if_body{if_body}, else_body{else_body} {
    if (else_body == nullptr) {
        this->else_body = new ASTBlockNode();
    }
}

ASTIfNode::~ASTIfNode() {
    delete cond;
    delete if_body;
    delete else_body;
}

void ASTIfNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "If." << std::endl;
    std::cout << repeat(INDENT_STR, indentation_level) << "(If) condition =" << std::endl;
    cond->print(indentation_level + 1);
    std::cout << repeat(INDENT_STR, indentation_level) << "(If) if_body =" << std::endl;
    if_body->print(indentation_level + 1);
    std::cout << repeat(INDENT_STR, indentation_level) << "(If) else_body =" << std::endl;
    else_body->print(indentation_level + 1);
}

void ASTEmptyStmtNode::print(int indentation_level) const {
    std::cout << repeat(INDENT_STR, indentation_level) << "EmptyStmt." << std::endl;
}
