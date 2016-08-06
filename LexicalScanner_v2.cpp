#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<regex>

using namespace std;

const string lineSeps=" \t+(<>)={/}[];,.";

const string keywords[] = {"void", "int", "float", "if", "else",  "for", "return", "then", "main", "class", "and", "not", "or", "get", "put"};
const string specialSymbol[] = {"==", "+", "(", "<>", ")", "<", "{", ">", "/", "}", "<=", "=", "[", ">=", "]", ";", ",", ".", "//", "/*", "*/", "%"};
regex integerExp("[0-9]+");
regex floatExp("[0-9]+\\.[0-9]+");
regex fraction("\\.[0-9]+");
regex idExp("[a-zA-Z]+[0-9|a-zA-Z|_]*");
regex comment("/\\*(.*?)\\*/");
regex alphaNum("[0-9|a-zA-Z|_]*");

vector<string> identifier_vec;
vector<string> alphaNumeric_vec;
vector<string> number_vec;
vector<string> fraction_vec;
vector<string> error_vec;
vector<string> reserved_vec;

bool isKeyword(string value){
	unsigned int i=0;
	for( i=0; i<sizeof(keywords)/sizeof(keywords[0]); i++){
		if(keywords[i]==value){
			return true;
		}	
	}	
	return false;
}

bool isSpecialSymbol(string value){
	unsigned int i=0;
	for(i=0; i<sizeof(specialSymbol)/sizeof(specialSymbol[0]); i++){
		if(specialSymbol[i]==value){
			return true;
		}
	}
	return false;
}


bool isFraction(string value){
	if(regex_match(value, fraction))
		return true;
	else
		return false;
}

bool isAlphanum(string value){
	if(regex_match(value, alphaNum))
		return true;
	else
		return false;
}


bool isIntNum(string value){

	if(regex_match(value, integerExp))
		return true;
	else
		return false;
}

bool isFloatNum(string value){

	if(regex_match(value, floatExp))
		return true;
	else
		return false;
}

bool isID(string value){

	if(regex_match(value, idExp))
		return true;
	else
		return false;
}

bool hasSpecialSymbol(string value, string& symbolFound){
	unsigned int i=0;
	for(i=0; i<sizeof(specialSymbol)/sizeof(specialSymbol[0]); i++){
		if(value.find(specialSymbol[i]) != string::npos){
			symbolFound=specialSymbol[i];
			return true;
		}
	}
	return false;
}

void removeComments(string& input){
	smatch matchedStr;
	string temp=input;


	if(regex_search(temp, matchedStr, comment)) {
		reserved_vec.push_back("/*");
		reserved_vec.push_back("*/");
	}

	input = regex_replace(input, comment, " ");	
}

bool analyzeLine(string& input, string tokenizer){
	vector<string> tokenOfLine;
	string specialSymbol;
	bool result=true;
	size_t prev = 0, pos;

	removeComments(input);    	

	while ((pos = input.find_first_of(tokenizer, prev)) != string::npos)
	{
		if (pos > prev)
			tokenOfLine.push_back(input.substr(prev, pos-prev));
		prev = pos+1;
	}
	if (prev < input.length())
		tokenOfLine.push_back(input.substr(prev, string::npos));


	 /** vipul start **/
        
        /** vipul end **/

	for(unsigned int i=0; i<tokenOfLine.size(); ++i)
	{
		
		if(isKeyword(tokenOfLine[i])){
			reserved_vec.push_back(tokenOfLine[i]);		
		}
		else if(isSpecialSymbol(tokenOfLine[i])){
			reserved_vec.push_back(tokenOfLine[i]);
		}
		else if(isIntNum(tokenOfLine[i])){
			number_vec.push_back(tokenOfLine[i]);
		}
		else if(isFloatNum(tokenOfLine[i])){
			number_vec.push_back(tokenOfLine[i]);
		}
		else if(isID(tokenOfLine[i])){
			identifier_vec.push_back(tokenOfLine[i]);
		}
		else if(hasSpecialSymbol(tokenOfLine[i], specialSymbol)){
			reserved_vec.push_back(tokenOfLine[i]);
		}else if(isAlphanum(tokenOfLine[i])){
			alphaNumeric_vec.push_back(tokenOfLine[i]);
		}else if(isFraction(tokenOfLine[i])){
			fraction_vec.push_back(tokenOfLine[i]);
		}
		else{
			error_vec.push_back(tokenOfLine[i]);
		}
	}

	return result;
}

