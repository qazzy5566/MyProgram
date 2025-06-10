// 109-2 PL Our Scheme
# include <iostream> // cout, endl
# include <string> // string, find_last_of, substr, clear
# include <vector> // vector, push_back
# include <cstdlib> // atoi, system
# include <cstring> // strcmp
# include <stdio.h> // printf scanf
# include <exception> // 
# include <stdlib.h> // exit() system pause

using namespace std ;

// this is project 3 
// 1.global���ȷ|�]��local�P�W�r�A�i��|�Q���N 
// 2.���ݩ�ۤvfunction���ܼơA�n�������

// �q�L�D��:1-4 6, 7 

enum TokenType {
  LP = 1, // ( ( ( ( (
  RP = 2, // ) ) ) ) )
  INT = 3, // e.g., '123', '+123', '-123'
  STRING = 4, // "string's (example)." (strings do not extend across lines)
  DOT = 5, // '.'
  FLOAT = 6, // '123.567', '123.', '.567', '+123.4', '-.123'
  NIL = 7, // 'nil' or '#f', but not 'NIL' nor 'nIL'
  T = 8, // 't' or '#t', but not 'T' nor '#T'
  QUOTE = 9, // '
  SYMBOL = 10,
  NOPROCEDURE = 11

} ; // TokenType()

struct Token { // Token��  

  // The imformation of a token	
  string name ;
  int line ; // �b�ĴX�� 
  int column ; // �b�ĴX�C 
  TokenType type ; // ����  

} ; // Token()

struct Node {

  Token token ;
  Node * left ;
  Node * right ;

} ; // Node()

typedef vector<Token> Exp ;
typedef Node* Tree ;
# define MAX_LENGTH 100

struct SymbolTable {

  string name ;

  Tree binding ;

} ; // SymbolTable()

struct FunctionTable {

  string name ; 
  Tree arg ;
  Tree binding ;

} ; // FunctionTable() 

// =========�|�Ψ쪺�S�wtoken�M�ܼ�=========== 

static int uTestNum ;
static bool uEof ; // eof�o�ʹN�����F
static bool uExit ;
static bool uError ; // Error
static bool uVerbose ;
static bool uProcedure ; // for prettyprint
static int uArg ; // �ΨӰO��localfunction��arg�ƶq�A�o�˴N���| 

static int uLine ; // ������m
static int uColumn ;

static Token uDotToken ;
static Token uRpToken ;
static Token uLpToken ;
static Token uNilToken ;
static Token uTToken ;

static Tree uNilNode ;
static Tree uTNode ;

static vector<SymbolTable> uSymbolTable ; // Global
static vector<SymbolTable> uLocalTable ; // Local
static vector<FunctionTable> uFunctionTable ; // global

// =========�|�Ψ쪺�S�wtoken===========

void Readline() ;
Tree EvalSexp( Tree tree, int level ) ;
bool IsInternalFunction( string name ) ;
bool IsboundF( string name ) ;
void PrintExp( Tree tree ) ;

// =========Tool==============

int CharToInt( char ch ) { // �u��ǼƦr���r���i�� FlOAT�|�ˤ��J�� 

  if ( ch == '0' )
    return 0 ;

  if ( ch == '1' )
    return 1 ;

  if ( ch == '2' )
    return 2 ;

  if ( ch == '3' )
    return 3 ;

  if ( ch == '4' )
    return 4 ;

  if ( ch == '5' )
    return 5 ;

  if ( ch == '6' )
    return 6 ;

  if ( ch == '7' )
    return 7 ;

  if ( ch == '8' )
    return 8 ;

  if ( ch == '9' )
    return 9 ;

  cout << "Unvalid use of this function, your parameter is: " << ch << endl ;
  return -1 ;  

} // CharToInt()

void PrintTree( Tree tree ) { // for test
  // cout << "stop" ;
  // system( "pause" ) ;

  if ( tree == NULL ) cout << "?????" << endl ;
  
  else if ( tree->left == NULL && tree->right == NULL ) {

    cout << tree->token.name << " " << endl ;
  } // else if

  else {

    cout << "Left" << endl ; 
    PrintTree( tree->left ) ;
    
    cout << "Right" << endl ;
    PrintTree( tree->right ) ;

  } // else

} // PrintTree()

void PrintList( Exp list ) { // for test
  // print a expression
  cout << "List" << endl ;

  for ( int i = 0 ; i < list.size() ; i++ )
    cout << list[i].name << " " ;

  cout << endl ;

} // PrintList()

void CreateToken( Token & token, string name, int line, int column, TokenType type ) {
  // �гy�S�w��token 

  token.name = name ;
  token.line = line ;
  token.column = column ;
  token.type = type ;

} // CreateToken()

void CreateNode( Tree & node, Token token ) {

  node = new Node() ;
  node->left = NULL ; 
  node->right = NULL ;

  node->token.name = token.name ;
  node->token.line = token.line ;
  node->token.column = token.column ;
  node->token.type = token.type ;

} // CreateNode()

bool NumToStr( float num, string & name ) { // ��Ʀr�ܦ��r�� 

  bool isF = false ; // �^�ǳo�ӧP�_���G�O����٬O�p��
  bool isNegative = false ;
  char * temp = new char[30] ;
  int index = 0 ;
  int integer ; // ��Ƴ��� 

  if ( num < 0 ) { // �t�ƳB�z 

    isNegative = true ;
    num = num * ( - 1 ) ;
  } // if()

  integer = num ;
  num = num - integer ; // �p�Ƴ��� 

  // ���ξ�Ƴ���
  int mod ; // mod 10 �� result 

  while ( integer != 0 ) { // ���Ƴ������X�� 

    mod = integer % 10 ;

    if ( mod == 0 ) {

      temp[index] = '0' ;
      index++ ;
    } // if()

    else if ( mod == 1 ) {

      temp[index] = '1' ;
      index++ ;
    } // else if()

    else if ( mod == 2 ) {

      temp[index] = '2' ;
      index++ ;
    } // else if()

    else if ( mod == 3 ) {

      temp[index] = '3' ;
      index++ ;
    } // else if()

    else if ( mod == 4 ) {

      temp[index] = '4' ;
      index++ ;
    } // else if()

    else if ( mod == 5 ) {

      temp[index] = '5' ;
      index++ ;
    } // else if()

    else if ( mod == 6 ) {

      temp[index] = '6' ;
      index++ ;
    } // else if()

    else if ( mod == 7 ) {

      temp[index] = '7' ;
      index++ ;
    } // else if()

    else if ( mod == 8 ) {

      temp[index] = '8' ;
      index++ ;
    } // else if()

    else if ( mod == 9 ) {

      temp[index] = '9' ;
      index++ ;
    } // else if()

    integer = integer / 10 ; 

  } // while()

  if ( isNegative ) { // �O�t�ƭn�[ '-'

    temp[index] = '-' ; 
    index++ ;

  } // if

  int l = 0 ; // left first
  int r = index - 1 ; // right first
  char t ;

  while ( l < r ) { // ���ƭ˹L�� 

    t = temp[r] ;
    temp[r] = temp[l] ;
    temp[l] = t ;

    r-- ;
    l++ ;
  } // while()

  if ( num != 0 ) { // �N���p�Ƴ���

    isF = true ; 
    temp[index] = '.' ;
    index++ ; 

    for ( int z = 0 ; z < 4 && num != 0 ; z++ ) { // �u�n���e�T�� 

      num = num * 10 ;
      integer = num ;

      if ( integer == 0 ) {

        temp[index] = '0' ;
        index++ ;
      } // if()

      else if ( integer == 1 ) {

        temp[index] = '1' ;
        index++ ;
      } // else if()

      else if ( integer == 2 ) {

        temp[index] = '2' ;
        index++ ;
      } // else if()

      else if ( integer == 3 ) {

        temp[index] = '3' ;
        index++ ;
      } // else if()

      else if ( integer == 4 ) {

        temp[index] = '4' ;
        index++ ;
      } // else if()

      else if ( integer == 5 ) {

        temp[index] = '5' ;
        index++ ;
      } // else if()

      else if ( integer == 6 ) {

        temp[index] = '6' ;
        index++ ;
      } // else if()

      else if ( integer == 7 ) {

        temp[index] = '7' ;
        index++ ;
      } // else if()

      else if ( integer == 8 ) {

        temp[index] = '8' ;
        index++ ;
      } // else if()

      else if ( integer == 9 ) {

        temp[index] = '9' ;
        index++ ;
      } // else if()

      num = num - integer ;

    } // for()

  } // if()

  temp[index] = '\0' ;
  name = temp ;

  // cout << "temp " << temp << "?" << endl ;
  // cout << "name " << name << "?" << endl ;

  // system("pause") ;
  return isF ;

} // NumToStr()

bool Is_Two_Tree_Same( Tree t1, Tree t2 ) {

  // if t1 and t2 are both atom
  if ( ( t1->left == NULL && t1->right == NULL ) && ( t2->left == NULL && t2->right == NULL ) ) {

    if ( ! strcmp( t1->token.name.c_str(), t2->token.name.c_str() ) )
      return true ;

    else return false ;

  } // if()

  // if one of the tree is not atom
  else if ( ( t1->left != NULL && t1->right != NULL ) && ( t2->left == NULL && t2->right == NULL ) )
    return false ;

  else if ( ( t1->left == NULL && t1->right == NULL ) && ( t2->left != NULL && t2->right != NULL ) )
    return false ;

  else { // both tree are not atom
    // check left part and right part
    if ( Is_Two_Tree_Same( t1->left, t2->left ) && Is_Two_Tree_Same( t1->right, t2->right ) )
      return true ;
    else
      return false ;

  } // else()

} // Is_Two_Tree_Same()

void Clear( Tree & tree ) { // Clear a tree

  if ( tree == NULL ) return ;

  else if ( tree->left == NULL && tree->right == NULL ) {

    if ( tree == uNilNode || tree == uTNode ) // �o���NODE����R�� 
      ; // cout << "YES" << endl ;
    else 
      delete tree ;

    tree = NULL ;

  } // else if()

  else {

    Clear( tree->left ) ;
    Clear( tree->right ) ;

    if ( tree->left == NULL && tree->right == NULL ) {

      if ( tree == uNilNode || tree == uTNode ) // �o���NODE����R�� 
        ; // cout << "YES2" << endl ; 
      else 
        delete tree ;

      tree = NULL ;

    } // if()

  } // else

} // Clear()

void GetTableSize() { // test

  cout << "Symbol table: " << uSymbolTable.size() << endl ;
  cout << "Function table: " << uFunctionTable.size() << endl ;
  cout << "Local table: " << uLocalTable.size() << endl ;
} // GetTableSize()

void Clean_lambda() { // ��lambda���w�q�bfunction table���R���A�]�����i��]��error�ɭP�S���R�����b

  if ( uFunctionTable.size() > 0 ) {

    int i = uFunctionTable.size() ;

    while ( i > 0 && ( strcmp( uFunctionTable[i-1].name.c_str(), "lambda" ) == 0 || 
                       strcmp( uFunctionTable[i-1].name.c_str(), "lambda expression" ) == 0 ) ) {

      uFunctionTable.pop_back() ;
      i = uFunctionTable.size() ;
    } // while()

  } // if()

} // Clean_lambda()

// =========Tool==============

// =========Error=============

class Error : public exception { // Syntax error 

  public:
  Error( int error, const char* msg, int row, int column ) { 
    
    if ( error == 1 ) { // ERROR (unexpected token) : atom...

      cout << "ERROR (unexpected token) : atom or '(' expected when token at " ;
      cout << "Line " << row << " Column " << column << " is >>" << msg << "<<" << endl ;

    } // if 

    else if ( error == 2 ) { // ERROR (unexpected token) : ')' expected...

      cout << "ERROR (unexpected token) : ')' expected when token at " ;
      cout << "Line " << row << " Column " << column << " is >>" << msg << "<<" << endl ;

    } // else if
    
    else if ( error == 3 ) { // ERROR (no closing quote)

      cout << "ERROR (no closing quote) : END-OF-LINE encountered at " ;
      cout << "Line " << row << " Column " << column + 1 << endl ;
    } // else if

    else if ( error == 4 ) { // ERROR (no more input)

      cout << "ERROR (no more input) : END-OF-FILE encountered" ;
      uEof = true ;

    } // else if

    Readline() ;

  } // Error()

} ; // Error()

// =========Error=============

// ==============================GetTokenStart===========================================

void Readline() { // Ū�����(error ����) 

  while ( cin.peek() != '\n' && cin.peek() != EOF )
    cin.get() ;

  // cin.get() ;
} // Readline()

bool IsSeparator( char t ) { // check seperator ( '\t', '\n', '(', ')', ';', '\'', '\"' ) 

  if ( t == '\t' || t == '\n' || t == ' ' )
    return true ; 

  else if ( t == '(' || t == ')' || t == ';' || t == '\'' || t == '\"' )
    return true ;

  else 
    return false ;

} // IsSeparator()

