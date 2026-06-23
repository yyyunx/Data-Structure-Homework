#include<bits/stdc++.h>
using namespace std;

void uni(vector<string> &v){
    int union_set[256]={0};
    for(auto &c: v[0])
        union_set[ c ]++;

    for(auto &c: v[1])
        union_set[ c ]++;

    cout << "A+B:{";
    for(int i=0 ; i<256 ; i++){        
        if(union_set[i]>0){
            cout << (char)i;
        }
    }
    cout << "}" << endl;
}

void intersection(vector<string> &v){
    int inter_set[256]={0};
    for(auto &c: v[0])
        inter_set[ c ]++;

    for(auto &c: v[1])
        inter_set[ c ]++;
    cout << "A*B:{" ;
    for(int i=0 ; i<256 ; i++){        
        if(inter_set[i]>1){
            cout << (char)i;
        }
    }
    cout << "}" << endl;
}

void difference1(vector<string> &v){
    int diff1_set[256]={0};
    for(auto &c: v[0])
        diff1_set[c]++;
    for(auto &c: v[1])
        diff1_set[c]--;
    
    cout << "A-B:{" ;
    for(int i=0 ; i<256 ; i++){
        if(diff1_set[i]>0)
           cout << (char)i;            
    }
    cout << "}" << endl;
}

void difference2(vector<string> &v){
    int diff2_set[256]={0};
    for(auto &c: v[0])
        diff2_set[c]--;
    for(auto &c: v[1])
        diff2_set[c]++;
    
    cout << "B-A:{" ;
    for(int i=0 ; i<256 ; i++){
       if(diff2_set[i]>0)
            cout << (char)i;            
    }
    cout << "}" << endl;      
}

int main(){
    int n;          // 有幾組測資
    cin>>n;
    int nums=1;     // 已處理幾組測資

    char array[256];
   
    
    char arr[256];
    arr[0]=array[0];
    
    
    while(nums<=n){  //有n組測資，處理到第nums組
        vector<string> v;
        
        for(int test=1 ; test<=2 ; test++){
            
            int k=1;
            cin>>array;
            arr[0]=array[0];

            //計算輸入幾個字元
            for(int i=1 ; array[i]!='\0' ; i++){
                bool repeat=false;
            
                //確定有無重複字元
                for(int j=0 ; j<i ; j++){
                    if(array[i]==array[j]){
                        repeat=true;
                        break;
                    }           
                }

                //如果不重複就輸出
                if(!repeat){
                    arr[k]=array[i];
                    k++;
                }        
            }

            //排序
            for(int i=0 ; i<k-1 ; i++){
                for(int j=i+1 ;j<k ; j++){

                    //冒泡排序
                    if(arr[i]>arr[j]){
                        swap(arr[i],arr[j]);
                    }
                }
            }
            

           
            arr[k]='\0';
            v.push_back(arr);
        }
        cout << "Test Case " << nums << ":" << endl;
        cout << "A:{" << v[0] << "}" << endl;
        cout << "B:{" << v[1] << "}" << endl;
        
        uni(v);
        intersection(v);
        difference1(v);
        difference2(v);

        nums++;
        cout << endl;
    }
    return 0;
}