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


void HuffmanDecode9821(string B,TreeNode* T,ofstream &out)
{
    TreeNode* p=T;
    if(p->left==NULL && p->right==NULL)
    {
        for(int i=0;i<B.size();i++)
        {
            out<<p->ch;
        }
    }
    else
    {
        for(int i=0;i<B.size();i++)
        {

            if(B[i]=='0')
                p=p->left;
            else
                p=p->right;
            if(p->left==NULL && p->right==NULL)
            {

                out<<p->ch;
                p=T;
            }
        }
    }

}

string charToBitString9821(char c)
{
    string s="";
    for (int i = 7; i >= 0; --i) {
        char ch= ((c & (1 << i))? '1' : '0');
        s.append(1,ch);
    }
    return s;
}



void decode9821(string infile)
{
    char *inf=new char[infile.size()+1];
    memcpy(inf,infile.c_str(),infile.size()+1);
    ifstream f(inf);

    int freq[256]={0};

    for(int i=0;i<256;i++)
    {
        int fr = 0;
        f.read((char*) (&fr), sizeof(fr));
        freq[i]=fr;
    }
    char c;
    int extrabit=0;
    if(f.get(c)) extrabit=(int) c;
    //cout<<extrabit;
    string dtext="";
    while (f.get(c))
    {
        for (int i = 7; i >= 0; --i) {
        char ch= ((c & (1 << i))? '1' : '0');
        dtext.append(1,ch);
        }
    }

    f.close();

    //cout<<dtext;
    dtext.erase(dtext.size()-extrabit,extrabit);


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

    size_t pos = infile.find(".");
    string outputfile = infile.substr(0, pos);
    char *outf=new char[outputfile.size()+1];
    memcpy(outf,outputfile.c_str(),outputfile.size()+1);
    ofstream out(outf);
    HuffmanDecode9821(dtext,T[0],out);
    out.close();
    free(inf);
    free(outf);
}



int main(int argc, char * argv[])
{
    vector<string> arguments(argv,argv+argc);
    if(arguments.size()<2)
    {
        cout<<"Input file missing from command line"<<endl;
        return 0;
    }
    decode9821(arguments[1]);
    //cout<<"finished"<<endl;
    return 0;
}