bool IsDigit( char t ) { // �ˬd���r���O���O�Ʀr 

  if ( t >= 48 && t <= 57 ) // ASCII Code 0~9
    return true ;
  else
    return false ;

} // IsDigit()

bool IsLetter( char t ) { // �ˬd�r���O���O�r��
   
  if ( t >= 65 && t <= 90 ) // ASCII Code A~Z  
    return true ;
  else if ( t >= 97 && t <= 122 ) // ASCII Code a~b
    return true ;
  else
    return false ;

} // IsLetter()

char GetNonWhiteSpaceChar( int & row, int & column ) { // Ū�@�ӫD�ťզr��	
  
  while ( cin.peek() == '\t' || cin.peek() == '\n' || cin.peek() == ' ' ) {

    if ( cin.peek() == '\n' ) {
      // �J�촫��n��
      row++ ;
      column = 0 ;

    } // if()

    else 
      column++ ;

    cin.get() ;

  } // while()

  return cin.peek() ;

} // GetNonWhiteSpaceChar()

void GetToken( Token & token ) {
  // Get a token
  // uLine, uColumn�ΨӰO�UTOKEN����m
  bool iseof = false ;
  char ch ; // �Ȧs�r��
  char * temptoken = new char[MAX_LENGTH] ; // �Ȧstoken�W�r
  int i = 1 ; // temptoken �� index
  TokenType newType ; // �s�@��token��TYPE

  ch = GetNonWhiteSpaceChar( uLine, uColumn ) ; // ��Ū�@�Ӧr���P�_�O����}�Y��TOKEN (First token set)

  if ( uLine == 0 && cin.peek() != ';' ) // ����( 1 2 ) ( 1 2 )�o�˧ڪ��ĤG��S-EXP��line�|�O�s 
    uLine++ ; // �[�W�P�_���ѲŸ��O���F�����o�ت��p: ( A ) ; 123 �o��Ū���e������ѳo�@��O���⪺ 

  if ( ch == EOF ) // ERROR (no more input) : END-OF-FILE encountered
    throw new Error( 4, temptoken, uLine, uColumn ) ;

  else if ( ch == '(' ) { // LP

    temptoken[0] = cin.get() ;
    temptoken[1] = '\0' ;
    newType = LP ;
    uColumn++ ;

    ch = GetNonWhiteSpaceChar( uLine, uColumn ) ;

    if ( cin.peek() == ')' ) {

      temptoken[1] = cin.get() ;
      temptoken[2] = '\0' ;
      newType = NIL ;
      uColumn++ ;

    } // if 

  } // if()

  else if ( ch == ')' ) { // RP

    temptoken[0] = cin.get() ;
    temptoken[1] = '\0' ;
    newType = RP ;
    uColumn++ ; 

  } // else if()
  
  else if ( ch == '\'' ) { // QUOTE

    temptoken[0] = cin.get() ;
    temptoken[1] = '\0' ;
    newType = QUOTE ;
    uColumn++ ;

  } // else if()

  else if ( ch == '\"' ) { // string
 
    temptoken[0] = cin.get() ;
    ch = cin.peek() ;
    uColumn++ ;

    while ( ch != '\"' ) {

      if ( ch == EOF ) // ERROR (no more input) : END-OF-FILE encountered
        throw new Error( 4, temptoken, uLine, uColumn ) ;

      else if ( ch == '\n' ) // ERROR (no closing quote) : END-OF-LINE encountered at Line X Column Y
        throw new Error( 3, temptoken, uLine, uColumn ) ;

      else if ( ch == '\\' ) { // ����r���A�ϱ׽u( \n, \t�O���\�઺ )�A�ҥH�n�P�_ \ ����@�Ӧr�� 

        cin.get() ; // ����ϱ׽u����
        uColumn++ ;

        // �P�_�U�@�Ӧr��
        ch = cin.peek() ;

        if ( ch == '\n' ) // ERROR (no closing quote) : END-OF-LINE encountered at Line X Column Y
          throw new Error( 3, temptoken, uLine, uColumn ) ;
        else if ( ch == 'n' ) 
          ch = '\n' ;
        else if ( ch == 't' )
          ch = '\t' ;
        else if ( ch == '\\' || ch == '\"' )
          ; // EX. '\\', '\"', '\\n', '\\t'
        else {
          temptoken[i] = '\\' ; // ���讳�����ϱ׽u��^�� 
          i++ ;

        } // else()

      } // else if()

      cin.get() ;

      uColumn++ ;
      temptoken[i] = ch ;
      i++ ;
      ch = cin.peek() ;      

    } // while()

    temptoken[i] = ch ; // this one is a "
    cin.get() ;
    temptoken[i+1] = '\0' ;
    newType = STRING ;
    uColumn++ ;

  } // else if()

  else if ( ch == '.' ) { // DOT, INT, SYMBOL

    temptoken[0] = ch ;
    cin.get() ;
    ch = cin.peek() ;
    uColumn++ ;

    bool symbol = false ;

    while ( ! IsSeparator( ch ) && ! iseof ) {

      if ( ch == EOF ) // ERROR (no more input) : END-OF-FILE encountered
        iseof = true ;

      else if ( ! IsDigit( ch ) ) // �u�n�L�����O�Ʀr���r���A�N�Osymbol
        symbol = true ;

      if ( ! iseof ) { // 2025/6/7 updated
        temptoken[i] = ch ;
        cin.get() ;
        i++ ;
        ch = cin.peek() ;
        uColumn++ ;
      } // if()

    } // while()

    temptoken[i] = '\0' ;

    if ( symbol ) 
      newType = SYMBOL ;
    else if ( i == 1 )
      newType = DOT ;
    else
      newType = FLOAT ;

  } // else if
  
  else if ( ch == ';' ) { // ���� 

    Readline() ;  

    // uLine++ ;
    uColumn = 0 ;

    GetToken( token ) ;

    return ;

  } // else if 

  else if ( IsDigit( ch ) || ch == '+' || ch == '-' ) { // INT, FLOAT, SYMBOL

    int dot = 0 ; // �p�� DOT ���ƶq
    bool letter = false ; // �p�G��true, this token contain letters
    bool digit = false ; // �p�G��true, this token contain numbers

    if ( IsDigit( ch ) )
      digit = true ;

    temptoken[0] = cin.get() ;
    ch = cin.peek() ;
    uColumn++ ;

    while ( ! IsSeparator( ch ) && ! iseof ) {

      if ( ch == EOF ) // ERROR (no more input) : END-OF-FILE encountered
        iseof = true ;
      else if ( ch == '.' )
        dot++ ;
      else if ( IsDigit( ch ) ) // ���]�t�Ʀr 
        digit = true ;
      else if ( ! IsDigit( ch ) ) // ���D�Ʀr�X�{�A�N�OSYMBOL 
        letter = true ;

      if ( ! iseof ) {
        temptoken[i] = cin.get() ;
        i++ ; // counter
        ch = cin.peek() ; // next
        uColumn++ ;
      } // if()

    } // while()

    if ( ( dot > 1 || letter ) || ! digit ) // symbol
      newType = SYMBOL ;
    else if ( dot == 1 ) // float
      newType = FLOAT ;
    else // int
      newType = INT ; 

    temptoken[i] = '\0' ;

  } // else if()

  else if ( IsLetter( ch ) || ch == '#' ) { // SYMBOL, NIL, T

    temptoken[0] = cin.get() ; 
    ch = cin.peek() ;
    uColumn++ ;

    while ( ! IsSeparator( ch ) && ! iseof ) {

      if ( ch == EOF ) // ERROR (no more input) : END-OF-FILE encountered
        iseof = true ;

      if ( ! iseof ) {
        temptoken[i] = ch ;
        cin.get() ;     
        i++ ;
        ch = cin.peek() ;     
        uColumn++ ;
      } // if()

    } // while()

    temptoken[i] = '\0' ;

    if ( strcmp( temptoken, "nil" ) == 0 || strcmp( temptoken, "#f" ) == 0 )
      newType = NIL ;

    else if ( strcmp( temptoken, "t" ) == 0 || strcmp( temptoken, "#t" ) == 0 )
      newType = T ;

    else
      newType = SYMBOL ;

  } // else if()

  else if ( ! IsSeparator( ch ) ) { // SYMBOL

    temptoken[0] = cin.get() ; 
    ch = cin.peek() ;
    uColumn++ ;

    while ( ! IsSeparator( ch ) && ! iseof ) {

      if ( ch == EOF ) // ERROR (no more input) : END-OF-FILE encountered
        iseof = true ;

      if ( ! iseof ) {
        temptoken[i] = ch ;
        cin.get() ;    
        i++ ;
        ch = cin.peek() ;     
        uColumn++ ;
      } // if()

    } // while()

    temptoken[i] = '\0' ;
    newType = SYMBOL ;    

  } // else if()

  else // ERROR (unexpected token) : atom or '(' expected when token at Line X Column Y is >>...<<
    throw new Error( 1, temptoken, uLine, uColumn ) ;

  token.name = temptoken ;
  token.type = newType ;
  token.line = uLine ; // �������ȬO�{�btoken����m
  token.column = uColumn ;

  /*
  cout << "Line: " << token.line ;
  cout << " Column: " << token.column ;
  cout << " Type: " << token.type ; 
  cout << " Token: " << token.name << endl ;
  */

} // GetToken()

// ==============================GetTokenEnd===========================================

// ==============================BuildTreeStart==========================================

Exp GetSexp( Exp & list ) { // ex. ( A B C ) �|���X A �^��( A�O�@��sExp )
  // list �O translation�L�᪺ 

  int lp = 0 ;
  Exp temp ;

  if ( list[0].type == LP && list.back().type == RP ) { // ��@�ե���M�k��R�� 

    list.pop_back() ;
    list.erase( list.begin() ) ;

  } // if

  if ( list[0].type == QUOTE || list.size() == 1 ) { // ��QUOTE�S�O�B�z�A�٦��u���@�Ӫ� 

    temp.push_back( list[0] ) ;
    list.erase( list.begin() ) ;

    return temp ;
 
  } // if 

  while ( ! list.empty() && ( lp != 0 || list[0].type != DOT ) ) {

    if ( list[0].type == LP )
      lp++ ;

    else if ( list[0].type == RP )
      lp-- ;

    temp.push_back( list[0] ) ;
    list.erase( list.begin() ) ;

  } // while

  if ( list[0].type == DOT )
    list.erase( list.begin() ) ; // ���Ipop��

  /*
  cout << "first:" << endl ;
  PrintList( temp ) ; 
  cout << "rest: " << endl ;
  PrintList( list ) ;
  */

  return temp ;

} // GetSexp()

bool Atom( Token token ) { // �Ψ������ؾ� 

  if ( token.type == SYMBOL )
    return true ;
  else if ( token.type == INT )
    return true ;
  else if ( token.type == FLOAT )
    return true ;
  else if ( token.type == STRING )
    return true ;
  else if ( token.type == NIL )
    return true ;
  else if ( token.type == T )
    return true ;
  else if ( token.type == QUOTE ) 
    return true ;
  else
    return false ;

} // Atom()

bool Atom( Exp tokenList, Exp & tList, int & i ) { // �Ψ����U�ഫ�� 

  if ( tokenList[i].type == SYMBOL )
    ;
  else if ( tokenList[i].type == INT )
    ;
  else if ( tokenList[i].type == FLOAT )
    ;
  else if ( tokenList[i].type == STRING )
    ;
  else if ( tokenList[i].type == NIL )
    ;
  else if ( tokenList[i].type == T )
    ; 
  else
    return false ;

  return true ;

} // Atom()

bool Translation( Exp tokenList, Exp & tList, int & i ) { // �Բӥ\��b�̭� 
  // Translate EVERY '(...)' thing into a format like the following :
  //  ( �� . ( �� . ( �� . ( �� . �� ) ) ) )
  // ���ˬd��k���覡�[�W . ( �� . nil 
  // tList = translate list
  // i == index

  // �����|�Ψ쪺
  int lp = 0 ; // lp���ƶq �̫�n�[�W�ۦP�� 
  
  tList.push_back( tokenList[i] ) ; // put first token into list

  if ( Atom( tokenList, tList, i ) ) {

    i++ ;
    return true ;

  } // if
  
  else if ( tokenList[i].type == LP ) {

    i++ ;

    if ( Translation( tokenList, tList, i ) ) { // if 1

      tList.push_back( uDotToken ) ; // �b�o�̥[ 
      tList.push_back( uLpToken ) ;
      lp++ ; 

      while ( Translation( tokenList, tList, i ) ) { // �X�j��ɡA�|�hŪ�@��token

        tList.push_back( uDotToken ) ; // �b�o�̥[ 
        tList.push_back( uLpToken ) ;
        lp++ ;  

      } // while()
      
      // �]���j��X�өΨS�i�j��i��|�h�[�@���A�ҥH��pop�� 
      tList.pop_back() ; 
      tList.pop_back() ;
      lp-- ;
      
      if ( tokenList[i].type == DOT ) { // if 2
      
        tList.push_back( tokenList[i] ) ; // ���I�I��i 
        i++ ;
      
        if ( ! Translation( tokenList, tList, i ) ) {

          tList.pop_back() ;
          return false ;

        } // if

      } // if 2

      else {

        tList.push_back( uDotToken ) ; // �b�o�̥[
        tList.push_back( uNilToken ) ;

      } // else

      if ( tokenList[i].type == RP ) {

        tList.push_back( tokenList[i] ) ;
        i++ ;

        for ( int x = 0 ; x < lp ; x++ )
          tList.push_back( uRpToken ) ;
        
        return true ;

      } // if

      else {

        tList.pop_back() ;
        return false ;

      } // else

    } // if 1

    else {

      tList.pop_back() ;
      return false ;
      
    } // else

  } // else if
  
  else if ( tokenList[i].type == QUOTE ) {

    tList.insert( tList.begin(), uLpToken ) ;
    i++ ;

    if ( Translation( tokenList, tList, i ) ) {

      tList.push_back( uRpToken ) ;

      return true ;

    } // if

  } // else if

  else {

    tList.pop_back() ;
    return false ;

  } // else 

  return false ;

} // Translation()

