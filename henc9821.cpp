/* SYDUR RAHAMAN  cs610 9821 prp*/
#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include<cstdlib>
using namespace std;

struct TreeNode {

    unsigned char ch;
    int freq;
    TreeNode *left;
    TreeNode *right;
    TreeNode(unsigned char x,int y) : ch(x),freq(y), left(NULL), right(NULL) {}
 };

string charToBitString9821(char c)
{
    string s="";
    for (int i = 7; i >= 0; --i) {
        char ch= ((c & (1 << i))? '1' : '0');
        s.append(1,ch);
    }
    return s;
}

void heapify9821(vector<TreeNode*> &T, int n, int i)
{
    int smallest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;

    if (l < n && T[l]->freq < T[smallest]->freq )
        smallest = l;

   if (r < n && T[r]->freq < T[smallest]->freq )
        smallest = r;

    if (smallest != i)
    {
        swap(T[i],T[smallest]);
        heapify9821(T, n, smallest);
    }
}

void BuildMinheap9821(vector<TreeNode*> &T)
{
    int heapsize=T.size();
    for (int i = heapsize / 2 - 1; i >= 0; i--)
        heapify9821(T, heapsize, i);
}

TreeNode* ExtractMin9821(vector<TreeNode*> &T)
{
    TreeNode* r=T[0];
    int n=T.size();
    swap(T[0],T[n-1]);
    T.erase(T.begin()+n-1);
    heapify9821(T,n-1, 0);
    return r;
}



void InsertToHeap9821(vector<TreeNode*> &T,TreeNode* node)
{
    T.push_back(node);
    int n=T.size()-1;
    int parentindex=(n-1)/2;
    while(parentindex>=0)
    {
        if(T[n]->freq < T[parentindex]->freq)
        {
            swap(T[n],T[parentindex]);
            n=parentindex;
        }
        else break;
        parentindex=(parentindex-1)/2;
    }
}

void BuildPrefixTable9821(vector<string> &pcodes,TreeNode* node,string code)
{
    if(node==NULL) return;

    if(node->left==NULL && node->right==NULL)
    {
        int asci=int(node->ch);
        pcodes[asci]=code;
    }

    BuildPrefixTable9821(pcodes,node->left,code+"0");
    BuildPrefixTable9821(pcodes,node->right,code+"1");
}

void PreOrderTreeChars9821(TreeNode *T, ofstream &out) {
  if (T==NULL) return;
    out<<T->ch;
    //cout<<T->ch<<"("<<charToBitString(T->ch)<<")"<<" ";
    PreOrderTreeChars9821(T->left, out);
    PreOrderTreeChars9821(T->right, out);
}

void InOrderTreeChars9821(TreeNode *T, ofstream &out) {
    if (T==NULL) return;
    InOrderTreeChars9821(T->left, out);
    out<<T->ch;
    //cout<<T->ch<<"("<<charToBitString(T->ch)<<")"<<" ";
    InOrderTreeChars9821(T->right, out);
}




void encode9821(char *infile)
{
    //reading file and calculating char frequencies
    //ifstream f(infile, ios::binary | ios::in);
    ifstream f(infile);
    char c;
    int freq[256]={0};
    vector<char> filechars;
    while (f.get(c))
    {
        filechars.push_back(c);
        int asci=int((unsigned char)c);
        freq[asci]++;
    }

    f.close();

    //creating a tree with each char as node and frequencies
    vector<TreeNode*> T;
    for(int i=0;i<256;i++)
    {
        if(freq[i]>0)
        {
            unsigned char ch=i;
            TreeNode* temp=new TreeNode(ch,freq[i]);
            T.push_back(temp);
        }
    }


    BuildMinheap9821(T);
    int cnt=-1;
    while(T.size()>1)
    {
         TreeNode* temp=new TreeNode(cnt--,0);
         temp->left=ExtractMin9821(T);
         temp->right=ExtractMin9821(T);
         temp->freq=temp->left->freq+temp->right->freq;
         InsertToHeap9821(T,temp);
    }
    vector<string> pcodes(256,"");
    if(T[0]!=NULL && T[0]->left==NULL && T[0]->right==NULL)
        pcodes[(int)T[0]->ch]="1";
    else BuildPrefixTable9821(pcodes,T[0],"");


    //reading file and replacing every character with encoded char
    //ifstream ff(infile, ios::binary | ios::in);

        //size_t len = strlen(infile);
     //outputfile name generation
    char b[]= ".huf";
    size_t sizeA = strlen(infile);
    size_t sizeB = strlen(b);
    size_t siz = sizeof(char) * (sizeA + sizeB + 1);

    char* outputfile= (char*) malloc(siz);
    memcpy(outputfile, infile, sizeA);
    memcpy(outputfile+ sizeA, b, sizeB);
    outputfile[sizeA + sizeB] = '\0';

    //ofstream out(outputfile,ofstream::binary);
    ofstream out(outputfile);


    string prefixbits="";

    for(int i=0;i<filechars.size();i++)
    {
        int asci=int((unsigned char)filechars[i]);
        prefixbits+=pcodes[asci];
    }
    /*
    while (ff.get(c))
    {
        int asci=int((unsigned char)c);
        prefixbits+=pcodes[asci];
        //cout<<"Char: "<< c<< " Prefix Code: "<<pcodes[asci]<<endl;
        //cout<<pcodes[asci];
    }*/



    //writing frequency table;
    for(int i=0;i<256;i++)
    {
        int fr = freq[i];
        out.write((char*)(&fr), sizeof(fr));
    }
    //writing extrabit
    int extrabit=prefixbits.size()%8;
    if(extrabit>0)
    {
        extrabit=8-extrabit;
    }
    //cout<<extrabit;
    out<<(char) extrabit;


    // writing the content to output file
    unsigned char byte =0;
    int bit=0;
    for(int i=0;i<prefixbits.size();i++)
    {
        if(prefixbits[i]=='1')
            byte= byte | (1<<(7-bit));
        bit++;
        if(bit==8)
        {
            out<<byte;
            bit=0;
            byte=0;
        }
    }

    if(bit>0)
    {
        out<<byte;
    }


    //ff.close();
    out.close();
    free(outputfile);
}



int main(int argc, char * argv[])
{
    vector<string> arguments(argv,argv+argc);
    if(arguments.size()<2)
    {
        cout<<"Input file missing from command line"<<endl;
        return 0;
    }
    encode9821(argv[1]);
    //cout<<"finished"<<endl;
    return 0;
}
