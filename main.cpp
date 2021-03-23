#include "def.h"
#include "stack_def.h"
#include "stu.h"

using namespace std;
int main()          // 主程序 交互菜单界面
{
    int order = 1; // 获取指令
    FILE *fp;
    while (order) {
        menu();
        scanf("%d", &order);
        if (order >= 0 && order <= 2) {

            switch (order) {
                case 1:
                    if ((fp = fopen(FileOpenName, "r")) == NULL) {
                        printf("文件读取失败\n");
                        exit(0);
                    }
                    while (!(feof(fp))) {
                        if (fgets(buffer, 256, fp) != NULL) {
                            printf("当前打开的文件是: test.txt 开始进行词法分析\n\n");
                            break;
                        }
                    }
                    KeyType word;      // 单词有自身值以及种类编码
                    do {
                        while (strlen(buffer)) {
                            word = ReturnWord();
                            printf("%s\t", word.keyname);    // 打印单词自身值
                            Type_Recognition(word.value);    // 打印单词所属类型
                        }
                        fgets(buffer, 256, fp);
                    } while (strlen(buffer));
                    printf("词法分析结束\n");
                    fclose(fp);
                    break;

                case 2:
                    if ((fp = fopen(FileOpenName, "r")) == NULL) {
                        printf("文件读取失败\n");
                        exit(0);
                    }
                    while (!(feof(fp))) {
                        if (fgets(buffer, 256, fp) != NULL) {
                            printf("当前打开的文件是: test.txt 开始进行语法分析\n\n");  // buffer数组读入 w从第一个单词开始读取
                            break;
                        }
                    }
                    AST_Node_program *begin;
                    begin = program();
                    if (error.value == Error_Type[2].value) {
                        printf("外部定义部分出错， 退出程序\n");
                        break;
                    }
                    if (error.value == Error_Type[5].value) {
                        printf("外部变量定义部分出错， 退出程序\n");
                        break;
                    }
                    if (error.value == Error_Type[11].value) {
                        printf("函数形参部分出错， 退出程序\n");
                        break;
                    }
                    if (error.value == Error_Type[12].value) {
                        printf("复合语句部分出错， 退出程序\n");
                        break;
                    }
                    if (error.value == Error_Type[14].value) {
                        printf("复合语句的变量定义部分出错， 退出程序\n");
                        break;
                    }
                    if (error.value == Error_Type[15].value) {
                        printf("复合语句的语句定义部分出错， 退出程序\n");
                        break;
                    }
                    break;
                case 0:
                    printf("---------------退出成功！---------------\n");
                    printf("-----------欢迎下次使用本系统！-----------\n");
                    break;
            }
        } else {
            printf("输入错误！\n");
            order = 1;

        }
    }
    return 0;
}