void Build( Exp sExp, Tree & tree ) { // ���;�
  // sExp : Translation�L��

  // �Ф@�ʥ��s���� 
  tree = new Node() ;
  tree->left = NULL ;
  tree->right = NULL ;

  Exp left = GetSexp( sExp ) ; // get���䪺exp

  if ( left.size() == 1 && Atom( left[0] ) ) { // ����쩳�F �i�H�����`�I��

    if ( left[0].type == NIL && strcmp( left[0].name.c_str(), "()" ) )
      tree->left = uNilNode ;
    else if ( left[0].type == T )
      tree->left = uTNode ;
    else
      CreateNode( tree->left, left[0] ) ;

  } // if
  
  else
    Build( left, tree->left ) ;

  if ( sExp.size() == 1 && Atom( sExp[0] ) ) { // �k��쩳

    if ( sExp[0].type == NIL )
      tree->right = uNilNode ;
    else if ( sExp[0].type == T )
      tree->right = uTNode ;
    else
      CreateNode( tree->right, sExp[0] ) ;

  } // if 

  else
    Build( sExp, tree->right ) ;

} // Build()

void BuildTree( Exp & sExp, Tree & tree ) { // �N�ؾ�

  int i = 0 ;
  Exp tList ;

  // Step 1:Translate  
  Translation( sExp, tList, i ) ; 
  // PrintList( tList ) ; // TEST

  // step 2:Build
  if ( tList.size() == 1 ) {

    CreateNode( tree, tList[0] ) ;

  } // if

  else
    Build( tList, tree ) ;

  /*
  cout << "Tree:\n" ; 
  PrintTree( tree ) ; // TEST
  */

} // BuildTree()

// ==============================BuildTreeEnd==========================================

// ==============================PrettyPrintStart=========================================== 

void PrintInt( string name ) { 

  cout << atoi( name.c_str() ) ;

} // PrintInt() 

void PrintFloat( string name ) {

  float sum = atof( name.c_str() ) ; 

  printf( "%.3f", sum ) ;

} // PrintFloat()

void PrintToken( Token token ) { // ��TOKEN�L�X�� 

  if ( token.type == INT )
    PrintInt( token.name ) ; // cout << token.name ; 

  else if ( token.type == FLOAT )
    PrintFloat( token.name ) ; // cout << token.name ;

  else if ( token.type == NIL )
    cout << "nil" ;

  else if ( token.type == T )
    cout << "#t" ;

  else if ( token.type == QUOTE )
    cout << "quote" ;

  else if ( IsInternalFunction( token.name ) && ! uError && uProcedure ) 
    cout << "#<procedure " << token.name << ">" ;

  else if ( IsboundF( token.name ) && ! uError && uProcedure )
    cout << "#<procedure " << token.name << ">" ;

  else
    cout << token.name ;

  cout << endl ;

} // PrintToken()

void PrettyPrint( Tree tree, int space ) {

  if ( tree == NULL ) // �z�פW�u����verbose = nil, define or clean�~�|�Ψ� 
    return ;

  else if ( tree->left == NULL && tree->right == NULL ) // ATOM
    PrintToken( tree->token ) ;

  else if ( tree->left != NULL && tree->right != NULL && ! strcmp( tree->token.name.c_str(), "lambda" ) )
    cout << "#<procedure lambda>" << endl ;

  else { // s is of the form : '(' s1 s2 ... sn [ '.' snn ] ')'

    cout << "( " ;
    PrettyPrint( tree->left, space + 2 ) ;

    Tree temp = tree->right ; // ���k�L�U�@��S-EXP

    while ( temp->left != NULL && temp->right != NULL ) {
      // ������P�_list[1]�A�]���bgetExp���ɭԡA�u�nlist�٦��F��A
      // �짹��|��R����'(' ')'�[�^�h�AEX (A . B)�� 'A' ���|�ܦ�( . B ) 

      for ( int i = 0 ; i < space + 2 ; i++ ) // print M+2 spaces
        cout << " " ;

      PrettyPrint( temp->left, space + 2 ) ; // print sn

      temp = temp->right ;

    } // while()

    if ( temp->token.type != NIL ) { 
      // if there are '.' and snn following sn
      // (A . nil)���� . nil �����ΦL�X�� 

      for ( int i = 0 ; i < space + 2 ; i++ ) // print M+2 spaces
        cout << " " ;

      PrintToken( uDotToken ) ; // print '.', print '\n'
      // PrintToken�̤w�g������F

      for ( int i = 0 ; i < space + 2 ; i++ ) // print M+2 spaces
        cout << " " ;

      PrettyPrint( temp, space + 2 ) ; // print snn

    } // if

    for ( int i = 0 ; i < space ; i++ ) // print M spaces
      cout << " " ;

    cout << ")\n" ; // print ')', print '\n'

  } // else

} // PrettyPrint()

void PrintExp( Tree tree ) {
  // list: Translation list
  // original: �쥻��list
  /*
  cout << "PrettyPrint\n" ;
  PrintList( original ) ;
  */

  if ( tree->token.type == NOPROCEDURE ) uProcedure = false ; // �קKquote�L�Xprocedure 

  PrettyPrint( tree, 0 ) ;

} // PrintExp()

// ==============================PrettyPrintEnd===========================================

// ==============================ParserStart=============================================

bool Atom( Exp & tokenList ) {

  if ( tokenList.back().type == SYMBOL )
    ;
  else if ( tokenList.back().type == INT )
    ;
  else if ( tokenList.back().type == FLOAT )
    ;
  else if ( tokenList.back().type == STRING )
    ;
  else if ( tokenList.back().type == NIL )
    ;
  else if ( tokenList.back().type == T )
    ;
  else
    return false ;

  return true ;

} // Atom()

bool S_exp( Exp & tokenList ) {

  // read and put first token
  Token tempToken ; // �s�Ĥ@��Token 
  GetToken( tempToken ) ; // Get first token
  tokenList.push_back( tempToken ) ; // put first token into list

  // start checking syntax
  if ( Atom( tokenList ) ) {

    return true ;

  } // if

  else if ( tempToken.type == LP ) {

    if ( S_exp( tokenList ) ) { // if 1

      while ( S_exp( tokenList ) ) // �X�j��ɡA�|�hŪ�@��token
        ;

      if ( tokenList.back().type == DOT ) { // if 2
      
        if ( ! S_exp( tokenList ) ) { // ERROR (unexpected token) : atom or '('...

          throw new Error( 1, tokenList.back().name.c_str(), 
                           tokenList.back().line, tokenList.back().column ) ;

        } // if

        // �o��]���α�while�j��hŪ��token�A�ҥH�AŪ�@�ӥΨӧP�_RP 
        GetToken( tempToken ) ; 
        tokenList.push_back( tempToken ) ;

      } // if 2

      if ( tokenList.back().type == RP ) {

        return true ;

      } // if

      else // ERROR (unexpected token) : ')' expected...
        throw new Error( 2, tokenList.back().name.c_str(), 
                         tokenList.back().line, tokenList.back().column ) ;

    } // if 1

    else // ERROR (unexpected token) : atom or '(' expected...
      throw new Error( 1, tokenList.back().name.c_str(), 
                       tokenList.back().line, tokenList.back().column ) ;

  } // else if
  
  else if ( tempToken.type == QUOTE ) {

    tempToken.type = SYMBOL ; // �� '(...) �ܦ� ( quote (...) )
    tempToken.name = "quote" ;
    tokenList.pop_back() ; // �[�A���A�n�[�bquote�e���A�ҥH����quote����
    tokenList.push_back( uLpToken ) ; // �� '('
    tokenList.push_back( tempToken ) ; // �M��A��^QUOTE 

    if ( S_exp( tokenList ) ) {

      tokenList.push_back( uRpToken ) ; // �̫�[�W ')' 

      return true ;
      
    } // if

    else // ERROR (unexpected token) : atom or '(' expected...
      throw new Error( 1, tokenList.back().name.c_str(), 
                       tokenList.back().line, tokenList.back().column ) ;

  } // else if

  else {
    return false ; 

  } // else()

  return false ;

} // S_exp()

void Parse( Exp & tokenList ) {
  // sExp: �s�@�Ӧ��Ī����O ( vector<Token> )
  // �@�����ͤ@�� S-Exp
  // �]�t�ؾ�

  uLine = 0 ; // �C��Ū�s��EXP�N���s��m��T 
  uColumn = 0 ;

  if ( tokenList.size() > 0 ) tokenList.clear() ;

  if ( S_exp( tokenList ) ) {

    ; // syntax no error

  } // if()

  else { // Error

    throw new Error( 1, tokenList.back().name.c_str(), tokenList.back().line, tokenList.back().column ) ;

  } // else()

} // Parse()

// ==============================ParserEnd=============================================

// ==============================EvalError============================================

class EvalError : public exception { // Eval error

  public:
  EvalError( int error, const char* msg, Tree tree ) {

    uError = true ;

    if ( error == 0 ) { // cond format error

      cout << "ERROR (" << msg << " format) : " ;
      PrintExp( tree ) ;

    } // if()

    else if ( error == 1 ) { // non list

      cout << "ERROR (non-list) : " ;
      PrintExp( tree ) ;

    } // else if()

    else if ( error == 2 ) { // unbound symbol

      cout << "ERROR (unbound symbol) : " << msg << endl ;

    } // else if()

    else if ( error == 3 ) { // non funct.

      uError = false ;
      cout << "ERROR (attempt to apply non-function) : " ;
      PrintExp( tree ) ;

    } // else if()

    else if ( error == 4 ) { // level error

      cout << "ERROR (level of " << msg << ")" << endl ;

    } // else if()

    else if ( error == 5 ) { // ERROR (incorrect number of arguments) : XXX

      cout << "ERROR (incorrect number of arguments) : " << msg << endl ;

    } // else if()

    else if ( error == 6 ) { // ERROR (no return value) : 

      cout << "ERROR (no return value) : " ;
      PrintExp( tree ) ;

    } // else if()

    else if ( error == 7 ) { // ERROR (xxx with incorrect argument type) : the-evaluated-result
      // msg = xxx, tree->token.name = evaluated-result
      uError = false ;
      cout << "ERROR (" << msg << " with incorrect argument type) : " ;
      PrintExp( tree ) ;

    } // else if() 

    else if ( error == 8 ) { // ERROR (division by zero)

      cout << "ERROR (division by zero) : " << msg << endl ;

    } // else if()

    else if ( error == 9 ) { // exit call

      uExit = true ; 

    } // else if()

    else if ( error == 10 ) { // let, lambda format error

      cout << "ERROR (" << msg << " format)" << endl ;

    } // else if()

    else if ( error == 11 ) {

      cout << "ERROR (unbound parameter) : " ;
      PrintExp( tree ) ;
    } // else if

    else if ( error == 12 ) {

      cout << "ERROR (unbound test-condition) : " ;
      PrintExp( tree ) ;
    } // else if

    else if ( error == 13 ) {

      cout << "ERROR (unbound condition) : " ;
      PrintExp( tree ) ;
    } // else if

    Clean_lambda() ;
    uLocalTable.clear() ;
    uError = false ;
  } // EvalError()

} ; // EvalError()

// ==============================EvalError============================================

// ==============================EvalStart============================================

Tree GetBinding( string name ) { // Get binding of symbol

  Tree result = new Node() ;
  result->left = NULL ;
  result->right = NULL ;

  int arg = uArg ; // define function ���ܼƼƶq 

  // local���ݪk�O�q�᭱�}�l�ݡA�]���i�঳function�̭��A�I�sfunction 
  for ( int i = uLocalTable.size() - 1 ; i >= 0 && arg > 0 ; i-- ) { // is local

    if ( strcmp( name.c_str(), uLocalTable[i].name.c_str() ) == 0 ) {

      result = uLocalTable[i].binding ;
      return result ;

    } // if()

    arg-- ;
  } // for()

  for ( int i = 0 ; i < uSymbolTable.size() ; i++ ) { // is bound to an S-expression

    if ( strcmp( name.c_str(), uSymbolTable[i].name.c_str() ) == 0 ) {

      result = uSymbolTable[i].binding ;
      return result ;

    } // if()

  } // for()

  CreateToken( result->token, name, 0, 0, SYMBOL ) ; // internal function or define function

  return result ;

} // GetBinding()

