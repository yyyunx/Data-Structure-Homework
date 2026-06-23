// Author: B133245007 李芸希
// Date  : 2025/11/12

/*
Purpose:
    給予一串數字，找出所有出現次數為奇數之數字
    -> 利用 Binary Search Tree 來解決此問題

    檢查每一個數字:
    1. 若找到相等者，表示之前已經出現過奇數次，則刪除該 node
    2. 若未找到相等者，表示之前出現過偶數次（含零次），則將該數字插入
*/
#include <bits/stdc++.h>
using namespace std;

class Node{
public:
    int val;                // 節點存放的值
    Node* left = nullptr;   // 左子節點
    Node* right = nullptr;  // 右子節點
    Node(int value): val(value) {}  // 建構子
};

class BST{
private:
    Node* top = nullptr; // 樹根節點指標

public:
    // 插入新節點
    void insert(int key){
        Node* curNode = top;        
        Node* prevNode = nullptr;   

        // 一直往下找到空位
        while(curNode != nullptr){
            if(key < curNode->val){         
                prevNode = curNode;
                curNode = curNode->left;
            } else {                         
                prevNode = curNode;
                curNode = curNode->right;
            }
        }

        Node* newNode = new Node(key);

        if(prevNode == nullptr)
            top = newNode;
        else if(prevNode->val <= key)
            prevNode->right = newNode;
        else
            prevNode->left = newNode;
    }

    // 刪除指定數字的節點
    void erase(int key){
        Node* curNode = top;
        Node* prevNode = nullptr;

        while(curNode != nullptr){
            if(curNode->val == key){
                Node* parentchild = nullptr; // 刪除後要接上的子節點

                // Case 1：沒有子節點
                if(curNode->left == nullptr && curNode->right == nullptr){
                    parentchild = nullptr;
                    delete curNode;
                }
                // Case 2：只有右子節點
                else if(curNode->left == nullptr){
                    parentchild = curNode->right;
                    delete curNode;
                }
                // Case 2：只有左子節點
                else if(curNode->right == nullptr){
                    parentchild = curNode->left;
                    delete curNode;
                }
                // Case 3：有左右子節點
                else {
                    // 找右子樹最小值 (inorder successor)
                    int MINNode = findMin(curNode->right, curNode);
                    curNode->val = MINNode;  
                    parentchild = curNode;   
                }

                // 重新接上父節點
                if(prevNode == nullptr)
                    top = parentchild;
                else if(key >= prevNode->val)
                    prevNode->right = parentchild;
                else
                    prevNode->left = parentchild;
                return;
            }
            // 尚未找到 → 繼續往左或往右找
            else if(key < curNode->val){
                prevNode = curNode;
                curNode = curNode->left;
            }
            else {
                prevNode = curNode;
                curNode = curNode->right;
            }
        }
    }

    // 查找節點是否存在
    bool find(int key){
        Node* curNode = top;
        while(curNode != nullptr){
            if(curNode->val == key) return true;  
            if(key < curNode->val)
                curNode = curNode->left;
            else
                curNode = curNode->right;
        }
        return false; 
    }

    // 找右子樹最小節點 (inorder successor)，並刪除該節點
    int findMin(Node* curNode, Node* parent){
        if(curNode->left != nullptr)
            return findMin(curNode->left, curNode); 
        else{
            if(curNode->val >= parent->val)
                parent->right = curNode->right;
            else
                parent->left = curNode->right;

            int res = curNode->val; 
            delete curNode;         
            return res;
        }
    }

    // 中序遍歷 (左→根→右)，儲存節點值與左右子節點
    void travel(vector<vector<int>>& data, Node* cur) const{
        if(cur->left != nullptr)
            travel(data, cur->left);

        data[0].push_back(cur->val);

        if(cur->left != nullptr)
            data[1].push_back(cur->left->val);
        else
            data[1].push_back(0);

        if(cur->right != nullptr)
            data[2].push_back(cur->right->val);
        else
            data[2].push_back(0);

        if(cur->right != nullptr)
            travel(data, cur->right);
    }

    // 輸出整棵樹 (Node / Left / Right)
    friend ostream& operator << (ostream& out, const BST& T){
        if(T.top == nullptr){
            out << "Empty Tree!\n";
            return out;
        }

        vector<vector<int>> data (3, vector<int>());
        T.travel(data, T.top);

        out << "Node  :";
        for(int i=0;i<data[0].size();i++)
            out << setw(3) << data[0][i];

        out << "\nLeft  :";
        for(int i=0;i<data[1].size();i++)
            out << setw(3) << data[1][i];

        out << "\nRight :";
        for(int i=0;i<data[2].size();i++)
            out << setw(3) << data[2][i];

        out << "\n";
        return out;
    }
};

int main(){
    int n;
    bool first = true; // 控制每組輸出之間的空白行

    while(cin >> n){   
        BST T; // 建立新的 BST

        do{
            // 若數字已存在 → 表示目前為奇數次 → 刪除
            if(T.find(n))
                T.erase(n);
            // 否則 → 插入新節點
            else
                T.insert(n);

            cin >> n;
        }while(n != -1);  // 直到遇到 -1 結束該組輸入

        // 每組之間空一行（第一組不空）
        if(!first) cout << endl;
        first = false;

        // 輸出結果
        cout << T << endl;
    }

    return 0;
}
