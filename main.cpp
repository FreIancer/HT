#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char ch; // ��Ÿýڵ��ַ�
    int weight;
    int parent, lchild, rchild;
}HTNode, *huffmanTree;
typedef char ** huffmanCode; // ָ������������ָ��

void tips(); // ��ӡ����
void huffmanCoding(huffmanTree &,char *, int *,int); // ������������
void select(huffmanTree HT, int j, int  *x, int *y); // ���ѽ��õĹ���������ѡ��parent=0��weight��С�������ڵ�
void init();
void coding(); // ����
void decoding(); // ����
void printCode(); // ��ӡ����������
void printTree(); // ��ӡ��������
int readTree(huffmanTree&); // ��ȡ�ļ��еĹ�������
void find(huffmanTree &HT, char *code, char *text, int i, int m); // ����ʱ����01�ַ���Ѱ��Ҷ�ӽڵ�
void convertTree(unsigned char T[100][100], int s,int *i,int j); // ���ڴ��еĹ�������תΪ��͹��Ĺ�������

huffmanTree HT;
int n=0;


int main() {
    char select;

    while (1){
        tips();
        scanf("%c", &select);
        switch (select) {
            case '1':
                init();
                break;
            case '2':
                coding();
                break;
            case '3':
                decoding();
                break;
            case '4':
                printCode();
                break;
            case '5':
                printTree();
                break;
            case '0':
                exit(1);
            default:
                printf("input error!\n");
        }
        getchar();
    }
    return 0;
}

void tips(){
    printf("--------------------------------------\n");
    printf("---            ��ѡ�����            ---\n");
    printf("--------------------------------------\n");
    printf("\n");
    printf("-------------1 ��ʼ���������� -----------\n");
    printf("-------------2 ����         -----------\n");
    printf("-------------3 ����         -----------\n");
    printf("-------------4 ��ӡ������ļ� -----------\n");
    printf("-------------5 ��ӡ��������   -----------\n");
    printf("-------------0 �˳�         -----------\n");
    printf("--------------------------------------\n");
}

void init(){
    FILE *fp;
    int i,n,w[52]; // ������������ַ�Ȩֵ
    char character[52];
    printf("\n�����ַ����� n:");
    scanf("%d",&n); // �����ַ�����С
    printf("����%d���ַ������Ӧ��Ȩֵ:\n",n);
    for (i=0;i<n;i++){
        char b=getchar();
        scanf("%c",&character[i]);
        scanf("%d",&w[i]); // ����n���ַ��Ͷ�ӦȨֵ
    }
    huffmanCoding(HT, character, w,n); // ������������

    if((fp= fopen("hfmtree.txt","w"))== NULL)
        printf("huffman tree file error\n");
    for(i=1;i<=2*n-1;i++){
        if(fwrite(&HT[i],sizeof(HTNode),1,fp)!= 1)
            printf("file write error\n");
    }
    printf("���������ѱ�����hfmtree.txt��\n");
    fclose(fp);

}

void huffmanCoding(huffmanTree &HT,char *character, int *w,int n){
    int m,i,x,y;
    huffmanTree p;
    if(n<=1) return;
    m=2*n-1;
    HT=(huffmanTree)malloc((m+1)*sizeof(HTNode));
    for(p=HT+1,i=1;i<=n;++i,++p,++character,++w){
        p->ch=*character;
        p->weight=*w;
        p->parent=0;
        p->lchild=0;
        p->rchild=0;
    }
    for(;i<=m;++i,++p){
        p->ch=0;
        p->weight=0;
        p->parent=0;
        p->lchild=0;
        p->rchild=0;
    }
    for(i=n+1;i<=m;++i){
        select(HT,i-1,&x,&y);
        HT[x].parent=i;
        HT[y].parent=i;
        HT[i].lchild=x;
        HT[i].rchild=y;
        HT[i].weight=HT[x].weight+HT[y].weight;
    }
}