bool Isbounded( string name ) { // check whether it is bound to an binding

  for ( int i = 0 ; i < uLocalTable.size() ; i++ ) { // check local table

    if ( strcmp( name.c_str(), uLocalTable[i].name.c_str() ) == 0 ) 
      return true ;

  } // for()

  for ( int i = 0 ; i < uSymbolTable.size() ; i++ ) { // check symbol table

    if ( strcmp( name.c_str(), uSymbolTable[i].name.c_str() ) == 0 ) 
      return true ;

  } // for()

  return false ;
} // Isbounded()

bool IsboundF( string name ) { // check whether it is bound to function

  for ( int i = 0 ; i < uFunctionTable.size() ; i++ ) { // check function table

    if ( strcmp( name.c_str(), uFunctionTable[i].name.c_str() ) == 0 )
      return true ;

  } // for()

  return false ;
} // IsboundF()

bool IsInternalFunction( string name ) { // check whether it is internal function

  // check internal function
  if ( strcmp( name.c_str(), "cons" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "list" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "quote" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "define" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "car" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "cdr" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "atom?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "pair?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "list?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "null?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "integer?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "real?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "number?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "string?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "boolean?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "symbol?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "+" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "-" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "*" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "/" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "not" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "and" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "or" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), ">" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), ">=" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "<" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "<=" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "=" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "string-append" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "string>?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "string<?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "string=?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "eqv?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "equal?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "begin" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "if" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "cond" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "clean-environment" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "lamdba" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "set!" ) == 0 )
    return true ; 
  else if ( strcmp( name.c_str(), "let" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "lambda" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "verbose" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "verbose?" ) == 0 )
    return true ;
  else if ( strcmp( name.c_str(), "exit" ) == 0 )
    return true ;
 
  else
    return false ;

} // IsInternalFunction()

bool IsAList( Tree tree ) { // check is a pure list or not

  Tree temp ;

  if ( tree->left == NULL && tree->right == NULL )
    return false ;

  temp = tree->right ;

  while ( temp->left != NULL && temp->right != NULL )
    temp = temp->right ;

  if ( temp->token.type != NIL )
    return false ;
  else
    return true ;

} // IsAList()

bool IsDoubleTon( Tree tree ) { // for checking cond 
  // DOUBLETON df= a list of two or more S-expressions
  int i = 0 ;

  Tree temp = tree ;

  if ( tree->left == NULL && tree->right == NULL )
    return false ;

  while ( temp->left != NULL && temp->right != NULL ) {

    i++ ;
    temp = temp->right ;

  } // while()

  if ( temp->token.type == NIL && i >= 2 )
    return true ;
  else
    return false ;

} // IsDoubleTon() 

bool IsLetPair( Tree tree ) { // check (...) is a pair
  // PAIR df = ( symbol S-expression )

  if ( tree->left == NULL && tree->right == NULL )
    return false ;

  Tree temp = tree->left ;

  if ( temp->left == NULL && temp->right == NULL && temp->token.type == SYMBOL ) { // first arg is sym

    temp = tree->right ;

    // �ˬd�O�_�u���@��S-EXP
    if ( temp->right->left == NULL && temp->right->right == NULL && temp->right->token.type == NIL )
      return true ;
    else
      return false ;

  } // if()

  else 
    return false ;

} // IsLetPair()

bool Check_Let_Pairs( Tree tree ) { // check (...) is zero or more pairs

  Tree temp = tree ;

  if ( tree->left == NULL && tree->right == NULL && ! strcmp( tree->token.name.c_str(), "()" ) ) // zero
    return true ;

  else if ( tree->left == NULL && tree->right == NULL ) // is atom
    return false ;

  while ( temp->left != NULL && temp->right != NULL ) {

    if ( ! IsLetPair( temp->left ) ) // �p�Garg���Opair 
      return false ;

    temp = temp->right ;
  } // while

  return true ;
} // Check_Let_Pairs()

bool Check_Format_Sym( Tree tree ) { // check (...) is zero or more sym

  Tree temp = tree ;

  if ( tree->left == NULL && tree->right == NULL && ! strcmp( tree->token.name.c_str(), "()" ) ) // zero
    return true ;

  else if ( tree->left == NULL && tree->right == NULL ) // is atom
    return false ;

  while ( temp->left != NULL && temp->right != NULL ) {

    // �p�Garg���Osymbol
    if ( temp->left->left == NULL && temp->left->right == NULL && temp->left->token.type != SYMBOL ) 
      return false ;

    temp = temp->right ;
  } // while

  return true ;
} // Check_Format_Sym()

bool Check_Cond_Format( Tree tree ) {

  Tree temp = tree ;

  if ( temp->left == NULL && temp->right == NULL )
    return false ;

  while ( temp->left != NULL && temp->right != NULL ) {

    if ( ! IsDoubleTon( temp->left ) )
      return false ;

    temp = temp->right ;

  } // while()

  if ( temp->left == NULL && temp->right == NULL && temp->token.type != NIL )
    return false ;
  else
    return true ;

} // Check_Cond_Format()

bool Check_Let_Format( Tree tree ) {
  // (let (zero-or-more-PAIRs) one-or-more-S-expressions )

  if ( tree->left == NULL && tree->right == NULL ) // (let) (let . ? )
    return false ;

  Tree left = tree->left ; // (zero-or-more-PAIRs)

  if ( ! Check_Let_Pairs( left ) ) // zero or more pairs
    return false ;

  Tree right = tree->right ;

  if ( right->left == NULL && right->right == NULL ) // ( let (A) . B ) ( let (A) ) 
    return false ;
  else // (let (A) B... )
    return true ;

} // Check_Let_Format()

bool Check_Lambda_Format( Tree tree ) {
  // (lambda (zero-or-more-symbols) one-or-more-S-expressions )

  if ( tree->left == NULL && tree->right == NULL ) // (lambda) (lambda . ? )
    return false ;

  Tree left = tree->left ; // (zero-or-more-symbol)

  if ( ! Check_Format_Sym( left ) ) // zero or more symbol
    return false ;

  Tree right = tree->right ;

  if ( right->left == NULL && right->right == NULL ) // ( lambda (A) . B ) ( lambda (A) ) 
    return false ;
  else // (lambda (A) B... )
    return true ;

} // Check_Lambda_Format()

bool Check_Define_Format2( Tree tree ) { // project 3 define function
  // (define (SYMBOL zero-or-more-symbols) one-or-more-S-expressions )

  if ( tree->left == NULL && tree->right == NULL ) // (define) (define . ? )
    return false ;

  Tree left = tree->left ; // (one-or-more-symbol)

  if ( left->left == NULL && left->right == NULL && ! strcmp( left->token.name.c_str(), "()" ) ) // zero
    return false ;

  if ( ! Check_Format_Sym( left ) ) // zero or more symbol
    return false ;

  Tree right = tree->right ;

  if ( right->left == NULL && right->right == NULL ) // ( define (A) . B ) ( define (A) ) 
    return false ;
  else // (define (A) B... )
    return true ;

} // Check_Define_Format2()

bool Check_Define_Format( Tree tree ) { // proj 2 define symbol

  if ( tree->left == NULL && tree->right == NULL ) // (define . ATOM) or (define) error
    return false ;

  else {

    Tree temp = tree->left ; // check first arg

    // �p�G�Ĥ@�ӬO�@��SYmbol�A 
    if ( temp->left == NULL && temp->right == NULL && temp->token.type == SYMBOL ) {

      if ( IsInternalFunction( temp->token.name ) ) // attempting to redefine system primitive
        return false ;

      temp = tree->right ; // check second arg

      if ( temp->left == NULL && temp->right == NULL ) // (define a . ATOM ) error
        return false ;

      else {

        temp = temp->right ;

        if ( temp->left == NULL && temp->right == NULL && temp->token.type == NIL )
          return true ;
        else
          return false ; // (define a b . ATOM) or (define a b c)

      } // else()

    } // if()

    else // (define (a b c) ... ) or (define a is not sym )
      return false ;

  } // else()

} // Check_Define_Format()

int Get_Num_Of_Arg( Tree tree ) {

  int arg = 0 ;
  Tree temp = tree ;

  while ( temp->left != NULL && temp->right != NULL ) {

    arg++ ;
    temp = temp->right ;
  } // while()

  return arg ;

} // Get_Num_Of_Arg() 

