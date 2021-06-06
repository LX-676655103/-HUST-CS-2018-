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
    printf("    	   0. �˳�ϵͳ\n");
	printf("    	   1. ���SAT������ʹ��CNF�ļ���\n");
	printf("    	   2. ��������������Ϸ����\n");
	printf("--------------------------------------------------------------\n");
	printf("      ��ѡ����Ĳ���[0~2]:");
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
	        printf("      ��ѡ����Ĳ���[0~4]:");
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
                        printf("      CNF�ļ���ȡ�ɹ���\n");
                    else printf("      �ļ���ʧ�ܣ�\n");
                    getchar();getchar();
                    break;
                case 2:
                    if(!root->next){ printf("      ��ǰδ����CNF�ļ������ȶ��룡\n");getchar();getchar(); break;}
                    printf("\n      ������Ϊ��%-8d ��ʽ��Ϊ��%-8d\n",num_of_liter,num_of_clause);
                    printClause(root);
                    getchar();getchar();
                    break;
                case 3:
                    opt=clock();
                    Mark=0;
                    if(!root->next){ printf("      ��ǰδ����CNF�ļ������ȶ��룡\n");getchar();getchar(); break;}
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
                        printf("      DPLL�㷨���ʧ�ܣ��ļ�������!\n");
                        printf("      time is %d ms\n",ent-opt);
                        flag=0;
                    }
                    if(SaveFile(ent-opt,filename,flag,num_of_liter))
                        printf("      �ļ�����ɹ�!\n");
                    else printf("      �ļ�����ʧ��!\n");
                    destroyList(root);
                    root=(Clause*)malloc(sizeof(Clause));
                    root->next=NULL;
                    getchar();getchar();
                    break;
                case 4:
                    opt=clock();
                    Mark=1;
                    if(!root->next){ printf("      ��ǰδ����CNF�ļ������ȶ��룡\n");getchar();getchar(); break;}
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
                        printf("      DPLL�㷨���ʧ�ܣ��ļ�������!\n");
                        printf("      time is %d ms\n",ent-opt);
                        flag=0;
                    }
                    if(SaveFile(ent-opt,filename,flag,num_of_liter))
                        printf("      �ļ�����ɹ�!\n");
                    else printf("      �ļ�����ʧ��!\n");
                    destroyList(root);
                    root=(Clause*)malloc(sizeof(Clause));
                    root->next=NULL;
                    getchar();getchar();
                    break;
                case 0:
                    destroyList(root);
                    break;
                default:
                    printf("      ����Ƿ�!\n");
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
	        printf("      ��ѡ����Ĳ���[0~4]:");
	        scanf("%d",&choice);
            switch(choice){
                case 1:
                    if(root->next)
                    {
                        printf("      ��ǰ�Ѿ�����/�����������������Ҫɾ��������1����ɾ��������0��");
                        scanf("%d",&i);
                        if(i){destroyList(root);root=(Clause*)malloc(sizeof(Clause));root->next=NULL;}
                        else {getchar();getchar(); break;}
                    }
                    printf("      ����������������Ľ��������������2ż�������������ɳ���10�׵���������");
                    scanf("%d",&n); num_of_clause=0;
                    if(n%2||n<4){ printf("      �����Ƿ�������ʧ�ܣ�\n");getchar();getchar();break; }
                    puzzle=(int*)malloc(sizeof(int)*n*n);
                    num_of_liter=trans_of_extraliter(2,n-1,n,n,2,n);
                    generate_of_puzzle(n,puzzle);
                    transPuzzle_to_CNF(root,num_of_clause,n,puzzle);
                    printf("      ���ɳɹ���\n");
                    printf("      ���ɵ����̲������£�\n");
                    printpuzzle(puzzle,n);
                    getchar();getchar();
                    break;
                case 2:
                    if(root->next)
                    {
                        printf("      ��ǰ�Ѿ�����/�����������������Ҫɾ��������1����ɾ��������0��");
                        scanf("%d",&i);
                        if(i){destroyList(root);root=(Clause*)malloc(sizeof(Clause));root->next=NULL;}
                        else {getchar();getchar(); break;}
                    }
                    printf("      ����������������Ľ��������������2��ż������");
                    scanf("%d",&n); num_of_clause=0;
                    if(n%2||n<4){ printf("      �����Ƿ�������ʧ�ܣ�\n");getchar();getchar();break; }
                    puzzle=(int*)malloc(sizeof(int)*n*n);
                    num_of_liter=trans_of_extraliter(2,n-1,n,n,2,n);
                    printf("      ��ѡ����뷽ʽ��\n");
                    printf("           1. �ļ�����\n");
                    printf("           2. ��������\n");
                    printf("      PS����....0.1.0..��Ϊʾ�������ʽ�������룺");
                    scanf("%d",&i);
                    if(i==1)
                    {
                        if(ReadPuzzle_File(puzzle,n))
                            printf("      �ļ�����ɹ���\n"),transPuzzle_to_CNF(root,num_of_clause,n,puzzle);
                        else printf("      �ļ���ʧ�ܣ�\n");
                    }
                    else if(i==2)
                    {
                        if(ReadPuzzle_Buffer(puzzle,n))
                            printf("      ����ɹ���\n"),transPuzzle_to_CNF(root,num_of_clause,n,puzzle);
                        else printf("      ����ʧ�ܣ�\n");
                    }
                    else printf("      ����Ƿ�������ʧ�ܣ�\n");
                    getchar();getchar();
                    break;
                case 3:
                    if(!root->next){ printf("      ��ǰδ����PUZZLE�����ȶ��룡\n");getchar();getchar(); break;}
                    if(SaveCNFFile(root,puzzle,n,num_of_liter,num_of_clause))
                        printf("      �ļ�����ɹ���\n");
                    else printf("      �ļ�����ʧ�ܣ�\n");
                    getchar();getchar();
                    break;
                case 4:
                    if(!root->next){ printf("      ��ǰδ����PUZZLE�����ȶ��룡\n");getchar();getchar(); break;}
                    opt=clock();
                    if(puzzleslover(root,n,num_of_clause))
                    {
                        for(int i=1;i<=n;i++)
                        for(int j=1;j<=n;j++)
                        if(liter[(i-1)*n+j]>=0) *(puzzle+((i-1)*n+j-1))=1;
                        else *(puzzle+((i-1)*n+j-1))=0;
                        printf("      ���ɹ���\n");
                        printf("      �������̲������£�\n");
                        printpuzzle(puzzle,n);
                        free(puzzle);
                    }
                    else printf("      ���ʧ�ܣ�\n");
                    destroyList(root);
                    root=(Clause*)malloc(sizeof(Clause));
                    root->next=NULL;
                    getchar();getchar();
                    break;
                case 0:
                    destroyList(root);
                    break;
                default:
                    printf("      ����Ƿ�!\n");
                    getchar();getchar();
                    break;
                }
	        }
            break;
        case 0:
            break;
        default:
            printf("      ����Ƿ�!\n");
            getchar();getchar();
            break;
	    }
    }
    printf("      ��ӭ�´���ʹ�ñ�ϵͳ��\n");
    getchar();getchar();
    return 0;
}
