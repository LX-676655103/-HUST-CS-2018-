/*--------------------------------------------*/
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
/*--------------------------------------------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
/*--------------------------------------------*/
#define N 100
#define LIST_INIT_SIZE 2000
/*--------------------------------------------*/
typedef int status;
typedef int Sign;  //���ؼ���Ԫ�����Ͷ���
typedef int ElemType; //����Ԫ�����Ͷ���
/*--------------------------------------------*/
typedef struct BiTNode{
    Sign Node_num; //���ؼ��ֱ���
    ElemType data; //������ݱ���
    struct BiTNode *lchild; //����ָ�����
    struct BiTNode *rchild; //�Һ���ָ�����
}BiTNode,*LinkBiTNode; //LinkBiTNodeΪ���ָ���������
typedef struct LNode{
    LinkBiTNode Node;
    struct LNode *next;
}LNode,*LinkList; //��Ϊ�洢������������������ʱ��ֱ����������
typedef struct{
    LinkBiTNode root;
    char name[30];
}BiTrees; //ɭ�����Ա����
typedef struct{
    LinkBiTNode Node;
    int num; //��¼��������������е���ţ����ڽ�������ͼ�λ����
}Node; //�������Ա�������洢�����Ϣ
/*--------------------------------------------*/
status ReadBiNode(LinkList &front,LinkList &rear); //�������������ǰ���������
status ReadBiNode2(LinkBiTNode a[],int &n); //����ǰ��������������
status ReadBiNode3(Sign b[],int &n);  //���������������
status CreateBiTree1(LinkBiTNode &T,LinkList &front,LinkList &rear); //�ô���������ǰ������������ɶ�����
status CreateBiTree2(LinkBiTNode &T,LinkBiTNode a[],Sign b[],int n); //��ǰ��+��������������ɶ�����
status CreateBiTree3(LinkBiTNode &T,LinkBiTNode a[],Sign b[],int n); //�ú���+��������������ɶ�����
status DestroyBiTree(LinkBiTNode &T); //���ٶ�����
status ClearBiTree(LinkBiTNode &T);   //��ն�����
status BiTreeEmpty(LinkBiTNode T);    //�жϿն�����
int BiTreeDepth(LinkBiTNode T);       //����������
LinkBiTNode LocateNode(LinkBiTNode T,Sign e);        //���ҽ��
status Assign(LinkBiTNode T,Sign &e,ElemType value); //��㸳ֵ
LinkBiTNode GetSibling(LinkBiTNode T,Sign e);  //����ֵܽ��
status InsertNode(LinkBiTNode T,Sign &e,int LR,LinkBiTNode c);
status DeleteNode(LinkBiTNode &T,Sign &e);
status PreOrderTraverse(LinkBiTNode T);
status InOrderTraverse(LinkBiTNode T);
status PostOrderTraverse(LinkBiTNode T);
status LevelOrderTraverse(LinkBiTNode T); //�������������
status OpenFile(LinkList &front,LinkList &rear); //���ļ��ж�ȡ����
status SaveFile(BiTrees T); //����������Ϣ�Դ���������ǰ������������ļ���
status ShowTree(LinkBiTNode T); //ͼ�λ���ʾ������Ϣ
int pow(int num,int n); //�Զ���ƽ�����������ڼ���������ƽ��
status Assign_name(BiTrees &T,BiTrees a[],int num);
/*--------------------------------------------*/
int main(){
BiTrees BiTList[N]; //ɭ�֣����������Ա����
int op=1; //�˵�ѡ��������
int i=0,j,k; //�����������ڼ�����������λ��
int num=0,n1,n2; //�������ĸ����������Լ�ǰ�򡢺����Լ�����ĳ���
Sign e;ElemType value; char name[30];
LinkBiTNode locate;
LinkList front,rear;
LinkBiTNode a[LIST_INIT_SIZE];
Sign b[LIST_INIT_SIZE];
while(op){
	system("cls");	printf("\n\n");
	printf("      Menu for Binary Tree On Binary Linked List Structure \n");
	printf("--------------------------------------------------------------\n");
	printf("    	  1. CreateBiTree       9. InsertNode\n");
	printf("    	  2. DestroyBiTree      10. DeleteNode\n");
	printf("    	  3. ClearBiTree        11. PreOrderTraverse\n");
	printf("    	  4. BiTreeEmpty        12. InOrderTraverse\n");
	printf("    	  5. BiTreeDepth        13. PostOrderTraverse\n");
	printf("    	  6. LocateNode         14. LevelOrderTraverse\n");
	printf("    	  7. Assign             15. SaveFile\n");
	printf("    	  8. GetSibling         16. BiTreeMessage\n"); //��ʾ���ж����������Լ������Ϣ
	printf("    	  0. Exit               17. ShowTree\n"); //������ʾͼ�λ���
	printf("--------------------------------------------------------------\n");
	printf("      ��ѡ����Ĳ���[0~16]:");
	scanf("%d",&op);
	if(op>=2&&op<=15||op==17)
    {
        if(num)  //num����������������������˴��ж��Ƿ��ж�����
        {
            printf("      ��������Ҫ�����Ķ����������ƣ�");//ͨ�����Ʋ��Ҷ�����
            scanf("%s",name);
            for(i=1;i<=num;i++) //��������
                if(strcmp(BiTList[i].name,name)==0) break;
            if(i>num) {printf("      �����ҵĶ����������ڣ������²�����\n\n"); getchar(); getchar();continue;}
        }
        else {printf("      ��ǰ�޶��������ڣ����ȴ�����������\n");getchar(); getchar();continue;}
    }
    switch(op){
        case 1:
        int cho,cho_1; //���ڴ��洴����������ʽ�ļ�����
        printf("      ��ѡ�񴴽��������ķ�ʽ��\n");
        printf("          1.ʹ�ô��������Ķ�����ǰ������������ɶ�����\n");
        printf("          2.ʹ�ö�����ǰ���������+��������������ɶ�����\n");
        printf("          3.ʹ�ö����������������+��������������ɶ�����\n      ");
        scanf("%d",&cho);
        switch(cho){
            case 1:
                printf("      ��ѡ��������еķ�ʽ��\n");
                printf("          1.ʹ�ü��̸�ֵ\n");
                printf("          2.ʹ���ļ���ֵ\n      ");
                scanf("%d",&cho_1);
                if(cho_1==1)
                {
                    printf("      ������ϴ���������ǰ��������У����е�ÿһ���ɹؼ���-����������ɣ�\n");
                    printf("      �м��ÿո����������֮���û��з��������ؼ���ΪΨһȷ������������\n");
                    printf("      ����Ϊ�����������������ؼ�����-1���棬������������������0��������Ctrl+Z������\n");
                    k=ReadBiNode(front,rear); //�������������ǰ���������
                    if(k==OVERFLOW) printf("      �����");
                }
                if(cho_1==2)
                {
                    k=OpenFile(front,rear); //���ļ���ȡ����������ǰ���������
                    if(k==OVERFLOW) printf("      �����");
                    if(k==ERROR) printf("      ���ļ�ʧ��!\n");
                    if(k==OK) printf("      ���ļ��ɹ�!\n");
                }
                j=CreateBiTree1(BiTList[++num].root,front,rear); //�ô���������ǰ��������д���������
                if(j==ERROR) //������Ƿ�ʱ
                {
                    while(j==ERROR)//һֱ��ʾ��������ֱ��������ȷ
                    {
                        num--; //ɭ�������ĸ�����һ
                        printf("      ����Ƿ�������������:");
                        ReadBiNode(front,rear); //���¶�������
                        j=CreateBiTree1(BiTList[++num].root,front,rear);
                    }
                }
                if(j==OK)
                {
                    printf("      �����������ɹ�����Զ���������������");
                    while(!Assign_name(BiTList[num],BiTList,num)) //�����������������и�ֵ
                        printf("      �ö����������Ѵ��ڣ����������룺");
                    printf("      �����������ɹ���\n");
                }
                break;
	        case 2: //����ǰ����������Լ�����������н��д������������ܽ����ο���
	            printf("      ������Ϸ���ǰ��������У����е�ÿһ���ɹؼ���-����������ɣ��м��ÿո������\n");
                printf("      ����֮���û��з��������ؼ���ΪΨһȷ����������������Ϊ����������������Ctrl+Z������\n");
                k=ReadBiNode2(a,n1); //����ǰ���������
                printf("      ������Ϸ�������������У����е�ÿһ��ֻ���йؼ��֣�\n");
                printf("      ����֮���ÿո�������ؼ���ΪΨһȷ������������������Ctrl+Z������\n");
                k=ReadBiNode3(b,n2); //���������������
                if(n1!=n2) {printf("      ����Ƿ������������������\n"); break;}
                j=CreateBiTree2(BiTList[++num].root,a,b,n1);
                if(j==OK)
                {
                    printf("      �����������ɹ�����Զ���������������");
                    while(!Assign_name(BiTList[num],BiTList,num))
                        printf("      �ö����������Ѵ��ڣ����������룺\n");
                    printf("      �����������ɹ���\n");
                }
                else
                {
                    printf("      ����Ƿ������������������\n");
                    num--;
                }
	            break;
	        case 3:  //���ú�����������Լ�����������н��д������������ܽ����ο���
                printf("      ������Ϸ��ĺ���������У����е�ÿһ���ɹؼ���-����������ɣ��м��ÿո������\n");
                printf("      ����֮���û��з��������ؼ���ΪΨһȷ����������������Ϊ����������������Ctrl+Z������\n");
                k=ReadBiNode2(a,n1); //��������������
                printf("      ������Ϸ�������������У����е�ÿһ��ֻ���йؼ��֣�\n");
                printf("      ����֮���ÿո�������ؼ���ΪΨһȷ������������������Ctrl+Z������\n");
                k=ReadBiNode3(b,n2); //���������������
                j=CreateBiTree3(BiTList[++num].root,a,b,n1);
                if(j==OK)
                {
                    printf("      �����������ɹ�����Զ���������������");
                    while(!Assign_name(BiTList[num],BiTList,num))
                        printf("      �ö����������Ѵ��ڣ����������룺\n");
                    printf("      �����������ɹ���\n");
                }
                else
                {
                    printf("      ����Ƿ������������������\n");
                    num--;
                }
	            break;
        }
        getchar();getchar();
        break;
	   case 2:
         if(DestroyBiTree(BiTList[i].root)==OK)
		 {
                printf("      ���������ٳɹ���\n");
                for(;i<num;i++)  //�������������Ҫ�����ٵĶ�����ɾ�����ǣ�BiTList[0]���洢��Ϣ
                    BiTList[i]=BiTList[i+1];
                num--;
		 }
		 getchar();getchar();
		 break;
	   case 3:
         if(ClearBiTree(BiTList[i].root)==OK)
            printf("      ��������ճɹ���\n");
         else printf("      �˶������Ѿ�Ϊ�գ�\n");
		 getchar();getchar();
		 break;
	   case 4:
         if(BiTreeEmpty(BiTList[i].root)==TRUE)
            printf("      �ö�����Ϊ�գ�\n");
         else printf("      �ö�������Ϊ�գ�\n");
		 getchar();getchar();
		 break;
	   case 5:
         printf("      �ö����������Ϊ%d\n",BiTreeDepth(BiTList[i].root));
		 getchar();getchar();
		 break;
	   case 6:
         printf("      ������������ҵĽ��Ĺؼ��֣�����������");
         scanf("%d",&e);
         locate=LocateNode(BiTList[i].root,e);
         if(locate) printf("      �����ɹ��������ҵĽ��������Ϊ%d\n",locate->data);
         else printf("      �������ҵĽ�㲻���ڣ�����ʧ�ܣ�\n");
		 getchar();getchar();
		 break;
	   case 7:
         printf("      ���������븳ֵ�Ľ��Ĺؼ��֣����������������");
         scanf("%d %d",&e,&value);
         Assign(BiTList[i].root,e,value);
         if(e==-1) printf("      ��㸳ֵ�ɹ���\n"); //����ֵ�����������У�e�ĸ�ֵ�ᷢ���ı䣨���������
         else printf("      ������ֵ�Ľ�㲻���ڣ�����ʧ�ܣ�\n");
		 getchar();getchar();
		 break;
	   case 8:
         printf("      �����������ȡ�ֵܽ��Ľ��Ĺؼ��֣�����������");
         scanf("%d",&e);
         locate=GetSibling(BiTList[i].root,e);   //locate���ڴ洢���ص�ָ��ֵ
         if(locate) printf("      �����ɹ��������ȡ���ֵܽ��Ĺؼ���Ϊ%d,������Ϊ%d\n",locate->Node_num,locate->data);
         else if(!LocateNode(BiTList[i].root,e)) printf("      �ý�㲻���ڣ�����ʧ�ܣ�\n"); //�Ƿ��������㲻����
         else printf("      �ֵܽ�㲻���ڣ�����ʧ�ܣ�\n"); //�Ƿ�������ֵܽ���㲻����
         getchar();getchar();
         break;
	   case 9:
         int LR; ElemType a; Sign b;
         LinkBiTNode c; //ָ�����ɵĽ��
         printf("      �����������ĸ��ڵ�ؼ��֣�����������");
         scanf("%d",&e);
         if(!LocateNode(BiTList[i].root,e)) //�жϲ���λ���Ƿ����
         {
             printf("      �ò���λ�㲻���ڣ�����ʧ�ܣ�\n");
             getchar();getchar();
             break;
         }
         printf("      ��������Ϊ���㣬������0����Ϊ�ҽ�㣬������1��");
         scanf("%d",&LR);
         if(LR!=0&&LR!=1)
         {
             printf("      LR������󣬲���ʧ�ܣ�\n");
             getchar();getchar();
             break;
         }
         printf("      ����������Ϣ����Ϣ�йؼ���-����������ɣ��м��ÿո������");
         while(scanf("%d %d",&b,&a)!=EOF&&LocateNode(BiTList[i].root,b))
              printf("      �ùؼ�����ʹ�ã�����������ؼ�����������:"); //�жϵ�ǰ�ؼ����Ƿ�Ϸ�
         c=(LinkBiTNode)malloc(sizeof(BiTNode)); //���ɽ��
         c->Node_num=b;
         c->data=a;
         InsertNode(BiTList[i].root,e,LR,c);
         if(e==-1) printf("      ������ɹ���\n");
         else printf("      ����λ�㲻���ڣ�����ʧ�ܣ�\n");
         getchar();getchar();
         break;
	   case 10:
         printf("      ����������ɾ���Ľ��Ĺؼ��֣�����������");
         scanf("%d",&e);
         DeleteNode(BiTList[i].root,e);
         if(e==-1) printf("      ���ɾ���ɹ���\n");
         else printf("      ��Ҫɾ���Ľ�㲻���ڣ�����ʧ�ܣ�\n");
         getchar();getchar();
         break;
	   case 11:
         printf("\n----------------all elements --------------------\n");
         PreOrderTraverse(BiTList[i].root);
         printf("\n------------------ end --------------------------\n");
         getchar();getchar();
         break;
	   case 12:
         printf("\n----------------all elements --------------------\n");
         InOrderTraverse(BiTList[i].root);
         printf("\n------------------ end --------------------------\n");
		 getchar();getchar();
		 break;
       case 13:
         printf("\n----------------all elements --------------------\n");
         PostOrderTraverse(BiTList[i].root);
         printf("\n------------------ end --------------------------\n");
         getchar();getchar();
         break;
       case 14:
         printf("\n----------------all elements --------------------\n");
         LevelOrderTraverse(BiTList[i].root);
         printf("\n------------------ end --------------------------\n");
         getchar();getchar();
         break;
       case 15:
         SaveFile(BiTList[i]);
         getchar();getchar();
         break;
       case 16:
         if(!num) {printf("      �����޶��������ڣ����������������\n"); getchar();getchar(); break;} //�ж��������Ա�
         printf("      ���ȶ�������Ϣ���£�\n");
         printf("      ����������Ϊ%d\n",num);
         printf("      �����������Ƽ�������£�\n");
         for(j=1;j<=num;j++)    //�������Ա����飬�������
         printf("      %20s    %d\n",BiTList[j].name,BiTreeDepth(BiTList[j].root));
         getchar();getchar();
         break;
       case 17:
         ShowTree(BiTList[i].root);   //ͼ�λ���ʾ��
         getchar();getchar();
         break;
	   case 0:
         break;
	}
  }
printf("\n��ӭ�´���ʹ�ñ�ϵͳ��\n");
}
/*--------------------------------------------*/
 //���ж��뺯�����������������ǰ��������У������ɽ�㴮����������������洢���
