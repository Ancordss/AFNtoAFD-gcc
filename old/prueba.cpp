#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <conio.h>


#define AFN_MAX 11
#define EPSILON -1
#define ACEPTA -2
#define STACKSIZE 100
#define MAXIMO 80


    enum bool {FALSE,TRUE};
            struct stack {
        int top;
        int items[STACKSIZE];
    };

typedef stack *STACKPTR;
bool empty(STACKPTR ps);
int pop(STACKPTR ps);
void push(STACKPTR ps,int x);
void init(STACKPTR ps);
void e_clausura(int* ,int,int* ,int &na);
void mover(int *,int,int* t,int &m,int c);
void print(int* t,int m,char c);
void copy(int*,int,int estadosD[][AFN_MAX],int &nest,int*);
bool no_marcado(int*,int,int estadosD[AFN_MAX][AFN_MAX],int nest);
void aceptacion();
void print_estadosD(int estadosD[][AFN_MAX],int nest,int nD[]);
// 0 1 2 3 4 5 6 7 8 9 10


int aristas[][AFN_MAX]=
{0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0,
 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0,
 0, 0, 0,'a', 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0,
 0, 0, 0, 0, 0,'b', 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0,
 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0,'a', 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0,'b', 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'b',
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2
};



    void main() // afnafd.cpp
        {
            int m,i,j,k;
            int t[AFN_MAX];
            int estadosD[AFN_MAX][AFN_MAX],nest=0;
            int nD[AFN_MAX];
            int a[AFN_MAX],na;
            char* alfabeto="ab";

        *t=0;m=1;

            cout << "estado : " << nest << endl;

            e_clausura(t,m,a,na);
            copy(a,na,estadosD,nest,nD);
            for(i=0;i<strlen(alfabeto);i++) {
            mover(a,na,t,m,alfabeto[i]);
            cout << "estado : " << nest << endl;
            print(t,m,alfabeto[i]);
            copy(t,m,estadosD,nest,nD);
            }
            for(k=1;k<nest;k++) {
            for(j=0;j<nD[k];j++)
            t[j]=estadosD[k][j];
            m=nD[k];cout << " K : " << k << endl;
            e_clausura(t,m,a,na);

            for(i=0;i<strlen(alfabeto);i++) {
            mover(a,na,t,m,alfabeto[i]);
            if(m) {
            if(no_marcado(t,m,estadosD,nest)) {
            cout << "estado : " << nest << endl;
            copy(t,m,estadosD,nest,nD);
            print(t,m,alfabeto[i]);
            }
            else
            print(t,m,alfabeto[i]);
        }
    }
}
aceptacion();
print_estadosD(estadosD,nest,nD);
}


void print_estadosD(int estadosD[][AFN_MAX],int nest,int nD[])
{
    register int i,j;
    
    cout << " AFD AFN " << endl;
cout << "---------------------------------------" << endl;
for(i=0;i<nest;i++) {
cout << setw(4) << i << " : ";
for(j=0;j<nD[i];j++)
cout << setw(4) << estadosD[i][j];
cout << endl;
}
cout << "---------------------------------------" << endl;
getch();
}
void aceptacion()
{
cout << "estados de aceptacion tienen los nodos : ";
 for(int i=0;i<AFN_MAX;i++)
if(aristas[i][i]==ACEPTA)
cout << setw(4) << i;
getch();
}
void e_clausura(int* s,int n,int* a,int &na)
{
int i,j,t,u;
STACKPTR p;
// meter todos los estados en pila
// inicializar cerradura a
init(p);
na=0;
for(i=0;i<n;i++) {
push(p,s[i]);
a[i]=s[i];
na++;
}
while(!empty(p)) {
t=pop(p);
 for(u=0;u<AFN_MAX;u++)
if(aristas[t][u]==EPSILON) {
i=0;
while(i<na && u!=a[i])
i++;
if(i==na) {
 // a¤adir u a cerradura
 a[na++]=u;
 // meter u a pila
 push(p,u);
}
}
}
cout << " T : " ;
for(j=0;j<na;j++)
cout << setw(4) << a[j];
cout << endl;
}
void print(int* t,int m,char c)
{
register int j;
 cout << " mover(T," << c << ") : ";
for(j=0;j<m;j++)
cout << setw(4) << t[j];
cout << endl;
}
void copy(int* t,int m,int estadosD[][AFN_MAX],int &nest,int* nD)
{
register int i;
for(i=0;i<m;i++)
estadosD[nest][i]=t[i];
nD[nest]=m;
++nest;
}
void mover(int* a,int na,int* t,int &m,int c)
{
int i,j,k;
m=0;
for(i=0;i<na;i++) {
k=a[i];
 for(j=0;j<AFN_MAX;j++)
if(aristas[k][j]==c)
t[m++]=j;
}
}
bool no_marcado(int* t,int m,int estadosD[][AFN_MAX],int nest)
{
int k=0,j,i;
for(k=0;k<nest;k++) {
i=0;
for(j=0;j<m;j++)
if(t[j]==estadosD[k][j])
i++;
if(i==m)
return FALSE;
}
return TRUE;
}
bool empty(STACKPTR ps)
{
if(ps->top==-1)
return TRUE;
else
return FALSE;
}
int pop(STACKPTR ps)
{
if(empty(ps)) {
cout << "\n pila vacia " << endl;
exit(1);
}
return (ps->items[ps->top--]);
}
void push(STACKPTR ps,int x)
{
if(ps->top==STACKSIZE-1) {
cout << "\n stack overflow" << endl;
exit(1);
}
else
ps->items[++(ps->top)]=x;
}
void init(STACKPTR ps)
{
ps->top=-1;
}