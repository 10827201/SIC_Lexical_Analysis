//10827201 簡湘媛 
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

typedef struct instrtype {
	string instr ;
	int  table ;
	int place ;
};

typedef struct datatype {
	string line ;
	vector <instrtype> instrlist ;
};

typedef struct tabletype{
	vector<string> datalist ;
};


void openfile( string & filename ){
	string name ;
	string name2 ;
	ifstream infile ; // 讀
	name2 = filename ;
	name = filename + ".txt" ;
	infile.open( (char*)name.c_str() ) ;
	if (  infile.is_open() ){
		infile.close() ;
		return ;
	}

	while ( !infile ) { // 無此檔案
		cout <<"### file does not exist! " << "###" << endl ;
		cout << "Input a file name [0: Quit]: ( EX: SIC_ipput 不須輸入 .txt) " << endl;
		cin >> filename;
		name = filename+".txt" ;
		if ( strcmp ( name.c_str(), "0") == 0  ){
			filename  = name2 ;
			return  ;
		}

		infile.open( (char*)name.c_str() ) ;
		if (  infile.is_open() ){
			infile.close() ;
			return ;
		}
    } // while

} // openfile()


string gettoken( int &first , int &i , string line ) {   // 以空白切字元 
		bool s = false ;
		if ( i == 0 )
			first = 0 ; // 從頭開始
		else
			first = i ; // 從被切過的地方繼續(尚未換行)

		int num = 0 ;

		while(  line[i] == ' ' ||  line[i] =='\t' ){
			first ++ ;
			i++;
		}
		
		if( line[i] == '.'){
			while( line[i] != '\0'){
				i++ ;
			}
			num++ ;
			return line.substr( first, num ) ;
		}

		while (  line[i] != ' ' && line[i] !='\t' &&  line[i] != '\0' && line[i] != '.' ) {
			if( line[i] == '\'' ){
				s = true ;
				num++ ;
				i++ ;
			}

			if ( s == true ){
				while( line[i] != '\''){
					num++ ;
					i++ ;
				}
				s = false ;
			}
			
			num++ ;
			i++ ;				

		} // while


		if( line[i] == '\0' || line[i] == '.')
			 ;
		else
			i++ ;

		return line.substr( first, num ) ; // 切token
} // token()


void tabletovec( vector<tabletype> & tablelist ){  // 將table存成vector 
	string filename ;
	string line ;
	string instr ;
	int start = 0 ;
	int end = 0 ;
	int i = 1 ;
	tabletype table ;
	while( i < 5 ){
		ostringstream num ;
		num << i ;
		filename = "Table"+ num.str() + ".table" ;
		ifstream infile ;
		infile.open( (char*)filename.c_str() ) ;
		while ( getline( infile, line ) ){
			instr = gettoken( start, end, line ) ; 
			table.datalist.push_back(instr) ;
			start = 0 ;
			end = 0  ;
		}
		infile.close() ;
		i++ ;
		tablelist.push_back(table) ;
		table.datalist.clear() ;
	}
}


int linearhash( string instr , tabletype & table){ 
	int index = 0 ;
	int place = 0 ;
	table.datalist.resize(100) ;
	for(int i = 0 ; (int)instr[i] != 0  ; i++ ){
		index = index + (int)instr[i] ;
	}
	index = index % 100 ;

	while( ! table.datalist[index].empty() ){
		if ( table.datalist[index] == instr ){
			return index ;
		}
		index++ ;
		if (index > 100 ){
			index = index % 100 ;
		}
	}

	table.datalist.insert( table.datalist.begin()+index, instr ) ;
	table.datalist.erase( table.datalist.begin()+index + 1 ) ;
	return index ;
}


string changeuplow(string instr){  // 變換大小寫 
	string change  = instr ;
	if ( islower( instr[0] ) ){
		for( int j = 0 ; j < change.size() ; j++  ){
			change[j] =  toupper(instr[j]) ;
		}
	}
	else {
		for( int j = 0 ; j < change.size() ; j++ ){
			change[j] =  tolower(instr[j]) ;
		}
	}

	return change ;
}

string sameuplow(string instr){ // 變成同一大小字元 
	string change  = instr ;
	if ( islower( instr[0] ) ){
		for( int j = 0 ; j < change.size() ; j++  ){
			change[j] =  tolower(instr[j]) ;
		}
	}
	else {
		for( int j = 0 ; j < change.size() ; j++ ){
			change[j] =  toupper(instr[j]) ;
		}
	}

	return change ;
}



