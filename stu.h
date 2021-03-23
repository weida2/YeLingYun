char ch;
char buffer[1024]; //单词缓冲区
char init_array[256] = {'\0'};   //重新初始化数组
char token_text[100] = {'\0'};   //存放构成单词的字符串数组
char Token_Text[256] = {'\0'};   //保存变量名或者函数名
char tokentext;  //
char Type[256];   //保存类型说明符
int line_count = 0;   // 行数计时器
KeyType error;
int flag;
int count_Formal_Var_List = 0;
int count_ExtDefList = 0;
FILE *fp2;


KeyType Key[N] = {{"ID",       0},
                  {"INT",      1},
                  {"auto",     2},
                  {"break",    3},
                  {"case",     4},
                  {"char",     5},
                  {"const",    6},
                  {"continue", 7},
                  {"default",  8},
                  {"do",       9},
                  {"double",   10},
                  {"else",     11},
                  {"enum",     12},
                  {"extern",   13},
                  {"float",    14},
                  {"for",      15},
                  {"goto",     16},
                  {"if",       17},
                  {"int",      18},
                  {"long",     19},
                  {"register", 20},
                  {"return",   21},
                  {"short",    22},
                  {"signed",   23},
                  {"sizeof",   24},
                  {"static",   25},
                  {"struct",   26},
                  {"switch",   27},
                  {"typedef",  28},
                  {"union",    29},
                  {"unsigned", 30},
                  {"void",     31},
                  {"volatile", 32},
                  {"while",    33},
                  {"=",        34},
                  {"+",        35},
                  {"-",        36},
                  {"*",        37},
                  {"/",        38},
                  {"%",        39},
                  {"==",       40},
                  {">=",       41},
                  {"<=",       42},
                  {"&&",       43},
                  {"||",       44},
                  {";",        45},
                  {"(",        46},
                  {")",        47},
                  {"#",        48},
                  {">",        49},
                  {"<",        50},
                  {"{",        51},
                  {"}",        52},
                  {",",        53},
                  {"//",       54}};

KeyType Error_Type[N]{{"error_program",           0},
                      {"error_ExtDefList",        1},
                      {"error_ExtDef",            2},
                      {"error_ExtVarDef",         3},
                      {"error_TypeName",          4},
                      {"error_ExtVarList",        5},
                      {"error_VarName",           6},
                      {"error_FuncDef",           7},
                      {"error_FuncName",          8},
                      {"error_FuncStruct",        9},
                      {"error_Formal_Var_List",   10},
                      {"error_Formal_Var",        11},
                      {"error_ComplexStatement",  12},
                      {"error_LocalVarDefList",   13},
                      {"error_LocalVarDef",       14},
                      {"error_Statement_List",    15},
                      {"error_Statement",         16},
                      {"error_if_else_statement", 17},
                      {"error_while_statement",   18},
                      {"error_for_statement",     19},
                      {"error_return_statement",  20},
                      {"error_break_statement",   21}};
/* --------------------------------------------------------
 * ---------------------词法分析部分------------------------
 * -------------------------------------------------------*/

/*enum ERROR {error_program, error_ExtDefList, error_ExtDef, error_ExtVarDef, error_TypeName, error_ExtVarList, error_VarName,
            error_FuncDef, error_FuncName, error_FuncStruct, error_Formal_Var_List, error_Formal_Var, error_ComplexStatement,
            error_LocalVarDefList, error_LocalVarDef, error_Statement_List, error_Statement, error_if_else_statement,
            error_while_statement, error_for_statement, error_return_statement, error_break_statement}; // 错误标记 */

KeyType ReturnWord(); // 返回一个单词的种类码和自身值
void GetChar();        // 读取一个字符到ch中
void GetBC();  // 读取非空白字符到ch中
void CharConCat();      // 字符ch相连函数 保存在token_text中
void Retract();     // 将ch的字符重新输回到缓冲区中
void Type_Recognition(int type_value); // 单词识别函数打印函数
int IsLetter();   // 单词是否为字母
int IsDigit(); // 单词是否为数字
int Cmp();  // 将token_text的单词与键值表对比 返回保留字类型

