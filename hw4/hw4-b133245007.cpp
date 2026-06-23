// Author：李芸希 B133245007
// Date：Oct. 12, 2025
/* Purpose：利用遞迴(recurvise)完成 Knight’s Tour。
            程式會從左上角(1,1)出發，使用 backtracking，
            走 n×n 棋盤上每一格，只拜訪一次並記錄步數。
            若有解則輸出棋盤步數順序；若無解則顯示 "no solution"。      
 */

#include<bits/stdc++.h>
using namespace std;

// 遞迴函式：嘗試讓騎士走完整個棋盤
bool knight(int board[7][7] ,int n , int i , int j , int step){

    // 若走滿 n*n 格，即成功，回傳 true
    if(step==n*n)   return true;

    // 騎士可以移動的 8 個方向
    int dx[8]={-2, -1, 1, 2, 2, 1, -1, -2};
    int dy[8]={1, 2, 2, 1, -1, -2, -2, -1};

    // 嘗試每一個方向
    for(int d=0 ; d<8 ; d++){

        // 計算下一步的座標
        int ni=i+dx[d];
        int nj=j+dy[d];

        // 檢查是否出界、新位置是否已走過
        if(ni>=0 && nj>=0 && ni<n && nj<n && board[ni][nj]==0){

            // 在位置標記是第幾步
            board[ni][nj]=step+1; 

            // 嘗試下一步
            if(knight(board,n,ni,nj,step+1)){
                return true;    // 若下一步成功則回傳 true
            }else{
                board[ni][nj]=0;    // 若下一步失敗則使位置為 0
            }
        }
    } 
    // 8 個方向都嘗試過，但失敗，回傳 false
    return false;
}

int main(){
    int board[7][7]; // 令棋盤最大 7*7

    // 嘗試 n = 1~6 
    for(int n=1 ; n<=6 ; n++){

        cout << "n = " << n << ":" << endl ;

        // 初始化棋盤為 0
        for(int x=0 ; x<7 ; x++){
            for(int y=0 ; y<7 ; y++){
                board[y][x]=0;
            } 
        }

        // 起點座標[0][0]，值為 1
        board[0][0]=1;

        // 判斷是否有解
        if(knight(board, n, 0, 0, 1)){  // 若有解

            // 輸出整個 board
            for(int i=0 ; i<n ; i++){
                for(int j=0 ; j<n ; j++){
                    cout << setw(3) << board[i][j];
                }
                cout << endl;
            }
            cout << endl;
        }else{  // 若無解
            cout<<"no solution"<< endl << endl;
        }
 
    }

    return 0;
}