bool Check_Num_Of_Arg( string name, Tree tree ) { // not done
  // name = function name
  // tree = right part of original main tree(���F�Ĥ@��arg�A�]�N�Ofunction name�H�~��)
  int arg = Get_Num_Of_Arg( tree ) ;

  if ( strcmp( name.c_str(), "cons" ) == 0 && arg != 2 )
    return false ;
  else if ( strcmp( name.c_str(), "list" ) == 0 && arg < 0 )
    return false ;
  else if ( strcmp( name.c_str(), "quote" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "car" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "cdr" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "atom?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "pair?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "list?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "null?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "integer?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "real?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "number?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "string?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "boolean?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "symbol?" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "+" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "-" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "*" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "/" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "not" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "and" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "or" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), ">" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), ">=" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "<" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "<=" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "=" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "string-append" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "string>?" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "string<?" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "string=?" ) == 0 && arg < 2 )
    return false ;
  else if ( strcmp( name.c_str(), "eqv?" ) == 0 && arg != 2 )
    return false ;
  else if ( strcmp( name.c_str(), "equal?" ) == 0 && arg != 2 )
    return false ;
  else if ( strcmp( name.c_str(), "begin" ) == 0 && arg < 1 )
    return false ;
  else if ( strcmp( name.c_str(), "if" ) == 0 && arg != 2 && arg != 3 )
    return false ;
  else if ( strcmp( name.c_str(), "clean-enviroment" ) == 0 && arg != 0 )
    return false ;
  else if ( strcmp( name.c_str(), "exit" ) == 0 && arg != 0 )
    return false ;
  else if ( strcmp( name.c_str(), "verbose" ) == 0 && arg != 1 )
    return false ;
  else if ( strcmp( name.c_str(), "verbose?" ) == 0 && arg != 0 )
    return false ;
  return true ;

} // Check_Num_Of_Arg()

// ===function===

bool CondOk( Tree tree, Tree & result ) { // �\�ରeval a doubleton

  Tree eval_s1 = EvalSexp( tree->left, 1 ) ; // �ھڦ����G�M�w�o��doubleton���\�P�_ 

  if ( eval_s1 == NULL ) throw new EvalError( 12, "error", tree->left ) ; // error

  if ( eval_s1->left == NULL && eval_s1->right == NULL && eval_s1->token.type == NIL ) { // �N����󤣦��� 
  
    return false ;

  } // if()

  else { // ���󦨥ߡAeval last arg

    Tree temp = tree->right ;

    while ( temp->right->left != NULL && temp->right->right != NULL ) { // go to last arg

      result = EvalSexp( temp->left, 1 ) ; 
      temp = temp->right ;
    } // while()

    result = EvalSexp( temp->left, 1 ) ; // get evaluate result

    return true ;

  } // else()

} // CondOk() 

Tree Define( Tree tree ) { // define symbol
  // ( define SYMBOL S-expression )
  // ( define ( SYMBOL zero-or-more-symbols ) one-or-more-S-expressions )

  Tree result ;
  Tree temp = tree->left ; // tree->left is symbol or ( sym sym .. )
  Tree temp2 = tree->right->left ; // tree->right->left is S-Exp or S-exp S-exp

  int indexOfSym = 0 ; // �p�G�w�g�Q�w�q�A�O�U��m
  int indexOfFun = 0 ;
  bool isboundSym = false ; // �ݳo��symbol�O���O�w�g�Q�w�q��S-exp 
  bool isboundFun = false ; // �ݳo��symbol�O���O�w�g�Q�w�q��function 

  while ( indexOfSym < uSymbolTable.size() && ! isboundSym ) { // check if is already bounded to s-exp

    if ( strcmp( temp->token.name.c_str(), uSymbolTable[indexOfSym].name.c_str() ) == 0 )
      isboundSym = true ;

    indexOfSym++ ;
  } // while

  while ( indexOfFun < uFunctionTable.size() && ! isboundFun ) { // check if is already bounded to funct

    if ( strcmp( temp->token.name.c_str(), uFunctionTable[indexOfFun].name.c_str() ) == 0 )
      isboundFun = true ;

    indexOfFun++ ;
  } // while

  Tree eval_s2 = EvalSexp( temp2, 1 ) ;

  if ( eval_s2 == NULL ) throw new EvalError( 6, "error", temp2 ) ; // error

  if ( isboundFun ) { // ���Q�w�q��function�A���O�n���s�w�q��s-exp 

    // �ҥH�n�R���bfunction table���w�q 
    uFunctionTable.erase( uFunctionTable.begin() + indexOfFun - 1 ) ;

  } // if()

  if ( isboundSym ) { // already bounded s-exp

    uSymbolTable[indexOfSym-1].binding = eval_s2 ;

  } // if()

  else {

    SymbolTable t ;
    t.name = temp->token.name ;
    t.binding = eval_s2 ;

    uSymbolTable.push_back( t ) ;

  } // else()

  if ( uVerbose ) {

    Token tempToken ;
    CreateToken( tempToken, temp->token.name + " defined", 0, 0, SYMBOL ) ;
    CreateNode( result, tempToken ) ;

  } // if()

  else { // ���ΦL�X���G 

    result = NULL ;

  } // else

  return result ;

} // Define()

Tree Define2( Tree tree ) { // define function

  Tree result ;
  Tree temp = tree->left ; // tree->left is ( sym ... )
  Tree temp2 = tree->right ; // tree->right is ( S-exp ... )

  int indexOfSym = 0 ; // �p�G�w�g�Q�w�q�A�O�U��m
  int indexOfFun = 0 ;
  bool isboundSym = false ; // �ݳo��symbol�O���O�w�g�Q�w�q��S-exp 
  bool isboundFun = false ; // �ݳo��symbol�O���O�w�g�Q�w�q��function 

  while ( indexOfSym < uSymbolTable.size() && ! isboundSym ) { // check if is already bounded to s-exp

    if ( strcmp( temp->left->token.name.c_str(), uSymbolTable[indexOfSym].name.c_str() ) == 0 )
      isboundSym = true ;

    indexOfSym++ ;
  } // while

  while ( indexOfFun < uFunctionTable.size() && ! isboundFun ) { // check if is already bounded to funct

    if ( strcmp( temp->left->token.name.c_str(), uFunctionTable[indexOfFun].name.c_str() ) == 0 )
      isboundFun = true ;

    indexOfFun++ ;
  } // while

  if ( isboundSym ) { // ���Q�w�q��s-exp�A���O�n���s�w�q��function

    // �ҥH�n�R���bsymbol table���w�q
    uSymbolTable.erase( uSymbolTable.begin() + indexOfSym - 1 ) ;

  } // if()

  if ( isboundFun ) { // already bounded to a function

    uFunctionTable[indexOfFun-1].arg = temp->right ;
    uFunctionTable[indexOfFun-1].binding = temp2 ;

  } // if()

  else {

    FunctionTable t ;
    t.name = temp->left->token.name ;
    t.arg = temp->right ;
    t.binding = temp2 ;

    uFunctionTable.push_back( t ) ;

  } // else()

  /*
  cout << "TEST: " << uFunctionTable.back().name << endl ;
  PrintExp( uFunctionTable.back().arg ) ;
  PrintExp( uFunctionTable.back().binding ) ;
  */

  if ( uVerbose ) {

    Token tempToken ;
    CreateToken( tempToken, temp->left->token.name + " defined", 0, 0, SYMBOL ) ;
    CreateNode( result, tempToken ) ;

  } // if()

  else { // ���ΦL�X���G 

    result = NULL ;

  } // else

  return result ;
} // Define2()

Tree Cond( Tree tree ) {

  Tree result ;
  Tree temp = tree->right ;

  while ( temp->right->left != NULL && temp->right->right != NULL ) { // �}�l�� �̫� sn�W�۳B�z 

    if ( CondOk( temp->left, result ) ) { // ��true��ܤw�g���� 

      return result ;

    } // if()

    else
      temp = temp->right ; // �U�@��

  } // while()

  temp = temp->left ; // �̫�@�� 

  // ��o�̪�ܳѳ̫�@��doubleton�A�p�G�o�Ӥ]�S��result�A�NError 
  if ( temp->left->left == NULL && temp->left->right == NULL &&
       strcmp( temp->left->token.name.c_str(), "else" ) == 0 ) { // �ˬd���S���S��symbol(KeyWord)

    // �o�{keyword
    // eval last arg
    temp = temp->right ;

    while ( temp->right->left != NULL && temp->right->right != NULL ) { // go to last arg

      result = EvalSexp( temp->left, 1 ) ;

      temp = temp->right ;
    } // while()

    result = EvalSexp( temp->left, 1 ) ; // get evaluate result
    // if ( result == NULL ) cout << "rvgerhui" ;
    return result ;

  } // if()

  else { // ���`��condok()

    if ( CondOk( temp, result ) ) { // ��true��ܤw�g���� 

      return result ;

    } // if()

    else // �٬O�S�� = no return value error �ᵹ�W�h�h�B�z 
      return NULL ; 

  } // else

} // Cond()

Tree If( Tree tree ) {

  Tree first ; // ��Ĥ@�Ӫ�eval���G 
  Tree result ; // �ݲĤ@�ӬOtrue or false�A�M�wresult 
  Tree temp = tree->right ; // �]���Ƕi�Ӫ��Omain s-exp

  first = EvalSexp( temp->left, 1 ) ;

  if ( first == NULL ) throw new EvalError( 12, "error", temp->left ) ; // error un bound condition

  temp = temp->right ; // go to s2

  if ( first->token.type != NIL ) { // eval s2

    result = EvalSexp( temp->left, 1 ) ;

    return result ;

  } // if()

  else { // eval s3 �p�G��

    temp = temp->right ; // go to s3
 
    if ( temp->left == NULL && temp->right == NULL ) { // NO return value

      result = NULL ;
      return result ;
    } // if()
 
    else {

      result = EvalSexp( temp->left, 1 ) ;

      // Clear( first ) ; // ���׺�X�ӤF �N���ΤF 
      return result ;

    } // else()
 
  } // else

} // If()

Tree And( Tree tree ) {
  // 'and' either returns the evaluated result of
  // the first one that is evaluated to nil
  // or the evaluated result of the last one
  Tree temp = tree ;
  Tree first ; // result of first arg
  Tree result ;  

  bool done = false ;

  while ( temp->right->left != NULL && temp->right->right != NULL && ! done ) {

    first = EvalSexp( temp->left, 1 ) ; // eval s1

    if ( first == NULL ) throw new EvalError( 13, "error", temp->left ) ; // error

    if ( first->left == NULL && first->right == NULL && first->token.type == NIL ) {

      done = true ;
      result = uNilNode ; 
    } // if()

    temp = temp->right ;
  } // while()

  if ( ! done ) // eval last one
    result = EvalSexp( temp->left, 1 ) ;

  return result ;

} // And()

Tree Or( Tree tree ) {
  // 'or' either returns the evaluated result of
  // the first one that is not evaluated to nil
  // or the evaluated result of the last one
  Tree temp = tree ;
  Tree first ; // result of first arg
  Tree result ;

  bool done = false ;

  while ( temp->right->left != NULL && temp->right->right != NULL && ! done ) {

    first = EvalSexp( temp->left, 1 ) ; // eval s1

    if ( first == NULL ) throw new EvalError( 13, "error", temp->left ) ; // error

    if ( first->token.type != NIL ) {

      done = true ;
      result = first ; 
    } // if()

    temp = temp->right ;
  } // while()

  if ( ! done ) // eval last one
    result = EvalSexp( temp->left, 1 ) ;

  return result ;

} // Or()

Tree Cons( Tree tree ) {

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  // cons �ؤ@�Ӿ�A���`�I�Os1, �k�`�I�Os2 
  result->left = apply->left ; // s1
  result->right = apply->right->left ;

  apply->left = NULL ;
  apply->right->left = NULL ;

  // Clear( apply ) ; // ���׺�X�ӤF �N���ΤF
  return result ;

} // Cons()

Tree List( Tree tree ) {

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  return apply ;

} // List()

Tree Quote( Tree tree ) {

  Tree result = tree->left ;

  result->token.type = NOPROCEDURE ;

  return result ;

} // Quote()

Tree Car( Tree tree ) {
  // the "left part" of a dotted pair

  Tree apply = EvalSexp( tree->left, 1 ) ; // get s1 �]��car��arg�u��1�� 

  if ( apply->left == NULL && apply->right == NULL ) // error incorrect type
    throw new EvalError( 7, "car", apply ) ;

  else {

    return apply->left ;

  } // else()

} // Car()

Tree Cdr( Tree tree ) {
  // the "right part" of a dotted pair

  Tree apply = EvalSexp( tree->left, 1 ) ; // get s1 �]��car��arg�u��1�� 

  if ( apply->left == NULL && apply->right == NULL ) { // error incorrect type

    throw new EvalError( 7, "cdr", apply ) ;

  } // if()

  else {
 
    return apply->right ;

  } // else()

} // Cdr()

Tree IsAtom( Tree tree ) {

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( apply->left == NULL && apply->right == NULL )
    return uTNode ;
  else
    return uNilNode ;

} // IsAtom()

Tree IsPair( Tree tree ) {

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( apply->left == NULL && apply->right == NULL ) 
    return uNilNode ;
  else
    return uTNode ;

} // IsPair()

Tree IsList( Tree tree ) {

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( IsAList( apply ) ) 
    return uTNode ;
  else
    return uNilNode ;

} // IsList()

Tree IsNull( Tree tree ) {

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( apply->left == NULL && apply->right == NULL && apply->token.type == NIL ) 
    return uTNode ;
  else
    return uNilNode ;

} // IsNull()

Tree IsInt( Tree tree ) { // integer

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( apply->left == NULL && apply->right == NULL && apply->token.type == INT ) 
    return uTNode ;
  else
    return uNilNode ;

} // IsInt()

Tree IsReal( Tree tree ) {

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( apply->left == NULL && apply->right == NULL && apply->token.type == INT ) 
    return uTNode ;
  else if ( apply->left == NULL && apply->right == NULL && apply->token.type == FLOAT )
    return uTNode ;
  else
    return uNilNode ;

} // IsReal()

Tree IsNum( Tree tree ) { // number

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( apply->left == NULL && apply->right == NULL && apply->token.type == INT ) 
    return uTNode ;
  else if ( apply->left == NULL && apply->right == NULL && apply->token.type == FLOAT )
    return uTNode ;
  else
    return uNilNode ;

} // IsNum()

Tree IsString( Tree tree ) {

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( apply->left == NULL && apply->right == NULL && apply->token.type == STRING ) 
    return uTNode ;
  else
    return uNilNode ;

} // IsString()

Tree IsBool( Tree tree ) { // boolean

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( apply->left == NULL && apply->right == NULL && apply->token.type == T ) 
    return uTNode ;
  else if ( apply->left == NULL && apply->right == NULL && apply->token.type == NIL ) 
    return uTNode ;
  else
    return uNilNode ;

} // IsBool()

Tree IsSym( Tree tree ) { // Symbol

  Tree apply = EvalSexp( tree->left, 1 ) ;

  if ( apply->left == NULL && apply->right == NULL && apply->token.type == SYMBOL ) {

    if ( ! IsInternalFunction( apply->token.name ) )
      return uTNode ;
    else
      return uNilNode ;

  } // if()
    
  else
    return uNilNode ;

} // IsSym()

Tree Add( Tree tree ) { // '+'

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == INT )
        ; // type ok
      else if ( currentA->left->token.type == FLOAT )
        ; // type ok
      else // type error
        throw new EvalError( 7, "+", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "+", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  // ��evaluate result �[�_�� 
  float ans = 0 ;
  bool flodiv = false ; // �T�{�O��ư��k�άO���B�I�� 
  Tree head_of_apply = apply ;

  while ( apply->left != NULL && apply->right != NULL ) {

    if ( apply->left->token.type == FLOAT ) // �T�{�O��ư��k�άO���B�I��
      flodiv = true ;

    ans = ans + atof( apply->left->token.name.c_str() ) ;

    apply = apply->right ;
  } // while

  // ��reuslt�˥X�� 
  string sum ;   
  
  if ( flodiv ) { // �T�{���G�O�_����ƩίB�I��

    NumToStr( ans, sum ) ;
    CreateToken( result->token, sum, 0, 0, FLOAT ) ;
  } // if()

  else { // �O��ƹB�� 

    NumToStr( ans, sum ) ;
    CreateToken( result->token, sum, 0, 0, INT ) ;
  } // else()

  // Clear( head_of_apply ) ; // ���׺�X�ӤF �N���ΤF

  return result ;

} // Add()

Tree Minus( Tree tree ) { // '-'

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == INT )
        ; // type ok
      else if ( currentA->left->token.type == FLOAT )
        ; // type ok
      else // type error
        throw new EvalError( 7, "-", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "-", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  // ��evaluate result �[�_�� 
  float ans = 0 ;
  bool flodiv = false ; // �T�{�O��ư��k�άO���B�I�� 
  Tree head_of_apply = apply ;

  if ( apply->left->token.type == FLOAT ) // �T�{�O��ư��k�άO���B�I��
    flodiv = true ;

  ans = atof( apply->left->token.name.c_str() ) ; // ����@�ө�i�h 
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL ) {

    if ( apply->left->token.type == FLOAT ) // �T�{�O��ư��k�άO���B�I��
      flodiv = true ;

    ans = ans - atof( apply->left->token.name.c_str() ) ;

    apply = apply->right ;
  } // while

  // ��reuslt�˥X�� 
  string sum ;   
  
  if ( flodiv ) { // �T�{���G�O�_����ƩίB�I��

    NumToStr( ans, sum ) ;
    CreateToken( result->token, sum, 0, 0, FLOAT ) ;
  } // if()

  else { // �O��ƹB�� 

    NumToStr( ans, sum ) ;
    CreateToken( result->token, sum, 0, 0, INT ) ;
  } // else()

  // Clear( head_of_apply ) ; // ���׺�X�ӤF �N���ΤF 

  return result ;

} // Minus()

Tree Mult( Tree tree ) { // '*'

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == INT )
        ; // type ok
      else if ( currentA->left->token.type == FLOAT )
        ; // type ok
      else // type error
        throw new EvalError( 7, "*", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "*", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  // ��evaluate result �[�_�� 
  float ans = 0 ;
  bool flodiv = false ; // �T�{�O��ư��k�άO���B�I�� 
  Tree head_of_apply = apply ;

  if ( apply->left->token.type == FLOAT ) // �T�{�O��ư��k�άO���B�I��
    flodiv = true ;

  ans = atof( apply->left->token.name.c_str() ) ; // ����@�ө�i�h 
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL ) {

    if ( apply->left->token.type == FLOAT ) // �T�{�O��ư��k�άO���B�I��
      flodiv = true ;

    ans = ans * atof( apply->left->token.name.c_str() ) ;

    apply = apply->right ;
  } // while

  // ��reuslt�˥X�� 
  string sum ;   
  
  if ( flodiv ) { // �T�{���G�O�_����ƩίB�I��

    NumToStr( ans, sum ) ;
    CreateToken( result->token, sum, 0, 0, FLOAT ) ;
  } // if()

  else { // �O��ƹB�� 

    NumToStr( ans, sum ) ;
    CreateToken( result->token, sum, 0, 0, INT ) ;
  } // else()

  // Clear( head_of_apply ) ; // ���׺�X�ӤF �N���ΤF  

  return result ; 

} // Mult()