void GetChar()
{
    int i = 0;
    if (strlen(buffer) > 0)
    {
        ch = buffer[i];
        for (; i < 256; i++)
            buffer[i] = buffer[i + 1];
    }
    else
        ch = '\0';
}

void GetBC()
{
    int i;
    while (strlen(buffer))
    {
        i = 0;
        ch = buffer[i];
        for (; i < 256; i++) buffer[i] = buffer[i + 1];
        if (ch != ' ' && ch != '\n' && ch != '\t') break;
    }
}

void CharConCat()
{
    char tail[2];
    tail[0] = ch;
    tail[1] = '\0';
    strcat(token_text, tail);
}

void Retract()
{
    if (ch != '\0')
    {
        buffer[256] = '\0';
        for (int i = 255; i > 0; i--)
            buffer[i] = buffer[i - 1];
        buffer[0] = ch;
    }
    ch = '\0';
}

int IsLetter()
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return 1;
    else
        return 0;
}

int IsDigit()
{
    if (ch >= '0' && ch <= '9')
        return 1;
    else
        return 0;
}

int Cmp()
{
    int i;
    for (i = 0; i < N; i++)
    {
        if (strcmp(token_text, Key[i].keyname) == 0)
            return Key[i].value;
    }
    return 0;
}

KeyType ReturnWord()
{
    strcpy(token_text, init_array); //初始化单词自身值为空
    int c;
    KeyType RW; // 要返回的单词
    GetBC();
    if (IsLetter()) //如果字符ch是字母
    {
        do{
            CharConCat();
            GetChar();
        } while (IsLetter() || IsDigit());
        Retract();
        c = Cmp();  //获取种类编码
        strcpy(RW.keyname, token_text);
        RW.value = c;
    }
    else if (IsDigit())  // 如果字符ch是数字
    {
        do{
            CharConCat();
            GetChar();
        } while (IsDigit());
        Retract();
        strcpy(RW.keyname, token_text);
        RW.value = 1;
    }
    else if (ch == '/'){
        CharConCat();
        GetChar();
        if (ch == '/')
            CharConCat();
        strcpy(RW.keyname, token_text);
        RW.value = Cmp();
    }
    else
    {
        CharConCat();
        strcpy(RW.keyname, token_text);
        RW.value = Cmp();
   //    if (RW.value == 0) return ERROR;
    }
    return RW;
}

int Type_Number;//用来确定 token_kind的值，这里的token_kind是keytype.value
void Type_Recognition(int type_value){
    //标识符
    if(type_value==0){
        Type_Number=0;
    }
    //整型变量
    if(type_value==1){
        Type_Number=1;
    }
    //C语言关键字
    if(type_value>=2&&type_value<=33){
        Type_Number=2;
    }
    //赋值号
    if(type_value==34){
        Type_Number=3;
    }
    //加号
    if(type_value==35){
        Type_Number=4;
    }
    //减号
    if(type_value==36){
        Type_Number=5;
    }
    //乘号
    if(type_value==37){
        Type_Number=6;
    }
    //除号
    if(type_value==38){
        Type_Number=7;
    }
    //求余号
    if(type_value==39){
        Type_Number=8;
    }
    //== 逻辑关系式
    if(type_value==40){
        Type_Number=9;
    }
    // >=
    if(type_value==41){
        Type_Number=10;
    }
    //<=
    if(type_value==42){
        Type_Number=11;
    }
    //&&
    if(type_value==43){
        Type_Number=12;
    }
    // 逻辑||
    if(type_value==44){
        Type_Number=13;
    }
    //分号
    if(type_value==45){
        Type_Number=14;
    }
    //左括号
    if(type_value==46){
        Type_Number=15;
    }
    //右括号
    if(type_value==47){
        Type_Number=16;
    }
    //预编译指令符
    if(type_value==48){
        Type_Number=17;
    }
    //大于号
    if(type_value==49){
        Type_Number=18;
    }
    //小于号
    if(type_value==50){
        Type_Number=19;
    }
    //正方括号
    if(type_value==51){
        Type_Number=20;
    }
    //反方括号
    if(type_value==52){
        Type_Number=21;
    }
    //逗号
    if(type_value==53){
        Type_Number=22;
    }
    //'//'结束符
    if(type_value==54){
        Type_Number=23;
    }


    switch (Type_Number)
    {
        case 0: printf("标识符\n");  break;
        case 1: printf("整形常量\n");  break;
        case 2: printf("关键字\n");  break;
        case 3: printf("赋值号\n");  break;
        case 4: printf("加号\n");  break;
        case 5: printf("减号\n");  break;
        case 6: printf("乘号\n");  break;
        case 7: printf("除号\n");  break;
        case 8: printf("求余号\n");  break;
        case 9: printf("逻辑等于\n");  break;
        case 10: printf("逻辑大于等于\n");  break;
        case 11: printf("逻辑小于等于\n");  break;
        case 12: printf("逻辑与\n");  break;
        case 13: printf("逻辑或\n");  break;
        case 14: printf("分号\n");  break;
        case 15: printf("左括号\n");  break;
        case 16: printf("右括号\n");  break;
        case 17: printf("预编译指令\n");  break;
        case 18: printf("大于号\n");  break;
        case 19: printf("小于号\n");  break;
        case 20: printf("正方括号\n");  break;
        case 21: printf("反方括号\n");  break;
        case 22: printf("逗号\n");  break;
        case 23: printf("注释符\n");  break;
        default: printf("错误！\n");   break;
    }

}
/* --------------------------------------------------------
 * ---------------------语法分析部分-------------------------
 * -------------------------------------------------------*/
