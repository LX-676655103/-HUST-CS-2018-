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
#define MAX_VERTEX_NUM 100
#define LIST_INIT_SIZE 2000
/*--------------------------------------------*/
typedef int status;
typedef int Sign;  //���ؼ���Ԫ�����Ͷ���
typedef int ElemType; //����Ԫ�����Ͷ���
int visited[MAX_VERTEX_NUM]; //�������飬���ڼ�¼����Ƿ����
/*--------------------------------------------*/
typedef struct InfoType{
    int weight; //Ȩ�أ�����ʵ�鲻����Ȩ�أ�
}InfoType; //������У������ݽṹ��
/*--------------------------------------------*/
typedef struct VRType{
    Sign Node_num1; //���1�Ĺؼ���
    Sign Node_num2; //���2�Ĺؼ���
    InfoType data;  //������
}VRType;   //����Ϣ�ṹ�壬���ڶ���ʹ洢����Ϣ
/*--------------------------------------------*/
typedef struct VertexType{
    Sign Node_num; //�ؼ���
    ElemType data; //������
}VertexType;  //�����Ϣ�ṹ�壬���ڶ���ʹ洢�����Ϣ
/*--------------------------------------------*/
typedef struct ArcNode{
    int adjvex;      //��ָ�����λ��
    struct ArcNode *nextarc; //���ָ��
    InfoType *info;  //����Ϣ��
}ArcNode;  //�ڽӱ�ı���ṹ��
/*--------------------------------------------*/
typedef struct VNode{
    VertexType data;   //������ݣ����а����ؼ�����������
    ArcNode *firstarc; //��ͷָ��
}VNode,AdList[MAX_VERTEX_NUM]; //��ͷ���ṹ��
/*--------------------------------------------*/
typedef struct{
    AdList vertices;   //ֱ�Ӵ���һ���ڽӱ�
    int vexnum,arcnum; //�����Ŀ�Լ�����Ŀ
    int kind;    //��¼ͼ�����ԣ�1��ʾ����ͼ��2��ʾ����ͼ
    char name[30];     //ͼ������
}ALGraph;  //��ͼ����ṹ��
/*--------------------------------------------*/
status CreateGraph(ALGraph &G,VertexType V[],VRType VR[]);
status DestroyGraph(ALGraph &G);
int LocateVex(ALGraph G,Sign u);
status PutVex(ALGraph &G,Sign u,ElemType value);
int FirstAdjVex(ALGraph G,int v);
int NextAdjVex(ALGraph G,int v,int w);
status InsertVex(ALGraph &G,VertexType v);
status DeleteVex(ALGraph &G,int v);
status InsertArc(ALGraph &G,int v,int w);
status DeleteArc(ALGraph &G,int v,int w);
status DFSTraverse(ALGraph &G);
status DFS(ALGraph &G,int u);
status BFSTraverse(ALGraph &G);
status Assign_name(ALGraph &G,ALGraph a[],int num);  //�������������Բ����Ƿ�����
status OpenFile(ALGraph &G,VertexType V[],VRType VR[]); //�ļ���ֵ����
status SaveFile(ALGraph G);  //�ļ����溯��
/*--------------------------------------------*/
int main()
{
    int op=1,i,j,v,w,k;
    int num=0,locate;
    Sign u;
    char name[30];
    VertexType V[MAX_VERTEX_NUM],vNode; //�����Ϣ����
    VRType VR[LIST_INIT_SIZE]; //����Ϣ����
    ALGraph GraList[N];  //��ͼ��������
    while(op){
	system("cls");	printf("\n\n");
	printf("      Menu for Graph On  Adjacency List \n");
	printf("-------------------------------------------------\n");
	printf("    	  1. CreateGraph     8. DeleteVex\n");
	printf("    	  2. DestroyGraph    9. InsertArc \n");
	printf("    	  3. LocateVex       10. DeleteArc\n");
	printf("    	  4. PutVex          11. DFSTraverse\n");
	printf("    	  5. FirstAdjVex     12. BFSTraverse\n");
	printf("    	  6. NextAdjVex      13. SaveFile\n");     //�ļ�����
	printf("    	  7. InsertVex       14. GraphMessage\n"); //��ʾ����ͼ����Ϣ
	printf("    	  0. Exit\n");
	printf("-------------------------------------------------\n");
	printf("      ��ѡ����Ĳ���[0~14]:");
	scanf("%d",&op);
	if(op>=2&&op<=13)  //ͨ��ͼ�����ƽ��в���
    {
        if(num)  //��ǰ��ͼ����
        {
            printf("      ��������Ҫ������ͼ�����ƣ�"); //ͨ�����Ʋ���ͼ
            scanf("%s",name);
            for(j=1;j<=num;j++)  //��������
                if(strcmp(GraList[j].name,name)==0) break;
            if(j>num) {printf("      �����ҵ�ͼ�����ڣ������²�����\n\n"); getchar(); getchar();continue;}
        }
        else {printf("      ��ǰ��ͼ���ڣ����ȴ���ͼ��\n");getchar(); getchar();continue;}
    }
    switch(op){
        case 1:
        int value;
        int n,i;
        printf("      ��ѡ��1.���м��̸�ֵ\n              2.��ȡ�ļ�\n      ");
        scanf("%d",&k);
	    printf("      ��������Ҫ������ͼ�����ͣ�\n");
	    printf("          1.����ͼ     2.����ͼ  \n      ");   //���������ʵ������ͼ������ͼ���ֹ���
        //printf("          3.������     4.������ \n      ");  //����ʵ�鲻ʵ����
	    scanf("%d",&GraList[++num].kind);
	    if(GraList[num].kind<1||GraList[num].kind>2)  //�Ƿ��ж�1����ͼ�����ͷǷ�
        {
            printf("      ����Ƿ��������½��в�����\n");
            break;
        }
        if(k==2) //�ļ���ֵ
        {
            if(OpenFile(GraList[num],V,VR)==ERROR)
                num--;
        }
        else
        {
            printf("      ������ͼ�Ķ�����Ŀ�����������0��С��%d����������",MAX_VERTEX_NUM);
	        scanf("%d",&n);
	        while(n<=0||n>=MAX_VERTEX_NUM)   //�Ƿ��ж�2�������������Ƿ���MAX_VERTEX_NUMΪ100
	        {
	            printf("      ����Ƿ�������������: ");
                scanf("%d",&n);
	        }
            GraList[num].vexnum=n;
	        printf("      ������Ϸ��Ķ�����Ϣ��ÿ�����������ɹؼ���-����������ɣ�\n");
            printf("      �м��ÿո����������֮���û��з��������ؼ���ΪΨһȷ������������\n");
            printf("      ����Ϊ����������������%d���������Ϣ��\n",n);
	        for(i=0;i<n;i++) scanf("%d %d",&V[i].Node_num,&V[i].data);
            printf("      ��������ɹ���\n");
            printf("      ������ͼ�л�����Ŀ����Ϊ����ͼ�������ظ�����ͬһ���ߣ���");
            scanf("%d",&GraList[num].arcnum);
            if(GraList[num].kind>=3)
            {
                printf("      ������Ϸ��Ļ�����Ϣ��ÿ�����������ɶ���1�ؼ���-����2�ؼ���-Ȩ�أ���������ɣ�\n");
                printf("      �м��ÿո����������֮���û��з��������ؼ���ΪΨһȷ����������\n");
                printf("      ����ȷ���ؼ��ִ��ڣ�Ȩ��Ϊ������������������%d�����ߵ���Ϣ��\n",GraList[num].arcnum);
                for(i=0;i<GraList[num].arcnum;i++) scanf("%d %d %d",&VR[i].Node_num1,&VR[i].Node_num2,&VR[i].data);
            }
            else
            {
                printf("      ������Ϸ��Ļ�����Ϣ��ÿ�����������ɶ���1�ؼ���-����2�ؼ��֣���������ɣ�\n");
                printf("      �м��ÿո����������֮���û��з��������ؼ���ΪΨһȷ������������������%d�����ߵ���Ϣ��\n",GraList[num].arcnum);
                for(i=0;i<GraList[num].arcnum;i++) scanf("%d %d",&VR[i].Node_num1,&VR[i].Node_num2);
            }
        }
        CreateGraph(GraList[num],V,VR);   //����ͼ
        printf("      ͼ�����ɹ������ͼ����������");
        while(!Assign_name(GraList[num],GraList,num))     //�����������������и�ֵ���������ж��ظ��Ĺ���
            printf("      ��ͼ�����Ѵ��ڣ����������룺"); //�Ƿ��ж�3����ͼ�������ظ�
        printf("      ͼ�����ɹ���\n");
        getchar();getchar();
        break;
	   case 2:
         if(DestroyGraph(GraList[j])==OK)
		 {
		     printf("      ͼ���ٳɹ���\n");
             for(;j<num;j++)  //��ͼ������Ҫ�����ٵ�ͼɾ������
                 GraList[j]=GraList[j+1];
             num--;
		 }
		 getchar();getchar();
		 break;
	   case 3:
         printf("      ������������ҵĶ���Ĺؼ��֣�����������");
         scanf("%d",&u);
         locate=LocateVex(GraList[j],u);  //locate�����洢����λ��
         if(locate!=-1) printf("      �����ɹ��������ҵĶ���λ��Ϊ%d,������Ϊ%d\n",locate,GraList[j].vertices[locate].data.data);
         else printf("      �������ҵĶ��㲻���ڣ�����ʧ�ܣ�\n");
		 getchar();getchar();
		 break;
	   case 4:
         printf("      ���������븳ֵ�Ķ���Ĺؼ��֣����������������");
         scanf("%d %d",&u,&value);
         if(PutVex(GraList[j],u,value)) printf("      ���㸳ֵ�ɹ���\n"); //����ֵ�����������У�e�ĸ�ֵ�ᷢ���ı䣨���������
         else printf("      ������ֵ�Ķ��㲻���ڣ�����ʧ�ܣ�\n");
		 getchar();getchar();
		 break;
	   case 5:
         printf("      �����������ȡ��һ�ڽӵ�Ķ����λ��λ��Ϊ���ڵ���0��С��%d����������",GraList[j].vexnum);
         scanf("%d",&v);
         while(v<0||v>=GraList[j].vexnum)  //���λ��Ƿ�
         {
             printf("      λ����Ϣ�Ƿ������������룺");
             scanf("%d",&v);
         }
         locate=FirstAdjVex(GraList[j],v); //locate�����洢����λ��
         if(locate!=-1) printf("      �����ɹ�����һ�ڽӵ��λ��Ϊ%d,������Ϊ%d\n",locate,GraList[j].vertices[locate].data.data);
         else printf("      ��һ�ڽӵ㲻���ڣ�����ʧ�ܣ�\n");
         getchar();getchar();
		 break;
	   case 6:
         printf("      �������ͷ�����λ��λ��Ϊ���ڵ���0��С��%d����������",GraList[j].vexnum);
         scanf("%d",&v);
         while(v<0||v>=GraList[j].vexnum) //���λ��Ƿ�
         {
             printf("      λ����Ϣ�Ƿ������������룺");
             scanf("%d",&v);
         }
         printf("      ���������ȡ��һ���ڽӵ�Ķ����λ��λ��Ϊ���ڵ���0��С��%d����������",GraList[j].vexnum);
         scanf("%d",&w);
         while(w<0||w>=GraList[j].vexnum) //���λ��Ƿ�
         {
             printf("      λ����Ϣ�Ƿ������������룺");
             scanf("%d",&w);
         }
         locate=NextAdjVex(GraList[j],v,w); //locate�����洢����λ��
         if(locate>=0) printf("      �����ɹ�����һ���ڽӵ��λ��Ϊ%d,������Ϊ%d\n",locate,GraList[j].vertices[locate].data.data);
         else if(locate==-1) printf("      ��һ�ڽӵ㲻���ڣ�����ʧ�ܣ�\n");     //ʧ�����1
         else if(locate==-2) printf("      λ��Ϊ%d�Ķ��㲻����λ��Ϊ%d���ڽӵ㣬����ʧ�ܣ�\n",v,w);  //ʧ�����2
		 getchar();getchar();
		 break;
	   case 7:
         ElemType a; Sign b;
         printf("      ��������붥����Ϣ����Ϣ�йؼ���-����������ɣ��м��ÿո������");
         while(scanf("%d %d",&b,&a)!=EOF&&LocateVex(GraList[j],b)!=-1)
              printf("      �ùؼ�����ʹ�ã�����������ؼ�����������:");  //�жϵ�ǰ�ؼ����Ƿ�Ϸ�
         vNode.Node_num=b; //���ɽ��������-�ؼ���
         vNode.data=a;     //���ɽ��������-������
         if(InsertVex(GraList[j],vNode)) printf("      �������ɹ���\n");
         else printf("      ������Ŀ�������ޣ�����ʧ�ܣ�\n");
		 getchar();getchar();
		 break;
	   case 8:
         printf("      ����������ɾ���Ķ���Ĺؼ��֣�����������");
         scanf("%d",&u);
         locate=LocateVex(GraList[j],u); //locate�����洢����λ��
         if(locate==-1)   //�ж���Ҫɾ���Ľ���Ƿ����
         {
             printf("      ��Ҫɾ���Ķ��㲻���ڣ�����ʧ�ܣ�\n");
             getchar();getchar();
             break;
         }
         if(DeleteVex(GraList[j],locate)) printf("      ���ɾ���ɹ���\n");
		 getchar();getchar();
         break;
	   case 9:
         int locate1,locate2;  //locate1��locate2�����洢����λ��
         printf("      ��������뻡����������ؼ��֣��м��ÿո������");
         scanf("%d %d",&a,&b);
         while(LocateVex(GraList[j],a)==-1||LocateVex(GraList[j],b)==-1)  //�ж���Ҫ���ؼ����Ƿ����
         {
             printf("      ���㲻���ڣ�������������������ؼ��֣�");
             scanf("%d %d",&a,&b);
         }
         locate1=LocateVex(GraList[j],a);
         locate2=LocateVex(GraList[j],b);
         if(GraList[j].kind==1) //����ͼֻ��Ҫ����һ����
         {
             if(InsertArc(GraList[j],locate1,locate2)) printf("      ������ɹ���\n");
             else printf("      �����Ѵ��ڣ������ظ����룡\n");
         }
         if(GraList[j].kind==2) //����ͼ��Ҫ����������
         {
             if(InsertArc(GraList[j],locate1,locate2)&&InsertArc(GraList[j],locate2,locate1))
                {printf("      ������ɹ���\n"); GraList[j].arcnum--;}
             else printf("      �����Ѵ��ڣ������ظ����룡\n");
         }
         getchar();getchar();
         break;
	   case 10:
         printf("      ������ɾ��������������ؼ��֣��м��ÿո������");
         scanf("%d %d",&a,&b);
         while(LocateVex(GraList[j],a)==-1||LocateVex(GraList[j],b)==-1)
         {
             printf("      ���㲻���ڣ�������������������ؼ��֣�");
             scanf("%d %d",&a,&b);
         }
         locate1=LocateVex(GraList[j],a); //locate1��locate2�����洢����λ��
         locate2=LocateVex(GraList[j],b); //locate1��locate2�����洢����λ��
         if(GraList[j].kind==1) //����ͼֻ��Ҫɾ��һ����
         {
             if(DeleteArc(GraList[j],locate1,locate2))
                printf("      ��ɾ���ɹ���\n");
             else printf("      ���߲����ڣ�ɾ��ʧ�ܣ�\n");
         }
         if(GraList[j].kind==2) //����ͼ��Ҫɾ��������
         {
             if(DeleteArc(GraList[j],locate1,locate2)&&DeleteArc(GraList[j],locate2,locate1))
                {printf("      ��ɾ���ɹ���\n");GraList[j].arcnum++;}
             else printf("      ���߲����ڣ�ɾ��ʧ�ܣ�\n");
         }
         getchar();getchar();
         break;
	   case 11:
         printf("\n----------------all elements --------------------\n");
         DFSTraverse(GraList[j]);
         printf("\n------------------ end --------------------------\n");
		 getchar();getchar();
		 break;
       case 12:
         printf("\n----------------all elements --------------------\n");
         BFSTraverse(GraList[j]);
         printf("\n------------------ end --------------------------\n");
         getchar();getchar();
         break;
       case 13:
         SaveFile(GraList[j]);
         getchar();getchar();
         break;
       case 14:
         if(!num) {printf("      ������ͼ���ڣ����������������\n"); getchar();getchar(); break;}
         printf("      ��ǰͼ����Ϣ���£�\n");
         printf("      ͼ�ĸ���Ϊ%d\n",num);
         printf("      ͼ�����ơ����͡��������Լ����������£�\n");
         for(j=1;j<=num;j++)    //�����������
         if(GraList[j].kind==1) printf("      %20s    ����ͼ    %d    %d\n",GraList[j].name,GraList[j].vexnum,GraList[j].arcnum);
         else if(GraList[j].kind==2) printf("      %20s    ����ͼ    %d    %d\n",GraList[j].name,GraList[j].vexnum,GraList[j].arcnum);
         getchar();getchar();
         break;
	   case 0:
         break;
	}
  }
printf("\n��ӭ�´���ʹ�ñ�ϵͳ��\n");
}
/*--------------------------------------------*/
status CreateGraph(ALGraph &G,VertexType V[],VRType VR[]) //����ͼ����
{

    int i,j;
    ArcNode *pNew1,*pNew2;  //pNew1ָ������ָ�������ɵı��㣬pNew2��������ͼ������
    for(i=0;i<G.vexnum;i++) //������ȴ洢���ڽӱ���
    {
        G.vertices[i].data=V[i];
        G.vertices[i].firstarc=NULL;  //ָ��ָ��յ�ַ����ָֹ������
    }
    for(i=0;i<G.arcnum;i++)
    {
        pNew1=(ArcNode*)malloc(sizeof(ArcNode));  //ʹ��malloc�������ɱ���
        if(G.kind==2) pNew2=(ArcNode*)malloc(sizeof(ArcNode)); //ʹ��malloc�������ɱ���
        for(j=0;j<G.vexnum;j++) //����������飬�������
        {
            if(VR[i].Node_num1==V[j].Node_num) //��ǰ���Ϊ������ʼ��
            {
                pNew1->nextarc=G.vertices[j].firstarc;
                G.vertices[j].firstarc=pNew1;  //ʹ���ײ巨���������ڽӱ�
            }
            if(VR[i].Node_num2==V[j].Node_num) //��ǰ���Ϊ�����յ�
                pNew1->adjvex=j;  //��¼λ��
            if(G.kind==2) //����ͼΪ����ͼʱ����Ҫͬʱ����������
            {
                if(VR[i].Node_num2==V[j].Node_num) //��ǰ���Ϊ������ʼ��
                {
                    pNew2->nextarc=G.vertices[j].firstarc;
                    G.vertices[j].firstarc=pNew2;  //ʹ���ײ巨���������ڽӱ�
                }
                if(VR[i].Node_num1==V[j].Node_num) //��ǰ���Ϊ�����յ�
                    pNew2->adjvex=j; //��¼λ��
            }
        }
    }
    return OK;
}
/*--------------------------------------------*/
status DestroyGraph(ALGraph &G) //ɾ��ͼ����
{
    int i;
	ArcNode *p,*temp;
	for(i=0;i<G.vexnum;i++)  //�������飬�ͷ�����ָ��ֵ
    {
        p=G.vertices[i].firstarc;
        while(p)  //�����ͷſռ�
        {
            temp=p;
            p=p->nextarc;
            free(temp);
        }
        G.vertices[i].firstarc=NULL;
        G.vertices[i].data.data=0;
        G.vertices[i].data.Node_num=-1;
    }
	G.vexnum=0;
	G.arcnum=0;
	G.kind=-1;
	return OK;
}
/*--------------------------------------------*/
int LocateVex(ALGraph G,Sign u) //��λ��㺯��
{
    int i;
	for(i=0;i<G.vexnum;i++)  //������λ���
		if(G.vertices[i].data.Node_num==u)
            return i;
	return -1;
}
/*--------------------------------------------*/
status PutVex(ALGraph &G,Sign u,ElemType value) //��ֵ����
{
    int i;
	for(i=0;i<G.vexnum;i++)  //������λ���
    if(G.vertices[i].data.Node_num==u)
    {
        G.vertices[i].data.data=value; //��ֵ
        return OK;
    }
	return ERROR;
}
/*--------------------------------------------*/
int FirstAdjVex(ALGraph G,int v) //��õ�һ�ڽӵ㺯��
{
    if(G.vertices[v].firstarc)   //��һ�ڽӵ����
        return G.vertices[v].firstarc->adjvex; //��õ�һ�ڽӵ�
	else return -1;
}
/*--------------------------------------------*/
int NextAdjVex(ALGraph G,int v,int w) //�����һ�ڽӵ㺯��
{
    int i;
    ArcNode *p=G.vertices[v].firstarc;
    while(p)
    {
        if(p->adjvex==w) //��ǰ�ڽӵ����
        {
            if(p->nextarc) return p->nextarc->adjvex; //��һ�ڽӵ㲻����
            else  return -1;
        }
        p=p->nextarc;
    }
    return -2; //��ǰ�ڽӵ㲻����
}
/*--------------------------------------------*/
status InsertVex(ALGraph &G,VertexType v) //�����㺯��
{
    if(G.vexnum==MAX_VERTEX_NUM)
        return ERROR;
    G.vertices[G.vexnum].data=v;
    G.vertices[G.vexnum].firstarc=NULL;
    G.vexnum++;  //����������
    return OK;
}
/*--------------------------------------------*/
status DeleteVex(ALGraph &G,int v)  //ɾ����㺯��
{
    int i;
    ArcNode *p,*temp;
    p=G.vertices[v].firstarc;
    while(p)  //����ɾ���ý������л�
    {
        temp=p->nextarc;
        G.arcnum--; //������Ŀ����
        free(p);
        p=temp;
    }
    for(i=v;i<G.vexnum;i++) //���к󷽽����ǰ�ƶ�
        G.vertices[i]=G.vertices[i+1];
    G.vexnum--;   //�������һ
    for(i=0;i<G.vexnum;i++) //�޸����б����λ�򣬷���λ�����v�ļ�һ������v��ɾ��
    {
        p=G.vertices[i].firstarc;
        if(!p) continue;   //��ǰ���û�б��㣨û�л��Ӹĵ��������ֱ�ӽ�����һ��ѭ��
        if(p->adjvex==v)   //��һ������ָ��λ��Ϊv�Ľ�㣬ɾ����ǰ���
        {
            G.vertices[i].firstarc=p->nextarc;
            G.arcnum--;  //������Ŀ��һ
            free(p);
        }
        else if(p->adjvex>v) //��һ������ָ��Ľ��λ�����v��v��һ
            p->adjvex--;
        temp=p->nextarc;
        while(temp) //��������
        {
            if(temp->adjvex==v) //��һ������ָ��λ��Ϊv�Ľ�㣬ɾ����ǰ���
            {
                p->nextarc=temp->nextarc;
                free(temp);
                G.arcnum--; //������Ŀ��һ
                temp=p->nextarc;
                continue;
            }
            else if(temp->adjvex>v) //��һ������ָ��Ľ��λ�����v��v��һ
                temp->adjvex--;
            p=temp;
            temp=temp->nextarc;
        }
    }
    return OK;
}
/*--------------------------------------------*/
status InsertArc(ALGraph &G,int v,int w) //���뻡�ߺ���
{
    ArcNode *p=G.vertices[v].firstarc,*temp;
    while(p)  //�������ҵ�ǰ��Ҫ���ɵı��Ƿ����
    {
        if(p->adjvex==w) return ERROR; //��ǰ��Ҫ���ɵĻ����ڣ�����ERROR
        p=p->nextarc;
    }
    p=G.vertices[v].firstarc;
    temp=(ArcNode*)malloc(sizeof(ArcNode));
    temp->adjvex=w;
    temp->nextarc=p;  //ʹ���ײ巨���뻡
    G.vertices[v].firstarc=temp;
    G.arcnum++;
    return OK;
}
/*--------------------------------------------*/
status DeleteArc(ALGraph &G,int v,int w)
{
    ArcNode *p=G.vertices[v].firstarc,*temp;
    if(!p) return ERROR;  //��ǰ���û�л����ڣ�����ERROR
    if(p->adjvex==w) //��һ������Ϊ��Ҫɾ���Ľ��
    {
        G.vertices[v].firstarc=p->nextarc;
        free(p);
        G.arcnum--;
        return OK;
    }
    p=G.vertices[v].firstarc;
    temp=p->nextarc;
    while(temp)  //��������
    {
        if(temp->adjvex==w) //��ǰ����Ϊ��Ҫɾ���Ľ��
        {
            p->nextarc=temp->nextarc;
            free(temp);
            G.arcnum--;
            return OK;
        }
        p=p->nextarc;
        temp=p->nextarc;
    }
    return ERROR; //��ǰ��Ҫɾ���Ļ������ڣ�����ERROR
}
/*--------------------------------------------*/
status DFSTraverse(ALGraph &G) //������ȱ�������
{
    int i;
	for(i=0;i<G.vexnum;i++) visited[i]=FALSE; //���÷�������
	for(i=0;i<G.vexnum;i++)
		if(visited[i]==FALSE) //��ǰ���δ���
            DFS(G,i); //������ȱ�������
	return OK;
}
status DFS(ALGraph &G,int u)  //�ݹ�������ȱ�������
{
    int i;
    ArcNode *p=G.vertices[u].firstarc;
    visited[u]=TRUE;  //�����ǰ��㣬�������鵱ǰ���λ�ñ��ΪTRUE
    printf("%d ",G.vertices[u].data.Node_num);
    while(p) //������ǰ���Ļ���
    {
        i=p->adjvex;
        if(visited[i]==FALSE) //��ǰ���δ���
            DFS(G,i); //������ȱ�������
        p=p->nextarc;
    }
    return OK;
}
/*--------------------------------------------*/
status BFSTraverse(ALGraph &G) //������ȱ�������
{
    AdList Q;  //����һ��������
    ArcNode *p;
    int i,front=0,rear=0;  //ʹ�ö��п��ƣ�frontָ���ͷ��rearָ���β
	for(i=0;i<G.vexnum;i++) visited[i]=FALSE; //���÷�������
    for(i=0;i<G.vexnum;i++)
    {
        if(visited[i]==FALSE) //��ǰ���δ���
        {
            visited[i]=TRUE; //�����ǰ��㣬�������鵱ǰ���λ�ñ��ΪTRUE
            Q[rear++]=G.vertices[i]; //������
            while(front<rear) //�ӷǿ�
            {
                printf("%d ",Q[front].data.Node_num);
                p=Q[front].firstarc;
                while(p) //����ǰ���Ļ�ָ���δ���ʵĽ��ȫ�����
                {
                    if(visited[p->adjvex]==FALSE)
                    {
                        Q[rear++]=G.vertices[p->adjvex];
                        visited[p->adjvex]=TRUE;
                    }
                    p=p->nextarc;
                }
                front++; //������
            }
        }
    }
    return OK;
}
/*--------------------------------------------*/
status Assign_name(ALGraph &G,ALGraph a[],int num) //��������
{
    int i;
    char name[30];
    scanf("%s",name);
    for(i=1;i<num;i++)  //���Ʋ���
        if(strcmp(a[i].name,name)==0) break;
    if(i>=num)
    {
        strcpy(G.name,name);
        return OK;
    }
    else return ERROR;
}
/*--------------------------------------------*/
status OpenFile(ALGraph &G,VertexType V[],VRType VR[]) //�ļ���ֵ����
{
    FILE* fp;
    char filename[30];
    int i;
    printf("      �������ļ�����");
    scanf("%s",filename);
    if (!(fp=fopen(filename,"r"))) {printf("      ���ļ�ʧ��!\n"); return ERROR;} //�ļ���ֵ�����ļ�
    fscanf(fp,"%d",&G.vexnum);
    for(i=0;i<G.vexnum;i++)
        fscanf(fp,"%d %d",&V[i].Node_num,&V[i].data);
    fscanf(fp,"%d",&G.arcnum);
    if(G.kind>=3)
        for(i=0;i<G.arcnum;i++)
            fscanf(fp,"%d %d %d",&VR[i].Node_num1,&VR[i].Node_num2,&VR[i].data);
    else
        for(i=0;i<G.arcnum;i++)
            fscanf(fp,"%d %d",&VR[i].Node_num1,&VR[i].Node_num2);
    printf("      ���ļ��ɹ�!\n");
    fclose(fp); //�ر��ļ�
    getchar();
    return OK;
}
/*--------------------------------------------*/
status SaveFile(ALGraph G)
{
    FILE* fp;
    int num;
    int i=0;
    ArcNode *p;
    char filename[30];
    printf("      ��ѡ��1.��ɾ��ԭ�еĶ������������ļ�����׷��\n              2.ɾ��ԭ�����ݻ��������ļ�\n\n");
    scanf("%d",&num);
    printf("      �������ļ�����");
    scanf("%s",filename);
    if(num==1&&(fp=fopen(filename,"a"))==NULL)   //�ļ��򿪷�ʽΪ׷�ӣ�����ԭ�е����ݽ����޸�
    {
        printf("      �ļ��ļ���ʧ�ܣ�\n");
        return ERROR;
    }
    else if(num==2&&(fp=fopen(filename,"w"))==NULL) //�ļ��򿪷�ʽΪ���»򴴽����ļ�����ɾ��ԭ������
    {
        printf("      �ļ��ļ���ʧ�ܣ�\n");
        return ERROR;
    }
    if(G.kind==1) fprintf(fp,"����ͼ\n");
    if(G.kind==2) fprintf(fp,"����ͼ\n");
    fputs(G.name,fp);
    fprintf(fp,"\n%d\n",G.vexnum);
    for(i=0;i<G.vexnum;i++)
        fprintf(fp,"%d %d\n",G.vertices[i].data.Node_num,G.vertices[i].data.data);
    fprintf(fp,"%d\n",G.arcnum);
    for(i=0;i<G.vexnum;i++)
    {
        p=G.vertices[i].firstarc;
        while(p)
        {
            if(G.kind==1)
                fprintf(fp,"%d %d\n",G.vertices[i].data.Node_num,G.vertices[p->adjvex].data.Node_num);
            if(G.kind==2&&p->adjvex>=i)
                fprintf(fp,"%d %d\n",G.vertices[i].data.Node_num,G.vertices[p->adjvex].data.Node_num);
            p=p->nextarc;
        }
    }
    fprintf(fp,"\n\n");
    fclose(fp);//�ر��ļ�
    printf("      ����ɹ���\n");
    return OK;
}
/*--------------------------------------------*/






