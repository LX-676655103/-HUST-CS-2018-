#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define N 100
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
/*--------------------------------------------*/
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
/*--------------------------------------------*/
typedef int status;
typedef int ElemType; //����Ԫ�����Ͷ���
/*--------------------------------------------*/
typedef struct LNode{  //���Ա���ʽ�ṹ��㣩�Ķ���
	ElemType data;
	struct LNode *next;//�洢���Ա�����
}LNode,*LinkList; //���ṹ���Լ����ָ��ṹ��
typedef struct{
    LinkList head;
    char name[30];
}LkList; //�洢���Ա��������Ϣ�Լ�ͷָ��
/*--------------------------------------------*/
status InitList(LinkList &L);//��ʼ�����Ա�
status DestroyList(LinkList &L);//�������Ա�
status ClearList(LinkList L);//������Ա�
status ListEmpty(LinkList L);//�ж����Ա��Ƿ�Ϊ��
int ListLength(LinkList L);//���
status GetElem(LinkList L,int i,ElemType &e);//��ȡԪ��
int LocateElem(LinkList L,ElemType e);//��λԪ��
status PriorElem(LinkList L,ElemType cur,ElemType &pre_e);//��ȡǰ��
status NextElem(LinkList L,ElemType cur,ElemType &next_e);//��ȡ���
status ListInsert(LinkList L,int i,ElemType e);//����Ԫ��
status ListDelete(LinkList L,int i,ElemType &e);//ɾ��Ԫ��
status ListTrabverse(LinkList L);//�������Ա�
status OpenFile(LinkList L);//��ȡ�ļ�
status SetValue(LinkList L);//������������
status SaveFile(LkList L);//�����ļ�
/*--------------------------------------------*/
int main(){
LkList List[N];  //����һ��ͷָ���������ڹ��������Ա����Ա�����ĵ�һ���ռ䲻ʹ�ã��Ա���в���
int op=1,i=0,j,num=0,k;
ElemType e;  char name[30];
while(op){
	system("cls");	printf("\n\n");
	printf("      Menu for Linear Table On Sequence Structure \n");
	printf("-------------------------------------------------\n");
	printf("    	  1. InitList       8. PriorElem\n");
	printf("    	  2. DestroyList    9. NextElem \n");
	printf("    	  3. ClearList      10. ListInsert\n");
	printf("    	  4. ListEmpty      11. ListDelete\n");
	printf("    	  5. ListLength     12. ListTrabverse\n");
	printf("    	  6. GetElem        13. SaveFile\n");
	printf("    	  7. LocateElem     14. ListMessage\n"); //����14���Է���ز鿴�������Ա����Ϣ���Ա������������
	printf("    	  0. Exit\n");
	printf("-------------------------------------------------\n");
	printf("      ��ѡ����Ĳ���[0~14]:");
	scanf("%d",&op);
	if(op>=2&&op<=13) //����2-13��ʼ����Ϊ���Ա���ڣ�����ǰ�ж�
    {
        if(num)//num�����������Ա���������˴��ж��Ƿ������Ա�
        {
            printf("      ��������Ҫ���������Ա�����ƣ�");//ͨ�����Ʋ������Ա�
            scanf("%s",name);
            for(i=1;i<=num;i++)//��������
                if(strcmp(List[i].name,name)==0) break;
            if(i>num) {printf("      �����ҵ����Ա����ڣ������²�����\n\n"); getchar(); getchar();continue;}
        }
        else {printf("      ��ǰ�����Ա���ڣ����ȴ������Ա�\n");getchar(); getchar();continue;}
    }
    switch(op){
        case 1:
         if(InitList(List[++num].head)==OK)//�������Ա����Ա������һ
         {
             printf("      ���Ա����ɹ���������Ա����������");
             scanf("%s",List[num].name);//�����Ա�����������Ա���к�������
         }
         else{ printf("      ���Ա���ʧ�ܣ�\n"); num--; getchar();getchar(); break;}
         printf("      ��ѡ��1.������Ա���м��̸�ֵ\n              2.��ȡ�ļ�\n\n");//���Ա��ʼ��ֵ
         scanf("%d",&k);
         switch(k){
            case 1: if(SetValue(List[num].head)==OVERFLOW){num--; printf("      ���Ա�ֵʧ�ܣ�\n");} break;//���̸�ֵ
            case 2: switch(OpenFile(List[num].head))//�ļ���ֵ
                    {
                       case OVERFLOW: num--; break;
                       case ERROR: num--; break;
                       case OK: break;
                    }
         }
         getchar();
		 break;
	   case 2:
		 if(DestroyList(List[i].head)==OK)
		 {
                printf("      ���Ա����ٳɹ���\n");
                for(;i<num;i++) //�����Ա������Ҫ�����ٵ����Ա�ɾ�����ǣ�List[0]���洢��Ϣ
                    List[i]=List[i+1];
                num--;
		 }
		 getchar();getchar();
		 break;
	   case 3:
		 if(ClearList(List[i].head)==OK) printf("      ���Ա���ճɹ���\n");
         else printf("      ���Ա����ڣ����ʧ�ܣ�\n");
		 getchar();getchar();
		 break;
	   case 4:
         if(ListEmpty(List[i].head)==FALSE) printf("      �����Ա�Ϊ�ձ�\n");
         else printf("      �����Ա�Ϊ�ձ�\n");
		 getchar();getchar();
		 break;
	   case 5:
         printf("      �����Ա�ĳ���Ϊ%d\n",ListLength(List[i].head));
		 getchar();getchar();
		 break;
	   case 6:
         if(ListEmpty(List[i].head)) {printf("      �����Ա�Ϊ�ձ����������������\n"); getchar();getchar();break;}
         printf("      ������������ҵ�Ԫ�ص���ţ�ע���벻Ҫ����С��1���ߴ���%d��������",ListLength(List[i].head));
         scanf("%d",&j);
         if(GetElem(List[i].head,j,e)==ERROR) printf("      ����������󣬲���ʧ�ܣ�\n");
         else printf("      �����ɹ�����%d��Ԫ��ֵΪ%d\n",j,e);
		 getchar();getchar();
		 break;
	   case 7:
         printf("      ������Ҫ���ҵ�Ԫ��ֵ��");
         scanf("%d",&e);
		 if(!LocateElem(List[i].head,e)) printf("      û�з���������Ԫ�ش��ڣ�\n");
		 else printf("      ����������Ԫ�ص�λ��Ϊ%d \n",LocateElem(List[i].head,e));
		 getchar();getchar();
		 break;
	   case 8:
         ElemType cur_e,pre_e;
		 printf("      ����������ǰ����Ԫ�أ�");
		 scanf("%d",&cur_e);
         if(PriorElem(List[i].head,cur_e,pre_e)==ERROR-1) printf("      ��Ԫ�ز�����ǰ��!\n");//���׸�Ԫ�ط����������᷵��-1
         else if(PriorElem(List[i].head,cur_e,pre_e)==ERROR) printf("      ��Ԫ�ز�����!\n");
         else printf("      ����ǰ��Ϊ%d\n",pre_e);
         getchar();getchar();
         break;
	   case 9:
		 ElemType next_e;
		 printf("      ���������ú�̵�Ԫ�أ�");
		 scanf("%d",&cur_e);
         if(NextElem(List[i].head,cur_e,next_e)==ERROR-1) printf("      ��Ԫ�ز����ں��!\n");//��β��Ԫ�ط����������᷵��-1
         else if(NextElem(List[i].head,cur_e,next_e)==ERROR) printf("      ��Ԫ�ز�����!\n");
         else printf("      ���ĺ��Ϊ%d\n",next_e);
         getchar();getchar();
         break;
	   case 10:
         printf("      ����������λ�úͲ���Ԫ��ֵ��ע���벻Ҫ����С��1���ߴ���%d��������",ListLength(List[i].head)+1);
         scanf("%d%d",&j,&e);
         if(!ListInsert(List[i].head,j,e)) printf("      ����������󣬲���ʧ�ܣ�\n");
         else printf("      �����ɹ�!\n");
         getchar();getchar();
         break;
	   case 11:
		 if(ListEmpty(List[i].head)) {printf("      �����Ա�Ϊ�ձ����������������\n");getchar();getchar(); break;}
         printf("      ��������ɾ����Ԫ�ص�λ�ã�ע���벻Ҫ����С��1���ߴ���%d��������",ListLength(List[i].head));
         scanf("%d",&j);
         if(!ListDelete(List[i].head,j,e)) printf("      ����������󣬲���ʧ�ܣ�\n");
         else printf("      �����ɹ�����ɾ����Ԫ��Ϊ%d\n",e);
         getchar();getchar();
         break;
	   case 12:
		 if(!ListTrabverse(List[i].head)) printf("      �����Ա��ǿձ����������������\n");
		 getchar();getchar();
		 break;
       case 13:
         SaveFile(List[i]);
         break;
       case 14:  //����14���Է���ز鿴�������Ա����Ϣ���Ա������������
         if(!num) {printf("      ���������Ա���ڣ����������������\n"); getchar();getchar(); break;} //�ж��������Ա�
         printf("      �������Ա���Ϣ���£�\n");
         printf("      ���Ա����Ϊ%d\n",num);
         printf("      ���Ա�����Ƽ��������£�\n");
         for(j=1;j<=num;j++) //�������Ա����飬�������
         printf("      %20s    %d\n",List[j].name,ListLength(List[j].head));
         getchar();getchar();
	   case 0:
         getchar();getchar();
         break;
	}
  }
printf("\n��ӭ�´���ʹ�ñ�ϵͳ��\n");
}
/*--------------------------------------------*/
status InitList(LinkList &L)
{
	L=(LinkList)malloc(sizeof(LNode)); //Ϊͷ������ռ䣬ͷ��㲻�������ݱ���
    if(!L) return OVERFLOW;
    L->data=0; //ͷ��㲻�������ݣ���ֵΪ��
    L->next=NULL;
	return OK;
}
/*--------------------------------------------*/
status DestroyList(LinkList &L)
{
    LinkList p=L,q; //���������ṹָ��p��q��pָ����Ҫ�ͷŵĽ��ռ䣬qָ����Ҫ�ͷŵĽ��ĺ�̣���ֹ��㶪ʧ
    if(!L) return ERROR;
    while(p)
    {
        q=p->next; //ָ����һ�����
        free(p); //�ͷſռ�
        p=q;
    }
    L=NULL;
    return OK;
}
/*--------------------------------------------*/
status ClearList(LinkList L)
{
    LinkList p=L->next,q; //����ͷ���ռ䲻���ͷţ�������ͷ���
    if(!L) return ERROR;
    L->next=NULL; //��ͷ����ָ��ָ���
    while(p)
    {
        q=p->next;//��ɾ������Ĳ���һ��
        free(p);
        p=q;
    }
    return OK;
}
/*--------------------------------------------*/
status ListEmpty(LinkList L)
{
    if(!L->next) return TRUE;
    else return FALSE;
}
/*--------------------------------------------*/
int ListLength(LinkList L)
{
    int length=0;
    if(!L) return ERROR;
    LinkList p=L->next;
    while(p) //��������ͳ�Ƴ���
    {
        length++;
        p=p->next;
    }
    return length;
}
/*--------------------------------------------*/
status GetElem(LinkList L,int i,ElemType &e)
{
    int j=1; //�����������ڼ�����ǰλ����Ϣ
    LinkList p=L->next; //���ڲ��øı�ǰ��ָ����Ϣ������p��j����ȫͬ����
    while(p&&j<i) //����Ԫ�ز��һ�δ��������λ��
    {
        p=p->next;
        j++;
    }
    if(!p||i<1) return ERROR; //������λ�ó������Ա�ĳ��ȼ�����λ��Ϊ����
    else e=p->data;
    return OK;
}
/*--------------------------------------------*/
int LocateElem(LinkList L,ElemType e)
{
    int i; //�����������ڴ洢λ��
    LinkList p=L->next; //���ڲ��øı�ǰ��ָ����Ϣ������p��i����ȫͬ����
    for(i=1;p;i++)
    {
        if(p->data==e) return i;
        p=p->next;
    }
    return 0;
}
/*--------------------------------------------*/
status PriorElem(LinkList L,ElemType cur,ElemType &pre_e)
{
    LinkList p=L->next,q;
    if(cur==p->data) return -1; //��Ԫ��Ϊ��Ҫ����ǰ����Ԫ�أ�����-1���Ա��ERROR����
    q=p; p=p->next; //���ڷ���ǰ��
    while(p) //�����������Ա�
    {
        if(p->data==cur){
            pre_e=q->data;
            return OK;
        }
        q=p; p=p->next;
    }
    return ERROR;//����Ԫ�ز����ڣ�����ERROR
}
/*--------------------------------------------*/
status NextElem(LinkList L,ElemType cur,ElemType &next_e)
{
    LinkList p=L->next;
    while(p->next)//�����������Ա�
    {
        if(p->data==cur){
            next_e=p->next->data;
            return OK;
        }
        p=p->next;
    }
    if(cur==p->data) return ERROR-1; //�ж����һ��Ԫ��
    return ERROR;//����Ԫ�ز����ڣ�����ERROR
}
/*--------------------------------------------*/
status ListInsert(LinkList L,int i,ElemType e)
{
    int j=1; //�����������ڴ洢λ��
    LinkList p=L,pNew;  //�붨λԪ�����ƣ�����������Ҫ�ı�ǰ��ָ����Ϣ������p��Ҫ�ͺ���jһ��λ�ã����ڲ���Ԫ��
    while(p&&j<i) //���ڱ�length��1��λ���Կ��Բ���Ԫ�أ������
    {
        p=p->next;
        j++;
    }
    if(i<1||!p) return ERROR; //���ɲ���Ԫ�أ�����
    pNew=(LinkList)malloc(sizeof(LNode)); //�����µĽ�㣬��ֵ������
    pNew->data=e;
    pNew->next=p->next;
    p->next=pNew;
    return OK;
}
/*--------------------------------------------*/
status ListDelete(LinkList L,int i,ElemType &e)
{
    int j=1; //�����������ڴ洢λ��
    LinkList p=L,q; //�붨λԪ�����ƣ�����������Ҫ�ı�ǰ��ָ����Ϣ������p��Ҫ�ͺ���jһ��λ�ã����ڲ���Ԫ��
    while(p->next&&j<i) //ֻ�ܴ���λ��С�ڵ���length��Ԫ��
    {
        p=p->next;
        j++;
    }
    if(i<1||!p->next||!L->next) return ERROR;
    q=p->next; //��qָ��ȡ����㣬�����ͷſռ�
    p->next=q->next;
    e=q->data;
    free(q);
    return OK;
}
/*--------------------------------------------*/
status ListTrabverse(LinkList L)
{
    LinkList p=L->next;
    printf("\n----------------all elements --------------------\n");
    while(p)
    {
        printf("%d ",p->data); //�������Ա����Ԫ��
        p=p->next;
    }
    printf("\n------------------ end ------------------------\n");
    return OK;
}
/*--------------------------------------------*/
status OpenFile(LinkList L)
{
    ElemType j;
    LinkList p=L,q;
    FILE* fp; char filename[30]; int i;
    printf("      �������ļ�����");
    scanf("%s",filename);
    if (!(fp=fopen(filename,"r"))) {printf("      ���ļ�ʧ��!\n"); return ERROR;} //�ļ���ֵ�����ļ�
	while(fscanf(fp,"%d",&j)!=EOF) //���ļ��ж�ȡ���ݣ������ļ�βֹͣ
    {
        q=(LinkList)malloc(sizeof(LNode));
        q->data=j;
        p->next=q;
        q->next=NULL;
        p=p->next;
    }
    printf("      ���ļ��ɹ�!\n");
    fclose(fp); //�ر��ļ�
    getchar();
    return OK;
}
/*--------------------------------------------*/
status SetValue(LinkList L) //������������
{
    ElemType j;
    LinkList p=L,q;
    printf("      ������Ԫ��ֵ����Ctrl+Z����:\n");
    while(scanf("%d",&j)!=EOF)
    {
        q=(LinkList)malloc(sizeof(LNode));
        q->data=j;
        p->next=q;
        q->next=NULL;
        p=p->next;

    }
    printf("      ��ֵ�ɹ���\n");
    return OK;
}
/*--------------------------------------------*/
status SaveFile(LkList L)
{
    FILE* fp;
    int num;
    LinkList p=L.head->next;
    char filename[30];
    printf("      ��ѡ��1.��ɾ��ԭ�е����Ա������ļ�����׷��\n              2.ɾ��ԭ�����Ա���������ļ�\n\n");
    scanf("%d",&num);
    printf("      �������ļ�����");
    scanf("%s",filename);
    if(num==1&&(fp=fopen(filename,"a"))==NULL) //�ļ��򿪷�ʽΪ׷�ӣ�����ԭ�е����ݽ����޸�
    {
        printf("      �ļ��ļ���ʧ�ܣ�\n");
        return ERROR;
    }
    else if(num==2&&(fp=fopen(filename,"w"))==NULL) //�ļ��򿪷�ʽΪ���»򴴽����ļ�����ɾ��ԭ������
    {
        printf("      �ļ��ļ���ʧ�ܣ�\n");
        return ERROR;
    }
    fputs(L.name,fp);//������Ա������
    fprintf(fp,"\n");//����
    while(p) //�����������
    {
        fprintf(fp,"%d ",p->data);
        p=p->next;
    }
    fprintf(fp,"\n");
    fclose(fp);//�ر��ļ�
    printf("      ����ɹ���\n");
    getchar();getchar();
    return OK;
}
/*--------------------------------------------*/









