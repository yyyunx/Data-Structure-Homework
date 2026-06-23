// Author: B133245007 李芸希
// Date  : 2025/12/01
/*
Purpose:
    本程式實作 Huffman 壓縮與解壓縮功能。
    壓縮時讀取輸入檔並建立 Huffman 樹，
    產生對應編碼後輸出 .huff 壓縮檔；
    解壓縮時由壓縮檔重建樹並還原原始資料。

    支援以下指令：
        huffman -c -i infile -o outfile   // 壓縮
        huffman -u -i infile -o outfile   // 解壓縮
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <cstdint>
#include <iomanip>
#include <algorithm>
using namespace std;

// --------------------------
// Huffman Node
// --------------------------
struct Node {
    uint8_t val;       // 葉節點的 byte 值
    int freq;          // 頻率或子樹頻率和
    string dict;       // 子樹所有字元字典序字串（用於 tie-break）
    Node* left;
    Node* right;

    // 葉節點建構子
    Node(uint8_t v, int f)
        : val(v), freq(f), dict(string(1, v)), left(nullptr), right(nullptr) {}

    // 內部節點建構子
    Node(Node* l, Node* r) : left(l), right(r) {
        freq = l->freq + r->freq;

        // 用 dict 字典序決定左右子樹順序
        if (l->dict < r->dict) {
            dict = l->dict + r->dict;
            val = l->val;
        } else {
            dict = r->dict + l->dict;
            val = r->val;
            swap(left, right);    // 固定左右結構
        }
    }
};

// --------------------------
// priority_queue 比較器
// --------------------------
struct Compare {
    bool operator()(Node* a, Node* b) {
        if (a->freq != b->freq) 
            return a->freq > b->freq;
        return a->dict > b->dict; // 頻率相同用 dict 字典序
    }
};

// --------------------------
// Huffman Tree
// --------------------------
class HuffmanTree {
public:
    Node* root = nullptr;

    // 建樹
    void build(const vector<uint8_t>& data) {
        int freq[256] = {0};

        for (uint8_t c : data) freq[c]++;

        priority_queue<Node*, vector<Node*>, Compare> pq;
        for (int i = 0; i < 256; i++)
            if (freq[i]) pq.push(new Node(i, freq[i]));

        if (pq.empty()) return;

        while (pq.size() > 1) {
            Node* a = pq.top(); pq.pop();
            Node* b = pq.top(); pq.pop();
            pq.push(new Node(a, b));
        }

        root = pq.top();
    }

    // 產生 Huffman code
    void genCode(Node* cur, string code, vector<string>& table) {
        if (!cur) return;

        if (!cur->left && !cur->right) {
            table[cur->val] = code.empty() ? "0" : code;
            return;
        }

        genCode(cur->left,  code + "0", table);
        genCode(cur->right, code + "1", table);
    }


    // 樹序列化（後序）
    void genPost(Node* cur, vector<uint8_t>& out) {
        if (!cur) return;

        if (!cur->left && !cur->right) {
            out.push_back(1);
            out.push_back(cur->val);
            return;
        }

        genPost(cur->left, out);
        genPost(cur->right, out);
        out.push_back(0);
    }

    // 重建樹（後序）
    void buildFromPost(const vector<uint8_t>& post) {
        stack<Node*> stk;

        for (size_t i = 0; i < post.size(); i++) {
            if (post[i] == 1) {
                i++;
                stk.push(new Node(post[i], 0));
            } else {
                Node* r = stk.top(); stk.pop();
                Node* l = stk.top(); stk.pop();
                stk.push(new Node(l, r));
            }
        }

        root = stk.top();
    }
};

// --------------------------
// bit 打包
// --------------------------
void writeBits(const vector<uint8_t>& data,
               const vector<string>& table,
               vector<uint8_t>& encoded)
{
    string bits;
    for (uint8_t c : data) bits += table[c];

    uint8_t buffer = 0;
    int count = 0;

    for (char b : bits) {
        buffer = (buffer << 1) | (b == '1');
        if (++count == 8) {
            encoded.push_back(buffer);
            buffer = 0;
            count = 0;
        }
    }

    if (count > 0) {
        buffer <<= (8 - count);
        encoded.push_back(buffer);
    }
}

// --------------------------
// bit 解碼
// --------------------------
vector<uint8_t> readBits(Node* root,
                          const vector<uint8_t>& encoded,
                          int originalSize)
{
    vector<uint8_t> result;
    Node* cur = root;

    for (uint8_t byte : encoded) {
        for (int i = 7; i >= 0; i--) {
            cur = ((byte >> i) & 1) ? cur->right : cur->left;

            if (!cur->left && !cur->right) {
                result.push_back(cur->val);
                if ((int)result.size() == originalSize)
                    return result;
                cur = root;
            }
        }
    }
    return result;
}

// --------------------------
// 壓縮
// --------------------------
void compress(const char* inFile, const char* outFile) {
    ifstream fin(inFile, ios::binary);
    vector<uint8_t> data((istreambuf_iterator<char>(fin)), {});
    fin.close();

    HuffmanTree tree;
    tree.build(data);

    vector<string> table(256);
    tree.genCode(tree.root, "", table);

    vector<uint8_t> encoded;
    writeBits(data, table, encoded);

    vector<uint8_t> post;
    tree.genPost(tree.root, post);

    ofstream fout(outFile, ios::binary);

    unsigned int originalSize   = data.size();
    unsigned int compressedSize = encoded.size();
    unsigned int postSize       = post.size();

    fout.write((char*)&originalSize, 4);
    fout.write((char*)&compressedSize, 4);
    fout.write((char*)&postSize, 4);

    for (uint8_t b : post) fout.write((char*)&b, 1);
    for (uint8_t b : encoded) fout.write((char*)&b, 1);

    fout.close();

    cout << "Original size (bytes): " << originalSize << "\n";
    cout << "Compressed size (bytes, with header): "
         << (12 + postSize + compressedSize) << "\n";
    cout << "Compression ratio: "
         << fixed << setprecision(3)
         << (double)originalSize / (12 + postSize + compressedSize) << "\n";

    // 印出編碼表
    cout << "Encoding table:\n";
    for (int i = 0; i < 256; i++)
        if (!table[i].empty())
            cout << (char)i << " = " << table[i] << "\n";
}

// --------------------------
// 解壓縮
// --------------------------
void decompress(const char* inFile, const char* outFile) {
    ifstream fin(inFile, ios::binary);

    unsigned int originalSize, compressedSize, postSize;
    fin.read((char*)&originalSize, 4);
    fin.read((char*)&compressedSize, 4);
    fin.read((char*)&postSize, 4);

    vector<uint8_t> post(postSize);
    for (uint8_t &b : post) fin.read((char*)&b, 1);

    vector<uint8_t> encoded(compressedSize);
    for (uint8_t &b : encoded) fin.read((char*)&b, 1);

    fin.close();

    HuffmanTree tree;
    tree.buildFromPost(post);

    vector<uint8_t> restored = readBits(tree.root, encoded, originalSize);

    ofstream fout(outFile, ios::binary);
    for (uint8_t b : restored) fout.write((char*)&b, 1);
    fout.close();

    cout << "Original size (bytes): " << originalSize << "\n";
    cout << "Compressed size (bytes, with header): "
         << (12 + postSize + compressedSize) << "\n";
    cout << "Compression ratio: "
         << fixed << setprecision(3)
         << (double)originalSize / (12 + postSize + compressedSize) << "\n";
}

// --------------------------
// Main
// --------------------------
int main(int argc, char* argv[]) {
    if (argc != 6) {
        cout << "Usage:\n"
             << "  huffman -c -i infile -o outfile\n"
             << "  huffman -u -i infile -o outfile\n";
        return 1;
    }

    string mode    = argv[1];
    string inFile  = argv[3];
    string outFile = argv[5];

    if (mode == "-c")
        compress(inFile.c_str(), outFile.c_str());
    else if (mode == "-u")
        decompress(inFile.c_str(), outFile.c_str());
    else
        cout << "Unknown mode: " << mode << "\n";

    return 0;
}
