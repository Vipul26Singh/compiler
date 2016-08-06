/** This process will take input file as argument and output file with name output.txt will be generated.
% is also included as input symbol. Alos, printf might seem that it is error but in reality it qualifies identifier type. 
Also this code will only do only Lexical Analysis, syntax and symentatic analysis are out of ots scope. Tab and space are treates as word seperator **/





#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<regex>

using namespace std;

const string lineSeps=" \t";

const string keywords[] = {"void", "int", "float", "if", "else",  "for", "return", "then", "main", "class", "and", "not", "or", "get", "put"};
const string specialSymbol[] = {"==", "+", "(", "<>", ")", "<", "=", "{", ">", "/", "}", "<=", "=", "[", ">=", "]", ";", ",", "//", "/*", "*/", "%", "\""};

/** regular expressionis for lex analyzer **/
regex integerExp("[0-9]+");
regex floatExp("[0-9]+\\.[0-9]+");
regex fraction("\\.[0-9]+");
regex idExp("[a-zA-Z]+[0-9|a-zA-Z|_]*");
regex comment("/\\*(.*?)\\*/");
regex alphaNum("[0-9|a-zA-Z|_]*");


/** vector to store result **/
vector<string> identifier_vec;
vector<string> alphaNumeric_vec;
vector<string> number_vec;
vector<string> fraction_vec;
vector<string> error_vec;
vector<string> reserved_vec;


/** check if string is keyword **/
bool isKeyword(string value){
	unsigned int i=0;
	for( i=0; i<sizeof(keywords)/sizeof(keywords[0]); i++){
		if(keywords[i]==value){
			return true;
		}	
	}	
	return false;
}


/** check if sprcial symbol **/
bool isSpecialSymbol(string value){
	unsigned int i=0;
	for(i=0; i<sizeof(specialSymbol)/sizeof(specialSymbol[0]); i++){
		if(specialSymbol[i]==value){
			return true;
		}
	}
	return false;
}

/** check if fraction **/
bool isFraction(string value){
	if(regex_match(value, fraction))
		return true;
	else
		return false;
}


/** check if alphanumeric ***/
bool isAlphanum(string value){
	if(regex_match(value, alphaNum))
		return true;
	else
		return false;
}


/** check if positive integer **/
bool isIntNum(string value){

	if(regex_match(value, integerExp))
		return true;
	else
		return false;
}


/** check if float **/
bool isFloatNum(string value){

	if(regex_match(value, floatExp))
		return true;
	else
		return false;
}


/** check if identifier **/
bool isID(string value){

	if(regex_match(value, idExp))
		return true;
	else
		return false;
}


/** check if special symbol are present **/
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

/** remove  comments **/
void removeComments(string& input){
	smatch matchedStr;
	string temp=input;


	if(regex_search(temp, matchedStr, comment)) {
		reserved_vec.push_back("/*");
		reserved_vec.push_back("*/");
	}

	input = regex_replace(input, comment, " ");	
}


/** analyzer **/
bool analyzeLine(string& input, string tokenizer){
	vector<string> tokenOfLine;
	string specialSymbol;
	bool result=true;
	size_t prev = 0, pos;

	removeComments(input);    	

	/** break line in tokens **/
	while ((pos = input.find_first_of(tokenizer, prev)) != string::npos)
	{
		if (pos > prev)
			tokenOfLine.push_back(input.substr(prev, pos-prev));
		prev = pos+1;
	}
	if (prev < input.length())
		tokenOfLine.push_back(input.substr(prev, string::npos));


	/*8 for each token check what criteria is qualified and store in vector **/

	for(unsigned int i=0; i<tokenOfLine.size(); ++i)
	{
	
		/** if token has special symbol along with other characters then create token on basis of this symbol **/
                if(hasSpecialSymbol(tokenOfLine[i], specialSymbol)){
                        result=analyzeLine(tokenOfLine[i], specialSymbol);
			if(specialSymbol!="\"")
			reserved_vec.push_back(specialSymbol);		
                }else if(isKeyword(tokenOfLine[i])){
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
		else if(isAlphanum(tokenOfLine[i])){
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


/** check size of each vector and display if any value is present **/
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
		cout<<"Fatal error: Input file not provided as argument"<<endl;
		return 1;
	}

	
	ifstream inputFile(argv[1]);
	ofstream outputFile("result.txt");

	/** open input file **/
	if(!inputFile.is_open()){
		cout<<"Fatal error: Unable to open input file"<<endl;
		return 1;
	}

	/** open out put file **/
	if(!outputFile.is_open()){
		cout<<"Fatal error: Unable to open output file (result.txt)"<<endl;
		return 1;
	}

	/** read each line and process **/
	while(getline(inputFile, inputLine)){
		analyzeLine(inputLine, lineSeps);
	}

	/** print result **/
	printResult(outputFile);	
	inputFile.close();
	outputFile.close();	
}
