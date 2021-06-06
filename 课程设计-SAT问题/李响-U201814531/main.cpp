#include "head.h"
#include "Binary_Puzzle.cpp"
#include "CNF_analysis.cpp"
#include "DPLL.cpp"
Sum_of_Liter a[N];
int liter[N];
int Mark;
int main()
{
    int op=1,lit,opt,ent,n,i,flag;
    char filename[101];
    int num_of_liter,num_of_clause,*puzzle;
    Sum_of_Liter temp;
    Clause* root;
    while(op){
    int choice=1;
	system("cls");	printf("\n\n");
	printf("      Menu for SAT Problem Solver On DPLL Algorithm \n");
	printf("--------------------------------------------------------------\n");
    printf("    	   0. 退出系统\n");
	printf("    	   1. 求解SAT算例（使用CNF文件）\n");
	printf("    	   2. 求解二进制数独游戏问题\n");
	printf("--------------------------------------------------------------\n");
	printf("      请选择你的操作[0~2]:");
	num_of_liter=0; num_of_clause=0;
	root=(Clause*)malloc(sizeof(Clause));
    root->next=NULL;
	scanf("%d",&op);
	switch(op){
	    case 1:
	        while(choice){
	        system("cls");	printf("\n\n");
            printf("      Menu for SAT Problem Solver On DPLL Algorithm \n");
	        printf("--------------------------------------------------------------\n");
          	printf("    	  1. ReadFile          3. NotImproveDPLL\n");
          	printf("    	  2. TraverseCNF       4. ImproveDPLL\n");
	        printf("    	  0. Return to main menu\n");
         	printf("--------------------------------------------------------------\n");
	        printf("      请选择你的操作[0~4]:");
	        scanf("%d",&choice);
            switch(choice){
                case 1:
                    if(root->next)
                    {
                        destroyList(root);
                        root=(Clause*)malloc(sizeof(Clause));
                        root->next=NULL;
                    }
                    if(ReadFile(root,num_of_liter,num_of_clause,filename))
                        printf("      CNF文件读取成功！\n");
                    else printf("      文件打开失败！\n");
                    getchar();getchar();
                    break;
                case 2:
                    if(!root->next){ printf("      当前未读入CNF文件，请先读入！\n");getchar();getchar(); break;}
                    printf("\n      文字数为：%-8d 公式数为：%-8d\n",num_of_liter,num_of_clause);
                    printClause(root);
                    getchar();getchar();
                    break;
                case 3:
                    opt=clock();
                    Mark=0;
                    if(!root->next){ printf("      当前未读入CNF文件，请先读入！\n");getchar();getchar(); break;}
                    preSolve(root,num_of_liter,num_of_clause);
                    lit=find_lit(num_of_liter);
                    if(DPLL(root,lit,num_of_liter,num_of_clause)||DPLL(root,-lit,num_of_liter,num_of_clause))
                    {
                        ent=clock();
                        for(int i=1;i<=num_of_liter;i++)
                             if(liter[i]) printf("%d ",liter[i]);
                             else printf("%d ",i);
                        printf("\ntime is %d ms\n",ent-opt);
                        flag=1;
                    }
                    else
                    {
                        ent=clock();
                        printf("      DPLL算法求解失败，文件不满足!\n");
                        printf("      time is %d ms\n",ent-opt);
                        flag=0;
                    }
                    if(SaveFile(ent-opt,filename,flag,num_of_liter))
                        printf("      文件保存成功!\n");
                    else printf("      文件保存失败!\n");
                    destroyList(root);
                    root=(Clause*)malloc(sizeof(Clause));
                    root->next=NULL;
                    getchar();getchar();
                    break;
                case 4:
                    opt=clock();
                    Mark=1;
                    if(!root->next){ printf("      当前未读入CNF文件，请先读入！\n");getchar();getchar(); break;}
                    preSolve(root,num_of_liter,num_of_clause);
                    Literalsum(root,num_of_liter);
                    temp=Literalmax(num_of_liter);
                    lit=temp.liter;
                    if(!root->next||DPLL(root,lit,num_of_liter,num_of_clause)||DPLL(root,-lit,num_of_liter,num_of_clause))
                    {
                        ent=clock();
                        for(int i=1;i<=num_of_liter;i++)
                             if(liter[i]) printf("%d ",liter[i]);
                             else printf("%d ",i);
                        printf("\ntime is %d ms\n",ent-opt);
                        flag=1;
                    }
                    else
                    {
                        ent=clock();
                        printf("      DPLL算法求解失败，文件不满足!\n");
                        printf("      time is %d ms\n",ent-opt);
                        flag=0;
                    }
                    if(SaveFile(ent-opt,filename,flag,num_of_liter))
                        printf("      文件保存成功!\n");
                    else printf("      文件保存失败!\n");
                    destroyList(root);
                    root=(Clause*)malloc(sizeof(Clause));
                    root->next=NULL;
                    getchar();getchar();
                    break;
                case 0:
                    destroyList(root);
                    break;
                default:
                    printf("      输入非法!\n");
                    getchar();getchar();
                    break;
                }
	        }
            break;
        case 2:
            while(choice){
	        system("cls");	printf("\n\n");
            printf("      Menu for Binary Puzzle Solver On DPLL Algorithm \n");
	        printf("--------------------------------------------------------------\n");
          	printf("    	 1. GeneratePuzzle        3. SaveCNFFile\n");
          	printf("    	 2. ReadPuzzle            4. SolvePuzzle\n");
	        printf("    	 0. Return to main menu\n");
         	printf("--------------------------------------------------------------\n");
	        printf("      请选择你的操作[0~4]:");
	        scanf("%d",&choice);
            switch(choice){
                case 1:
                    if(root->next)
                    {
                        printf("      当前已经生成/输入二进制数独，需要删除请输入1，不删除请输入0：");
                        scanf("%d",&i);
                        if(i){destroyList(root);root=(Clause*)malloc(sizeof(Clause));root->next=NULL;}
                        else {getchar();getchar(); break;}
                    }
                    printf("      请输入二进制数独的阶数（请输入大于2偶数，不建议生成超过10阶的数独）：");
                    scanf("%d",&n); num_of_clause=0;
                    if(n%2||n<4){ printf("      阶数非法，操作失败！\n");getchar();getchar();break; }
                    puzzle=(int*)malloc(sizeof(int)*n*n);
                    num_of_liter=trans_of_extraliter(2,n-1,n,n,2,n);
                    generate_of_puzzle(n,puzzle);
                    transPuzzle_to_CNF(root,num_of_clause,n,puzzle);
                    printf("      生成成功！\n");
                    printf("      生成的棋盘布局如下：\n");
                    printpuzzle(puzzle,n);
                    getchar();getchar();
                    break;
                case 2:
                    if(root->next)
                    {
                        printf("      当前已经生成/输入二进制数独，需要删除请输入1，不删除请输入0：");
                        scanf("%d",&i);
                        if(i){destroyList(root);root=(Clause*)malloc(sizeof(Clause));root->next=NULL;}
                        else {getchar();getchar(); break;}
                    }
                    printf("      请输入二进制数独的阶数（请输入大于2的偶数）：");
                    scanf("%d",&n); num_of_clause=0;
                    if(n%2||n<4){ printf("      阶数非法，操作失败！\n");getchar();getchar();break; }
                    puzzle=(int*)malloc(sizeof(int)*n*n);
                    num_of_liter=trans_of_extraliter(2,n-1,n,n,2,n);
                    printf("      请选择读入方式：\n");
                    printf("           1. 文件读入\n");
                    printf("           2. 键盘输入\n");
                    printf("      PS：“....0.1.0..”为示范输入格式，请输入：");
                    scanf("%d",&i);
                    if(i==1)
                    {
                        if(ReadPuzzle_File(puzzle,n))
                            printf("      文件读入成功！\n"),transPuzzle_to_CNF(root,num_of_clause,n,puzzle);
                        else printf("      文件打开失败！\n");
                    }
                    else if(i==2)
                    {
                        if(ReadPuzzle_Buffer(puzzle,n))
                            printf("      读入成功！\n"),transPuzzle_to_CNF(root,num_of_clause,n,puzzle);
                        else printf("      读入失败！\n");
                    }
                    else printf("      输入非法，操作失败！\n");
                    getchar();getchar();
                    break;
                case 3:
                    if(!root->next){ printf("      当前未读入PUZZLE，请先读入！\n");getchar();getchar(); break;}
                    if(SaveCNFFile(root,puzzle,n,num_of_liter,num_of_clause))
                        printf("      文件保存成功！\n");
                    else printf("      文件保存失败！\n");
                    getchar();getchar();
                    break;
                case 4:
                    if(!root->next){ printf("      当前未读入PUZZLE，请先读入！\n");getchar();getchar(); break;}
                    opt=clock();
                    if(puzzleslover(root,n,num_of_clause))
                    {
                        for(int i=1;i<=n;i++)
                        for(int j=1;j<=n;j++)
                        if(liter[(i-1)*n+j]>=0) *(puzzle+((i-1)*n+j-1))=1;
                        else *(puzzle+((i-1)*n+j-1))=0;
                        printf("      求解成功！\n");
                        printf("      求解的棋盘布局如下：\n");
                        printpuzzle(puzzle,n);
                        free(puzzle);
                    }
                    else printf("      求解失败！\n");
                    destroyList(root);
                    root=(Clause*)malloc(sizeof(Clause));
                    root->next=NULL;
                    getchar();getchar();
                    break;
                case 0:
                    destroyList(root);
                    break;
                default:
                    printf("      输入非法!\n");
                    getchar();getchar();
                    break;
                }
	        }
            break;
        case 0:
            break;
        default:
            printf("      输入非法!\n");
            getchar();getchar();
            break;
	    }
    }
    printf("      欢迎下次再使用本系统！\n");
    getchar();getchar();
    return 0;
}
