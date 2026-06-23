// Author：李芸希 B133245007
// Date：Nov. 4, 2025
/* Purpose：

 */
#include<bits/stdc++.h>
using namespace std;
class node{
public:
    list<pair<int, int>> nums; // <係數，次方>

    // 輸入多項式
    void input(int P) {
        nums.clear();
        for (int i = 0; i < P; i++) {
            int coef, exp;
            cin >> coef >> exp;
            nums.push_back({coef, exp});
        }
    }
    
    // 依次方由大到小排序
    void sortexp()
    {  
        nums.sort([](auto &a, auto &b){
            return a.second > b.second; 
        });
    }
    
    // 刪除多項式中係數為0的項
    void deletezero()
    {
        for(auto it=nums.begin() ; it!=nums.end() ;){
            if(it->first==0){   // 當係數為 0
                it=nums.erase(it); //  刪除該項，回傳下一個有效位置
            }else{
                ++it;  
            }
        }
        if(nums.empty()){ // 若多項式中所有項係數都是 0
            nums.push_back({0,0}); // 則加入 0 項
        }
    }  
    
    void combine(){  // 合併多項式中相同次方的項
        sortexp();
        for(auto it=nums.begin() ; it!=nums.end() ;){
            auto itnext=next(it);
            if(itnext!=nums.end() && (it->second == itnext->second)){//判斷(nextIt 還有下一項)及(it 和 nextIt 的次方是否一樣)
                itnext->first += it->first; //將 nextIt 的係數加上 it 的係數
                it=nums.erase(it); // 刪除重複的項，且得新的list 
            }else{  // 若次方不同
                ++it;  // 則往下一項檢查
            }
        }
        deletezero(); // 刪除係數為 0 的項
    }

    void print(){  //印出多項式
        for(auto [coef, exp]: nums){
            cout << coef << " " << exp << endl; 
        }  
    }   

    node operator + (const node &N){ // 合併兩個多項式 
        node result=*this;
        for(auto [coef, exp] : N.nums){
            result.nums.push_back({coef, exp});
        }
        result.combine();
        return result;
        
        
    } 
        
    node operator * (const node &N){ // 相乘兩個多項式 
        node result;
        for(auto [coef1, exp1] : this->nums){
            for(auto [coef2, exp2] : N.nums){
                result.nums.push_back({coef1*coef2, exp1+exp2});
            }
        } 
        result.combine();
        return result;
    }
};  


int main(){
    node p, q;
    int P, Q;  
    int c =1;

    do{
        cin >> P ;  // 多項式1的項數
        p.input(P);
        cin >> Q ;  // 多項式2的項數
        q.input(Q);
        
        if(P==0 && Q==0){
            break;
        }
        
        cout << "Case" << c << ":" << endl;
    
       

        cout << "ADD" << endl;  
        node Add = p + q;
        Add.print();
    

        cout << "MULTIPLY" << endl;
        node Multi = p * q;
        Multi.print();
        

    }while(1);
      
    return 0;
}