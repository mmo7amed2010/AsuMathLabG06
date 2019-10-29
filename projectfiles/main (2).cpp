#include <iostream>
#include <stdio.h>
#include "CMatrix.h"
#include "stdarg.h"
#include <algorithm>
#include <string>
#include <fstream>
//F = A / B
using namespace std;
#define size 100
enum MI{ MI_ZEROS, MI_ONES, MI_EYE, MI_RAND, MI_VALUE };
int num_elements = 0;
bool is_found(char c, char*  array_chars);
void reserve(CMatrix* array_matrices, string x);
void insert_name(char* array_chars, char y);
int get_index(char * array_chars, char operand_name);
string operationg(CMatrix* array_matrices, char* array_chars, int size_array,char first_operand, string operation, char output,char second_operand,double d=0 );
void check_print (string output, bool is_semicolon);
int main(int argc, char* argv[])
{
char array_chars [size];
CMatrix array_matrices [size] ;
//if (argc == 2)
//{
//argv[1]
ifstream infile("/home/mido/Downloads/bigexample.m");
if (!infile.bad())
{
	string get_input;
	string body_matrix;
	bool start_package,end_package,is_semicolon=false,start_operation=false;
	int end;
 	char c;
	while(getline(infile,get_input))
		{
	is_semicolon=false;
	int length=get_input.length();
	if(get_input=="\r"||get_input=="")
	{
        continue;
    }
	end=get_input.find("]");
	if(get_input.find("[")!=-1)
		start_package=true;
    else
        start_package=false;
	if(get_input.find("]")!=-1)
	{
		end_package=true;
	}else{
		end_package=false;
		}
	if(start_package)
	{
        c=*((const char*)get_input.substr(0,1).c_str());
		insert_name(array_chars,c);
	}
	if(end_package&&start_package)
	{
		body_matrix=get_input.substr(4,end-3);
		reserve(array_matrices, body_matrix);
		body_matrix.clear();
		if(get_input.substr(end+1,1)==";")
			is_semicolon=true;
		if(!is_semicolon)
		{
			cout<<array_matrices[get_index(array_chars,c)].getString()<<endl;
		}
	}
	if(end_package&&!start_package)
	{
		body_matrix+=get_input.substr(0,end+1);
		reserve(array_matrices, body_matrix);
		body_matrix.clear();
		if(get_input.substr(end+1,1)==";")
			is_semicolon=true;
		if(!is_semicolon)
		{
			cout<<array_matrices[get_index(array_chars,c)].getString();
		}
	}
	if(!end_package&&!start_package&&get_input.find("=")!=-1)
		{
		start_operation = true;
		string req;
		char* g = (char*)get_input.c_str();
		char* spearators = " ";
		char* temp = strtok(g,spearators);
		while(temp)
		{
            req += string(temp);
            temp = strtok(NULL,spearators);
		}
		get_input = req;
		c=*((const char*)get_input.substr(0,1).c_str());
		insert_name(array_chars, c);
		if(get_input.find("'")!=-1)
		{
		if(get_input.find(";")==4)
			is_semicolon=true;
		check_print(operationg(array_matrices,array_chars,num_elements,*((const char*)get_input.substr(2,1).c_str()),get_input.substr(3,1),c,0),is_semicolon);
		}else if(get_input.find(".")==-1)
		{
		if(get_input.find(";")!=-1)
			is_semicolon=true;
		check_print(operationg(array_matrices,array_chars,num_elements,*((const char*)get_input.substr(2,1).c_str()),get_input.substr(3,1),c,*((const char*)get_input.substr(4,1).c_str())),is_semicolon);
		}else if((*((const char*)get_input.substr(2,1).c_str())>='A'||*((const char*)get_input.substr(2,1).c_str())<='Z')&&(*((const char*)get_input.substr(5,1).c_str())>='A'||*((const char*)get_input.substr(5,1).c_str()))&&get_input.substr(3,1)==".")
		{
		if(get_input.find(";")!=-1)
			is_semicolon=true;
		check_print(operationg(array_matrices,array_chars,num_elements,*((const char*)get_input.substr(5,1).c_str()),get_input.substr(3,2),c,0,atof(get_input.substr(2,1).c_str())),is_semicolon);
		}
		}
		if (((!end_package&&start_package)||(!end_package&&!start_package))&&(!start_operation))
	{
	//&&get_input.substr(length-1,1)=="\n"
		if(get_input.substr(length-1,1)=="\r")
		{
            if(start_package)
			body_matrix+=get_input.substr(4,length-5);
			else
			body_matrix+=get_input.substr(0,length-1);
		}
	}
	}
infile.close();
}else
cout<<"File read failed."<<endl;
return 0;
}
istream& operator >> (istream &is, CMatrix& m)
{
	string s;
	getline(is, s, ']');
	s += "]";
    CMatrix v (s);
	m = v;
	return is;
}
ostream& operator << (ostream &os, CMatrix& m)
{
	os << m.getString();
	return os;
}
//to check if the character is found or not
bool is_found(char c, char*  array_chars)
{
	for (int i = 0; i < num_elements; i++)
	{
		if (c == array_chars[i])
			return false;
	}
	return true;
}
//give this function body of matrix [2 1, 4 5]
void reserve(CMatrix* array_matrices, string x)
{
		array_matrices[num_elements-1] = x;
		//cout<<array_matrices[num_elements-1];
}
// reserve char in array which is the name of matrices
void insert_name(char* array_chars,char y)
{
	if (num_elements==0)
	{
		array_chars[num_elements] = y;
		num_elements++;
	}
	if (is_found(y,array_chars))
	{
		array_chars[num_elements] = y;
		num_elements++;
	}
}
//array_chars is the dynamic array of characters
int get_index(char * array_chars, char operand_name)
{
	for (int i = 0; i < num_elements; i++)
	{
		if (array_chars[i] == operand_name)
			return i;
	}
}
//array_matrices is the dynamic array of matrices
string operationg(CMatrix* array_matrices, char* array_chars, int size_array,char first_operand, string operation,char output, char second_operand,double d )
{
	int index_first_operand, index_second_operand, index_output;
	index_first_operand = get_index(array_chars, first_operand);
	if(second_operand!=0)
	index_second_operand = get_index(array_chars, second_operand);
	index_output = get_index(array_chars, output);
	string required;

	if(operation=="+")
	{
		array_matrices[index_output] = array_matrices[index_first_operand] + array_matrices[index_second_operand];
		required = array_matrices[index_output].getString();
    }else if(operation=="-")
    {
		array_matrices[index_output] = array_matrices[index_first_operand]-array_matrices[index_second_operand];
		required = array_matrices[index_output].getString();
    }else if(operation=="*")
    {
		array_matrices[index_output] = array_matrices[index_first_operand]*array_matrices[index_second_operand];
		required = array_matrices[index_output].getString();
    }else if(operation=="/")
	{
	try{
		array_matrices[index_output] = array_matrices[index_first_operand]/array_matrices[index_second_operand];
		required = array_matrices[index_output].getString();
		}
		catch(string* fault){cout<<*(fault)<<endl;}
    }else if(operation=="'")
	{
		array_matrices[index_output] = array_matrices[index_first_operand].getTranspose();
		required = array_matrices[index_output].getString();
    }else if(operation=="./")
	{
		array_matrices[index_output] = division_by_element(d,array_matrices[index_first_operand]);
		required = array_matrices[index_output].getString();
    }
	return required;
}
//to check to print or not
void check_print (string output, bool is_semicolon)
{
	if (!is_semicolon)
		cout << output<<endl;

}
 CMatrix& division_by_element(double d,CMatrix& X)
{
    static CMatrix output(X.nR, X.nC);
	for(int i=0;i<X.nR;i++)
    {
        for(int j=0;j<X.nC;j++)
        {
        double d1=d/X.values[i][j];
          output.values[i][j]=d1;
        }
    }
    return output;
}

