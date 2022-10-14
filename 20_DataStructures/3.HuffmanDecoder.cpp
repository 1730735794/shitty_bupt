#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<queue>
#include<map>
#include<string>
#include<iomanip>
using namespace std;
const unsigned int EOF_WEIGHT = 1;//to meet req1

struct symbol_table
{
    char symbol;
    int weight;
};

struct HTNode
{
    symbol_table data;
    string Huffman_code;
    HTNode *left, *right;

    HTNode(char s, int w)
    {
        data.symbol = s;
        data.weight = w;
        left = NULL;
        right = NULL;
    }

    HTNode(HTNode *lc, HTNode *rc)
    {
        left = lc;
        right = rc;
        data.weight = lc->data.weight + rc->data.weight;
        data.symbol = '\0';
    }
};

struct cmp  //for pque
{
    bool operator()(HTNode *x, HTNode *y) const
    {
        return x->data.weight > y->data.weight;
    }
};

char menu_select();
HTNode* initialization();
HTNode* read_hfmTreefile();
HTNode* tree_build(vector<symbol_table> data);
void mapping(HTNode *root, map<char, string> &to_code);
void mapping(HTNode *root, map<string, char> &to_text);
void mapping(HTNode *root, string s = "");
void encoding(HTNode *root);
void decoding(HTNode *root);
void print();
void tree_printing(HTNode *root);
void node_printing(HTNode *root, ofstream &out, int max_depth, int depth);
int max_depth(HTNode *root);
void word_frequency_analysis();

int main()
{
    char c;
    HTNode* root = NULL;
    while(c = menu_select())
    {
        if(c == 'I' || c == 'i')
            root = initialization();
        if(c == 'E' || c == 'e')
        {
            if(root == NULL)
                root = read_hfmTreefile();
            encoding(root);
        }
        if(c == 'D' || c == 'd')
        {
            if(root == NULL)
                root = read_hfmTreefile();
            decoding(root);
        }
        if(c == 'P' || c == 'p')
            print();
        if(c == 'T' || c == 't')
        {
            if(root == NULL)
                    root = read_hfmTreefile();
            tree_printing(root);
        }
        if(c == 'W' || c == 'w')
            word_frequency_analysis();
        if(c == 'Q' || c == 'q')
            break;
    }
}

char menu_select()
{
    cout << "---------MainMenu---------" << endl;
    cout << "Press 'I' -> Initialization" << endl;
    cout << "Press 'E' -> Encoding" << endl;
    cout << "Press 'D' -> Decoding" << endl;
    cout << "Press 'P' -> Print" << endl;
    cout << "Press 'T' -> Tree printing" << endl;
    cout << "Press 'W' -> Word Frequency Analysis(extra function to meet requirement2)" << endl;
    cout << "Press 'Q' -> Quit" << endl;
    char c;
    cin >> c;
    return c;
}

HTNode* initialization()
{
    int size;
    cout << "please input the size of symbol table" << endl;
    cin >> size;
    vector<symbol_table> data(size);
    cout << "please input the pair of symbol and weight separated by a blank space" << endl;
    for(int i = 0; i < size; i++)
    {
        getchar();
        data[i].symbol = getchar();
        cin >> data[i].weight;
    }
    HTNode *root = tree_build(data);
    string des;
    cout << "please input the name of the output file" << endl;
    cin >> des;
    ofstream out(des);
    out << data.size() << endl;
    for(int i = 0; i < size; i++)
        out << data[i].symbol << data[i].weight << endl;
    out.close();
    cout << des << " file created" << endl;
    return root;
}

HTNode* tree_build(vector<symbol_table> data)
{
    priority_queue<HTNode*, vector<HTNode*>, cmp> pque;
    for(auto i = data.begin(); i != data.end(); i++)
    {
        HTNode *tmp = new HTNode(i->symbol, i->weight);
        pque.push(tmp);
    }
        HTNode *tmp = new HTNode(EOF, EOF_WEIGHT);
        pque.push(tmp);
    while(pque.size() != 1)
    {
        HTNode *tmp1, *tmp2;
        tmp1 = pque.top();
        pque.pop();
        tmp2 = pque.top();
        pque.pop();
        HTNode *parent = new HTNode(tmp1,tmp2);
        pque.push(parent);
    }
    mapping(pque.top());
    cout << "Huffman tree building completed" << endl;
    return pque.top();
}

HTNode* read_hfmTreefile()
{
    cout << "Huffman tree not found, please input the name of the Huffman tree file" << endl;
    string src;
    cin >> src;
    ifstream in(src);
    int size;
    char tmp;
    in >> noskipws >> size;
    vector<symbol_table> data(size);
    for(int i = 0; i < size; i++)
    {
        in >> tmp;
        in >> data[i].symbol;
        in >> data[i].weight;
    }
    in.close();
    return tree_build(data);
}

void mapping(HTNode *root, map<char, string> &to_code)
{
    if(root->data.symbol != '\0')
    {
        to_code.insert(make_pair(root->data.symbol, root->Huffman_code));
        return;
    }
    mapping(root->left, to_code);
    mapping(root->right, to_code);
}

void mapping(HTNode *root, map<string, char> &to_text)
{
    if(root->data.symbol != '\0')
    {
        to_text.insert(make_pair(root->Huffman_code, root->data.symbol));
        return;
    }
    mapping(root->left, to_text);
    mapping(root->right, to_text);
}