/*for(i=0;i<num_of_liter;i++)
        printf("%d %d\n",literal[i].liter,literal[i].sum);*/



/*while(p)
    {
       // printf("1\n");
        q=root->next; r=root; judge=0;
        if(isUnitClause(p))
        {
           // printf("2222\n");
            liter=p->first->liter;
            while(q)
            {
                //printf("3333\n");
                pl=q->first; pl1=pl; judge=0;
                if(pl->liter+liter==0)
                {
                    if(!pl->next) return ERROR;
                    q->first=pl->next;
                    pl1=q->first;
                    free(pl);
                    pl=pl1;
                    //printf("5555\n");
                }
                else if(pl->liter==liter) judge=1;
                else pl=pl1,pl=pl->next;
                while(pl)
                {
                    //printf("%d\n",pl->liter);
                    if(pl->liter==liter){judge=1; break;}
                    else if(pl->liter+liter==0){pl1->next=pl->next;free(pl);pl=pl1->next;}
                    else {pl1=pl;pl=pl->next;}
                }
                if(judge==1)
                {
                    r->next=q->next;
                    destroyClause(q);
                    q=r->next;
                    num_of_clause--;
                    //printf("%d\n",num_of_clause);
                }
                else r=q,q=q->next;
            }
            p=root->next;
        }
        else p=p->next;
    }*/




