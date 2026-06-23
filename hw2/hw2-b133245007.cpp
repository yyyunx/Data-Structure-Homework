// Author：李芸希 B133245007
// Date：Sep.14,2025
// Purpose：To set the setclasses and find union,intersection,difference,contain.

#include <bits/stdc++.h>
using namespace std;

// Class representing a set of ASCII characters using a boolean array
class TSet {
public:
    bool array[256]; // array[i] = true if character with ASCII code i is in the set

    // Reset all elements to false (empty set)
    void reset() {
        for (int i = 0; i < 256; i++) {
            array[i] = false;
        }
    }

    // Default constructor: create an empty set
    TSet() {
        reset();
    }

    // Constructor: build set from a string
    TSet(const string s) {
        reset();
        for (auto &c : s) {
            array[(unsigned char)c] = true; // mark this character as present
        }
    }

    // Union of two sets (A + B)
    TSet operator+(const TSet& T) {
        TSet res;
        for (int i = 0; i < 256; i++) {
            if (array[i] || T.array[i]) { // if element exists in A or B
                res.array[i] = true;
            }
        }
        return res;
    }

    // Intersection of two sets (A * B)
    TSet operator*(const TSet& T) {
        TSet res;
        for (int i = 0; i < 256; i++) {
            if (array[i] && T.array[i]) { // if element exists in both A and B
                res.array[i] = true;
            }
        }
        return res;
    }

    // Difference of two sets (A - B)
    TSet operator-(const TSet& T) {
        TSet res = *this; // start with copy of A
        for (int i = 0; i < 256; i++) {
            if (array[i]) {              // if element exists in A
                if (T.array[i]) {        // and also exists in B
                    res.array[i] = false; // remove it from result
                }
            }
        }
        return res;
    }

    // Containment check (A >= B means A contains B)
    bool operator>=(const TSet& T) {
        for (int i = 0; i < 256; i++) {
            if (T.array[i]) {       // if B has this element
                if (!array[i]) {    // but A does not have it
                    return false;   // then A does not contain B
                }
            }
        }
        return true; // all elements of B exist in A
    }

    // Membership check: is character c in the set?
    bool in(const char& c) {
        if (array[(unsigned char)c]) { // if marked as present
            return true;
        } else {                       // if not marked
            return false;
        }
    }

    // Overload << to print set elements
    friend ostream& operator<<(ostream& out, const TSet& T) {
        out << "{";
        for (int i = 0; i < 256; i++) {
            if (T.array[i]) {        // if element exists in set
                out << (char)i;
            }
        }
        out << "}";
        return out;
    }
};

int main() {

    // 開啟輸出檔
    ofstream out("hw2-b133245007.out");

    int t; // number of test cases
    cin >> t;

    // Process each test case
    for (int i = 1; i <= t; i++) {
        string sA, sB;
        char x;
        cin >> sA >> sB >> x; // input: set A, set B, and one character

        // Build sets from input
        TSet A = sA;
        TSet B = sB;

        // Perform set operations
        TSet C = A + B; // union
        TSet D = A * B; // intersection
        TSet E = A - B; // A - B
        TSet F = B - A; // B - A

        // Output results
        out << "Test Case " << i << ":\n";
        out << "A: " << A << "\n";
        out << "B: " << B << "\n";
        out << "A + B: " << C << "\n";
        out << "A * B: " << D << "\n";
        out << "A - B: " << E << "\n";
        out << "B - A: " << F << "\n";

        // Containment checks
        out << "A " << ((A >= B) ? "contains" : "does not contain") << " B\n";
        out << "B " << ((B >= A) ? "contains" : "does not contain") << " A\n";

        // Membership checks
        out << "'" << x << "' is" << (A.in(x) ? "" : " not") << " in A\n";
        out << "'" << x << "' is" << (B.in(x) ? "" : " not") << " in B\n";

        out << "\n"; // blank line between test cases
    }
    out.close();// close the output file
    return 0;
}