void printResult(ofstream& outFile){
	int count=0;

	if(reserved_vec.size()>0){
		outFile<<"Operators, punctuations, and reserved words:"<<endl;
		cout<<"Operators, punctuations, and reserved words:"<<endl;

		for(auto token:reserved_vec){
			outFile<<token<<" ";		
			cout<<token<<" ";
			count++;
			if(count%4==0){
				outFile<<endl;            
				cout<<endl;
				count=0;
			}
		}
		outFile<<endl<<endl<<endl;
		cout<<endl<<endl<<endl;	
	}

	if(identifier_vec.size()){
		outFile<<"id:"<<endl;
		cout<<"id:"<<endl;
		count=0;
		for(auto token:identifier_vec){
			outFile<<token<<" ";            
			cout<<token<<" ";
			count++;
			if(count%4==0){
				outFile<<endl;               
				cout<<endl;
				count=0;
			}
		}
		outFile<<endl<<endl;
		cout<<endl<<endl;
	}

	if(alphaNumeric_vec.size()>0){
		outFile<<"alphanum:"<<endl;
		cout<<"alphanum:"<<endl;
		count=0;
		for(auto token:alphaNumeric_vec){
			outFile<<token<<" ";
			cout<<token<<" ";
			count++;
			if(count%4==0){
				outFile<<endl;
				cout<<endl;
				count=0;
			}
		}
		outFile<<endl<<endl;
		cout<<endl<<endl;
	}

	if(number_vec.size()>0){
		outFile<<"number:"<<endl;
		cout<<"number:"<<endl;
		count=0;
		for(auto token:number_vec){
			outFile<<token<<" ";
			cout<<token<<" ";
			count++;
			if(count%4==0){
				outFile<<endl;
				cout<<endl;
				count=0;
			}
		}
		outFile<<endl<<endl;  
		cout<<endl<<endl;
	}	

	if(fraction_vec.size()>0){
		outFile<<"fraction:"<<endl;
		cout<<"fraction:"<<endl;
		count=0;
		for(auto token:fraction_vec){
			outFile<<token<<" ";
			cout<<token<<" ";
			count++;
			if(count%4==0){
				outFile<<endl;
				cout<<endl;
				count=0;
			}
		}
		outFile<<endl<<endl;
		cout<<endl<<endl;
	}

	if(error_vec.size()>0){
		outFile<<"error token:"<<endl;
		cout<<"error token:"<<endl;
		count=0;
		for(auto token:error_vec){
			outFile<<token<<" ";
			cout<<token<<" ";
			count++;
			if(count%4==0){
				outFile<<endl;
				cout<<endl;
				count=0;
			}
		}
		outFile<<endl<<endl;
		cout<<endl<<endl;
	}


}

int main(int argc, char *argv[]){
	string inputLine;

	if(argc<2){
		cout<<"Fatal error: Input file not provided"<<endl;
		return 1;
	}

	ifstream inputFile(argv[1]);
	ofstream outputFile("result.txt");

	if(!inputFile.is_open()){
		cout<<"Fatal error: Unable to open input file"<<endl;
		return 1;
	}

	if(!outputFile.is_open()){
		cout<<"Fatal error: Unable to open output file (result.txt)"<<endl;
		return 1;
	}

	while(getline(inputFile, inputLine)){
		analyzeLine(inputLine, lineSeps);
	}

	printResult(outputFile);	
	inputFile.close();
	outputFile.close();	
}
