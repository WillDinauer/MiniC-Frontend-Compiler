/*
 * AST for CS57 -- C++ version
 *
 * 19 Apr 2022  bjk  Creation.
 * 22 Apr 2022  jpb  v1 release for class.
 */

#pragma once

#include <string>
#include <vector>

// Relational operators
typedef enum {
    LT,  // <
    GT,  // >
    LEQ, // <=
    GEQ, // >=
    EQ,  // ==
    NEQ, // !=
} r_op_t;
std::string stringify_r_op_t(r_op_t r_op);

// Binary operators
typedef enum {
    ADD, // +
    SUB, // -
    DIV, // /
    MUL, // *
} b_op_t;
std::string stringify_b_op_t(b_op_t b_op);

// Unary operators
typedef enum {
    POS, // +
    NEG, // -
} u_op_t;
std::string stringify_u_op_t(u_op_t u_op);

// The types in miniC
typedef enum { FLOAT_T, INT_T, VOID_T, BOOL_T } data_type_t;
std::string stringify_data_type_t(data_type_t data_type);

// Abstract struct from which all node structes in the AST derive
struct ASTNode {
    virtual ~ASTNode(){};
    virtual void print(int indentation_level = 0) const = 0;
};

// Abstract struct from which all statements derive.
// A statement is (informally) anything that could sit on its own in a block.
struct ASTStmtNode : public ASTNode {};

// Abstract struct from which variable and function declarations derive.
struct ASTDeclNode : public ASTStmtNode {};

// Abstract struct from which all expressions derive.
// An expression is a term, a function call, or expressions combined with
// appropriate operators.
struct ASTExprNode : public ASTStmtNode {};

// An assignment (eg. "a = 1;")
struct ASTAsgnNode : public ASTStmtNode {
    // The left-hand side of the assignment
    std::string *lhs;
    // The right-hand side of the assignment
    ASTExprNode *rhs;

    ASTAsgnNode(std::string *lhs, ASTExprNode *rhs);
    ~ASTAsgnNode();
    void print(int indentation_level = 0) const;
};

// A block (eg. "{ int a; a = 10 + b; exit(1); return will_never_get_here; }")
struct ASTBlockNode : public ASTStmtNode {
    // The statements that make up the block
    std::vector<ASTStmtNode *> *stmt_list;

    ASTBlockNode(std::vector<ASTStmtNode *> *stmt_list = nullptr);
    ~ASTBlockNode();
    void print(int indentation_level = 0) const;
};

// A function definition (eg. "int the_identity_fn(int n) { return n; }")
struct ASTFuncDefNode : public ASTNode {
    // The return type of the function
    data_type_t return_type;
    // The name of the function
    std::string *name;
    // The parameter type of the function
    data_type_t param_type;
    // The parameter name of the function
    std::string *param_name; // Pass in nullptr if there's no param name. Will
                             // get set to a new ""
    // The function body
    ASTBlockNode *body;

    ASTFuncDefNode(data_type_t return_type, std::string *name, data_type_t param_type,
                   std::string *param_name, ASTBlockNode *body);
    ~ASTFuncDefNode();
    void print(int indentation_level = 0) const;
};

// The program. The root of the AST.
struct ASTRootNode : public ASTNode {
    // List of function and variable declarations.
    // Must be at the top of the file.
    std::vector<ASTDeclNode *> *decls;
    // List of variable assignments for global variables.
    // Must be after declarations and before functions in the file.
    std::vector<ASTAsgnNode *> *asgns;
    // List of function definitions.
    // Must be at the end of the file.
    std::vector<ASTFuncDefNode *> *funcs;

    ASTRootNode(std::vector<ASTDeclNode *> *decls, std::vector<ASTAsgnNode *> *asgns,
                std::vector<ASTFuncDefNode *> *funcs);
    ~ASTRootNode();
    void print(int indentation_level = 0) const;
};

// A variable declaration. (eg. "int a;")
struct ASTVarDeclNode : public ASTDeclNode {
    // Whether the variable is extern
    bool is_extern;
    // The type of the variable
    data_type_t data_type;
    // The name of the variable
    std::string *name;

    ASTVarDeclNode(bool is_extern, data_type_t data_type, std::string *name);
    ~ASTVarDeclNode();
    void print(int indentation_level = 0) const;
};

struct ASTFuncDeclNode : public ASTDeclNode {
    // Whether the function is extern
    bool is_extern = is_extern;
    // The return type of the function
    data_type_t return_type;
    // The name of the function
    std::string *name;
    // The type of the parameter to the function
    data_type_t param_type;
    // Note that parameter names don't matter in C function declarations because
    // all arguments are positional. Thus, we don't store that info in the AST.