KeyType w;
//声明函数名称
AST_Node_program *program();
AST_Node_ExtDefList *ExtDefList();
void *ExtDef();
AST_Node_ExtVarList *ExtVarList();
AST_Node_ExtVarDef *ExtVarDef();
AST_Node_FuncDef *FuncDef();
AST_Node_Formal_Var_List *Formal_Var_List();
AST_Node_FuncStruct *FuncStruct();
AST_Node_Formal_Var *Formal_Var();
AST_Node_ComplexStatement *ComplexStatement();
AST_Node_LocalVarDef_List *LocalVarDef_List();
AST_Node_LocalVarDef *LocalVarDef();
AST_Node_LocalVarName_List *LocalVarName_List();
AST_Node_Statement_List *Statement_List();
AST_Node_Statement *Statement();
AST_Node_expression *expression();

AST_Node_program *program()
{
    fp2 = fopen(FileWriteName, "w");
    w = ReturnWord(); // w = int
    AST_Node_program *root_program;
    root_program = (AST_Node_program *)malloc(sizeof (AST_Node_program));
    root_program->child_ExtDefList = ExtDefList();
    fclose(fp2);
    if (root_program->child_ExtDefList != NULL) return root_program;
    else{
        error = Error_Type[0];
        return root_program;
    }
}

AST_Node_ExtDefList *ExtDefList()  // 45是分号 47是右括号
{
    count_ExtDefList++;
    if (w.value == 54) return NULL;  // 如果w读到文件结束最后一个字符
    AST_Node_ExtDefList *root_ExtDefList;
    root_ExtDefList = (AST_Node_ExtDefList *)malloc(sizeof (AST_Node_ExtDefList));
    root_ExtDefList->child_ExtDef = ExtDef();
    if (root_ExtDefList->child_ExtDef == NULL) return NULL;
    root_ExtDefList->child_ExtDefList = ExtDefList();
    return root_ExtDefList;
}

void *ExtDef()
{
    if (!(w.value == 5 || w.value == 10 || w.value == 14 || w.value == 18))  //w不是类型关键字
    {
        error = Error_Type[2];
        return NULL;
    }
    strcpy(Type, w.keyname); //保存类型说明符
    w = ReturnWord();  // w = i
    if (w.value != 0) //如果w不是标识符报错
    {
        error = Error_Type[2];
        return NULL;
    }
    strcpy(Token_Text, w.keyname);  //保存第一个变量名或者函数名到Token_Text
    w = ReturnWord();  // w = ,
    if (w.value != 46) // 如果 w != (
    {
        printf("外部变量定义:\n");
        AST_Node_ExtVarDef *p;
        p = ExtVarDef();   // 调用外部变量定义子程序
        if (p) return p;
    }
    else
    {
        printf("函数定义:\n");
        AST_Node_FuncDef *q;
        q = FuncDef();  // 调用函数定义子程序
        if (q) return q;
    }
}

