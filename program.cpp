#include<bits/stdc++.h>
#include<fstream>
#include<sstream>
using namespace std;

class HuffTree{
    public:
        int val;
        char letter;
        HuffTree *left;
        HuffTree *right;

        HuffTree(int value, char c){
            this->val = value;
            this->letter = c;
            this->left = NULL;
            this->right = NULL;
        }
};



string file_to_text()
{
    string str1;
    ifstream fin;
    string s;
    cout<<"Enter path  of the text file you want to compress(dont use quotations):"<<endl;
    cin>>s;
    replace(s.begin(), s.end(), '\\', '/');

    fin.open(s);
  
    stringstream buffer;
    buffer<<fin.rdbuf();
    str1 = buffer.str();
    fin.close();
	return str1;
  	
}


vector<pair<int, char>> frequencyDect(string s){
    map<char, int> mp;
    for(int i=0; i<s.size(); i++){
        mp[s[i]]++; 
    }
    vector<pair<int, char>> v;
    for(int i=0; i<s.size(); i++){
        if(mp[s[i]] != 0){
            v.push_back(make_pair(mp[s[i]], s[i]));
            mp[s[i]] = 0;
        }
    }
    return v;

}

class Compare{
    public:
        bool operator()(HuffTree* a, HuffTree *b)
        {
            if(a->val > b->val)return true;
            else return false;
        }
};



void print (priority_queue<HuffTree*, vector<HuffTree*>, Compare> q){
    while(q.size()){
        cout<<q.top()->letter<<" "<<q.top()->val<<endl;
        q.pop();
    } 
}

HuffTree* makeTree(vector<pair<int, char>> &frq){
    priority_queue<HuffTree*, vector<HuffTree*>, Compare> q;
    for(int i=0; i< frq.size(); i++){
        q.push(new HuffTree(frq[i].first, frq[i].second));
         
    }
    //print(q);
    while(q.size()>1){
        HuffTree *left = q.top();
        q.pop();
        HuffTree *right = q.top();
        q.pop();

        HuffTree *nd = new HuffTree(left->val+right->val,'~');
        nd->left = left;
        nd->right = right;
        q.push(nd);
    }

    return q.top();
}

void getValues(HuffTree *root, string s, map<char, string> &vtr){
    if(!root){
        return;
    }
    //if root is a leaf note
    if(root->letter != '~'){
        vtr[root->letter] = s;
    }
    else {
        getValues(root->left, s+"0", vtr);
        getValues(root->right, s+"1", vtr);
    }
}


string decode(HuffTree *tree, string s, int &i){
    string str="";

    while(i<=s.size()){
        if(tree->letter!='~'){
            str+=tree->letter;
            return str;
        }
        else if(s[i]=='0'){
            if(tree->left){
                tree=tree->left;
                i++;
            }
            else{
                return str;
            }
        }
        else if(s[i]=='1'){
            if(tree->right){
                tree=tree->right;
                i++;
            }
            else{
                return str;
            }
        }
    }
    return "";
}


void decodeFile(HuffTree *tree){
    string str1;
    ifstream fin;
    fin.open("encoded.dat");
    stringstream buffer;
    buffer<<fin.rdbuf();
    str1 = buffer.str();
    fin.close();

    string back = "";
    for(int i=0; i<str1.size();){
        back += decode(tree, str1, i);
    }

    
    ofstream out1;
	out1.open("decode.txt");
    out1<<back;
    out1.close();


}

void encodeFile(string res){
    ofstream out("encoded.dat", ios::out | ios::binary);
    
    out<<res;
    out.close();
}

int main(){
    string s;
    
    HuffTree *tree;
    vector<pair<int, char>> frq;
    map<char, string> vtr;
    string res = "";

    int ch;
    while(true){
        cout<<endl;
        cout<<"Enter 1 for encoding\nEnter 2 for decoding\nEnter 3 for exit"<<endl;
        cout<<"Enter Choice:";
        cin>>ch;
        switch(ch){
            case 1:
            s = file_to_text();

            frq = frequencyDect(s);
            sort(frq.begin(), frq.end());
            tree = makeTree(frq);

            getValues(tree, "", vtr);
            
            
            for(auto i:s){
                res += vtr[i];
            }
            encodeFile(res);
            cout<<endl<<"File encoded successfully"<<endl;
            break;

            case 2:
            decodeFile(tree);
            cout<<endl<<"File decoded successfully"<<endl;
            break;
            
            case 3:
            exit(0);
            
            default:
            cout<<"INVALID CHOICE"<<endl;

        }
    }

}   