int findplace(string line, tabletype table) {  // 找在table中的位置 
	int num = 0 ;
	bool finded = false ;
	string instr ;
	instr = sameuplow( line ) ;	
	string change ;
	change = changeuplow( instr ) ;
	vector<string>::iterator it;
	
	if ( find( table.datalist.begin(), table.datalist.end(), line) != table.datalist.end() ){
		it = find(table.datalist.begin(), table.datalist.end(), instr )	;
		num = distance(table.datalist.begin(), it ) ;		
	}
	else if ( find( table.datalist.begin(), table.datalist.end(), instr) != table.datalist.end() ){
		it = find(table.datalist.begin(), table.datalist.end(), instr )	;
		num = distance(table.datalist.begin(), it ) ;
	}
	else{
		it = find(table.datalist.begin(), table.datalist.end(), change )	;
		num = distance(table.datalist.begin(), it ) ;
	}

	return num  ;
}


int findtable( string line, vector<tabletype> tablelist ){ // 找在哪個table中 
	int i = 0 ;
	string instr ;
	instr = sameuplow( line ) ;
	string change ;
	change = changeuplow( instr ) ;
	while ( i < 7 ){
		if ( i < 4 ){
			if ( find(tablelist[i].datalist.begin(), tablelist[i].datalist.end(), instr ) != tablelist[i].datalist.end() ){
				return i + 1 ;
			}
			if ( find(tablelist[i].datalist.begin(), tablelist[i].datalist.end(), change ) != tablelist[i].datalist.end() ){
				return i + 1 ;
			}			
		}
		else{
			if ( find(tablelist[i].datalist.begin(), tablelist[i].datalist.end(), line ) != tablelist[i].datalist.end() ){
				return i + 1 ;
			}			
		}

		i++ ;
	}
	return i+1 ;
}


bool isstring ( string instr ){
	for( int j = 0 ; instr[j] != '\0'  ; j++  ){
		if ( instr[j] == '\'' ){
			return true ;
		}
	}
	return false;
}


bool issymbol( string line, vector<tabletype> tablelist ){
	int i = 0 ;
	string instr ;
	instr = sameuplow( line ) ;
	string change ;
	change = changeuplow( instr ) ;
	while ( i < 4 ){
		if ( find(tablelist[i].datalist.begin(), tablelist[i].datalist.end(), instr ) != tablelist[i].datalist.end() ){
			return false ;
		}

		if ( find(tablelist[i].datalist.begin(), tablelist[i].datalist.end(), change ) != tablelist[i].datalist.end() ){
			return false ;
		}
		i++ ;
	}
	return true ;
}


bool isnum( string instr){
	//for( int j = 0 ; instr[j] != '\0'  ; j++  ){
		if ( '0' <= instr[0] && instr[0] <= '9' ){
			return true ;
		}
	//}
	return false ;
}


string cutinstr( int &first , int &i,  string instr, vector<tabletype> tablelist ){
		if ( i == 0 )
			first = 0 ; // 從頭開始
		else
			first = i ; // 從被切過的地方繼續(尚未換行)

		int num = 0 ;

		string temp ;
		temp.assign(1, instr[i]) ;


		if (  find(tablelist[3].datalist.begin(), tablelist[3].datalist.end(), temp ) != tablelist[3].datalist.end() ){
			i++;
			return instr.substr( first, 1 ) ;
		}

		while ( find(tablelist[3].datalist.begin(), tablelist[3].datalist.end(), temp ) == tablelist[3].datalist.end() && instr[i] != '\0' ) {
			num++ ;
			i++ ;
			temp.assign( 1, instr[i] ) ;
		} // while

		return instr.substr( first, num ) ; // 切token
}


