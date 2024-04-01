#include <iostream>
#include <string>
#include <deque>
#include <algorithm>
using namespace std;

int main(){
    deque<string> dq;
    string operation;
    while(cin>>operation){
        if(operation == "Insert"){
            string s;
            cin>>s;
            dq.push_back(s);
        }
        else if(operation == "Shift"){
            int n;
            cin>>n;
            char c;
            cin>>c;
            dq[n].erase(dq[n].begin());
            dq[n].push_back(c);
        }
        else if(operation == "Reverse"){
            int n;
            cin>>n;
            reverse(dq[n].begin(), dq[n].end());
        }
        else if(operation == "Print"){
            for(auto s : dq){
                for(auto i : s){
                    cout<<i<<" ";
                }
                cout<<endl;
            }
        }
        else if(operation == "Transpose"){
            auto *its = new string::iterator [dq.size()];
            for(int i=0; i<dq.size(); i++){
                its[i] = dq[i].begin();
            }
            for(int j = 0; j<dq[0].size(); j++){
                for(int i=0; i<dq.size(); i++){
                    cout<<*its[i]<<" ";
                    its[i]++;
                    
                }
                cout<<endl;
            } 
            delete[] its;
        }
    }
}