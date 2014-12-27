#include<iostream>
#include<stack>
#include<string>
using namespace std;
#define _ NFA ::

//---------EdgeList--------------//
class EdgeList
{
	char t;
	int p,q;
	EdgeList *next;
    public:
	EdgeList(int a,char tr,int b);
	void setNext(EdgeList *n);
  	EdgeList* getNext();
        void print(char v);
};
EdgeList :: EdgeList(int a,char tr,int b)
{
	p=a;
	t=tr;
	q=b;
	next=0;
}
void EdgeList :: setNext(EdgeList *n)
{
	next=n;
}

EdgeList* EdgeList :: getNext(){
  		return next;
  	}
void EdgeList :: print(char v){
   	cout<<v<<p<<" : "<<t<<" : "<<v<<q<<endl;
 }
//-------Few Global Declaration ------//
int sn=0;
EdgeList *el = new EdgeList(sn,'@',sn+1);
EdgeList *root=el;
void setel(EdgeList *l){
	el->setNext(l);el=l;
}

//-----------NFA--------------------//

class NFA{
	int p,q;
  public:

  	NFA();
	NFA(int a,int b,char h,int flg);
	int getInit();
  	int getFinal();
};

_ NFA(){}
_ NFA(int a,int b,char h='@',int flg=0){
	p=a;
	q=b;
	if(flg){
		EdgeList *l=new EdgeList(a,h,b);
		el->setNext(l);
		el=l;
	}
}
int _ getInit(){
  	return p;
 }
int _ getFinal(){
	return q;
}

//---------------------------//
int priority(char v);
NFA concat(NFA a,NFA b);
NFA unionn(NFA a,NFA b);
NFA kleene(NFA a);
//--------------------------//

//---------MAIN-------//
int main()
{
 stack<char> opst;
 string infix,rpn;
 char c='a',t;
 getline(cin,infix); 
 cout<<"Infix : "<<infix<<endl;
//------------------INFIX TO RPN-------------------------//
 for(int i=0;i<infix.length();i++){
	c=infix[i];
	if(priority(c)>1)
	{
		if(opst.empty())
			opst.push(c);
		else
		{	
			while((!(priority(c) > priority(opst.top()))) || opst.empty()){
				t=opst.top();										
				opst.pop();
				rpn+=t;				
			}
			opst.push(c);
		}	
	}
	else if(priority(c) == 1)
	{
		if(c=='(')
			opst.push(')');
		else if (c==')')
		{
			while(opst.top() != ')'){
				t=opst.top();										
				opst.pop();
				rpn+=t;
			}
			opst.pop();
		}
		
	}
	else
	{
		rpn+=c;
	}

 }
 while(!(opst.empty())){
	t=opst.top();										
	opst.pop();
	rpn+=t;
}
 cout<<"RPN : "<<rpn<<endl;
//---------------RPN EVALUVATION------------------------//
	stack<NFA> s;
	NFA z,y,x;
	string str=rpn;
	for(int i=0;i<str.length();i++)
	{
		if(str[i] == '+'){
			z=s.top();
			s.pop();
			y=s.top();
			s.pop();
			x=unionn(y,z);
		}
		else if(str[i] == '.'){
			z=s.top();
			s.pop();
			y=s.top();
			s.pop();
			x=concat(y,z);
		}
		else if(str[i] == '*'){
			z=s.top();
			s.pop();
			x=kleene(z);
		}
		else
		{
			NFA u(sn,sn+1,str[i],1);
			sn+=2;
			s.push(u);
		}
	}	
//-------------Printing----------------------//
EdgeList *tmp=root->getNext();
	while(tmp){
		tmp->print('S');
		tmp=tmp->getNext();	
    }
    cout<<"Initial : "<<x.getInit()<<endl;
    cout<<"Final : "<<x.getFinal()<<endl;
    cout<<"Total Number of States : "<<sn+1<<endl;
//------------------------------------------//

return 0;
}

int priority(char v)
{
  if(v == '*')
	return 4;
  else if (v=='.')
	return 3;
  else if (v=='+')
	return 2;
  else if (v=='(' || v==')')
	return 1;
  else 
	return 0;
}

NFA concat(NFA a,NFA b){
	int x,y;
	EdgeList *l=new EdgeList(a.getFinal(),'@',b.getInit());
	setel(l);
	x=sn;
	EdgeList *m=new EdgeList(sn,'@',a.getInit());
	setel(m);
	sn++;
	y=sn;
	EdgeList *n=new EdgeList(b.getFinal(),'@',sn);
	setel(n);
	NFA c(x,y);
	return c;
}

NFA unionn(NFA a,NFA b){
	
	int x,y;
	
	x=sn;
	EdgeList *m=new EdgeList(sn,'@',a.getInit());
	setel(m);
	EdgeList *n=new EdgeList(sn,'@',b.getInit());
	setel(n);
	
	sn++;
	
	y=sn;
	EdgeList *k=new EdgeList(a.getFinal(),'@',sn);
	setel(k);
	EdgeList *l=new EdgeList(b.getFinal(),'@',sn);
	setel(l);
	
	NFA c(x,y);
	return c;
}

NFA kleene(NFA a){
	int x,y;
	
	x=sn;
	EdgeList *m=new EdgeList(sn,'@',a.getInit());
	setel(m);
 	
 	sn++;

	EdgeList *l=new EdgeList(a.getFinal(),'@',a.getInit());
	setel(l);
	
	y=sn;
	EdgeList *k=new EdgeList(a.getFinal(),'@',sn);
	setel(k);

	EdgeList *z=new EdgeList(x,'@',y);
	setel(z);
	
	NFA c(x,y);
	return c;
	
}
