// Author：李芸希 B133245007
// Date：Sep.16,2025
// Purpose：階乘運算，透過逐位數進位來計算大數階乘，
//          來避免整數溢位問題，最終輸出正確的階乘結果。
         
#include<bits/stdc++.h>
using namespace std;

int main(){
   
    int n;         //輸入測資
    int num[100];  //存放每一位值
    int ch[100];   // 標記哪一位有值
    
    vector<int> nums;  // 存放測資的vector

    // 讀取多筆輸入，直到輸入0結束
    while(cin>>n && n!=0 ){
        nums.push_back(n);
    }

    // 處理每一筆測資
    for(int N=0;N<nums.size();N++){  
        // 從1!~n!
        for(int y=1;y<=nums[N];y++){  
            fill(ch, ch+ 100, 0);     // 把ch[0]~ch[99]設為 0
            fill(num, num+ 100, 0);   // 把num[0]~num[99]設為 0

            num[0]=1;   // 階乘初始值 = 1
            ch[0]=1;    // 標記第0位有數字

            cout<<y<<"!="; 

            // 計算 y!
            for(int j=1;j<=y;j++){  
                // n的每個位數都乘n-1
                for(int i=0;i<100;i++){  
                    num[i]*=j;
                }
                // 每個位數進位
                for(int x=0;x<100;x++){  
                        if(num[x]>=10){   
                            num[x+1]+=num[x]/10;  // 把進位加到下一位
                            num[x]%=10;           // 保留該位數字 (0~9)
                            ch[x+1]++;            // 標記該位有值存放
                        }
                    }           
            }
            // 輸出階層答案
            for(int k=99;k>=0;k--){  
                if(ch[k]!=0){
                    cout<<num[k];      
                }
            }
            cout<<endl;
        }  
        cout<<endl;
    }    
    
    return 0;
}