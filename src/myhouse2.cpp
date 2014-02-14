// FILE: myhouse2.cpp begins

//  Compares solving the system a*x = b by Householder method 
//  to solving by SVDsolve and pseudoinverse. 

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;
#include "nr.h"
#include "matutl02.h"
#include "solve.h"

/* house overwrites a by its upper triangular Householder matrix
     See Heath p. 124 */

void house2(Mat_DP &a)
  /* Overwrites a by its upper triangular Householder matrix
   See Heath p. 124 */
{
  int m= a.nrows(), n= a.ncols();
  if ( (m <2) || (n <2) )
    {cout<<"Argument error in house2,m,n=" 
         <<m<<",  "<<n<<endl;
    abort();}
  Mat_DP v(m,1), vT(1,m), e(m,1);
  DP alf, beta, gam;
  for (int k=0;k<n;k++)
    {
      DP s=0.0;
      v=0.0; vT=0.0;
      for (int kk=0;kk<m;kk++) e[kk][0]=(kk==k)? 1: 0;
      for (int kk=k;kk<m;kk++)
	{ s+= a[kk][k]* a[kk][k];
	v[kk][0]=a[kk][k]; 
	}
      alf=(a[k][k]>0)? pow(s, 0.5): -pow(s, 0.5);
      v[k][0]=v[k][0]- alf;
      transp(v,vT); 
      s=0.0;
      for (int kk=0;kk<m;kk++) s+= v[kk][0]*v[0][kk];
      beta=s;
      if (beta>1e-14) /* if beta =0 go to next k */
	{
	  for (int j=k;j<n;j++)
	    {gam=0.0;
	    for (int jj=0;jj<m;jj++) gam+=v[jj][0]*a[jj][j];
	    for (int jj=0;jj<m;jj++) 
	      a[jj][j]= a[jj][j]-(2*gam/beta)*v[jj][0];
	    }
	}
    }
}

void house_vec(Mat_DP &a, int col_idx, Vec_DP &w )
  /* Returns as w for column col_idx the Householder
     vector v that  makes zero the subdiagonal entries
     of col_idx of a 
*/
{
  int m= a.nrows(), n= a.ncols();
  if ( (m <2) || (n <2) ||(m<n) || (col_idx>=n) )
    {cout<<"Argument error in house_vec,m,n, col_idx=" 
         <<m<<",  "<<n<< ", "<< col_idx<<endl;
    abort();}
  Mat_DP v(m,1), vT(1,m), e(m,1);
  DP alf, beta, gam;
  for (int k=col_idx;k<=col_idx;k++)
    {
      DP s=0.0;
      v=0.0; vT=0.0;
      for (int kk=0;kk<m;kk++) e[kk][0]=(kk==k)? 1: 0;
      for (int kk=k;kk<m;kk++)
	{ s+= a[kk][k]* a[kk][k];
	v[kk][0]=a[kk][k]; 
	}
      alf=(a[k][k]>0)? pow(s, 0.5): -pow(s, 0.5);
      v[k][0]=v[k][0]- alf;
      //v=v-alf*e;    
      transp(v,vT); 
      s=0.0;
      for (int kk=0;kk<m;kk++) s+= v[kk][0]*v[0][kk];
      beta=s;
      if (beta>1e-14) /* if beta =0 go to next k */
	{
	  for (int j=k;j<n;j++)
	    {gam=0.0;
	    for (int jj=0;jj<m;jj++) gam+=v[jj][0]*a[jj][j];
	    for (int jj=0;jj<m;jj++) 
	      a[jj][j]= a[jj][j]-(2*gam/beta)*v[jj][0];
	    }
	}
    }
  for (int i=0;i<a.nrows();i++) w[i]=v[i][0];
}

