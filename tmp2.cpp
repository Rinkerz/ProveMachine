#include<iostream>
#include<sstream>
#include<vector>
#include<set>
#include<map>
#include<string>
#define ERR "ERROR"
#define NON "NONE"
#define BASIC_NUM 5

using namespace std;

typedef struct statement{
	char state;//0是元命题,1是否定,2是蕴含;
	int cnt;//几次mp能证得，公理为0,未能为-1
	string first;//前件
	string last;//后件
	friend string getStringof(const statement& p);
	statement()=default;
	statement(char state,int cnt,string f,string l):state(state),cnt(cnt),first(f),last(l){}
	statement(char state,int cnt,string f,statement l):state(state),cnt(cnt),first(f),last(getStringof(l)){}
	statement(char state,int cnt,statement f,string l):state(state),cnt(cnt),first(getStringof(f)),last(l){}
	statement(char state,int cnt,statement f,statement l):state(state),cnt(cnt),first(getStringof(f)),last(getStringof(l)){}
	bool operator<(const statement& p)const{return (this->first)>p.first||(this->last)>p.last;}
	friend ostream& operator<<(ostream& out,const statement& p);
}statement;

ostream& operator<<(ostream& out,const statement& p)
{
	if(p.state==0) out<<p.first;
	else if(p.state==1)
        if(p.first.length()!=1) out<<"~"<<"("<<p.first<<")";
        else out<<"~"<<p.first;
	else if(p.state==2) out<<"("<<p.first<<")->("<<p.last<<")";
	else out<<"p.state is error!!";
	return out;
}

string getStringof(const statement& p)//将命题转化为字符串
{
    if(p.state==0) return p.first;
	else if(p.state==1)
        if(p.first.length()!=1) return "~("+p.first+")";
        else return "~"+p.first;
	else if(p.state==2) return "("+p.first+")->("+p.last+")";
	else return "p.state is error!!";
}

set<statement> gamma;
map<statement,int> STAtoID;
map<string,int> STRtoID;
vector<statement> IDtoSTA;
vector<string> IDtoSTR;


int getIDof(statement s)
{
    if(STAtoID.count(s)) return STAtoID[s];
    IDtoSTA.push_back(s);
    IDtoSTR.push_back(getStringof(s));
    STRtoID[getStringof(s)]=IDtoSTR.size()-1;
    return STAtoID[s]=IDtoSTA.size()-1;
}

statement Not(statement s){statement q(1,-1,s,"");return q;}
statement Has(statement s1,statement s2){return statement(2,-1,s1,s2);}
statement L1(statement s1,statement s2){return statement(2,0,s1,Has(s2,s1));}
statement L2(statement s1,statement s2,statement s3){return statement(2,0,Has(s1,Has(s2,s3)),Has(Has(s1,s2),Has(s1,s3)));}
statement L3(statement s1,statement s2){return statement(2,0,Has(Not(s2),Not(s1)),Has(s1,s2));}
//statement MP(statement s1,statement s2){};

int main(void)
{
    for(int i=0;i<BASIC_NUM;i++){
        stringstream ss;ss<<i;
        gamma.insert(statement(0,-1,ss.str(),""));
    }

    for(auto i=gamma.cbegin();i!=gamma.cend();i++)
        cout<<*i<<endl;

    int t=1;
    while(t--)
    {
        auto b=gamma.cbegin();auto e=gamma.cend();
        for(auto i=b;i!=e;i++)
            for(auto j=b;j!=e;j++){
                statement s1=*i,s2=*j;
                if(i!=j){
                    gamma.insert(L1(s1,s2));
                    gamma.insert(L3(s1,s2));
                }
                for(auto k=b;k!=e;k++){
                    statement s3=*k;
                    if(j!=k)gamma.insert(L2(s1,s2,s3));
                }
            }
    }

    for(auto i=gamma.cbegin();i!=gamma.cend();i++)
        cout<<*i<<endl;

    cout<<gamma.size();
	return 0;
}