Tree Divide( Tree tree ) { // '/'

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  // PrintTree( currentS ) ;

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == INT )
        ; // type ok
      else if ( currentA->left->token.type == FLOAT )
        ; // type ok
      else // type error
        throw new EvalError( 7, "/", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "/", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  // ��evaluate result �[�_�� 
  float ans = 0 ; 
  float divider = 0 ; // ���� �Ψ��ˬd���S�����H�s�����p
  bool flodiv = false ; // �T�{�O��ư��k�άO���B�I��
  Tree head_of_apply = apply ;

  if ( apply->left->token.type == FLOAT ) // �T�{�O��ư��k�άO���B�I��
    flodiv = true ;

  ans = atof( apply->left->token.name.c_str() ) ; // ����@�ө�i�h
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL ) {

    if ( apply->left->token.type == FLOAT ) // �T�{�O��ư��k�άO���B�I��
      flodiv = true ;

    divider = atof( apply->left->token.name.c_str() ) ;
    if ( divider == 0 )
      throw new EvalError( 8, "/", tree ) ;
    else
      ans = ans / divider ;

    apply = apply->right ;
  } // while

  // ��reuslt�˥X�� 
  string sum ;
  int ansInt = ans ; // ��ƪ��� 

  if ( flodiv ) { // �T�{���G�O�_����ƩίB�I��

    NumToStr( ans, sum ) ;
    CreateToken( result->token, sum, 0, 0, FLOAT ) ;
  } // if()

  else { // �O��ƹB�� 

    NumToStr( ansInt, sum ) ;
    CreateToken( result->token, sum, 0, 0, INT ) ;
  } // else()

  // Clear( head_of_apply ) ; // ���׺�X�ӤF �N���ΤF  

  return result ;

} // Divide()

Tree Not( Tree tree ) {

  Tree apply = EvalSexp( tree->left, 1 ) ;
  Tree result ;

  if ( apply == NULL ) throw new EvalError( 11, "error", tree->left ) ;

  if ( apply->left == NULL && apply->right == NULL && apply->token.type == NIL )
    result = uTNode ;
  else
    result = uNilNode ;

  // Clear( apply ) ; // ���׺�X�ӤF �N���ΤF 

  return result ;

} // Not()

Tree Big( Tree tree ) { // '>'

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == INT )
        ; // type ok
      else if ( currentA->left->token.type == FLOAT )
        ; // type ok
      else // type error
        throw new EvalError( 7, ">", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, ">", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  float s1 = 0, s2 = 0 ;
  bool yes = true ;
  Tree head_of_apply = apply ;

  s1 = atof( apply->left->token.name.c_str() ) ; // ����@�ө�i�h 
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL && yes ) {

    s2 = atof( apply->left->token.name.c_str() ) ;

    if ( s1 > s2 ) // ����u�n����᭱�N���Τ�F 
      ;
    else
      yes = false ;

    s1 = s2 ; // ���U�@�Ӱ���� 
    apply = apply->right ;
  } // while

  if ( yes ) 
    result = uTNode ;
  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // Big()

Tree BigE( Tree tree ) { // '>='

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == INT )
        ; // type ok
      else if ( currentA->left->token.type == FLOAT )
        ; // type ok
      else // type error
        throw new EvalError( 7, ">=", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, ">=", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  float s1 = 0, s2 = 0 ;
  bool yes = true ;
  Tree head_of_apply = apply ;

  s1 = atof( apply->left->token.name.c_str() ) ; // ����@�ө�i�h 
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL && yes ) {

    s2 = atof( apply->left->token.name.c_str() ) ;

    if ( s1 >= s2 ) // ����u�n����᭱�N���Τ�F 
      ;
    else
      yes = false ;

    s1 = s2 ; // ���U�@�Ӱ���� 
    apply = apply->right ;
  } // while

  if ( yes ) 
    result = uTNode ;
  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // BigE()

Tree Small( Tree tree ) { // '<'

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == INT )
        ; // type ok
      else if ( currentA->left->token.type == FLOAT )
        ; // type ok
      else // type error
        throw new EvalError( 7, "<", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "<", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  float s1 = 0, s2 = 0 ;
  bool yes = true ;
  Tree head_of_apply = apply ;

  s1 = atof( apply->left->token.name.c_str() ) ; // ����@�ө�i�h 
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL && yes ) {

    s2 = atof( apply->left->token.name.c_str() ) ;

    if ( s1 < s2 ) // ����u�n����᭱�N���Τ�F 
      ;
    else
      yes = false ;

    s1 = s2 ; // ���U�@�Ӱ���� 
    apply = apply->right ;
  } // while

  if ( yes ) 
    result = uTNode ;
  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // Small()

Tree SmallE( Tree tree ) { // '<='

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == INT )
        ; // type ok
      else if ( currentA->left->token.type == FLOAT )
        ; // type ok
      else // type error
        throw new EvalError( 7, "<=", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "<=", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  float s1 = 0, s2 = 0 ;
  bool yes = true ;
  Tree head_of_apply = apply ;

  s1 = atof( apply->left->token.name.c_str() ) ; // ����@�ө�i�h 
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL && yes ) {

    s2 = atof( apply->left->token.name.c_str() ) ;

    if ( s1 <= s2 ) // ����u�n����᭱�N���Τ�F 
      ;
    else
      yes = false ;

    s1 = s2 ; // ���U�@�Ӱ���� 
    apply = apply->right ;
  } // while

  if ( yes ) 
    result = uTNode ;
  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // SmallE()

Tree Equals( Tree tree ) { // '='

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == INT )
        ; // type ok
      else if ( currentA->left->token.type == FLOAT )
        ; // type ok
      else // type error
        throw new EvalError( 7, "=", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "=", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  float s1 = 0, s2 = 0 ;
  bool yes = true ;
  Tree head_of_apply = apply ;

  s1 = atof( apply->left->token.name.c_str() ) ; // ����@�ө�i�h 
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL && yes ) {

    s2 = atof( apply->left->token.name.c_str() ) ;

    if ( s1 == s2 ) // ����u�n����᭱�N���Τ�F 
      ;
    else
      yes = false ;

    s1 = s2 ; // ���U�@�Ӱ���� 
    apply = apply->right ;
  } // while

  if ( yes ) 
    result = uTNode ;
  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // Equals()

Tree Str_Append( Tree tree ) {

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == STRING )
        ; // type ok
      else // type error
        throw new EvalError( 7, "string-append", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "string-append", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  // �}�l��str
  int index = 1 ;
  char * str = new char[150] ;
  str[0] = '\"' ;
  string temp ;
  Tree head_of_apply = apply ;

  while ( apply->left != NULL && apply->right != NULL ) {

    temp = apply->left->token.name ;

    for ( int i = 1 ; i < temp.size() - 1 ; i++ ) {

      str[index] = temp[i] ;
      index++ ;
    } // for()

    apply = apply->right ;

  } // while

  str[index] = '\"' ;
  str[index+1] = '\0' ;

  CreateToken( result->token, str, 0, 0, STRING ) ;

  // Clear( head_of_apply ) ;

  return result ;

} // Str_Append()

Tree Str_Big( Tree tree ) { // string>?

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == STRING )
        ; // type ok
      else // type error
        throw new EvalError( 7, "string>?", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "string>?", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  // �}�l���str
  string str1, str2 ;
  bool yes = true ;
  Tree head_of_apply = apply ;

  str1 = apply->left->token.name ;
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL && yes ) {

    str2 = apply->left->token.name ;

    if ( strcmp( str1.c_str(), str2.c_str() ) > 0 ) // str1 > str2
      ; // true
    else
      yes = false ;

    apply = apply->right ;
    str1 = str2 ;
  } // while

  if ( yes ) 
    result = uTNode ;
  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // Str_Big()

Tree Str_Small( Tree tree ) { // string<?

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == STRING )
        ; // type ok
      else // type error
        throw new EvalError( 7, "string<?", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "string<?", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  // �}�l���str
  string str1, str2 ;
  bool yes = true ;
  Tree head_of_apply = apply ;

  str1 = apply->left->token.name ;
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL && yes ) {

    str2 = apply->left->token.name ;

    if ( strcmp( str1.c_str(), str2.c_str() ) < 0 ) // str1 > str2
      ; // true
    else
      yes = false ;

    apply = apply->right ;
    str1 = str2 ;
  } // while

  if ( yes ) 
    result = uTNode ;
  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // Str_Small()