AST_Node_ExtVarList *ExtVarList()
{
    AST_Node_ExtVarList *root_ExtVarList;
    root_ExtVarList = (AST_Node_ExtVarList *)malloc(sizeof (AST_Node_ExtVarList));
    root_ExtVarList->child_VarName = (AST_Node_VarName *)malloc(sizeof (AST_Node_VarName));
    strcpy(root_ExtVarList->child_VarName->Var_Name, Token_Text);// 生成第一个外部变量名结点
    printf("\t\tID:\t%s\n", root_ExtVarList->child_VarName->Var_Name);
    fprintf(fp2, "%s", root_ExtVarList->child_VarName->Var_Name);
    strcpy(Token_Text, init_array);
  //  w = ReturnWord();  // w = ,
    if (w.value != 53 && w.value != 45)  // 如果 w != , && w != ; 报错
    {
        error = Error_Type[5];
        free(root_ExtVarList);
        return NULL;
    }
    if (w.value == 45)  //如果w为分号
    {
        fprintf(fp2, "%s\r\n", w.keyname);
        w = ReturnWord();
        return root_ExtVarList;
    }
    fprintf(fp2, "%s", w.keyname);  // w = ,
    w = ReturnWord();  // w = b
    if (w.value != 0)  //如果w不是标识符
    {
        error = Error_Type[5];
        free(root_ExtVarList);
        return NULL;
    }
    strcpy(Token_Text, w.keyname);  // 保存第二个变量名
    w = ReturnWord();
    root_ExtVarList->child_ExtVarList = (AST_Node_ExtVarList *)malloc(sizeof (AST_Node_ExtVarList));
    root_ExtVarList->child_ExtVarList = ExtVarList();
    return root_ExtVarList;
}

AST_Node_ExtVarDef *ExtVarDef()
{
    AST_Node_ExtVarDef *root_ExtVarDef;
    root_ExtVarDef = (AST_Node_ExtVarDef *)malloc(sizeof (AST_Node_ExtVarDef));
    root_ExtVarDef->child_TypeName = (AST_Node_TypeName *)malloc(sizeof (AST_Node_TypeName));
    strcpy(root_ExtVarDef->child_TypeName->Type_Name, Type);  //生成外部变量类型的结点，作为root的第一个孩子
    printf("\t类型:\t\t%s\n", root_ExtVarDef->child_TypeName->Type_Name);
    fprintf(fp2, "%s ", root_ExtVarDef->child_TypeName->Type_Name);
    strcpy(Type, init_array); //重新初始化Type类型名数组
    printf("\t变量名:\n");
    root_ExtVarDef->child_ExtVarList = ExtVarList();
    if (root_ExtVarDef->child_ExtVarList == NULL) return NULL;
    return root_ExtVarDef;
}

AST_Node_FuncDef *FuncDef()
{
    AST_Node_FuncDef *root_FuncDef;
    root_FuncDef = (AST_Node_FuncDef *)malloc(sizeof (AST_Node_FuncDef));
    root_FuncDef->child_TypeName = (AST_Node_TypeName *)malloc(sizeof (AST_Node_TypeName));
    strcpy(root_FuncDef->child_TypeName->Type_Name, Type); //生成返回值类型结点，作为root的第一个孩子
    printf("\t类型:\t\t%s\n", root_FuncDef->child_TypeName->Type_Name);
    fprintf(fp2, "%s ", root_FuncDef->child_TypeName->Type_Name);
    root_FuncDef->child_FuncName = (AST_Node_FuncName *) malloc(sizeof(AST_Node_FuncName));
    strcpy(root_FuncDef->child_FuncName->Func_Name, Token_Text);  //生成函数名的结点
    printf("\t函数名:\t\t%s\n", root_FuncDef->child_FuncName->Func_Name);
    fprintf(fp2, "%s", root_FuncDef->child_FuncName->Func_Name);
    strcpy(Type, init_array);
    strcpy(Token_Text, init_array);
    printf("\t函数形参:\n");
    fprintf(fp2, "%s", w.keyname);
    root_FuncDef->child_FuncName->child_Formal_Var_List = Formal_Var_List();  // w = (
    AST_Node_Formal_Var_List *p;
    p = root_FuncDef->child_FuncName->child_Formal_Var_List;
    for (int i = 0; i < count_Formal_Var_List; i++) {
        if (!p) return NULL;
        p = p->child_Formal_Var_List;
    }
    root_FuncDef->child_FuncStruct = FuncStruct();
    return root_FuncDef;
}