    ASTFuncDeclNode(bool is_extern, data_type_t return_type, std::string *name,
                    data_type_t param_type);
    ~ASTFuncDeclNode();
    void print(int indentation_level = 0) const;
};

// A variable term (eg. the "a" in "b = 10 + a * 2.1;")
struct ASTVarNode : public ASTExprNode {
    // The variable's name
    std::string *name;
    // Note that we don't know the variable's type here.
    // We'll get to that when we decorate the AST.

    ASTVarNode(std::string *name);
    ~ASTVarNode();
    void print(int indentation_level = 0) const;
};

// An integer constant term (eg. the "10" in "b = 10 + a * 2.1;")
struct ASTIntLiteralNode : public ASTExprNode {
    // The constant's value
    int value;

    ASTIntLiteralNode(int value);
    ~ASTIntLiteralNode() = default;
    void print(int indentation_level = 0) const;
};

// A floating point constant term (eg. the "2.1" in "b = 10 + a * 2.1;")
struct ASTFloatLiteralNode : public ASTExprNode {
    // The constant's value
    float value;

    ASTFloatLiteralNode(float value);
    ~ASTFloatLiteralNode() = default;
    void print(int indentation_level = 0) const;
};

// A boolean constant term (eg. "false")
struct ASTBoolLiteralNode : public ASTExprNode {
    // The constant's value
    bool value;

    ASTBoolLiteralNode(bool value);
    ~ASTBoolLiteralNode() = default;
    void print(int indentation_level = 0) const;
};

// A relational expression (eg. "10 > 2")
struct ASTRExprNode : public ASTExprNode {
    // The left-hand side of the relational operator
    ASTExprNode *lhs;
    // The right-hand side of the relational operator
    ASTExprNode *rhs;
    // The relational operator
    r_op_t op;

    ASTRExprNode(ASTExprNode *lhs, ASTExprNode *rhs, r_op_t op);
    ~ASTRExprNode();
    void print(int indentation_level = 0) const;
};

struct ASTBExprNode : public ASTExprNode {
    // The left-hand side of the binary operator
    ASTExprNode *lhs;
    // The right-hand side of the binary operator
    ASTExprNode *rhs;
    // The binary operator
    b_op_t op;

    ASTBExprNode(ASTExprNode *lhs, ASTExprNode *rhs, b_op_t op);
    ~ASTBExprNode();
    void print(int indentation_level = 0) const;
};

struct ASTUExprNode : public ASTExprNode {
    // The expression to which the unary operation applies
    ASTExprNode *expr;
    // The unary operator
    u_op_t op;

    ASTUExprNode(ASTExprNode *expr, u_op_t op);
    ~ASTUExprNode();
    void print(int indentation_level = 0) const;
};

// A function call (eg. "set_coolness_factor(100);")
struct ASTCallNode : public ASTExprNode {
    // The name of the function being called
    std::string *name;
    // The parameter to the function
    ASTExprNode *param; // Could be nullptr

    ASTCallNode(std::string *name, ASTExprNode *param = nullptr);
    ~ASTCallNode();
    void print(int indentation_level = 0) const;
};

// A return statement (eg. "return 10;")
struct ASTRetNode : public ASTStmtNode {
    // The thing being returned
    ASTExprNode *expr; // Could be nullptr

    ASTRetNode(ASTExprNode *expr);
    ~ASTRetNode();
    void print(int indentation_level = 0) const;
};

// A while statement (eg. "while (1 == 1) { 1; }")
struct ASTWhileNode : public ASTStmtNode {
    // The loop condition
    ASTExprNode *cond;
    // The loop body
    ASTBlockNode *body;

    ASTWhileNode(ASTExprNode *cond, ASTBlockNode *body);
    ~ASTWhileNode();
    void print(int indentation_level = 0) const;
};

struct ASTIfNode : public ASTStmtNode {
    // The if condition
    ASTExprNode *cond;
    // The if body
    ASTBlockNode *if_body;
    // The else body
    ASTBlockNode *else_body; // Won't be nullptr because we'll put an empty
                             // block if we need to

    ASTIfNode(ASTExprNode *cond, ASTBlockNode *if_body, ASTBlockNode *else_body = nullptr);
    ~ASTIfNode();
    void print(int indentation_level = 0) const;
};

// An empty statement (eg. ";")
// (This could just be ASTStmtNode, but I'd rather have that be an abstract
// struct)
struct ASTEmptyStmtNode : public ASTStmtNode {
    ASTEmptyStmtNode() = default;
    ~ASTEmptyStmtNode() = default;

    void print(int indentation_level = 0) const;
};