Tree Str_Equ( Tree tree ) { // string=?

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    if ( currentA->left->left == NULL && currentA->left->right == NULL ) {

      if ( currentA->left->token.type == STRING )
        ; // type ok
      else // type error
        throw new EvalError( 7, "string=?", currentA->left ) ;

    } // if()

    else // type error
      throw new EvalError( 7, "string=?", currentA->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  // �}�l���str
  string str1, str2 ;
  bool yes = true ;
  Tree head_of_apply = apply ;

  str1 = apply->left->token.name ;
  apply = apply->right ;

  while ( apply->left != NULL && apply->right != NULL && yes ) {

    str2 = apply->left->token.name ;

    if ( strcmp( str1.c_str(), str2.c_str() ) == 0 ) // str1 > str2
      ; // true
    else
      yes = false ;

    apply = apply->right ;
    str1 = str2 ;
  } // while

  if ( yes ) 
    result = uTNode ;
  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // Str_Equ()

Tree Eqv( Tree tree ) {

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  Tree s1, s2 ;
  Tree head_of_apply = apply ;

  s1 = apply->left ; // first arg

  s2 = apply->right->left ; // second arg

  if ( s1->left == NULL && s1->right == NULL && s1->token.type != STRING ) { // s1 is atom

    if ( s2->left == NULL && s2->right == NULL && s2->token.type != STRING ) { // and s2 is atom

      if ( strcmp( s1->token.name.c_str(), s2->token.name.c_str() ) == 0 )
        result = uTNode ;
      else
        result = uNilNode ;

    } // if()

    else // but s2 is not atom
      result = uNilNode ;

  } // if()

  else if ( s1 == s2 ) // if both occupy the same memory space
    result = uTNode ;

  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // Eqv()

Tree Equal( Tree tree ) { // equal?

  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentA->left == NULL ) throw new EvalError( 11, "error", currentS->left ) ;

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  Tree s1, s2 ;
  Tree head_of_apply = apply ;

  s1 = apply->left ; // first arg

  s2 = apply->right->left ; // second arg

  if ( s1->left == NULL && s1->right == NULL ) { // s1 is atom

    if ( s2->left == NULL && s2->right == NULL ) { // and s2 is atom

      if ( strcmp( s1->token.name.c_str(), s2->token.name.c_str() ) == 0 )
        result = uTNode ;
      else
        result = uNilNode ;

    } // if()

    else // but s2 is not atom
      result = uNilNode ;

  } // if()

  else if ( Is_Two_Tree_Same( s1, s2 ) ) // if two tree is the same
    result = uTNode ;

  else
    result = uNilNode ;

  // Clear( head_of_apply ) ;

  return result ;

} // Equal()

Tree Begin( Tree tree ) {
  // ( begin s1 s2 s3 s4 ... sn ) 
  // �ݰ_�ӬO���o sn 
  Tree result = new Node() ; // result
  result->left = NULL ;
  result->right = NULL ;

  Tree apply = new Node() ; // s-exp after evaluate every arg
  apply->left = NULL ;
  apply->right = NULL ;

  Tree currentS = tree ; // main s-exp
  Tree currentA = apply ; // s-exp after evaluate every arg 

  while ( currentS->left != NULL && currentS->right != NULL ) { // ��s1 s2 .. evaluate�M���ˬdtype 

    currentA->left = EvalSexp( currentS->left, 1 ) ;

    if ( currentS->right->left == NULL && currentS->right->right == NULL ) {

      if ( currentA->left == NULL ) return NULL ;

    } // if()

    currentA->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    currentA->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    currentA->right->right = NULL ;

    currentA = currentA->right ;
    currentS = currentS->right ; // go to next arg of main s-exp
  } // while()

  currentA->token = uNilToken ; // �̫�

  while ( apply->right->left != NULL && apply->right->right != NULL ) {

    apply = apply->right ;
  } // while()

  return apply->left ;

} // Begin()

Tree Clean_Environment( Tree tree ) { // clean-environment

  Tree result ;
  Tree temp ;
  Tree temp1 ;

  for ( int i = 0 ; i > uSymbolTable.size()  ; i++ ) { // symbol Table

    temp = uSymbolTable[i - 1].binding ;
    Clear( temp ) ;

  } // for() 

  for ( int i = 0 ; i > uFunctionTable.size()  ; i++ ) { // Function Table

    temp1 = uFunctionTable[i - 1].arg ;
    temp = uFunctionTable[i - 1].binding ;
    Clear( temp1 ) ;
    Clear( temp ) ;

  } // for() 

  for ( int i = 0 ; i > uLocalTable.size()  ; i++ ) { // local Table�A�z�פW���|���F��A���٬O�O�I�@�U 

    temp = uLocalTable[i - 1].binding ;
    Clear( temp ) ;

  } // for() 

  uSymbolTable.clear() ;
  uFunctionTable.clear() ;
  uLocalTable.clear() ;

  if ( uVerbose ) {
    Token tempToken ;
    CreateToken( tempToken, "environment cleaned", 0, 0, SYMBOL ) ;
    CreateNode( result, tempToken ) ;
  } // if()

  else { // ���ΦL�X

    result = NULL ; 
 
  } // else()

  return result ;

} // Clean_Environment()

Tree Verbose( Tree tree ) { // verbose

  Tree temp = tree->left ;

  if ( temp->left == NULL && temp->right == NULL && temp->token.type == NIL ) {

    uVerbose = false ;
    return uNilNode ;
  } // if()

  else {

    uVerbose = true ;
    return uTNode ;
  } // else

} // Verbose()

Tree VerboseQ() { // verbose?

  if ( uVerbose )
    return uTNode ;
  else
    return uNilNode ;

} // VerboseQ()

void Local_Symbol_Define( Tree tree ) {
  // �Ƕi�Ӫ������ӭn�O pair = ( SYMBOL S-expression )

  SymbolTable temp ;
  Tree pairs = tree ;

  temp.name = pairs->left->token.name ; // SYMBOL
  pairs = pairs->right ;
  temp.binding = EvalSexp( pairs->left, 1 ) ; // S-exp

  if ( temp.binding == NULL ) throw new EvalError( 6, "error", pairs->left ) ; // error

  uLocalTable.push_back( temp ) ;

} // Local_Symbol_Define()

Tree Let( Tree tree ) {

  Tree result ;
  int arg = Get_Num_Of_Arg( tree->left ) ;

  // ���� local symbol ��define 
  Tree left = tree->left ; 
  while ( left->left != NULL && left->right != NULL ) {

    Local_Symbol_Define( left->left ) ;
    left = left->right ;
  } // while()

  uArg = uArg + arg ;
  
  // TEST Code
//  cout << ": Test Local Table:" << endl ;
//  for ( int i = 0 ; i < uLocalTable.size() ; i++ ) {
//    cout << "name: " << uLocalTable[i].name << endl ;
//    cout << "binding: " ; PrintExp( uLocalTable[i].binding ) ; cout << endl ;
//  } // for()
//  cout << "Arg: " << uArg << endl ;

  uArg = uArg + arg ;
  // �M��eval�᭱��S-exp 
  Tree right = tree->right ;
  while ( right->left != NULL && right->right != NULL ) {

    result = EvalSexp( right->left, 1 ) ;
    right = right->right ;
  } // while()

  for ( int i = 0 ; i < arg ; i++ )
    uLocalTable.pop_back() ;

  return result ;

} // Let()

Tree Lambda( Tree tree ) { // undo

  Tree result = new Node() ;

  result->left = tree->left ;
  result->right = tree->right ; 

  CreateToken( result->token, "lambda", 0, 0, SYMBOL ) ;

  return result ;

} // Lambda()

void Exit( Tree tree ) {

  throw new EvalError( 9, "exit", tree ) ;

} // Exit()

Tree CallFunction( string name,  Tree tree ) { // call internal function

  Tree result = NULL ;

  if ( strcmp( name.c_str(), "cons" ) == 0 )
    result = Cons( tree ) ;
  else if ( strcmp( name.c_str(), "list" ) == 0 )
    result = List( tree ) ;
  else if ( strcmp( name.c_str(), "quote" ) == 0 )
    result = Quote( tree ) ;
  else if ( strcmp( name.c_str(), "car" ) == 0 )
    result = Car( tree ) ;
  else if ( strcmp( name.c_str(), "cdr" ) == 0 )
    result = Cdr( tree ) ;
  else if ( strcmp( name.c_str(), "atom?" ) == 0 )
    result = IsAtom( tree ) ;
  else if ( strcmp( name.c_str(), "pair?" ) == 0 )
    result = IsPair( tree ) ;
  else if ( strcmp( name.c_str(), "list?" ) == 0 )
    result = IsList( tree ) ;
  else if ( strcmp( name.c_str(), "null?" ) == 0 )
    result = IsNull( tree ) ;
  else if ( strcmp( name.c_str(), "integer?" ) == 0 )
    result = IsInt( tree ) ;
  else if ( strcmp( name.c_str(), "real?" ) == 0 )
    result = IsReal( tree ) ;
  else if ( strcmp( name.c_str(), "number?" ) == 0 )
    result = IsNum( tree ) ;
  else if ( strcmp( name.c_str(), "string?" ) == 0 )
    result = IsString( tree ) ;
  else if ( strcmp( name.c_str(), "boolean?" ) == 0 )
    result = IsBool( tree ) ;
  else if ( strcmp( name.c_str(), "symbol?" ) == 0 )
    result = IsSym( tree ) ;
  else if ( strcmp( name.c_str(), "+" ) == 0 )
    result = Add( tree ) ;
  else if ( strcmp( name.c_str(), "-" ) == 0 )
    result = Minus( tree ) ;
  else if ( strcmp( name.c_str(), "*" ) == 0 )
    result = Mult( tree ) ;
  else if ( strcmp( name.c_str(), "/" ) == 0 )
    result = Divide( tree ) ;
  else if ( strcmp( name.c_str(), "not" ) == 0 )
    result = Not( tree ) ;
  else if ( strcmp( name.c_str(), ">" ) == 0 )
    result = Big( tree ) ;
  else if ( strcmp( name.c_str(), ">=" ) == 0 )
    result = BigE( tree ) ;
  else if ( strcmp( name.c_str(), "<" ) == 0 )
    result = Small( tree ) ;
  else if ( strcmp( name.c_str(), "<=" ) == 0 )
    result = SmallE( tree ) ;
  else if ( strcmp( name.c_str(), "=" ) == 0 )
    result = Equals( tree ) ;
  else if ( strcmp( name.c_str(), "string-append" ) == 0 )
    result = Str_Append( tree ) ;
  else if ( strcmp( name.c_str(), "string>?" ) == 0 )
    result = Str_Big( tree ) ;
  else if ( strcmp( name.c_str(), "string<?" ) == 0 )
    result = Str_Small( tree ) ;
  else if ( strcmp( name.c_str(), "string=?" ) == 0 )
    result = Str_Equ( tree ) ;
  else if ( strcmp( name.c_str(), "eqv?" ) == 0 )
    result = Eqv( tree ) ;
  else if ( strcmp( name.c_str(), "equal?" ) == 0 )
    result = Equal( tree ) ;
  else if ( strcmp( name.c_str(), "begin" ) == 0 )
    result = Begin( tree ) ;
  else if ( strcmp( name.c_str(), "clean-environment" ) == 0 )
    result = Clean_Environment( tree ) ;
  else if ( strcmp( name.c_str(), "exit" ) == 0 )
    Exit( tree ) ;
  else if ( strcmp( name.c_str(), "verbose" ) == 0 )
    result = Verbose( tree ) ;
  else if ( strcmp( name.c_str(), "verbose?" ) == 0 )
    result = VerboseQ() ;

  return result ;

} // CallFunction()

Tree Call_Define_Function( string name, Tree tree ) { // call define function
  // name = function name
  // tree = parameter

  // Get define function �����e�A�o��]�n�q�᭱��A�N��call stack??
  int index = uFunctionTable.size() - 1 ;
  for ( bool done = false ; index >= 0 && ! done ; index-- ) { // get function

    if ( strcmp( name.c_str(), uFunctionTable[index].name.c_str() ) == 0 )
      done = true ;

  } // for() 

  Tree funct = uFunctionTable[index+1].binding ; // function���e
  Tree arg = uFunctionTable[index+1].arg ; // function parameter

  // check num or arg
  int arg_of_define = Get_Num_Of_Arg( arg ) ; // �W�w��arg�ƶq
  int arg_of_call = Get_Num_Of_Arg( tree ) ; // call function �����ƶq

  if ( arg_of_define != arg_of_call ) // incorrect num of arg
    throw new EvalError( 5, name.c_str(), tree ) ;

  // arg�S���D���ܡA���ܼƭȶ�ilocal table 
  // �p�Garg�ƶq�ۦP�A�N��𪺲`�����ӷ|�@�� 
  SymbolTable temp ;
  Tree parameter = new Node() ; // �ǤJ���Ѽ�(eval�L��) 
  parameter->left = NULL ;
  parameter->right = NULL ;
  Tree current = parameter ;
  Tree head_of_tree = tree ;

  while ( tree->left != NULL && tree->right != NULL ) { // ��eval�Ѽ� �H�K�W���Ĥ@�I�����D 

    current->left = EvalSexp( tree->left, 1 ) ;

    // no return value error ERROR (unbound parameter) : <code of the actual parameter

    if ( current->left == NULL ) throw new EvalError( 11, "error", tree->left ) ;

    current->right = new Node() ; // �ؤU�@�Ӹ`�I�A��U�@��evaluate result 
    current->right->left = NULL ; // �p�G�S���F�A�o�ӴN��nil token 
    current->right->right = NULL ;

    tree = tree->right ;
    current = current->right ;
  } // while()

  current->token = uNilToken ;

  while ( arg->left != NULL && arg->right != NULL ) {

    temp.name = arg->left->token.name ; // (f x y) �w�q���ѼƦW��
    temp.binding = parameter->left ; // (f 1 2) �Ǫ��Ѽƭ� �nEval�L�A�N��define�@�� 

    uLocalTable.push_back( temp ) ; 

    arg = arg->right ; 
    parameter = parameter->right ;
  } // while() 

  /*
  // TEST
  cout << name << ": Test Local Table:" << endl ;
  for ( int i = 0 ; i < uLocalTable.size() ; i++ ) {
    cout << "name: " << uLocalTable[i].name << endl ;
    cout << "binding: " ; PrintExp( uLocalTable[i].binding ) ; cout << endl ;
  } // for()

  cout << "Symbol Table" << endl ;
  for ( int i = 0 ; i < uSymbolTable.size() ; i++ ) {
    cout << "name: " << uSymbolTable[i].name << endl ;
    cout << "binding: " ; PrintExp( uSymbolTable[i].binding ) ; cout << endl ;
  } // for()
  */

  // �i�H�}�leval funct�F�Aresult = last eval
  Tree result ;
  while ( funct->left != NULL && funct->right != NULL ) {

    uArg = arg_of_define ; // �o��function�N�o��hlocal variable�A�o�˴N���|�N���Lfunction�� 
    result = EvalSexp( funct->left, 1 ) ;
    funct = funct->right ;
    uArg = arg_of_define ;
  } // while()
  

  // ���������LocalTable�R��
  for ( int i = 0 ; i < arg_of_define ; i++ )
    uLocalTable.pop_back() ;

  // cout << name << ": " ; PrintExp( result ) ;

  return result ;

} // Call_Define_Function()

Tree Call_Lambda( Tree body, Tree para, string name ) { // ����lambda function
  // name �O�ΨӧP�_�O���@��lambda: lambda or lambda expression

  FunctionTable temp ;

  temp.name = name ;
  temp.arg = body->left ;
  temp.binding = body->right ;

  uFunctionTable.push_back( temp ) ; // �Ȯ����L�i�Jfunction table 

  Tree result = Call_Define_Function( name, para ) ;

  uFunctionTable.pop_back() ; // ������R��

  return result ;
} // Call_Lambda()

// ===function===

Tree EvalSexp( Tree tree, int level ) { // Eval

  Tree result ;

  if ( tree->left == NULL && tree->right == NULL && tree->token.type != SYMBOL ) {
    // what is being evaluated is an atom but not a symbol

    result = new Node() ;
    result->left = NULL ;
    result->right = NULL ;

    // return that atom
    CreateToken( result->token, tree->token.name, tree->token.line, tree->token.column, tree->token.type ) ;
    return result ;

  } // if() what is being evaluated is an atom but not a symbol

  else if ( tree->left == NULL && tree->right == NULL && tree->token.type == SYMBOL ) {
    // what is being evaluated is a symbol
    // check whether it is bound to an S-expression or an internal function

    if ( Isbounded( tree->token.name ) || IsInternalFunction( tree->token.name ) || 
         IsboundF( tree->token.name ) ) {
      // return that S-expression or internal function (i.e., its binding)

      result = GetBinding( tree->token.name ) ;
      return result ;

    } // if()

    else { // ERROR (unbound symbol) : XXX

      throw new EvalError( 2, tree->token.name.c_str(), tree ) ;

    } // else() 

  } // else if() what is being evaluated is a symbol

  else { // what is being evaluated is (...)

    Tree temp = tree->left ;
    Tree bind ; // temp tree

    if ( ! IsAList( tree ) ) { // if (...) is not a (pure) list

      // error (non-list) : (...)�nPrettyPrint
      throw new EvalError( 1, "non-list", tree ) ;

    } // if() non lsit

    else if ( temp->left == NULL && temp->right == NULL && temp->token.type != SYMBOL ) {
      // else if first argument of (...) is an atom ��, which is not a symbol
      // error non-function 
      throw new EvalError( 3, temp->token.name.c_str(), temp ) ;

    } // else if() non function

    else if ( temp->left == NULL && temp->right == NULL && temp->token.type == SYMBOL ) {
      // else if first argument of (...) is a symbol SYM

      // SYM has a binding
      if ( ( Isbounded( temp->token.name ) || IsInternalFunction( temp->token.name ) || 
             IsboundF( temp->token.name ) ) && temp->token.type ) { 

        bind = GetBinding( temp->token.name ) ; // binding of first arg of (...)

        // and that binding is an internal function
        if ( IsInternalFunction( bind->token.name ) && bind->token.type == SYMBOL ) {

          // if the current level is not the top level, and SYM is 'clean-environment' or    
          // or 'define' or�@'exit' then error
          if ( level != 0 && ( ! strcmp( bind->token.name.c_str(), "clean-environment" ) ) ) {

            // error level of...
            throw new EvalError( 4, "CLEAN-ENVIRONMENT", tree ) ;

          } // if() level != 0...

          if ( level != 0 && ( ! strcmp( bind->token.name.c_str(), "define" ) ) ) {

            // error level of...
            throw new EvalError( 4, "DEFINE", tree ) ;

          } // if() level != 0...

          if ( level != 0 && ( ! strcmp( bind->token.name.c_str(), "exit" ) ) ) {

            // error level of...
            throw new EvalError( 4, "EXIT", tree ) ;

          } // if() level != 0...

          // if SYM is 'define' or 'set!' or 'let' or 'cond' or 'lambda'
          if ( ! strcmp( bind->token.name.c_str(), "define" ) ) {

            // check format, if no error then evaluate
            if ( Check_Define_Format( tree->right ) ) { // format 1

              // evaluate (...)
              result = Define( tree->right ) ;

              return result ;

            } // if()

            else if ( Check_Define_Format2( tree->right ) ) { // format 2

              // evaluate (...)
              result = Define2( tree->right ) ;

              return result ;

            } // if()

            else { // format error

              throw new EvalError( 0, "DEFINE", tree ) ;

            } // else

          } // if() define

          else if ( ! strcmp( bind->token.name.c_str(), "cond" ) ) {

            // check format, if no error then evaluate
            if ( Check_Cond_Format( tree->right ) ) {

              // evaluate (...)
              result = Cond( tree ) ;
              
              if ( result == NULL ) throw new EvalError( 6, "no-return-value", tree ) ; // 2025/6/7 updated
              else return result ;

            } // if()

            else { // format error

              throw new EvalError( 0, "COND", tree ) ;

            } // else

          } // if() cond

          else if ( ! strcmp( bind->token.name.c_str(), "let" ) ) {

            // check format, if no error then evaluate
            if ( Check_Let_Format( tree->right ) ) {

              // evaluate (...)
              result = Let( tree->right ) ;

              return result ;

            } // if()

            else { // format error

              throw new EvalError( 10, "let", tree ) ;

            } // else 

          } // else if()

          else if ( ! strcmp( bind->token.name.c_str(), "lambda" ) ) {

            // �N��n����lambda�w�q�X�Ӫ�function�A�o�ɭ�tree->right is arg 
            if ( bind->left != NULL && bind->right != NULL && ! 
                 strcmp( bind->token.name.c_str(), "lambda" ) ) {

              result = Call_Lambda( bind, tree->right, "lambda" ) ;

              if ( result == NULL && ! level ) throw new EvalError( 6, "error", tree ) ;

              return result ;
            } // if()

            // check format, if no error then evaluate
            else if ( Check_Lambda_Format( tree->right ) ) {

              // evaluate (...)
              result = Lambda( tree->right ) ;

              return result ;

            } // else if()

            else { // format error

              throw new EvalError( 10, "lambda", tree ) ;

            } // else

          } // else if()

          // if SYM is 'if' or 'and' or 'or'
          else if ( ! strcmp( bind->token.name.c_str(), "if" ) ) {

            // check num of arg, if no error then evaluate
            if ( Check_Num_Of_Arg( bind->token.name, tree->right ) ) {

              // evaluate
              result = If( tree ) ; // �o���main S-EXP���Fno return value(Print main S-EXP)

              if ( result == NULL && level == 0 ) throw new EvalError( 6, "error", tree ) ;

              return result ;
            } // if

            else { // ERROR (incorrect number of arguments) : XXX

              throw new EvalError( 5, temp->token.name.c_str(), tree ) ;

            } // else()

          } // else if() if

          else if ( ! strcmp( bind->token.name.c_str(), "and" ) ) {

            // check num of arg, if no error then evaluate
            if ( Check_Num_Of_Arg( bind->token.name, tree->right ) ) {

              // evaluate
              result = And( tree->right ) ;

              return result ;
            } // if

            else { // ERROR (incorrect number of arguments) : XXX

              throw new EvalError( 5, temp->token.name.c_str(), tree ) ;

            } // else()

          } // else if() and

          else if ( ! strcmp( bind->token.name.c_str(), "or" ) ) {

            // check num of arg, if no error then evaluate
            if ( Check_Num_Of_Arg( bind->token.name, tree->right ) ) {

              // evaluate
              result = Or( tree->right ) ;

              return result ;
            } // if

            else {

              // ERROR (incorrect number of arguments) : XXX
              throw new EvalError( 5, temp->token.name.c_str(), tree ) ;

            } // else()

          } // else if() or

          // SYM is a known function name 'abc', which is neither
          else { // 'define' nor 'let' nor 'cond' nor 'lambda'

            // check num of arg
            if ( Check_Num_Of_Arg( bind->token.name, tree->right ) ) {

              ; // �n�����ηF�� 

            } // if

            else { // ERROR (incorrect number of arguments) : XXX

              throw new EvalError( 5, temp->token.name.c_str(), tree ) ;

            } // else()

          } // else() SYM is a known function name 'abc'...

        } // if() // and that binding is an internal function

        else if ( IsboundF( bind->token.name ) ) { // is a user define function

          // call define function
          result = Call_Define_Function( bind->token.name, tree->right ) ;

          if ( result == NULL && level == 0 ) throw new EvalError( 6, "error", tree ) ;

          return result ;
        } // else if()

        else { // SYM is not the name of a known function

          // error non function
          throw new EvalError( 3, temp->token.name.c_str(), bind ) ;

        } // else()

      } // if()

      else { // SYM has no binding

        // error unbound symbol
        throw new EvalError( 2, temp->token.name.c_str(), tree ) ;

      } // else()

    } // else if else if first argument of (...) is a symbol SYM

    else { // the first argument of ( ... ) is ( �C�C�C ), i.e., it is ( ( �C�C�C ) ...... )

      bind = EvalSexp( tree->left, 1 ) ;

      if ( bind == NULL ) throw new EvalError( 6, "error", tree->left ) ;
      // check the result is an internal function
      else if ( IsInternalFunction( bind->token.name ) && bind->token.type == SYMBOL ) {

        // �N��n����lambda�w�q�X�Ӫ�function�A�o�ɭ�tree->right is arg 
        if ( bind->left != NULL && bind->right != NULL && ! strcmp( bind->token.name.c_str(), "lambda" ) ) {

          result = Call_Lambda( bind, tree->right, "lambda" ) ;

          if ( result == NULL && ! level ) throw new EvalError( 6, "error", tree ) ;
          return result ;
        } // if()

        if ( Check_Num_Of_Arg( bind->token.name, tree->right ) ) { // check num of arg

          ; // �n���S���n�F��

        } // if()

        else { // num-of-arguments is NOT correct

          throw new EvalError( 5, bind->token.name.c_str(), tree ) ;

        } // else()

      } // else if()

      else if ( IsboundF( bind->token.name ) ) { // is a user define function

        // call define function
        result = Call_Define_Function( bind->token.name, tree->right ) ;

        if ( result == NULL && ! level ) throw new EvalError( 6, "error", tree ) ;

        return result ;
      } // else if()

      else { // if the evaluated result is not an internal function

        // error nonfunction
        throw new EvalError( 3, bind->token.name.c_str(), bind ) ;

      } // else()

    } // else() the first argument of ( ... ) is ( �C�C�C )

    result = CallFunction( bind->token.name, tree->right ) ;

    if ( result == NULL && ! level ) throw new EvalError( 6, "error", tree ) ;

    return result ;

  } // else what is being evaluated is (...)

} // EvalSexp()

void Eval( Exp tokenList ) {

  Tree result = NULL ;
  Tree tree = NULL ;

  // if ( uTestNum == 1 ) exit( 0 ) ; // ���ե� 

  BuildTree( tokenList, tree ) ;
  // PrintList( tokenList ) ;
  // PrintExp( tree ) ;

  result = EvalSexp( tree, 0 ) ; // ��error�|����, 0 = top level
  // cout << "OK" << endl ;

  if ( result == NULL ) ; // error

  PrintExp( result ) ;

  // GetTableSize() ; // test

  uProcedure = true ;
} // Eval()

// ==============================EvalEnd============================================

// ==============================Main==================================================

int main() {

  scanf( "%d", &uTestNum ) ;
  // if ( uTestNum == 1 ) cout << "." << endl ;
  // �|�Ψ쪺
  CreateToken( uDotToken, ".", 0, 0, DOT ) ;
  CreateToken( uRpToken, ")", 0, 0, RP ) ;
  CreateToken( uLpToken, "(", 0, 0, LP ) ;
  CreateToken( uNilToken, "nil", 0, 0, NIL ) ;
  CreateToken( uTToken, "#t", 0, 0, T ) ;
  CreateNode( uNilNode, uNilToken ) ;
  CreateNode( uTNode, uTToken ) ;

  Exp tokenList ;

  uExit = false ;
  uEof = false ; // // eof�o�ʹN���Φb���F
  uError = false ;
  uVerbose = true ;
  uProcedure = false ;
  uArg = 0 ;

  cout << "Welcome to OurScheme!" << endl ;
  cout << endl ;
  cout << "> " ;

  while ( ! uExit && ! uEof ) {

    try {

      Parse( tokenList ) ; // ReadSExp(expr) ;

      if ( ! uExit )
        Eval( tokenList ) ;

    } // try

    catch( Error* e ) {
      ;
    } // catch()

    catch( EvalError* x ) {
      ;
    } // catch()

    catch( const exception& er ) {
      cout << "Error " << endl ;
    } // catch

    if ( ! uExit && ! uEof ) // �٨S����
      cout << endl << "> " ;

  } // while()

  cout << endl ;
  cout << "Thanks for using OurScheme!" ;

} // main()

/*
BBS�H�H����k: �n�JBBS��->���"�H����ò�"->"�����H�H"->��J"LogicExTA"->�N�i�H�}�l�g�H�F

���ҫH�c: �ӤH�u��->���U���->�q�l�H�c->��J�Ǯժ��H�c (@cycu.edu.tw)
*/