AST_Node_Formal_Var_List *Formal_Var_List()
{
    count_Formal_Var_List++;
    AST_Node_Formal_Var_List *root_Formal_Var_List;
    root_Formal_Var_List = (AST_Node_Formal_Var_List *)malloc(sizeof (AST_Node_Formal_Var_List));
    root_Formal_Var_List->child_Formal_Var = (AST_Node_Formal_Var *)malloc(sizeof (AST_Node_Formal_Var));
    root_Formal_Var_List->child_Formal_Var = Formal_Var();  //第一颗子树
    if (root_Formal_Var_List->child_Formal_Var == NULL) return NULL;
    if (w.value == 47) return root_Formal_Var_List;
    w = ReturnWord();  // 结束符 )
    if (w.value == 53) {
        fprintf(fp2, "%s ", w.keyname);
        root_Formal_Var_List->child_Formal_Var_List = (AST_Node_Formal_Var_List *)malloc(sizeof (AST_Node_Formal_Var_List));
        root_Formal_Var_List->child_Formal_Var_List = Formal_Var_List(); // 如果w = , 读取下一个形参序列
    }
    return root_Formal_Var_List;
}

AST_Node_Formal_Var *Formal_Var()
{
    AST_Node_Formal_Var *root_Formal_Var;
    root_Formal_Var = (AST_Node_Formal_Var *)malloc(sizeof (AST_Node_Formal_Var));
    w = ReturnWord();  // 相当于 w = int
  //  if (w.value == 47)
    if (!(w.value == 5 || w.value == 10 || w.value == 14 || w.value == 18))  //w不是类型关键字
    {
        error = Error_Type[11];
        return NULL;
    }
    root_Formal_Var->child_TypeName = (AST_Node_TypeName *)malloc(sizeof (AST_Node_TypeName));
    strcpy(root_Formal_Var->child_TypeName->Type_Name, w.keyname);
    w = ReturnWord(); // w = a
    if (w.value != 0)  //如果w不是标识符
    {
        error = Error_Type[11];
        free(root_Formal_Var);
        return NULL;
    }
    root_Formal_Var->child_VarName = (AST_Node_VarName *)malloc(sizeof (AST_Node_VarName));
    strcpy(root_Formal_Var->child_VarName->Var_Name, w.keyname);
    printf("\t\t类型名\t%s, 参数名:\t%s\n", root_Formal_Var->child_TypeName->Type_Name, root_Formal_Var->child_VarName->Var_Name);
    fprintf(fp2, "%s %s", root_Formal_Var->child_TypeName->Type_Name, root_Formal_Var->child_VarName->Var_Name);
    return root_Formal_Var;
}

AST_Node_FuncStruct *FuncStruct()
{
    AST_Node_FuncStruct *root_FuncStruct;
    root_FuncStruct = (AST_Node_FuncStruct *)malloc(sizeof (AST_Node_FuncStruct));
    fprintf(fp2, "%s\r\n", w.keyname);
    w = ReturnWord(); // w = {
    if (w.value == 45) {
        fprintf(fp2, "%s\r\n", w.keyname);
        return NULL; //如果w是分号，就是函数原型声明，函数体子树为空
    }
    fprintf(fp2, "%s\r\n", w.keyname);
    printf("\t复合语句:\n");
    root_FuncStruct->child_ComplexStatement = (AST_Node_ComplexStatement *)malloc(sizeof (AST_Node_ComplexStatement));
    root_FuncStruct->child_ComplexStatement = ComplexStatement();
    return root_FuncStruct;
}

