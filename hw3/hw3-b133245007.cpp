// Author：李芸希 B133245007
// Date：Oct.07,2025
/* Purpose：利用自己實作的堆疊(stack)取代遞迴實現西洋棋騎士巡邏(Knight’s Tour)。
            程式會從左上角(1,1)出發，嘗試回溯法(backtracking)
            走訪 n×n 棋盤上每一格，只拜訪一次並記錄步數；若有解則輸出棋盤步數順序，
            若無解則顯示 "no solution"。                                         */

#include<bits/stdc++.h>
using namespace std;

// 節點：位置(i, j)、前進的方向(d)
struct Node{
    int i;
    int j; 
    int d;
};

// 自己實作的 stack 類別
class stk{
    public:
        // 騎士可以移動的 8 個方向
        int dx[8]={-2, -1, 1, 2, 2, 1, -1, -2};
        int dy[8]={1, 2, 2, 1, -1, -2, -2, -1};


        Node data[100]={0};
        int top=-1;
        
        // push 進一個 Node
        void push(Node N){
            top++;
            data[top]=N;
        }

        // pop 掉一個  
        void pop(){
            top--;
        }
        
        // 清空 stack
        void clear(){
            top=-1;
        }

        bool knight(int n, int board[7][7]){

            clear();        // 清空 stack

            int i=0, j=0;   // 起點座標
            int step=1;     // 目前是第幾步

            // 初始化棋盤，全部為 0
            for(int x=0 ; x<n ; x++){
                for(int y=0 ; y<n ; y++){
                    board[x][y]=0;
                }
            }

            // 起點為 1 ，位置在 (0,0)
            board[0][0]=step;   
            push({0, 0, 0});  // 起點 push 進 stack
            

            // 開始執行，直到走不下去（stack 空）
            do{

                // 若填滿格子，結束
                if(step==n*n)  return true;  

                // 從 stack 頂端取目前位置和方向
                Node cur = data[top];
                i = cur.i;
                j = cur.j;
                int d = cur.d;

                bool move=false;

                 // 從 stack 頂端那一格開始試，嘗試沒走過的方向(d)
                for(int d=data[top].d ; d<8 ; d++){

                    // 移動的新座標
                    int ni=i+dx[d];
                    int nj=j+dy[d];

                    // 判斷是否可以走
                    if(board[ni][nj]==0 && ni>=0 && nj>=0 && ni<n &&nj<n){
                        move=true;      // 可走

                        // 更新目前節點的方向，下次回來從下一方向繼續
                        data[top].d=d+1;

                        board[ni][nj]=++step;   //  新位置標記為第 step 步
                        push({ni, nj, 0});      //  push 新格子(方向從 0 開始)
                        i=ni;
                        j=nj;
                        break;   // 找到就往下走，不再試其他方向 
                    }
                }

                // 當無法走
                if(!move){      
                    board[i][j]=0; // 回復原狀
                    pop();      // 從 stack 拿掉這步
                    step--;     // 步數倒退
                }      
            }while( top!=-1  ); // 只要還有步驟，就繼續嘗試

            return false; // stack 空了，找不到解
        }
};



int main(){
    stk stk;

    // 嘗試 n=1 ~ 6 
    for(int n=1 ; n<=6 ; n++){
        int board[7][7];    // 棋盤最大 7x7
        bool found = stk.knight(n, board);

        cout << "n=" << n << " Knight's Tour:" << endl;

        // 如果有解，就印出棋盤的走法
        if(found){
            for(int i=0 ; i<n ; i++){
                for(int j=0 ; j<n ; j++){
                    cout << setw(3) << board[i][j] <<" ";
                }
                cout << endl;
            }
        //  如果無解，輸出no solution
        }else{
            cout << "no solution" <<endl;
        }
        cout << endl;
    }

    return 0;
}

