#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*-----------------状态变量宏定义-----------------*/
#define N 100000
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
/*-------------数据结构与全局变量定义-------------*/
typedef int status;
typedef struct Literal{
    int liter;
    Literal* next;
}Literal;
typedef struct Clause{
    Literal* first;
    Clause* next;
}Clause;
typedef struct{
    int liter;
    double sum;
    int num_of_positive;
    int num_of_negative;
}Sum_of_Liter;
extern Sum_of_Liter a[N];
extern int liter[N];
extern int Mark;
/*---------------CNF解析模块函数声明--------------*/
status ReadFile(Clause* root,int &num_of_liter,int &num_of_clause,char filename[]);
Clause* createClause(int n,int lit[]);
status destroyClause(Clause* p);
status addClause(Clause* root,Clause* pNew);
status isUnitClause(Clause* p);
status printClause(Clause* root);
Clause* copyList(Clause* root);
status destroyList(Clause* root);
status addliter(Clause* p,int lit);
/*-------------核心DPLL模块函数声明----------------*/
int find_lit(int num_of_liter);
status law_of_UnitClause(Clause* root,Clause* p_of_unitCla,int &num_of_clause);
status preSolve(Clause* root,int num_of_liter,int &num_of_clause);
status Literalsum(Clause *root,int num_of_liter);
Sum_of_Liter Literalmax(int num_of_liter);
status Recovery_lit(Clause* p);
status DPLL(Clause* root,int lit,int num_of_liter,int num_of_clause);
status SaveFile(int time,char filename[],int flag,int num_of_liter);
/*-------------二进制数独模块函数声明--------------*/
int trans_of_comliter(int i,int j,int n,int flag);
int trans_of_extraliter(int a,int b,int c,int d,int e,int n);
status firstlaw(Clause* root,int &num_of_clause,int n,int flag);
status secondlaw_of_DEP(Clause* root,int &num_of_clause,int layer,int i,int j,int n,int flag1,int flag2);
status secondlaw(Clause* root,int &num_of_clause,int n);
status thirdlaw(Clause* root,int &num_of_clause,int n);
status ReadPuzzle_File(int *puzzle,int n);
status ReadPuzzle_Buffer(int *puzzle,int n);
status transPuzzle_to_CNF(Clause* root,int &num_of_clause,int n,int *a);
status SaveCNFFile(Clause* root,int *puzzle,int n,int num_of_liter,int num_of_clause);
status puzzleslover(Clause* root,int n,int num_of_clause);
status puzzles(int n,int *a);
status generate_of_puzzle(int n,int *c);
status printpuzzle(int *puzzle,int n);


