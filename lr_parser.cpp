#include<iostream>
#include<regex>
#include<cstring>
#include<vector>
using namespace std;

using namespace std;

struct actionTable{
	string action;	  /** it can be shift, reduce or goto **/
	int state;
	string token;
};

struct ruleTable{
	string LHS;
	string RHS;
	int idCount;
};

actionTable actions[16][7];
ruleTable rules[6];
regex idExp("[a-zA-Z]+[0-9|a-zA-Z|_]*");
vector<string> inputVal;
stack<string> algoStack;
stack<int> ruleApplied;
stack<string> derivation;
string specialToken[]={"*", "="};

void createTable(){
	actions[0][0]={"SHIFT", 4, "*"};
	actions[0][1]={"SHIFT", 5, "id"};
	actions[0][2]={"GOTO", 1, "S"};
	actions[0][3]={"GOTO", 2, "L"};
	actions[0][4]={"GOTO", 3, "R"};

	actions[1][0]={"ACCEPT", -1, "$"};
	
	actions[2][0]={"SHIFT", 6, "="};
        actions[2][1]={"REDUCE", 5, "$"};

	actions[3][0]={"REDUCE", 2, "$"};

	actions[4][0]={"SHIFT", 4, "*"};
        actions[4][1]={"SHIFT", 5, "id"};
        actions[4][2]={"GOTO", 7, "R"};
        actions[4][3]={"GOTO", 8, "L"};

	actions[5][0]={"REDUCE", 4, "="};
        actions[5][1]={"REDUCE", 4, "$"};

	actions[6][0]={"SHIFT", 12, "*"};
        actions[6][1]={"SHIFT", 11, "id"};
        actions[6][2]={"GOTO", 9, "R"};
        actions[6][3]={"GOTO", 10, "L"};

	actions[7][0]={"REDUCE", 3, "="};
        actions[7][1]={"REDUCE", 3, "$"};

	actions[8][0]={"REDUCE", 5, "="};
        actions[8][1]={"REDUCE", 5, "$"};

	actions[9][0]={"REDUCE", 1, "$"};	

	actions[10][0]={"REDUCE", 5, "$"};

	actions[11][0]={"REDUCE", 4, "$"};

	actions[12][0]={"SHIFT", 12, "*"};
        actions[12][1]={"SHIFT", 15, "id"};
        actions[12][2]={"GOTO", 13, "R"};
        actions[12][3]={"GOTO", 14, "L"};

	actions[13][0]={"REDUCE", 3, "$"};

	actions[14][0]={"REDUCE", 5, "$"};

	actions[15][0]={"REDUCE", 4, "$"};

	cout<<"Parse table applied is as follow:"<<endl;
	cout<<"Convention(FROM_STATE->token->Action)"<<endl;

	for(int i=0; i<16; i++){
		for(int j=0; j<8; j++){
			if(actions[i][j].action=="GOTO" || actions[i][j].action=="REDUCE" || actions[i][j].action=="SHIFT" || actions[i][j].action=="ACCEPT"){
				cout<<i<<"->"<<actions[i][j].token<<"->"<<actions[i][j].action<<actions[i][j].state<<endl;
			}else{
				break;
			}
		}
	}	

}

void printAppliedRules(){
	stack<string> temp;
	stack<int> tempInt;
	cout<<"Algorithm as per LR parser is as follows:"<<endl;
	while(!derivation.empty()){
		temp.push(derivation.top());
		derivation.pop();
	}

	while(!temp.empty()){
                cout<<temp.top()<<endl;
                temp.pop();
        }

	cout<<endl;
	cout<<"Rules applied to verify grammer are as follows:"<<endl;

	
	while(!ruleApplied.empty()){
		tempInt.push(ruleApplied.top());
                ruleApplied.pop();
        }

	while(!tempInt.empty()){
		int temp=tempInt.top();
		cout<<rules[temp].LHS<<"->"<<rules[temp].RHS<<endl;
                tempInt.pop();
        }


}



