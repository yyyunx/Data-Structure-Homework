// B133245007 李芸希  
// 2024/11/13  
// 題目：用自建的 linked list 來計算多項式的加法與乘法

#include <bits/stdc++.h>
using namespace std; 

// 建立一個多項式類別，每個節點代表一個項（係數 + 次方）
class poly{
	public:
		// 建構子：初始化所有欄位
		poly(){
			coef = 0;   // 預設係數是 0
			exp = 0;    // 預設次方是 0
			pre = next = head = tail = NULL; // 指標全部設成空
		}

		// 把一項 (coef, exp) 加進 linked list
		void push(int coef, int exp);

		// 多項式加法的運算子多載
		friend poly operator +(poly A , poly B);

		// 用來印出多項式內容的運算子多載
		friend ostream& operator << (ostream& outs , poly A);

		// 多項式乘法的運算子多載
		friend poly operator *(poly A , poly B);

		// 刪除係數為 0 的節點
		void deletenode();

		// 節點儲存的資料
		int coef; // 係數
		int exp;  // 次方

		// 下面四個是指標，用來組成雙向 linked list
		class poly *pre, *next, *head, *tail;
};

int main() {
	int thecase = 1; // 用來記錄第幾個測資案例
	
	while (1) {
		poly A, B;   // 宣告兩個多項式
		int p, q;    // p, q 是項數

		// 輸入第一個多項式
		cin >> p;
		for (int i = 0; i < p; i++) {
			int coef, exp;
			cin >> coef >> exp;
			A.push(coef, exp); // 把每項丟進 A 裡
		}

		// 輸入第二個多項式
		cin >> q;
		for (int i = 0; i < q; i++) {
			int coef, exp;
			cin >> coef >> exp;
			B.push(coef, exp); // 把每項丟進 B 裡
		}

		// 當 p 和 q 都是 0，就代表輸入結束
		if (p == 0 && q == 0) {
			break;
		}
			
		cout << "Case" << thecase << ":" << endl;	

		// 做加法 
		poly C = A + B; 
		C.deletenode(); // 把結果中係數為 0 的項刪掉
		cout << "ADD" << endl;
		cout << C;      // 輸出加法結果
		
		// 做乘法 
		poly D = A * B; 
		D.deletenode(); // 把結果中係數為 0 的項刪掉
		cout << "MULTIPLY" << endl;
		cout << D;      // 輸出乘法結果
		
		thecase++; // 下一組測資
		cout << endl;
	}
	return 0;
}

// 把一項 (coef, exp) 插入 linked list
void poly::push(int coef, int exp){
	poly *newnode, *current, *middlenode;
	newnode = new poly;       // 建立一個新的節點
	newnode->coef = coef;     // 設定它的係數
	newnode->exp = exp;       // 設定它的次方
	newnode->pre = NULL;
	newnode->next = NULL;
	
	current = head;

	// 先檢查整個 list，看看有沒有一樣的次方
	while (current != NULL) {
		if (current->exp == newnode->exp) {
			break; // 找到相同次方的項
		}
		else if (current == tail) {
			break; // 已經走到尾端就停止
		}
		current = current->next;
	}
	
	// 如果 list 是空的（第一次插入）
	if (head == NULL) {
		head = tail = newnode;
	}
	// 如果次方相同，就把係數加在一起
	else if (current->exp == newnode->exp) {
		current->coef += newnode->coef;
	}
	// 如果是新的次方，要插入正確位置
	else {
		// 如果新項次方比頭還大，要插在最前面
		if (newnode->exp > head->exp) {
			head->pre = newnode;
			newnode->next = head;
			head = newnode;
		}
		// 如果新項次方比尾還小，要插在最後
		else if (newnode->exp < tail->exp) {
			tail->next = newnode;
			newnode->pre = tail;
			tail = newnode;
		}
		// 否則插在中間
		else {
			middlenode = head;
			// 找到介於 head 和 tail 之間的插入位置
			if (!(middlenode->exp > newnode->exp && middlenode->next->exp < newnode->exp)) {
				middlenode = middlenode->next;
			}		
			newnode->pre = middlenode;
			newnode->next = middlenode->next;
			middlenode->next->pre = newnode;
			middlenode->next = newnode;
		}
	}
}

// 多項式加法 A + B
poly operator +(poly A, poly B){
	poly thenew, *current;

	// 先把 A 的所有項加進 thenew
	current = A.head;
	while (current != NULL) {
		thenew.push(current->coef, current->exp);
		current = current->next;
	}

	// 再把 B 的所有項加進 thenew
	current = B.head;
	while (current != NULL) {
		thenew.push(current->coef, current->exp);
		current = current->next;
	}

	return thenew;	
}

// 輸出多項式內容
ostream& operator <<(ostream& outs, poly A){
	poly *current;
	current = A.head;

	// 如果整個 list 是空的，就印出 0 0
	if (current == NULL) {
		outs << 0 << " " << 0 << endl;
	}
	else {
		// 依序輸出每個節點的係數與次方
		while (current != NULL) {
			outs << current->coef << " " << current->exp << endl;
			current = current->next;
		}
	}
	return outs;
}

// 多項式乘法 A * B
poly operator *(poly A, poly B){
	poly thenew, *Acurrent, *Bcurrent;
	Acurrent = A.head; // A 從頭開始
	
	// 雙層迴圈：A 的每一項都去乘上 B 的每一項
	while (Acurrent != NULL) {
		Bcurrent = B.head; // B 也從頭開始
		while (Bcurrent != NULL) {
			// 係數相乘，次方相加
			thenew.push(Acurrent->coef * Bcurrent->coef, Acurrent->exp + Bcurrent->exp);
			Bcurrent = Bcurrent->next;
		}
		Acurrent = Acurrent->next;
	}
	return thenew;	
}

// 刪除係數為 0 的節點
void poly::deletenode(){
	poly *current, *temp;
	current = head; // 從頭開始檢查

	while (current != NULL) {
		// case 1：只有一個節點，而且係數是 0
		if (current == head && current == tail && current->coef == 0) {
			current = head = tail = NULL;
		}
		// case 2：刪除頭節點
		else if (current == head && current->coef == 0) {
			temp = head;
			head->next->pre = NULL;
			head = head->next;
			current = head;
			delete temp;
		}
		// case 3：刪除中間節點
		else if (current != head && current != tail && current->coef == 0) {
			temp = current;
			current->pre->next = current->next;
			current->next->pre = current->pre;
			current = current->next;
			delete temp;
		}
		// case 4：刪除尾節點
		else if (current == tail && current->coef == 0) {
			temp = tail;
			tail->pre->next = NULL;
			tail = tail->pre;
			current = tail;
			delete temp;
		}
		// 沒要刪除就往下一個節點走
		else {
			current = current->next;
		}
	}
}