AST_Node_ComplexStatement *ComplexStatement()  //到复合语句时 已经读到单词 { ,遇到 } 结束
{
    AST_Node_ComplexStatement *root_ComplexStatement;
    root_ComplexStatement = (AST_Node_ComplexStatement *)malloc(sizeof (AST_Node_ComplexStatement));
    w = ReturnWord(); // w = int
    if (w.value == 5 || w.value == 10 || w.value == 14 || w.value == 18) //w 是类型关键字 int float char double
    {
        printf("\t\t复合语句的变量定义:\n");
        root_ComplexStatement->child_LocalVarDef_List = (AST_Node_LocalVarDef_List *)malloc(sizeof (AST_Node_LocalVarDef_List));
        root_ComplexStatement->child_LocalVarDef_List = LocalVarDef_List();
    }
    else
    {
        root_ComplexStatement->child_LocalVarDef_List = NULL;
    }
    printf("\t\t复合语句的语句部分:\n");
    root_ComplexStatement->child_Statement_List = (AST_Node_Statement_List *)malloc(sizeof (AST_Node_Statement_List));
    root_ComplexStatement->child_Statement_List = Statement_List();
    if (w.value != 52) {
        error = Error_Type[12];
        free(root_ComplexStatement);
        return NULL;
    }
    w = ReturnWord();
    return root_ComplexStatement;
}

AST_Node_LocalVarDef_List *LocalVarDef_List()
{
    AST_Node_LocalVarDef_List *root_LocalVarDef_List;
    root_LocalVarDef_List = (AST_Node_LocalVarDef_List *)malloc(sizeof (AST_Node_LocalVarDef_List));
    root_LocalVarDef_List->child_LocalVarDef = (AST_Node_LocalVarDef *)malloc(sizeof (AST_Node_LocalVarDef));
    root_LocalVarDef_List->child_LocalVarDef = LocalVarDef();
    return root_LocalVarDef_List;
}

AST_Node_LocalVarDef *LocalVarDef()
{
    AST_Node_LocalVarDef *root_LocalVarDef;
    root_LocalVarDef = (AST_Node_LocalVarDef *)malloc(sizeof (AST_Node_LocalVarDef));
    strcpy(Type, w.keyname);
    root_LocalVarDef->child_TypeName = (AST_Node_TypeName *)malloc(sizeof (AST_Node_TypeName));
    strcpy(root_LocalVarDef->child_TypeName->Type_Name, Type);  //生成外部变量类型的结点，作为root的第一个孩子
    printf("\t\t\t类型:\t\t%s\n", root_LocalVarDef->child_TypeName->Type_Name);
    fprintf(fp2, "\t%s ", root_LocalVarDef->child_TypeName->Type_Name);
    strcpy(Type, init_array); //重新初始化Type类型名数组
    w = ReturnWord(); // w = m
    printf("\t\t\t局部变量名:\n");
    root_LocalVarDef->child_LocalVarName_List = (AST_Node_LocalVarName_List *)malloc(sizeof (AST_Node_LocalVarName_List));
    root_LocalVarDef->child_LocalVarName_List = LocalVarName_List();
    return root_LocalVarDef;
}

AST_Node_LocalVarName_List *LocalVarName_List()
{
    AST_Node_LocalVarName_List *root_LocalVarName_List;
    root_LocalVarName_List = (AST_Node_LocalVarName_List *)malloc(sizeof (AST_Node_LocalVarName_List));
    strcpy(Token_Text, w.keyname);
    root_LocalVarName_List->child_VarName = (AST_Node_VarName *)malloc(sizeof (AST_Node_VarName));
    strcpy(root_LocalVarName_List->child_VarName->Var_Name, Token_Text);  // 生成第一个外部变量名结点
    printf("\t\t\t\tID:\t%s\n", root_LocalVarName_List->child_VarName->Var_Name);
    fprintf(fp2, "%s", root_LocalVarName_List->child_VarName->Var_Name);
    strcpy(Token_Text, init_array);
    w = ReturnWord();
    if (w.value != 53 && w.value != 45)  // 如果 w != , && w != ;
    {
        error = Error_Type[14];
        free(root_LocalVarName_List);
        return NULL;
    }
    if (w.value == 45)  //如果w为分号
    {
        fprintf(fp2, "%s\r\n", w.keyname);
        w = ReturnWord();
        return root_LocalVarName_List;
    }
    fprintf(fp2, "%s", w.keyname);
    w = ReturnWord();  // w = 局部变量的第二个变量名
    if (w.value != 0)  //如果w不是标识符
    {
        error = Error_Type[14];
        free(root_LocalVarName_List);
        return NULL;
    }
    strcpy(Token_Text, w.keyname);  // 保存第二个变量名
    root_LocalVarName_List->child_LocalVarName_List = (AST_Node_LocalVarName_List *)malloc(sizeof (AST_Node_LocalVarName_List));
    root_LocalVarName_List->child_LocalVarName_List = LocalVarName_List();
    return root_LocalVarName_List;
}