void Usolve(Mat_DP &a, Vec_DP &b, Vec_DP &sol)
  /*
Usolve returns the solution x of U x=b for
an upper triangular matrix U
*/
{
  int m= a.nrows(), n= a.ncols();
  if ( (m!=n) || (m<2))
    {cout<<"Argument error in Usolve,m,n="
         <<m<<",  "<<n<<endl;
    abort();}
  Vec_DP z(m);
  DP s;
  for (int i=m-1;i>=0;i--) /* Solve U z = b */
    {s=0.0;
    for (int j=i+1;j<m;j++) s+=a[i][j]*z[j];
    if (fabs(a[i][i])<1e-16) 
      {cout<<"Division by zero in Usolve\n"
           <<endl;
      abort();}
    z[i]=(b[i]-s)/(a[i][i]);
    }
  sol=z;
}

void edit_matrix(Mat_DP &a)
{
  int m=a.nrows(), n=a.ncols(), imx, jmx;
  DP amx;
  amx=fabs(a[0][0]); imx=0;jmx=0;
  for (int i=0;i<m;i++)
    for (int j=0;j<n;j++)
      {
	if (fabs(a[i][j]) >amx)
	  { imx=i; jmx=j; amx=fabs(a[i][j]);}
      }
  amx=amx*1e-14;
  if(amx<1e-14) amx=1e-14;
  for (int i=0;i<m;i++)
    for (int j=0;j<n;j++)
      if (fabs(a[i][j]) <amx) a[i][j]=0.0;
}

void HOUSEsolve(Mat_DP &a, Vec_DP &b, Vec_DP &sol)
  /*   Solves a*x = b by the Householder method. 
     writing a = QR with orthog. Q and upper triangular R
     a will be overwritten.
*/
{
  int m= a.nrows(), n= a.ncols();
  if ( (m <2) || (n <2) || (m<n) ||(b.size() !=m) ||(sol.size()!=n))
    {cout<<"Argument error in HOUSEsolve,m,n=" 
         <<m<<",  "<<n<<endl;
    abort();}
  Vec_DP v(m),tmp(m);
  tmp=b;
  for (int i=0;i<n;i++)
    {
      house_vec(a,i,v);
      DP beta=vnormp(v,2.0);
      beta=beta*beta;
      if (beta>1e-14)
	{
	  DP gam=0.0;
	  for (int ii=0;ii<m;ii++)
	    {gam+= v[ii]*tmp[ii];}
	  gam=2*gam/beta;   
	  for (int ii=0;ii<m;ii++)
	    tmp[ii]=tmp[ii]-gam*v[ii];
	}
    }
  Mat_DP aa(n,n);
  Vec_DP  rhs(n);
  for (int i=0;i<n;i++)
    for (int j=0;j<n;j++) aa[i][j] =a[i][j];
  for (int i=0;i<n;i++) rhs[i]=tmp[i];
  Usolve(aa,rhs,sol);
}

void CompareMethods(Mat_DP &a)
  /*
Compare three methods to solve a LSQ problem:
a) SVDsolve
b) HOUSEsolve
c) banal solution: Solution for a*x =b is given
   as x= pseudoinv(a)*b
*/
{
  Mat_DP ac(a);
  Vec_DP v(a.nrows()),sol(a.ncols());
  v=1.0;
  sol=1.0;
  matvecmul(ac,sol,v);
  sol =0.0;
  HOUSEsolve(ac,v, sol);  
  cout << "HOUSEsolve:resid ="<<residual(a,sol,v)<<endl;  
  Vec_DP b(v.size());
  sol =1.0;
  matvecmul(a,sol,b);
  SVDsolve(a,b, sol);  
  cout << "SVDsolve: Resid="<<residual(a,sol,b)<<endl;  
  Mat_DP pia(a.ncols(),a.nrows());
  pseudoinv(a,pia);
  matvecmul(pia,b,sol);
  cout << "pseudoinv: Resid="<<residual(a,sol,b)<<endl;  
  getchar();
}

// FILE: myhouse2.cpp ends