void createRule(){
	rules[1].LHS="S";
	rules[1].RHS="L=R";
	rules[1].idCount=3;

	rules[2].LHS="S";
        rules[2].RHS="R";
        rules[2].idCount=1;

	rules[3].LHS="L";
        rules[3].RHS="*R";
        rules[3].idCount=2;

	rules[4].LHS="L";
        rules[4].RHS="id";
        rules[4].idCount=1;

	rules[5].LHS="R";
        rules[5].RHS="L";
        rules[5].idCount=1;

	cout<<"Grammer rules are as below"<<endl;
	cout<<rules[1].LHS<<"->"<<rules[1].RHS<<endl;
	cout<<rules[2].LHS<<"->"<<rules[2].RHS<<endl;
	cout<<rules[3].LHS<<"->"<<rules[3].RHS<<endl;
	cout<<rules[4].LHS<<"->"<<rules[4].RHS<<endl;
	cout<<rules[5].LHS<<"->"<<rules[5].RHS<<endl<<endl<<endl<<endl;
}

string getAction(string val, int& moveToState){
	string action;
	moveToState =-1;
	int state=stoi(algoStack.top());	
	for(int i=0; i<7; i++){
		if(actions[state][i].token==val){
			moveToState=actions[state][i].state;
			return  actions[state][i].action;
		}
	}
	return "INVALID";
}

bool validGrammer(){
	string action;
	int moveToState;
	string reducingAction;
	for (auto it: inputVal)
	{
		action=getAction(it, moveToState);	
		
		if(action=="INVALID"){
			return false;
		}
		else if(action=="SHIFT"){
			derivation.push("Apply s"+to_string(moveToState));
			algoStack.push(it);
			algoStack.push(to_string(moveToState));
		}
		else if(action=="REDUCE"){
			string currAction;
			do{
				int tempMove;
				ruleApplied.push(moveToState);
				derivation.push("Apply r"+to_string(moveToState));
				for(int i=0; i<rules[moveToState].idCount; i++){
					algoStack.pop();
					algoStack.pop();
				}
		
				currAction=getAction(rules[moveToState].LHS, tempMove);		
				algoStack.push(rules[moveToState].LHS);
				if(currAction=="GOTO")
					algoStack.push(to_string(tempMove));	
				else{
					cout<<"GOTO not found"<<endl;
					return false;
				}
				currAction=getAction(it, moveToState);
			}while(currAction=="REDUCE");
			
			if(currAction=="INVALID"){
                        	return false;
                	}
                	else if(currAction=="SHIFT"){
				derivation.push("Apply s"+to_string(moveToState));
                	        algoStack.push(it);
        	                algoStack.push(to_string(moveToState));
	                }else if(currAction=="ACCEPT"){
	                        return true;
        	        }else if(currAction=="GOTO"){
				derivation.push("Apply GOTO"+to_string(moveToState));
				algoStack.push(to_string(moveToState));
			}
		}else if(action=="GOTO"){
				derivation.push("Apply GOTO"+to_string(moveToState));
                                algoStack.push(to_string(moveToState));
                }
		else if(action=="ACCEPT"){
			return true;
		}
	}
	
	return true;
}
 
int main(){
	string inputStr;
	string tokens;
	string temp;
	createTable();
	createRule();
	cout<<"Enter String:";
	cin>>inputStr;
	cout<<endl<<endl;

	for(auto x: inputStr){
		if(x==' '||x=='='||x=='*'){
			inputVal.insert(inputVal.end(), temp);
			temp="";
			temp.push_back(x);
			inputVal.insert(inputVal.end(), temp);
			temp="";
		}
		else
			temp.push_back(x);	
	}
	if(temp!=""){
		inputVal.insert(inputVal.end(), temp);
	}
	inputVal.insert(inputVal.end(), "$");
	algoStack.push("0");
	if(!validGrammer()){
		cout<<"String can't be derived"<<endl;
	}
	else{
		cout<<"String is accepted by LR parser:"<<endl;
		printAppliedRules();
	}
	return 0;
}
