// Author：李芸希 B133245007
// Date：Sep.14,2025
// Purpose：產生1~9的奇數階魔方陣

#include<bits/stdc++.h>
using namespace std;

void magic_square(int n, ofstream &out){
    
    // 建立 n×n 的2D陣列，且初始化為 0
    vector<vector<int>> mat(n, vector<int>(n, 0));

    int i=0,j=n/2;  // 設定起點：第一列中間
    int k=2;        // 從數字 2 開始填
    mat[i][j]=1;    // 1 填在起點

    // 重複直到填滿 n*n 個數字
    while(k<=n*n){
        
        // 若左上位置有數字，則下移一格
        if(mat[(i-1+n)%n][(j-1+n)%n]!=0){
            i=(i+1)%n;          // 往下移
            mat[i%n][j]=k;      // 直接填入
        }else{
            // 否則往左上填入
            mat[(i-1+n)%n][(j-1+n)%n]=k;
            i=(i-1+n)%n;
            j=(j-1+n)%n;
        }
        k++;    // 下個數字
    }
    // 輸出魔方陣
    for(int i=0 ;i<n ; i++){
        for(int j=0 ; j<n ; j++){
            out<<setw(4)<<mat[i][j];   // 讓數值對齊
        }
        out<<endl;

    }

}

// 主程式：產生並印出 1~9 中的所有奇數階魔方陣
int main(){

    // 開啟輸出檔
    ofstream out("hw1a-B133245007.out");

    // 取奇數：1, 3, 5, 7, 9
    for(int n=1;n<=9;n=n+2){

        out<<"\nMagic Square of size "<<n<<":\n\n";
        magic_square(n, out);
        out<<endl;
    }

    // 關閉檔案
    out.close();

    return 0;
}