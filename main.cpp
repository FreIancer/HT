#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char ch; // 存放该节点字符
    int weight;
    int parent, lchild, rchild;
}HTNode, *huffmanTree;
typedef char ** huffmanCode; // 指向哈夫曼编码的指针

void tips(); // 打印操作
void huffmanCoding(huffmanTree &,char *, int *,int); // 建立哈夫曼树
void select(huffmanTree HT, int j, int  *x, int *y); // 从已建好的哈夫曼树中选择parent=0，weight最小的两个节点
void init();
void coding(); // 编码
void decoding(); // 译码
void printCode(); // 打印译码后的内容
void printTree(); // 打印哈夫曼树
int readTree(huffmanTree&); // 读取文件中的哈夫曼树
void find(huffmanTree &HT, char *code, char *text, int i, int m); // 译码时根据01字符串寻找叶子节点
void convertTree(unsigned char T[100][100], int s,int *i,int j); // 将内存中的哈夫曼树转为凹凸表的哈夫曼树

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
    printf("---            请选择操作            ---\n");
    printf("--------------------------------------\n");
    printf("\n");
    printf("-------------1 初始化哈夫曼树 -----------\n");
    printf("-------------2 编码         -----------\n");
    printf("-------------3 译码         -----------\n");
    printf("-------------4 打印编码后文件 -----------\n");
    printf("-------------5 打印哈夫曼树   -----------\n");
    printf("-------------0 退出         -----------\n");
    printf("--------------------------------------\n");
}

void init(){
    FILE *fp;
    int i,n,w[52]; // 数组用来存放字符权值
    char character[52];
    printf("\n输入字符个数 n:");
    scanf("%d",&n); // 输入字符集大小
    printf("输入%d个字符及其对应的权值:\n",n);
    for (i=0;i<n;i++){
        char b=getchar();
        scanf("%c",&character[i]);
        scanf("%d",&w[i]); // 输入n给字符和对应权值
    }
    huffmanCoding(HT, character, w,n); // 建立哈夫曼树

    if((fp= fopen("hfmtree.txt","w"))== NULL)
        printf("huffman tree file error\n");
    for(i=1;i<=2*n-1;i++){
        if(fwrite(&HT[i],sizeof(HTNode),1,fp)!= 1)
            printf("file write error\n");
    }
    printf("哈夫曼树已保存至hfmtree.txt中\n");
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
//查找 weight 最小的结点
    for (i=1;i<=j;i++)
        if (HT[i].parent==0){
            *x=i;
            break;
        }
    for (;i<=j;i++)
        if ((HT[i].parent==0)&&(HT[i].weight<HT[*x].weight))
            *x=i;
    HT[*x].parent=1;
//查找 weight 次小的结点
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
        n=readTree(HT);//从文件 hfmtree.txt 中读入赫夫曼树,返回叶子结点数
                          //求赫夫曼树中各叶子节点的字符对应的的编码,并存于 HC 指向的空间中
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
            if(HT[i].ch==temp) break; //在赫夫曼树中查找字符所在的位置
        for(int r=0;HC[i][r]!='\0';r++) //将字符对应的编码存入文件
            fputc(HC[i][r],fw);
        fscanf(fp,"%c",&temp); //从文件读入下一个字符
    }
    fclose(fw);
    fclose(fp);
    printf("已将文件 hfmtree.txt 成功编码,并已存入 codefile.txt 中!\n");
}