void select(huffmanTree HT, int j, int  *x, int *y){
    int i;
//���� weight ��С�Ľ��
    for (i=1;i<=j;i++)
        if (HT[i].parent==0){
            *x=i;
            break;
        }
    for (;i<=j;i++)
        if ((HT[i].parent==0)&&(HT[i].weight<HT[*x].weight))
            *x=i;
    HT[*x].parent=1;
//���� weight ��С�Ľ��
    for (i=1;i<=j;i++)
        if (HT[i].parent==0){
            *y=i;
            break;
        }
    for (;i<=j;i++)
        if ((HT[i].parent==0)&&(i!=*x)&&(HT[i].weight<HT[*y].weight))
            *y=i;
}

void coding(){
    FILE *fp,*fw;
    int i,f,c,start;
    char *cd;
    huffmanCode HC;
    if(n==0)
        n=readTree(HT);//���ļ� hfmtree.txt �ж���շ�����,����Ҷ�ӽ����
                          //��շ������и�Ҷ�ӽڵ���ַ���Ӧ�ĵı���,������ HC ָ��Ŀռ���
    {
        HC=(huffmanCode)malloc((n+1)*sizeof(char*));
        cd=(char *)malloc(n*sizeof(char));
        cd[n-1]='\0';
        for(i=1;i<=n;++i){
            start=n-1;
            for(c=i,f=HT[i].parent;f!=0;c=f,f=HT[f].parent)
                if(HT[f].lchild==c)
                    cd[--start]='0';
                else cd[--start]='1';
            HC[i]=(char *)malloc((n-start)*sizeof(char));
            strcpy(HC[i],&cd[start]);
        }
        free(cd);
    }
    if((fp=fopen("tobetrans.txt","rb"))==NULL)
        printf("Open file tobetrans.txt error!\n");
    if((fw=fopen("codefile.txt","wb+"))==NULL)
        printf("Open file codefile.txt error!\n");
    char temp;
    fscanf(fp,"%c",&temp);

    while(!feof(fp)){
        for(i=1;i<=n;i++)
            if(HT[i].ch==temp) break; //�ںշ������в����ַ����ڵ�λ��
        for(int r=0;HC[i][r]!='\0';r++) //���ַ���Ӧ�ı�������ļ�
            fputc(HC[i][r],fw);
        fscanf(fp,"%c",&temp); //���ļ�������һ���ַ�
    }
    fclose(fw);
    fclose(fp);
    printf("�ѽ��ļ� hfmtree.txt �ɹ�����,���Ѵ��� codefile.txt ��!\n");
}

void decoding(){
    FILE *fp,*fw;
    int m,i;
    char *code,*text,*p;
    if(n==0)
        n=readTree(HT);//���ļ� hfmtree.txt �ж���շ�����,����Ҷ�ӽ����
    if((fp=fopen("codefile.txt","rb"))==NULL)
        printf("Open file codefile.txt error!\n");
    if((fw=fopen("textfile.txt","wb+"))==NULL)
        printf("Open file textfile.txt error!\n");
    code=(char *)malloc(sizeof(char));
    fscanf(fp,"%c",code); //���ļ�����һ���ַ�

    for(i=1;!feof(fp);i++)
    {
        code=(char *)realloc(code,(i+1)*sizeof(char)); //���ӿռ�
        fscanf(fp,"%c",&code[i]); //���ļ�������һ���ַ�
    }
    code[i-1]='\0'; // codefile.txt �ļ��е��ַ���ȫ������,����� code ������
    text=(char *)malloc(100*sizeof(char));
    p=text;
    m=2*n-1;
    if(*code=='0')
        find(HT,code,text,HT[m].lchild,m); //�Ӹ��ڵ��������ȥ��
    else
        find(HT,code,text,HT[m].rchild,m); //�Ӹ��ڵ��������ȥ��
    for(i=0;p[i]!='\0';i++) // �� �� �� �� �� �� �� �� �� �� ��textfile.txt ��
        fputc(p[i],fw);
    fclose(fp);
    fclose(fw);
    printf("�ѽ� codefile.txt �ļ��ɹ�����,�Ѵ��� textfile.txt �ļ�\n");
}