status ReadBiNode(LinkList &front,LinkList &rear)
{
    LinkList pNew;
    ElemType Ndata;
    Sign Snum;
    front=(LinkList)malloc(sizeof(LNode));
    if(!front) return OVERFLOW;
    front->Node==NULL;  //��������һ��ָ������ָ��������Լ�һ��ָ����
    front->next=NULL;
    rear=front; //��������
    while(scanf("%d %d",&Snum,&Ndata)!=EOF) //��������
    {
        pNew=(LinkList)malloc(sizeof(LNode)); //�����µ�������
        if(!pNew) return OVERFLOW;
        pNew->Node=(LinkBiTNode)malloc(sizeof(BiTNode)); //�����µ������
        pNew->next=NULL;
        pNew->Node->Node_num=Snum; //���������и�ֵ
        pNew->Node->data=Ndata;
        rear->next=pNew; //�����Ա�
        rear=pNew; //βָ�����
    }
    return OK;
}
/*--------------------------------------------*/
//���ж��뺯����������ͨǰ��������У������ɽ�㴮���������������Ա�洢���
status ReadBiNode2(LinkBiTNode a[],int &n)
{
    int i=0;
    LinkBiTNode pNew;
    ElemType Ndata;
    Sign Snum;
    while(scanf("%d %d",&Snum,&Ndata)!=EOF)
    {
        pNew=(LinkBiTNode)malloc(sizeof(BiTNode)); //�����µĽ��
        if(!pNew) return OVERFLOW;
        pNew->Node_num=Snum;
        pNew->data=Ndata;
        a[i++]=pNew;    //�����������Ա�
    }
    n=i; //����n���ڷ��س��ȣ������ж��Ƿ�Ϸ�
    return OK;
}
/*--------------------------------------------*/
status ReadBiNode3(Sign b[],int &n) //���ж��뺯������������������У���������������洢���
{
    int i=0;
    while(scanf("%d",&b[i++])!=EOF);
    n=i-1; //����n���ڷ��س��ȣ������ж��Ƿ�Ϸ�
    return OK;
}
/*--------------------------------------------*/
status CreateBiTree1(LinkBiTNode &T,LinkList &front,LinkList &rear) //����������������ʹ�ô���������ǰ��������д���������
{
    int flag=1;  //�жϱ�������flag==1�������Ϊ���㣻��flag==0�������Ϊ�ҽ��
    LinkList head=front->next,top=front,temp; //headָ��ָ��Ϊ�������Ľ�㴮��topָ������ָ��һ��ģ��ջ
    LinkBiTNode root; //rootָ����Ҫ�����ӽ�㸳ֵ�Ľ��
    if(!front->next) return ERROR;
    T=head->Node;     //�����һ�����
    top=NULL;         //ջΪ��
    temp=head->next;  //ָ����һ����Ҫ����Ľ��
    head->next=top;   //�����ջ
    top=head;         //�����ջ
    while(top&&temp)  //��ջ��Ϊ��������Ҫ����Ľ��
    {
        root=top->Node;
        if(!flag) top=top->next; //�������ӽ�㣬������ջ
        head=temp;    //���ڽ������ջ
        if(head->Node->Node_num!=-1) //������Ľ�㲻Ϊ��
        {
            if(flag) root->lchild=head->Node; //����ֻ��������Ϊ��ʱ���Ż�����ҽ�㣬��flagΪ1ʱһֱ���������
            else root->rchild=head->Node;
            temp=head->next; //ָ����һ����Ҫ�����Ľ��
            head->next=top;  //�����ջ
            top=head;        //�����ջ
            flag=1;          //��һ������Բ��������
        }
        else //������Ľ��Ϊ��
        {
            if(flag) root->lchild=NULL;
            else root->rchild=NULL;
            temp=head->next;
            flag=0;         //��һ�����������ұ�
        }
    }
    //�Ƿ�����ж�
    if(top&&!temp) return ERROR; //�Ƿ����1��ջΪ�յ����н����Ҫ����
    if(!top&&temp)               //�Ƿ����2��ջ��Ϊ�յ�û�н����Բ���
    {
        DestroyBiTree(T);
        return ERROR;
    }
    return OK;
}
/*--------------------------------------------*/
status CreateBiTree2(LinkBiTNode &T,LinkBiTNode a[],Sign b[],int n) //����������������ʹ��ǰ��+����������д���������
{
    int i=0;
    if(n==0){
        T=NULL;
        return OK;
    }   //������Ϊ�գ�����㸳ֵΪ��
	for(i=0;i<n;i++)  //���ҽ��
    {
        if(b[i]==a[0]->Node_num)//���ҳɹ�
        {
            T=a[0]; //��������
            if(CreateBiTree2(T->lchild,&a[1],b,i)==ERROR) return ERROR; //�������
            if(CreateBiTree2(T->rchild,&a[1+i],&b[i+1],n-i-1)==ERROR) return ERROR; //�ҽ�����
            break;
        }
    }
    if(i>=n) return ERROR;
    else return OK;
}
/*--------------------------------------------*/
status CreateBiTree3(LinkBiTNode &T,LinkBiTNode a[],Sign b[],int n)
{
    int i=0;
	if(n==0) { T=NULL; return OK; }
    for(i=0;i<n;i++)
    {
        if (b[i]==a[n-1]->Node_num)
        {
            T=a[n-1];
            if(CreateBiTree3(T->lchild,a,b,i)==ERROR) return ERROR; //�������
            if(CreateBiTree3(T->rchild,&a[1+i],&b[i+1],n-i-1)==ERROR) return ERROR; //�ҽ�����
            break;
        }
    }
	if(i>=n) return ERROR;
    else return OK;
}
/*--------------------------------------------*/
status DestroyBiTree(LinkBiTNode &T)
{
    if(!T) return OK;
    else
    {
        DestroyBiTree(T->rchild);
        DestroyBiTree(T->lchild);
        free(T); //�ͷſռ�
        T=NULL;  //���ָ��յ�ַ
        return OK;
    }
}
/*--------------------------------------------*/
status ClearBiTree(LinkBiTNode &T)
{
    if(!T) return ERROR;
    else
    {
        ClearBiTree(T->rchild);
        ClearBiTree(T->lchild);
        free(T); //�ͷſռ�
        T=NULL;  //���ָ��յ�ַ
        return OK;
    }
}
/*--------------------------------------------*/
status BiTreeEmpty(LinkBiTNode T)
{
    if(!T) return TRUE;
    else return FALSE;
}
/*--------------------------------------------*/
int BiTreeDepth(LinkBiTNode T)
{
    int ldepth,rdepth;
    if(!T) return 0; //�������ս�㣬��ֱ�ӷ���0
    ldepth=BiTreeDepth(T->lchild)+1;
    rdepth=BiTreeDepth(T->rchild)+1;
    if(ldepth>=rdepth) return ldepth;
    else return rdepth; //������ȸ������ֵ

}
/*--------------------------------------------*/
LinkBiTNode LocateNode(LinkBiTNode T,Sign e)
{
    LinkBiTNode lp,rp;
    if(!T) return NULL; //�ݹ����1���ҵ��ս��
    if(e==T->Node_num) return T; //�ݹ����2���ҵ����
    lp=LocateNode(T->lchild,e);
    if(lp) return lp;
    rp=LocateNode(T->rchild,e);
    if(rp) return rp;
    return NULL;
}
/*--------------------------------------------*/
status Assign(LinkBiTNode T,Sign &e,ElemType value)
{
    LinkBiTNode lp,rp;
    if(!T) return OK;
    if(e==T->Node_num) //�����ҳɹ�
    {
        T->data=value;
        e=-1;    //��Ǹ�ֵ
        return OK;
    }
    Assign(T->lchild,e,value); //�ݹ��������
    Assign(T->rchild,e,value); //�ݹ�����ҽ��
    return OK;
}
/*--------------------------------------------*/
LinkBiTNode GetSibling(LinkBiTNode T,Sign e)
{
    LinkBiTNode lp,rp;
    if(!T||!T->lchild||!T->rchild) return NULL; //���Լ��ٵݹ����
    if(e==T->lchild->Node_num) return T->rchild;
    else if(e==T->rchild->Node_num) return T->lchild;
    lp=GetSibling(T->lchild,e);
    if(lp) return lp;
    rp=GetSibling(T->rchild,e);
    if(rp) return rp;
    return NULL;
}
/*--------------------------------------------*/
status InsertNode(LinkBiTNode T,Sign &e,int LR,LinkBiTNode c)
{
    LinkBiTNode temp;
    if(!T) return OK;
    if(e==T->Node_num) //�ɹ����ҵ����
    {
        e=-1;   //���ڱ���Ƿ��Ѿ�����
        if(!LR) //LR==0�����Ϊ����
        {
            temp=T->lchild;
            T->lchild=c;
        }
        else if(LR==1)  //LR==1�����Ϊ�ҽ��
        {
            temp=T->rchild;
            T->rchild=c;
        }
        else return ERROR;
        c->rchild=temp;
        c->lchild=NULL;
        return OK;
    }
    if(!InsertNode(T->lchild,e,LR,c)) return ERROR;
    if(!InsertNode(T->rchild,e,LR,c)) return ERROR;
    return OK;
}
/*--------------------------------------------*/
status DeleteNode(LinkBiTNode &T,Sign &e)
{
    LinkBiTNode temp,p;
    if(!T) return OK;
    if(e==T->Node_num)
    {
        e=-1;  //���ڱ���Ƿ��Ѿ�����
        temp=T;
        if(temp->lchild) //��Ҫɾ���Ľ������Ӵ���
        {
            T=temp->lchild; //�����������Ҫɾ���Ľ��
            if(temp->rchild) //�ҽ�����
            {
                p=temp->lchild;
                while(p->rchild) p=p->rchild; //���������������ҽ��
                p->rchild=temp->rchild;   //����������
                free(temp);
            }
            else free(temp);
        }
        else
        {
            if(temp->rchild) T=temp->rchild; //ֻ��һ�������������ֱ�����
            else T=NULL;
        }
        return OK;
    }
    DeleteNode(T->lchild,e);
    DeleteNode(T->rchild,e);
    return OK;
}
/*--------------------------------------------*/
status PreOrderTraverse(LinkBiTNode T) //�ǵݹ�ʵ�ֵ�ǰ��������
 {
     int k=0;
     LinkBiTNode st[LIST_INIT_SIZE],temp=T; //ʹ��ջ������
     if(!T) return ERROR; //����
     while(k||temp)   //ջ��Ϊ�ջ��ߵ�ǰ���δ����
     {
         while(temp)  //ѭ������������
         {
             st[k++]=temp; //��ջ
             printf("%d ",temp->data); //������
             temp=temp->lchild; //����������
         }
         temp=st[--k]; //����Ϊ�գ�����ǰ��㸳����Ҫ�����temp����ջ
         temp=temp->rchild; //ָ���ҽ��
     }
     return OK;
 }