void mapping(HTNode *root, string s)
{
    root->Huffman_code = s;
    if(root->data.symbol != '\0')
        return;
    mapping(root->left, s + "0");
    mapping(root->right, s + "1");
}

void encoding(HTNode *root)
{
    map<char, string> to_code;
    mapping(root, to_code);
    string src, des;
    cout << "please input the name of the source file" << endl;
    cin >> src;
    cout << "please input the name of the output file" << endl;
    cin >> des;
    ifstream uncoded(src);
    ofstream encoded(des, ios::binary);
    string text, code, tmp;
    while(getline(uncoded, tmp))
        text += tmp + '\n';
    text += EOF;
    for(int i = 0; i < text.length(); i++)
        code += to_code[text[i]];
    while(code.length() % 8)
        code += '0';
    char buf = 0;
    for(int i = 0; i < code.length(); i++)
    {
        buf <<= 1;
        buf += code[i] - '0';
        if((i + 1) % 8 == 0)
        {
            encoded.write(&buf, 1);
            buf = 0;
        }
    }
    uncoded.close();
    encoded.close();
    cout << "encode completed, saved in .\\" << des << endl;
}

void decoding(HTNode *root)
{
    map<string, char> to_text;
    mapping(root, to_text);
    string src, des;
    cout << "please input the name of the source file" << endl;
    cin >> src;
    cout << "please input the name of the output file" << endl;
    cin >> des;
    ifstream encoded(src, ios::binary);
    ofstream decoded(des);
    string code, tmp;
    unsigned char buf;
    while(encoded.read((char*)(&buf), 1))
        for(int i = 7 ; i >= 0; i--)
            code += '0' + ((buf >> i) & 1);
    for(int i = 0; i < code.length(); i++)
    {
        tmp += code[i];
        if(to_text.find(tmp) != to_text.end())
        {
            if(to_text.find(tmp)->second == EOF)
                break;
            decoded << to_text.find(tmp)->second;
            tmp = "";
        }
    }
    encoded.close();
    decoded.close();
    cout << "decode completed, saved in .\\" << des << endl;
}

void print()
{
    string src, des;
    cout << "please input the name of the source file" << endl;
    cin >> src;
    cout << "please input the name of the output file" << endl;
    cin >> des;
    ifstream code(src, ios::binary);
    ofstream code_text(des);
    string s;
    unsigned char buf;
    while(code.read((char*)(&buf), 1))
        for(int i = 7 ; i >= 0; i--)
        {
            int x = (buf >> i) & 1;
            s += '0' + ((buf >> i) & 1);
        }
    cout << "printing .\\" << src << endl;
    for(int i = 0 ; i < s.length(); i++)
    {
        cout << s[i];
        code_text << s[i];
        if((i + 1) % 50 == 0)
        {
            cout << endl;
            code_text << endl;
        }
    }
    cout << endl;
    code.close();
    code_text.close();
    cout << "print completed, saved in .\\" << des << endl;
}

void tree_printing(HTNode *root)
{
    cout << "please input the name of the output file" << endl;
    string des;
    cin >> des;
    ofstream out(des);
    node_printing(root, out, max_depth(root), 1);
    out.close();
    cout << "tree printing completed, saved in .\\" << des << endl;
}

void node_printing(HTNode *root, ofstream &out, int max_depth, int depth)
{
    if(root == NULL)
        return;
    node_printing(root->right, out, max_depth, depth + 1);
    string tmp = to_string(root->data.weight);
    int digits = tmp.length();
    if(root->data.symbol == '\0')
    {
        for(int i = 0; i < 7 - digits; i++)
            tmp += '-';
        tmp += '|';
    }
    else
    {
        if(root->data.symbol == EOF)
        {
            tmp += "-\'EOF\'";
            for(int i = 0; i < 2 - digits; i++)
                tmp += ' ';
        }
        else if(root->data.symbol == '\n')
        {
            tmp += "-\'\\n\'";
            for(int i = 0; i < 3 - digits; i++)
                tmp += ' ';
        }
        else
        {
            tmp = tmp + "-\'" + root->data.symbol + "\'";
            for(int i = 0; i < 4 - digits; i++)
                tmp += ' ';
        }
    }
    for(int i = 0; i < root->Huffman_code.length(); i++)
    {
        if(root->Huffman_code[i] != root->Huffman_code[i+1])
        {
            cout << setw(8) << "|";
            out << setw(8) << "|";
        }
        else
        {
            cout << setw(8) << " ";
            out << setw(8) << " ";
        }
    }
    cout << setw(8) << tmp << endl;
    out << setw(8) << tmp << endl;
    node_printing(root->left, out, max_depth, depth + 1);
}

int max_depth(HTNode *root)
{
    if(root == NULL)
        return 0;
    return max(max_depth(root->left), max_depth(root->right)) + 1;
}

void word_frequency_analysis()
{
    cout << "this function will read and analysis a text file, and save the result the same way as hfmTree file" << endl;
    map<char, int> bucket;
    string src, des;
    cout << "please input the name of the source file" << endl;
    cin >> src;
    cout << "please input the name of the output file" << endl;
    cin >> des;
    ifstream text_file(src);
    ofstream result(des);
    string text, code;
    while(getline(text_file, text))
    {
        for(int i = 0; i < text.length(); i++)
            bucket[text[i]]++;
        bucket['\n']++;
    }
    result << bucket.size() << endl;
    for(auto i = bucket.begin(); i != bucket.end(); i++)
        result << i->first << i->second << endl;
    text_file.close();
    result.close();
    cout << "analysis completed, saved in .\\" << des << endl;
}