void printCode(){
    FILE *fp,*fw;
    char temp;
    int i;
    if((fp=fopen("codefile.txt","rb"))==NULL)
        printf("Open file codefile.txt error!\n");
    if((fw=fopen("codeprint.txt","wb+"))==NULL)
        printf("Open file codeprint.txt error!\n");
    printf("\n �ļ� codefi1e ��ʾ����:\n");
    fscanf(fp,"%c",&temp); //���ļ�����һ���ַ�

    for (i=1;!feof(fp);i++)
    {
        printf("%c",temp);
        if(i%50==0) printf("\n");
        fputc(temp,fw); //�����ַ������ļ� codeprint.txt ��
        fscanf(fp,"%c",&temp); //���ļ�����һ���ַ�
    }
    printf("\n �ѽ����ַ���ʽ�ı���д���ļ� codeprint.txt ��\n");
    fclose(fp);
    fclose(fw);
}

void printTree(){
    unsigned char T[100][100];
    int i,j,m=0;
    FILE *fp;
    if(n==0)
        n=readTree(HT); //���ļ� hfmtree.txt �ж���շ�����,����Ҷ�ӽ����
    convertTree(T,0,&m,2*n-1); //���ڴ��еĺշ�����ת���ɰ�͹����ʽ�������������� T ��

    if((fp=fopen("treeprint.txt","wb+"))==NULL)
        printf("Open file treeprint.txt error!\n");
    printf("\n ��ӡ�ѽ��õĺշ�����:\n");
    for(i=1;i<=2*n-1;i++){
        for (j=0;T[i][j]!=0;j++){
            if(T[i][j]==' ') {
                printf(" ");fputc(T[i][j],fp);
            }
            else{
                printf("%d",T[i][j]);
                fprintf(fp,"%d\n",T[i][j]);
            }
        }
        printf("\n");
    }
    fclose(fp);
    printf("\n �ѽ����ַ���ʽ�Ĺ�������д���ļ� treeprint.txt ��!\n");
}

int readTree(huffmanTree &HT) {
    FILE *fp;
    int i, n;
    HT = (huffmanTree) malloc(sizeof(HTNode));
    if ((fp = fopen("hfmtree.txt", "r")) == NULL)
        printf("Open file hfmtree.txt error!\n");

    for (i=1;!feof(fp);i++)
    {
        HT=(huffmanTree)realloc(HT,(i+1)*sizeof(HTNode)); //���ӿռ�
        fread(&HT[i],sizeof(HTNode),1,fp); //����һ���ڵ���Ϣ
    }
    fclose(fp);
    n=(i-1)/2;
    return n;
}

void find(huffmanTree &HT, char *code, char *text, int i, int m){
    if(*code!='\0'){ //������δ����
        code++;
        if(HT[i].lchild==0&&HT[i].rchild==0){ //���ҵ�Ҷ�ӽڵ�
            *text=HT[i].ch; //��Ҷ�ӽڵ���ַ����� text ��
            text++;
            if((*code=='0'))
                find(HT,code,text,HT[m].lchild,m); //�Ӹ��ڵ����������
            else
                find(HT,code,text,HT[m].rchild,m); //�Ӹ��ڵ����������
        }
        else //�������Ҷ�ӽڵ�
        if(*code=='0')
            find(HT,code,text,HT[i].lchild,m); //�Ӹýڵ��������ȥ��
        else
            find(HT,code,text,HT[i].rchild,m); //�Ӹýڵ��������ȥ��
    }
    else
        *text='\0'; //�������
}

void convertTree(unsigned char T[100][100], int s,int *i,int j){
    int k,l;
    l=++(*i);
    for(k=0;k<s;k++)
        T[l][k]=' ';
    T[l][k]=HT[j].weight;
    if(HT[j].lchild)
        convertTree(T,s+1,i,HT[j].lchild);
    if(HT[j].rchild)
        convertTree(T,s+1,i,HT[j].rchild);
    T[l][++k]='\0';
}