/*
#include <iostream>
#include "CMatrix.h"
#include "stdarg.h"
#include <algorithm>
using namespace std;
CMatrix& eye(int nr,int nc)
{
static CMatrix w;
CMatrix temp(nr,nc,2);
w=temp;
return w;
}
CMatrix& rand(int nr,int nc)
{
static CMatrix w;
CMatrix temp(nr,nc,3);
w=temp;
return w;
}
CMatrix& zeros(int nr,int nc)
{
static CMatrix w;
CMatrix temp(nr,nc,0);
w=temp;
return w;
}
CMatrix& ones(int nr,int nc)
{
static CMatrix w;
CMatrix temp(nr,nc,1);
w=temp;
return w;
}
int main()
{
    CMatrix A ;
    A = "[1.2 3.4 5.6 7.8 1.0 3.2 2.1 2.3 1.4 2.6 2.7 2.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 3.7 1.2 3.4 5.6 7.8 9.0 1.2 2.1 2.3 2.4 1.6; 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4; 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 7.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 4.1 2.3; 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 1.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1; 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2; 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 17.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0; 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8; 9.0 2.2 2.1 2.3 2.4 1.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 2.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6; 7.4 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4; 5.6 7.8 9.0 2.2 4.4 2.3 2.4 2.6 2.7 1.2 3.4 5.6 2.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2; 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 1.8 9.0 1.2 2.1 2.3 2.4 8.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7; 1.2 3.4 5.6 7.8 9.0 2.2 5.1 4.3 12.4 2.6 2.7 1.2 3.4 5.6 97.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 3.4 2.6; 2.7 1.2 3.4 5.6 7.5 9.0 2.2 2.1 3.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 5.1 2.3 2.4 5.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4; 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 8.2 2.1 2.3; 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 6.6 2.7 1.2 3.4 5.6 2.8 9.0 1.2 2.1; 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 5.1 2.3 2.4 0.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 0.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2; 0.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0; 4.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 5.2 2.1 2.3 2.4 9.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8; 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 5.3 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.3; 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 3.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 3.4 2.6 3.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 8.4 2.6 2.7 1.2 3.4; 5.6 7.8 9.0 2.2 2.1 2.3 3.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 1.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2; 3.4 1.6 7.8 9.0 2.2 3.1 6.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7; 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 4.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 1.6; 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 1.4 2.6 2.7 1.2 3.4 9.6 7.8 9.0 2.2 2.1 2.3 2.4; 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 1.4 2.6 2.7 2.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3; 2.4 2.6 2.7 1.2 3.4 5.6 7.8 1.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 8.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 2.6 7.8 9.0 2.2 2.1; 7.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 3.6 1.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2; 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 7.2 3.4 5.6 7.8 9.0; 2.2 2.1 2.3 2.4 2.6 7.7 9.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.9 2.4 2.6 2.7 1.2 3.4 5.6 7.8; 9.0 2.2 2.1 2.3 2.4 2.6 7.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6; 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4; 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 2.7 1.2 3.4 5.6 7.8 9.0 2.2 2.1 2.3 2.4 2.6 5.7 9.3 3.4 5.6 3.8 9.0 2.2 5.1 2.3 2.4 2.6 2.7 1.2];";
   // cout<<A.FastestDeterminant()<<endl;
    //CMatrix B ;
   // A="[1 -1 0 2; 2 1 0 0; 1 1 2 2; 0 0 1 1];";
    //cout<<"\n"<<A;
    //cout<<"\n"<<B;
    //CMatrix C=A+B;
    //cout<<"\n"<<C;
    C=A-B;
    cout<<"\n"<<C;
    C=A*B;
    cout<<"\n"<<C;
    C=A/B;
    cout<<A.FastestDeterminant()<<endl;
    cout<<eye(4,4);
    cout<<rand(4,4);
    cout<<ones(4,4);
    cout<<zeros(4,4);
    return 0;
}
istream& operator >> (istream &is, CMatrix& m)
{
	string s;
	getline(is, s, ']');
	s += "]";
    CMatrix v (s);
	m = v;
	return is;
}
ostream& operator << (ostream &os, CMatrix& m)
{
	os << m.getString();
	return os;
}
string handling_operations(string s)
{
    string temp;
    while(s.length()==0)
    {
        if(s.find("(")!=0)
        {
        temp=s.substr(s.find("(")+1,s.find(")")-(s.find("(")+1));

        }
    }
}
string do_operation(string x)
{
    int length=x.length();
    string arr_op [5] = {"^","/","*","+","-"};
    for(int j=0;j<5;j++)
    {
        for(int i=0;i<length;i++)
            {

            }
    }
}
*/