AST_Node_Statement_List *Statement_List()
{
    AST_Node_Statement_List *root_Statement_List;
    root_Statement_List = (AST_Node_Statement_List *)malloc(sizeof (AST_Node_Statement_List));
    AST_Node_Statement *r1;
    r1 = Statement();
    if (r1 == NULL)  //没有分析到第一条语句,error>0处理错误，否则表示语句序列已经结束
    {
        error = Error_Type[15];
        return NULL;
    }
    else
    {
        root_Statement_List->child_Statement = (AST_Node_Statement *)malloc(sizeof (AST_Node_Statement));
        root_Statement_List->child_Statement = r1;
        root_Statement_List->child_Statement_List = Statement_List();
        return root_Statement_List;
    }
}

AST_Node_Statement *Statement() // w = if 或者 for while
{
    AST_Node_Statement *root_Statement;
    root_Statement = (AST_Node_Statement *)malloc(sizeof (AST_Node_Statement));
    switch (w.value) {
        case 17: //分析条件语句
            fprintf(fp2, "\t%s", w.keyname);
            printf("\t\t 条件语句(IF_THEN_ELSE):\n");
            w = ReturnWord(); // w = (
            if (w.value != 46)
            {
                error = Error_Type[16];
                return NULL;
            }
            fprintf(fp2, "%s", w.keyname);
            w = ReturnWord(); // w = a
            if_else_statement *root_if_else_statement;
            root_if_else_statement = (if_else_statement *)malloc(sizeof (if_else_statement));
            printf("\t\t\t条件:\n");
            root_if_else_statement->child_condition = (AST_Node_expression *)malloc(sizeof (AST_Node_expression));
            root_if_else_statement->child_condition = expression();
            printf("\t\t\tIF字句\n");
            root_if_else_statement->child_if_expression = (AST_Node_expression *) malloc(sizeof(AST_Node_expression));
            fprintf(fp2, "\t\t");
            root_if_else_statement->child_if_expression = expression();
            printf("\t\t\tELSE字句\n");
            root_if_else_statement->child_else_expression = (AST_Node_expression *) malloc(sizeof(AST_Node_expression));
            root_if_else_statement->child_else_expression = expression();
            root_Statement->child_if_else_statement = (if_else_statement *) malloc(sizeof(if_else_statement));
            root_Statement->child_if_else_statement = root_if_else_statement;
            return root_Statement;
        case 33: // while语句
            fprintf(fp2, "\t%s", w.keyname);
            printf("\t\t while语句:\n");
            w = ReturnWord(); // w = (
            if (w.value != 46) {
                error = Error_Type[16];
                return NULL;
            }
            fprintf(fp2, "%s", w.keyname);
            w = ReturnWord(); // w = a
            while_statement *root_while_statement;
            root_while_statement = (while_statement *) malloc(sizeof(while_statement));
            printf("\t\t\t条件:\n");
            root_while_statement->child_condition = expression();
            fprintf(fp2, "\t\t");
            root_while_statement->child_expression = expression();
            root_Statement->child_while_statement = root_while_statement;
            return root_Statement;
        case 21: // return 语句
            printf("\t\t 返回语句:\n");
            fprintf(fp2, "\t%s ", w.keyname);
            return_statement *root_return_statement;
            root_return_statement = (return_statement *)malloc(sizeof (return_statement));
            w = ReturnWord(); // w = m
            root_return_statement->return_var = (AST_Node_VarName *)malloc(sizeof (AST_Node_VarName));
            strcpy(root_return_statement->return_var->Var_Name, w.keyname);
            printf("\t\t\tID: %s\n", root_return_statement->return_var->Var_Name);
            fprintf(fp2, "%s", root_return_statement->return_var->Var_Name);
            root_Statement->child_return_statement = (return_statement *)malloc(sizeof (return_statement));
            root_Statement->child_return_statement = root_return_statement;
            w = ReturnWord(); // w = ;
            if (w.value != 45) {
                error = Error_Type[16];
                return NULL;
            }
            fprintf(fp2, "%s\r\n", w.keyname);
            w = ReturnWord();
            return root_Statement;
        case 51: // { 复合语句
            root_Statement->child_ComplexStatement = ComplexStatement();
            return root_Statement;
        case 52: // }语句序列结束符号
            fprintf(fp2, "%s\r\n", w.keyname);
            return NULL;
        default:
            error = Error_Type[16];
            return NULL;
    }
}