void othercase( string  instr, vector<instrtype> & instrlist, vector<tabletype> & tablelist  ){
	vector <string> list ;
	instrtype instr1 ; // table4 字元
	string temp ;
	temp = instr ;
	int start = 0 ;
	int end = 0 ;
	int num = 0 ;
	int i = 0 ;
	if ( isstring( instr ) == true ){
		
		while(instr[i] != '\'' ){
			i++ ;
		}
		
		
		instr1.instr = instr.substr(i, 1) ;
		instr1.table = findtable( instr1.instr , tablelist ) ;
		instr1.place =  findplace(instr1.instr, tablelist[instr1.table - 1 ]) ;
		instrlist.push_back( instr1 ) ;		
		i++ ;
		
		int first = i ;
		while( instr[i] != '\'' ){
			num++ ;
			i++;
		}
		instr1.instr = instr.substr( first, num ) ;
		instr1.table = 7 ;
		instr1.place = linearhash(instr1.instr, tablelist[instr1.table - 1 ]) ;
		instrlist.push_back( instr1 ) ;

		instr1.instr = instr.substr(i,1) ;
		instr1.table = findtable( instr1.instr , tablelist ) ;
		instr1.place =  findplace(instr1.instr, tablelist[instr1.table - 1 ]) ;
		instrlist.push_back( instr1 ) ;
	}
	else {		
		while( instr[end] != '\0' ){
			temp = cutinstr( start , end,  instr , tablelist ) ;
			instr1.instr = temp ;					
			if( isnum(instr1.instr) == true ){
				instr1.table = 6 ; 			
				instr1.place = linearhash(instr1.instr, tablelist[instr1.table - 1 ]) ;
			}
			else if ( issymbol( instr1.instr, tablelist ) == true){
				instr1.table = 5 ; 	
				instr1.place = linearhash(instr1.instr, tablelist[instr1.table - 1 ]) ;							
			}
			else {				
				instr1.table = findtable(instr1.instr, tablelist) ;
				instr1.place = findplace(instr1.instr, tablelist[instr1.table - 1 ]) + 1 ; 				
			}
			instrlist.push_back( instr1 ) ;	
		}											
	}	
}


vector<instrtype> lineinstr( string line, vector<tabletype> & tablelist ) {
	int start = 0  ;
	int end = 0 ;
	instrtype instrdata ;
	vector<instrtype> instrlist ;
	string instr ;
	int num = 0 ; 
	while( line[end] != '\0' ){
		instr = gettoken(start, end, line) ;
		num = findtable(instr, tablelist) ;
		if ( num < 8  ){ 
			instrdata.instr = instr ;
			instrdata.table = findtable(instr, tablelist) ;
			if ( num < 5 )
				instrdata.place = findplace( instr, tablelist[ instrdata.table-1 ] ) + 1  ;
			else
				instrdata.place = findplace( instr, tablelist[ instrdata.table-1 ] )   ;				
			instrlist.push_back( instrdata ) ;
		}
		else{
			othercase( instr, instrlist, tablelist  ) ;
		}

	}

	if (line[end] == '.'){
		instrdata.instr = "." ;
		instrdata.table = findtable( "." , tablelist) ;
		instrdata.place = findplace( "." , tablelist[ instrdata.table-1] )  ;
		instrlist.push_back( instrdata ) ;
	}

	return instrlist ;
}

vector<tabletype> tablelist ;

int main (){

	string filename ;
	string line ;
    string name ;	
	vector <datatype> content ;
	datatype instruction ;
	tabletovec( tablelist )	 ;
	tablelist.resize(7) ;

	cout << "Input a file name( EX: SIC_ipput 不須輸入 .txt)" << endl ;
	cin >> filename ;
	openfile( filename ) ;

	ifstream infile ; // 讀
	name = filename + ".txt" ;
	infile.open( (char*)name.c_str() ) ;


	while ( getline( infile , line ) ) {
		instruction.line = line ;
		instruction.instrlist = lineinstr( line, tablelist );
		content.push_back( instruction ) ;
	}
	infile.close() ;

	ofstream outfile;
	name =  filename+"_output.txt" ;
    outfile.open( (char*)name.c_str() );
	for( int i = 0 ; i < content.size() ; i++  ){
		outfile<< content[i].line << endl  ;
		for( int j = 0 ; j < content[i].instrlist.size() ; j++ ){
			outfile << "(" << content[i].instrlist[j].table << "," <<  content[i].instrlist[j].place << ")" ;
		}
		outfile << endl ;
	}

	outfile.close() ;

    for(int i = 5 ; i < 8 ; i++ ){
    	stringstream num ;
    	num << i ;
    	name = "Table" + num.str() +".table" ;
    	outfile.open((char*)name.c_str()) ;
		for(int j = 0 ; j < tablelist[i-1].datalist.size() ; j++ ){
			outfile << tablelist[i-1].datalist[j] ;
			if ( j < tablelist[i-1].datalist.size() - 1 ){
				outfile << endl ;
			}
		}
		outfile.close() ;
	}


}
