#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "iostream"
#define N 55
#define FileOpenName "E://test.txt"
#define FileWriteName "E://test11.txt"
#define Ture 1
//#define Filename "E://test.txt"
using namespace std;

typedef struct KeyType{
    char keyname[256];      // 保存单词自身值
    int value;        // 种类编码整数值
}KeyType;

typedef struct Word{
    KeyType data;
    struct Word *next;
}Word;

enum ERROR {error_program, error_ExtDefList, error_ExtDef, error_ExtVarDef, error_TypeName, error_ExtVarList, error_VarName,
            error_FuncDef, error_FuncName, error_FuncStruct, error_Formal_Var_List, error_Formal_Var, error_ComplexStatement,
            error_LocalVarDefList, error_LocalVarDef, error_Statement_List, error_Statement, error_if_else_statement,
            error_while_statement, error_for_statement, error_return_statement, error_break_statement}; // 错误标记
/* --------------------------------------------------------
 * ---------------------结点域结构定义-----------------------
 * -------------------------------------------------------*/

//1.<程序>的结点 program
typedef struct AST_Node_program{
    struct AST_Node_ExtDefList *child_ExtDefList;  // 指向外部定义序列的子树
}AST_Node_program;

//2.<外部定义序列>的结点 ExtDefList
typedef struct AST_Node_ExtDefList{
    void *child_ExtDef;      // 指向外部定义的子树
    struct AST_Node_ExtDefList *child_ExtDefList;  // 指向外部定义序列的子树
}AST_Node_ExtDefList;

//3.<外部定义>的结点 ExtDef
typedef struct AST_Node_ExtDef{
    struct AST_Node_ExtVarDef *child_ExtVarDef;  //指向外部变量定义的子树
    struct AST_Node_FuncDef *child_FuncDef;  //指向函数定义的子树
}AST_Node_ExtDef;

//4.<外部变量定义>的结点 ExtVarDef
typedef struct AST_Node_ExtVarDef{
    struct AST_Node_TypeName *child_TypeName;  //指向类型名的子树
    struct AST_Node_ExtVarList *child_ExtVarList;  //指向外部变量序列的子树
}AST_Node_ExtVarDef;

//<类型名>的结点 TypeName (int char float)
typedef struct AST_Node_TypeName{
    char Type_Name[256];
}AST_Node_TypeName;

//<外部变量序列>的结点 ExtVarList
typedef struct AST_Node_ExtVarList{
    struct AST_Node_VarName *child_VarName;  // 指向变量名的子树
    struct AST_Node_ExtVarList *child_ExtVarList;  // 指向外部变量序列的子树
}AST_Node_ExtVarList;

//<变量名>的结点 VarName (i, j)
typedef struct AST_Node_VarName{
    char Var_Name[256];
}AST_Node_VarName;

//5.<函数定义>的结点 FuncDef
typedef struct AST_Node_FuncDef{
    struct AST_Node_TypeName *child_TypeName;   //指向类型名的子树
    struct AST_Node_FuncName *child_FuncName;   //指向函数名的子树
    struct AST_Node_FuncStruct *child_FuncStruct;  //指向函数体的子树
}AST_Node_FuncDef;

//<函数名>的结点 FuncName  (fun, main)
typedef struct AST_Node_FuncName{
    char Func_Name[256];
    struct AST_Node_Formal_Var_List *child_Formal_Var_List;  //指向形参序列的子树
}AST_Node_FuncName;

//<函数体>的结点 FuncStruct  指向复合语句
typedef struct AST_Node_FuncStruct{
    struct AST_Node_ComplexStatement *child_ComplexStatement;  //指向复合语句的子树
}AST_Node_FuncStruct;

//<形参序列>的结点  Formal_Var_List
typedef struct AST_Node_Formal_Var_List{
    struct AST_Node_Formal_Var *child_Formal_Var;  //指向形参的子树
    struct AST_Node_Formal_Var_List *child_Formal_Var_List;  //指向形参序列的子树
}AST_Node_Formal_Var_List;

//<形参>的结点 Formal_Var
typedef struct AST_Node_Formal_Var{
    struct AST_Node_TypeName *child_TypeName;  //指向类型名的子树
    struct AST_Node_VarName *child_VarName;  //指向变量名的子树
}AST_Node_Formal_Var;

//6.<复合语句>的结点 ComplexStatement
typedef struct AST_Node_ComplexStatement{
    struct AST_Node_LocalVarDef_List *child_LocalVarDef_List;  //指向局部变量定义序列的子树
    struct AST_Node_Statement_List *child_Statement_List;  //指向语句序列的子树
}AST_Node_ComplexStatement;

//<局部变量定义序列>的结点 LocalVarDef_List
typedef struct AST_Node_LocalVarDef_List{
    struct AST_Node_LocalVarDef *child_LocalVarDef;  //指向局部变量定义的子树
}AST_Node_LocalVarDef_List;

//<局部变量定义>的结点 LocalVarDef
typedef struct AST_Node_LocalVarDef{
    struct AST_Node_TypeName *child_TypeName;  //指向类型名的子树
    struct AST_Node_LocalVarName_List *child_LocalVarName_List;  //指向局部变量名的子树
}AST_Node_LocalVarDef;

//<局部变量名序列>
typedef struct AST_Node_LocalVarName_List{
    struct AST_Node_VarName *child_VarName;
    struct AST_Node_LocalVarName_List *child_LocalVarName_List;
}AST_Node_LocalVarName_List;

//7.<语句序列>的子树   Statement_List
typedef struct AST_Node_Statement_List{
    struct AST_Node_Statement *child_Statement;  //指向语句的子树
    struct AST_Node_Statement_List *child_Statement_List;  //指向语句序列的子树
}AST_Node_Statement_List;

//<语句>的结点 Statement
typedef struct AST_Node_Statement{
    struct if_else_statement *child_if_else_statement;  //指向if语句的子树
    struct while_statement *child_while_statement;  //指向while语句的子树
    struct for_statement *child_for_statement;  //指向for语句的子树
    struct break_statement *child_break_statement;  //指向break语句的子树
    struct return_statement *child_return_statement;  //指向return语句的子树
    struct AST_Node_ComplexStatement *child_ComplexStatement;  //指向复合语句的子树
}AST_Node_Statement;

//if 语句的结点
typedef struct if_else_statement{
    struct AST_Node_expression *child_condition; //指向条件字句的子树
    struct AST_Node_expression *child_if_expression;  //指向if赋值语句的子树
    struct AST_Node_expression *child_else_expression;  //指向else赋值语句的子树
}if_else_statement;

//while 语句的结点
typedef struct while_statement{
   struct AST_Node_expression *child_condition;  //指向条件字句的子树
   struct AST_Node_expression *child_expression;  //指向执行语句的子树
}while_statement;

//for 语句的结点
typedef struct for_statement{
    struct AST_Node_expression *child_condition;  //指向条件字句的子树
    struct AST_Node_expression *child_expression;  //指向指向语句的子树
}for_statement;

//return 语句的结点
typedef struct return_statement{
    struct AST_Node_VarName *return_var;
}return_statement;

//<表达式>语句的结点 AST_Node_expression
typedef struct AST_Node_expression{
    struct op *child_op;
}AST_Node_expression;

//<操作数>的结点 opn
typedef struct opn{
    char opn_var[256];
}opn;

//<操作符>的结点 on
typedef struct op{
    char L_opn[20];
    char R_opn[20];
    char on_var[20];
}op;