AST_Node_expression *expression()
{
 /*   StackInfo_st *op;  //运算符栈
    op->topOfStack = 0;
    flag = stack_push(op, Key[49]);
    StackInfo_st *opn; //操作数栈
    opn->topOfStack = 0;
    error = 0;
    KeyType op_first, op_end;
    KeyType value;
    op_first = op->stack[op->topOfStack - 1];
    while((w.value != 48 || op_first.value != 48) && !error)  // 当运算符栈栈顶不是起止符号， 并没有错误时
    {
        if (w.value == 1 || w.value == 0)
        {
            stack_push(opn, w);
            w = ReturnWord(); // w = '>'
        }
        else if (w.value == 34 || w.value == 49 || w.value == 50) {
            switch (w.value) {
                case 50: // w = '<'
                    stack_push(op, w);
                    w = ReturnWord(); // w = b
                    break;
                case 34:  // w = '='
                    KeyType *t;
                    if (!stack_pop(op, t)) error++;
                    w = ReturnWord(); // w = b;
                    break;
                case 49:  // w = '<'
                    KeyType *t1, *t2;
                    if (!stack_pop(opn, t2)) error++;
                    if (!stack_pop(opn, t1)) error++;
                    if (!stack_pop(op, t)) error++;
                    break;
                default:
                    if (w.value == 45 || w.value == 47) w.value = 48;
                    else error = 1;
            }
        }
            else if (w.value == 45 || w.value == 47) w.value = 48;

            else error = 1;
    }
    op_end = op->stack[op->topOfStack - 1];
    if (opn->topOfStack == 1 && op_end.value == 48)
        return
    }*/
    int error_e = 1;
    AST_Node_expression *root_expression;
    root_expression = (AST_Node_expression *)malloc(sizeof (AST_Node_expression));

    op *root_op;
    root_op = (op *)malloc(sizeof (op));
    if (w.value == 11 || w.value == 17) {
        fprintf(fp2, "\t%s\r\n\t\t", w.keyname);
        w = ReturnWord();  // 如果w的值是else或者if 跳到下一位
    }
    if (w.value == 0 || w.value == 1)
    {
        char L_Node[20];
        strcpy(L_Node, w.keyname);
        w = ReturnWord(); //w = '='
        if (w.value == 34 || w.value == 49 || w.value == 50)
        {
            strcpy(root_op->on_var, w.keyname);
            if (w.value == 34) {
                printf("\t\t\t 表达式语句\n");
                printf("\t\t\t %s\n", root_op->on_var);
            }
            else
                printf("\t\t\t %s\n", root_op->on_var);
            strcpy(root_op->L_opn, L_Node);
            printf("\t\t\t\tID:%s\n", root_op->L_opn);
            w = ReturnWord(); // w = b
            if (w.value == 0 || w.value == 1) {
                strcpy(root_op->R_opn, w.keyname);
                printf("\t\t\t\tID:%s\n", root_op->R_opn);
                fprintf(fp2, "%s%s%s", root_op->L_opn, root_op->on_var, root_op->R_opn);
            }
            else
                error_e = 0;
        }
        else
            error_e = 0;
    }
    else error_e = 0;
    w = ReturnWord(); // w = ; || w = )
    fprintf(fp2, "%s\r\n", w.keyname);
    if (!(w.value == 45 || w.value == 47))
        error_e = 0;
    root_expression->child_op = (op *)malloc(sizeof (op));
    root_expression->child_op = root_op;
    w = ReturnWord(); // w = a
    if (error_e) return root_expression;
    return NULL;
}

//打印菜单
int menu()
{
    printf("------------开始进行抽象语法树生成------------\n");
    printf("[1]词法分析\t\t\t[2]语法分析\t\t\t[0]退出\n");
    printf("请输入指令\n");
    return 0;
}