/*--------------------------------------------*/
status InOrderTraverse(LinkBiTNode T) //�ǵݹ�ʵ�ֵ���������������ǰ������
{
    int k=0;
    LinkBiTNode st[LIST_INIT_SIZE],temp=T;
    if(!T) return ERROR;
    while(k||temp)
    {
        while(temp)
        {
            st[k++]=temp;
            temp=temp->lchild;
        }
        temp=st[--k];
        printf("%d ",temp->data);//����ڳ�ջʱ�����ǰ���������ջʱ���
        temp=temp->rchild;
    }
    return OK;
}
/*--------------------------------------------*/
status PostOrderTraverse(LinkBiTNode T)
{
    if(!T) return OK;
    else
    {
        PostOrderTraverse(T->lchild); //�ݹ����������
        PostOrderTraverse(T->rchild); //�ݹ����������
        printf("%d ",T->data);        //���ʸ�ָ��
    }
    return OK;
}
/*--------------------------------------------*/
status LevelOrderTraverse(LinkBiTNode T) //�������������һ��������ģ��Ķ���ʵ��
{
    LinkList front,rear,pNew; //��ͷָ�롢��βָ���Լ���ǰָ�����
    LinkBiTNode temp=T;
    front=(LinkList)malloc(sizeof(LNode));
    front->Node=T;   //�����׸���㣨����������ͷ��㣩
    front->next=NULL;
    rear=front;
    printf("%d ",T->data);
    while(front) //�ӷǿ�
    {
        temp=front->Node;
        if(temp->lchild) //�������
        {
            pNew=(LinkList)malloc(sizeof(LNode));
            pNew->Node=temp->lchild;
            pNew->next=NULL;
            rear->next=pNew;
            rear=pNew;
            printf("%d ",rear->Node->data);
        }
        if(temp->rchild) //�ҽ�����
        {
            pNew=(LinkList)malloc(sizeof(LNode));
            pNew->Node=temp->rchild;
            pNew->next=NULL;
            rear->next=pNew;
            rear=pNew;
            printf("%d ",rear->Node->data);
        }
        front=front->next; //������
    }
    return OK;
}
/*--------------------------------------------*/
status OpenFile(LinkList &front,LinkList &rear)//����뺯�����ƣ�����Ϊ��ȡ�ļ�ģʽ
{
    LinkList pNew;
    ElemType Ndata;
    Sign Snum;
    FILE* fp; char filename[30]; int i;
    printf("      �������ļ�����");
    scanf("%s",filename);
    if (!(fp=fopen(filename,"r"))) return ERROR;
    front=(LinkList)malloc(sizeof(LNode));
    if(!front) return OVERFLOW;
    front->Node==NULL;
    front->next=NULL;
    rear=front;
    while(fscanf(fp,"%d %d",&Snum,&Ndata)!=EOF)
    {
        pNew=(LinkList)malloc(sizeof(LNode));
        if(!pNew) return OVERFLOW;
        pNew->Node=(LinkBiTNode)malloc(sizeof(BiTNode));
        pNew->next=NULL;
        pNew->Node->Node_num=Snum;
        pNew->Node->data=Ndata;
        rear->next=pNew;
        rear=pNew;
    }
    fclose(fp); //�ر��ļ�
    getchar();
    return OK;
}
/*--------------------------------------------*/
status SaveFile(BiTrees T)
{
    FILE* fp;
    int num;
    int k=0;
    LinkBiTNode st[LIST_INIT_SIZE],temp=T.root;
    char filename[30];
    printf("      ��ѡ��1.��ɾ��ԭ�еĶ������������ļ�����׷��\n              2.ɾ��ԭ�����ݻ��������ļ�\n\n");
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
    fputs(T.name,fp);
    fprintf(fp,"\n");//����
    while(k||temp)
    {
        while(temp)
        {
            st[k++]=temp;
            fprintf(fp,"%d ",temp->Node_num);
            fprintf(fp,"%d\n",temp->data);
            temp=temp->lchild;
        }
        fprintf(fp,"-1 0\n");
        temp=st[--k];
        temp=temp->rchild;
    }
    fprintf(fp,"-1 0\n");
    fprintf(fp,"\n\n");
    fclose(fp);//�ر��ļ�
    printf("      ����ɹ���\n");
    return OK;
}
/*--------------------------------------------*/
status ShowTree(LinkBiTNode T) //ͼ�λ���ʾ�������������ö��У����Խ����ļ����
{
    FILE* fp;
    char filename[30];
    int i,front=0,front2=0,rear=0,j,k,op;
    int n=BiTreeDepth(T);
    Node num[2000];
    LinkBiTNode temp=T;
    num[rear].Node=temp;//���ڴ洢���
    num[rear++].num=1;  //num�����洢��������������е����
    while(front<rear)
    {
        temp=num[front].Node;
        if(temp->lchild) //�������
        {
            num[rear].Node=temp->lchild;
            num[rear++].num=num[front].num*2;   //���������������ʿ�֪�����ӵ����Ϊ�������ŵ�����
        }
        if(temp->rchild) //�Һ������
        {
            num[rear].Node=temp->rchild;
            num[rear++].num=num[front].num*2+1; //���������������ʿ�֪���Һ��ӵ����Ϊ�������ŵ�����+1
        }
        front++;
    }
    printf("      ��ѡ��������ķ�ʽ��\n");
    printf("          1.����Ļ��ʾ����������ʾ��ȳ���5������\n");
    printf("          2.���浽�ļ�\n");
    scanf("%d",&op);
    if(op==2)
    {
        printf("      �������ļ�����");
        scanf("%s",filename);
        if((fp=fopen(filename,"w"))==NULL) //�ļ��򿪷�ʽΪ���»򴴽����ļ�����ɾ��ԭ������
        {
            printf("      �ļ��ļ���ʧ�ܣ�\n");
            return ERROR;
        }
    }
    for(i=0,front=0;i<n;i++)  //i����ֵ��ʾ�������Ĳ�����1������ѭ������
    {
        for(k=0;k<pow(2,n-i)-2;k++) //��ӡ��ÿһ���ʼ�Ŀո�
        {
            if(op==2) fprintf(fp," ");
            else printf(" ");
        }
        for(j=pow(2,i);j<pow(2,i+1);j++) //�ڶ������������������������ű���
        {
            if(j<num[front].num) //����ǰ��ű�ʾ�Ľ��Ϊ�գ����ӡ�ո�
                for(k=0;k<pow(2,n-i+1);k++) //ѭ����ӡ�ո�
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
            else if(j==num[front].num) //����ǰ��ű�ʾ�Ľ�����
            {
                if(num[front].Node->lchild)  //�����Ӵ��ڣ����ӡ�»���
                    for(k=0;k<pow(2,n-i)-2;k++)
                    {
                        if(op==2) fprintf(fp,"_");
                        else printf("_");
                    }
                else for(k=0;k<pow(2,n-i)-2;k++) //�����Ӳ����ڣ����ӡ�ո�
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
                if(op==2) fprintf(fp,"%04d",num[front].Node->Node_num); //��ӡ���
                else printf("%04d",num[front].Node->Node_num);
                if(num[front].Node->rchild) //���Һ��Ӵ��ڣ����ӡ�»���
                    for(k=0;k<pow(2,n-i)-2;k++)
                    {
                        if(op==2) fprintf(fp,"_");
                        else printf("_");
                    }
                else for(k=0;k<pow(2,n-i)-2;k++)//���Һ��Ӳ����ڣ����ӡ�ո�
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
                front++;
            }
            for(k=0;k<pow(2,n-i+1);k++) //��ӡ�������֮��Ŀո�
            {
                if(op==2) fprintf(fp," ");
                else printf(" ");
            }
        }
        if(op==2) fprintf(fp,"\n"); //����
        else printf("\n");
        for(k=0;k<pow(2,n-i)-3;k++)  //��ӡ��ÿһ���ʼ�Ŀո�
        {
            if(op==2) fprintf(fp," ");
            else printf(" ");
        }
        for(j=pow(2,i);j<pow(2,i+1);j++)  //�ڶ������������������������ű����������ͷ�������ʽ���һ��ѭ���������
        {
            if(j<num[front2].num)
            {
                for(k=0;k<pow(2,n-i+1)+2;k++)
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
            }
            else if(j==num[front2].num)
            {
                if(num[front2].Node->lchild)
                {
                    if(op==2) fprintf(fp,"��");
                    else printf("��");
                }
                else
                {
                    if(op==2) fprintf(fp,"  ");
                    else printf("  ");
                }
                for(k=0;k<pow(2,n-i+1)-2;k++)
                {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
                }
                if(num[front2].Node->rchild)
                {
                    if(op==2) fprintf(fp,"��");
                    else printf("��");
                }
                else
                {
                    if(op==2) fprintf(fp,"  ");
                    else printf("  ");
                }
                front2++;
            }
            for(k=0;k<pow(2,n-i+1)-2;k++)
            {
                    if(op==2) fprintf(fp," ");
                    else printf(" ");
            }
        }
        if(op==2) fprintf(fp,"\n");
        else printf("\n");
    }
    if(op==2)
    {
        fclose(fp);//�ر��ļ�
        printf("      ����ɹ���\n");
    }
    return OK;
}
/*--------------------------------------------*/
int pow(int num,int n)
{
    int i,result=1;
    for(i=0;i<n;i++)
        result=result*num;
    return result;
}
/*--------------------------------------------*/
status Assign_name(BiTrees &T,BiTrees a[],int num)
{
    int i;
    char name[30];
    scanf("%s",name);
    for(i=1;i<num;i++)
        if(strcmp(a[i].name,name)==0) break;
    if(i>=num)
    {
        strcpy(T.name,name);
        return OK;
    }
    else return ERROR;
}
/*--------------------------------------------*/