void decoding(){
    FILE *fp,*fw;
    int m,i;
    char *code,*text,*p;
    if(n==0)
        n=readTree(HT);//从文件 hfmtree.txt 中读入赫夫曼树,返回叶子结点数
    if((fp=fopen("codefile.txt","rb"))==NULL)
        printf("Open file codefile.txt error!\n");
    if((fw=fopen("textfile.txt","wb+"))==NULL)
        printf("Open file textfile.txt error!\n");
    code=(char *)malloc(sizeof(char));
    fscanf(fp,"%c",code); //从文件读入一个字符

    for(i=1;!feof(fp);i++)
    {
        code=(char *)realloc(code,(i+1)*sizeof(char)); //增加空间
        fscanf(fp,"%c",&code[i]); //从文件读入下一个字符
    }
    code[i-1]='\0'; // codefile.txt 文件中的字符已全部读入,存放在 code 数组中
    text=(char *)malloc(100*sizeof(char));
    p=text;
    m=2*n-1;
    if(*code=='0')
        find(HT,code,text,HT[m].lchild,m); //从根节点的左子树去找
    else
        find(HT,code,text,HT[m].rchild,m); //从根节点的右子树去找
    for(i=0;p[i]!='\0';i++) // 把 译 码 好 的 字 符 存 入 文 件textfile.txt 中
        fputc(p[i],fw);
    fclose(fp);
    fclose(fw);
    printf("已将 codefile.txt 文件成功译码,已存入 textfile.txt 文件\n");
}

void printCode(){
    FILE *fp,*fw;
    char temp;
    int i;
    if((fp=fopen("codefile.txt","rb"))==NULL)
        printf("Open file codefile.txt error!\n");
    if((fw=fopen("codeprint.txt","wb+"))==NULL)
        printf("Open file codeprint.txt error!\n");
    printf("\n 文件 codefi1e 显示如下:\n");
    fscanf(fp,"%c",&temp); //从文件读入一个字符

    for (i=1;!feof(fp);i++)
    {
        printf("%c",temp);
        if(i%50==0) printf("\n");
        fputc(temp,fw); //将该字符存入文件 codeprint.txt 中
        fscanf(fp,"%c",&temp); //从文件读入一个字符
    }
    printf("\n 已将此字符形式的编码写入文件 codeprint.txt 中\n");
    fclose(fp);
    fclose(fw);
}

void printTree(){
    unsigned char T[100][100];
    int i,j,m=0;
    FILE *fp;
    if(n==0)
        n=readTree(HT); //从文件 hfmtree.txt 中读入赫夫曼树,返回叶子结点数
    convertTree(T,0,&m,2*n-1); //将内存中的赫夫曼树转换成凹凸表形式的树存于数组 T 中

    if((fp=fopen("treeprint.txt","wb+"))==NULL)
        printf("Open file treeprint.txt error!\n");
    printf("\n 打印已建好的赫夫曼树:\n");
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
    printf("\n 已将该字符形式的哈夫曼树写入文件 treeprint.txt 中!\n");
}

int readTree(huffmanTree &HT) {
    FILE *fp;
    int i, n;
    HT = (huffmanTree) malloc(sizeof(HTNode));
    if ((fp = fopen("hfmtree.txt", "r")) == NULL)
        printf("Open file hfmtree.txt error!\n");

    for (i=1;!feof(fp);i++)
    {
        HT=(huffmanTree)realloc(HT,(i+1)*sizeof(HTNode)); //增加空间
        fread(&HT[i],sizeof(HTNode),1,fp); //读入一个节点信息
    }
    fclose(fp);
    n=(i-1)/2;
    return n;
}

void find(huffmanTree &HT, char *code, char *text, int i, int m){
    if(*code!='\0'){ //若译码未结束
        code++;
        if(HT[i].lchild==0&&HT[i].rchild==0){ //若找到叶子节点
            *text=HT[i].ch; //将叶子节点的字符存入 text 中
            text++;
            if((*code=='0'))
                find(HT,code,text,HT[m].lchild,m); //从根节点的左子树找
            else
                find(HT,code,text,HT[m].rchild,m); //从根节点的右子树找
        }
        else //如果不是叶子节点
        if(*code=='0')
            find(HT,code,text,HT[i].lchild,m); //从该节点的左子树去找
        else
            find(HT,code,text,HT[i].rchild,m); //从该节点的右子树去找
    }
    else
        *text='\0'; //译码结束
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