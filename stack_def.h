#define STACK_SIZE 64 /*栈大小*/
#define TOP_OF_STACK 0 /*栈顶位置*/
typedef KeyType ElementType; /*栈元素类型为word类型的单词w*/
#define SUCCESS 1
#define FAILURE 0
/*定义栈结构*/
typedef struct StackInfo
{
    int topOfStack; /*记录栈顶位置*/
    ElementType stack[STACK_SIZE]; /*栈数组，也可以使用动态数组实现*/
}StackInfo_st;
/*函数声明*/
int stack_push(StackInfo_st *s,ElementType value);
int stack_pop(StackInfo_st *s,ElementType *value);
ElementType stack_top(StackInfo_st *s,ElementType *value);
int stack_is_full(StackInfo_st *s);
int stack_is_empty(StackInfo_st *s);
/*入栈，0表示成，非0表示出错*/
int stack_push(StackInfo_st *s,ElementType value)
{
    if(stack_is_full(s))
        return FAILURE;
    /*先增加topOfStack，再赋值*/
    s->stack[s->topOfStack] = value;
    s->topOfStack++;
    return SUCCESS;
}
/*出栈*/
int stack_pop(StackInfo_st *s,ElementType *value)
{
    /*首先判断栈是否为空*/
    if(stack_is_empty(s))
        return FAILURE;
    s->topOfStack--;
    *value = s->stack[s->topOfStack];
    return SUCCESS;
}
/*访问栈顶元素*/
ElementType stack_top(StackInfo_st *s,ElementType value)
{
    /*首先判断栈是否为空*/
    if(stack_is_empty(s))
    {
        //栈空
        return value;
    }
    value = s->stack[s->topOfStack];
    return value;
}
/*判断栈是否已满，满返回1，未满返回0*/
int stack_is_full(StackInfo_st *s)
{
    if (s->topOfStack == STACK_SIZE) return SUCCESS;
    else return FAILURE;
}
/*判断栈是否为空，空返回1，非空返回0*/
int stack_is_empty(StackInfo_st *s)
{
    if (s->topOfStack == 0) return SUCCESS;
    else return FAILURE;
}

