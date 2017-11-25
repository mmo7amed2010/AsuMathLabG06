#include "CMatrix.h"
#include "stdarg.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
CMatrix::CMatrix()
{
	nR = nC = 0;
	values = NULL;
}
CMatrix::~CMatrix()
{
	reset();
}
CMatrix::CMatrix(int nR, int nC, int initialization, double initializationValue)
{
	this->nR = nR;
	this->nC = nC;
	if ((nR*nC) == 0){ values = NULL; return; }
	values = new double*[nR];
	for (int iR = 0; iR<nR; iR++)
	{
		values[iR] = new double[nC];
		for (int iC = 0; iC<nC; iC++)
		{
			switch (initialization)
			{
			case MI_ZEROS: values[iR][iC] = 0; break;
			case MI_ONES: values[iR][iC] = 1; break;
			case MI_EYE: values[iR][iC] = (iR == iC) ? 1 : 0; break;
			case MI_RAND: values[iR][iC] = (rand() % 1000000) / 1000000.0; break;
			case MI_VALUE: values[iR][iC] = initializationValue; break;
			}
		}
	}
}
CMatrix::CMatrix(int nR, int nC, double first, ...)
{
	this->nR = nR;
	this->nC = nC;
	if ((nR*nC) == 0){ values = NULL; return; }
	values = new double*[nR];
	va_list va;
	va_start(va, first);
	for (int iR = 0; iR<nR; iR++)
	{
		values[iR] = new double[nC];
		for (int iC = 0; iC<nC; iC++)
		{
			values[iR][iC] = (iC == 0 && iR == 0) ? first : va_arg(va, double);
		}
	}
	va_end(va);
}
CMatrix::CMatrix(CMatrix& m)
{
	nR = nC = 0;
	values = NULL;
	copy(m);
}
CMatrix::CMatrix(string s)
{
	nR = nC = 0;
	values = NULL;
	copy(s);
}
void CMatrix::copy(CMatrix& m)
{
	reset();
	this->nR = m.nR;
	this->nC = m.nC;
	if ((nR*nC) == 0){ values = NULL; return; }
	values = new double*[nR];
	for (int iR = 0; iR<nR; iR++)
	{
		values[iR] = new double[nC];
		for (int iC = 0; iC<nC; iC++)
		{
			values[iR][iC] = m.values[iR][iC];
		}
	}
}
CMatrix::CMatrix(double d)
{
	nR = nC = 0;
	values = NULL;
	copy(d);
}
void CMatrix::copy(double d)
{
	reset();
	this->nR = 1;
	this->nC = 1;
	values = new double*[1];
	values[0] = new double[1];
	values[0][0] = d;
}
void CMatrix::copy(string s)
{
	reset();
	char* buffer = new char[s.length() + 1];
	strcpy(buffer, s.c_str());
	char* lineContext;
	char* lineSeparators = ";\r\n";
	char* line = strtok_r(buffer, lineSeparators, &lineContext);
	while (line)
	{
		CMatrix row;
		char* context;
		char* separators = " []";
		char* token = strtok_r(line, separators, &context);
		while (token)
		{
			CMatrix item (atof(token));
			row.addColumn(item);
			token = strtok_r(NULL, separators, &context);
		}
		if (row.nC>0 && (row.nC == nC || nR == 0))
			addRow(row);
		line = strtok_r(NULL, lineSeparators, &lineContext);
	}
	delete[] buffer;
}
void CMatrix::reset()
{
	if (values)
	{
		for (int i = 0; i<nR; i++)
			delete[] values[i];
		delete[] values;
	}
	nR = nC = 0;
	values = NULL;
}
string CMatrix::getString()
{
	string s;
	for (int iR = 0; iR<nR; iR++)
	{
		for (int iC = 0; iC<nC; iC++)
		{
			char buffer[50];
			snprintf(buffer, 50, "%g\t", values[iR][iC]);
			s += buffer;
		} s
			+= "\n";
	}
	return s;
}
CMatrix& CMatrix::operator=(CMatrix& m)
{
	copy(m);
	return *this;
}
CMatrix& CMatrix::operator=(double d)
{
	copy(d);
	return *this;
}
CMatrix& CMatrix::operator=(string s)
{
	copy(s);
	return *this;
}
void CMatrix::operator+=(CMatrix& m)
{
	*this=add(m);
}
void CMatrix::operator+=(double d)
{
    CMatrix a (nR, nC, MI_VALUE, d);
	*this=add(a);
}
CMatrix& CMatrix::operator+(CMatrix& m)
{
    return add(m);
}
CMatrix& CMatrix::operator+(double d)
{
    CMatrix a (nR, nC, MI_VALUE, d);
    return add(a);
}
void CMatrix::operator-=(CMatrix& m)
{
	*this=sub(m);
}
void CMatrix::operator-=(double d)
{
    CMatrix a (nR, nC, MI_VALUE, d);
	*this=sub(a);
}
CMatrix& CMatrix::operator-(CMatrix& m)
{
    return sub(m);
}
CMatrix& CMatrix::operator-(double d)
{
	CMatrix a (nR, nC, MI_VALUE, d);
    return sub(a);
}
void CMatrix::operator*=(CMatrix& m)
{
	*this=mul(m);
}
void CMatrix::operator*=(double d)
{
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		values[iR][iC] *= d;
}
CMatrix& CMatrix::operator*(CMatrix& m)
{
    return mul(m);
}
CMatrix& CMatrix::operator*(double d)
{
CMatrix r(nR,nC);
	for(int iR=0;iR<nR;iR++)
		for(int iC=0;iC<nC;iC++)
			r.values[iR][iC]= values[iR][iC] * d;
static CMatrix t;
t=r;
    return t;
}
CMatrix& CMatrix::operator++()
{
    CMatrix a (nR, nC, MI_VALUE, 1.0);
	*this=add(a);
	return *this;
}
CMatrix& CMatrix::operator++(int)
{
	CMatrix a (nR, nC, MI_VALUE, 1.0);
	*this=add(a);
	return *this;
}
CMatrix& CMatrix::operator--()
{
    CMatrix a (nR, nC, MI_VALUE, -1.0);
	*this=add(a);
	return *this;
}
CMatrix& CMatrix::operator--(int)
{
	CMatrix a (nR, nC, MI_VALUE, -1.0);
	*this=add(a);
	return *this;
}
CMatrix& CMatrix::operator-()
{
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		values[iR][iC] = -values[iR][iC];
	return *this;
}
CMatrix& CMatrix::operator+()
{
	return *this;
}
void CMatrix::setSubMatrix(int r, int c, CMatrix& m)
{
	if ((r + m.nR)>nR || (c + m.nC)>nC)throw("Invalid matrix dimension");
	for (int iR = 0; iR<m.nR; iR++)
	for (int iC = 0; iC<m.nC; iC++)
		values[r + iR][c + iC] = m.values[iR][iC];
}
CMatrix& CMatrix::getSubMatrix(int r, int c, int nr, int nc)
{
	if ((r + nr)>nR || (c + nc)>nC)throw("Invalid matrix dimension");
	CMatrix m(nr, nc);
	for (int iR = 0; iR<m.nR; iR++)
	for (int iC = 0; iC<m.nC; iC++)
		m.values[iR][iC] = values[r + iR][c + iC];
    *this=m;
	return *this;
}
void CMatrix::addColumn(CMatrix& m)
{
	CMatrix n(max(nR, m.nR), nC + m.nC);
	n.setSubMatrix(0, 0, *this);
	n.setSubMatrix(0, nC, m);
	*this = n;
}
void CMatrix::addRow(CMatrix& m)
{
	CMatrix n(nR + m.nR, max(nC, m.nC));
	n.setSubMatrix(0, 0, *this);
	n.setSubMatrix(nR, 0, m);
	*this = n;
}
CMatrix& CMatrix::getCofactor(int r, int c)
{

	if (nR <= 1 && nC <= 1)throw("Invalid matrix dimension");
	int i=nR-1,j=nC-1;
    CMatrix m(i ,j);
	for (int iR = 0; iR<m.nR; iR++)
	{
	for (int iC = 0; iC<m.nC; iC++)
	{
		int sR = (iR<r) ? iR : iR + 1;
		int sC = (iC<c) ? iC : iC + 1;
		m.values[iR][iC] = values[sR][sC];
	}
	}
	static CMatrix a ;
	a=m;
	return a;
}
double CMatrix::getDeterminant()
{
     double value1;
	if (nR != nC)throw("Invalid matrix dimension");
	if (nR == 2&& nC == 2)
	{
	value1= (this->values[0][0]*this->values[1][1])-(this->values[0][1]*this->values[1][0]);
	//if(value1==0)
      //  throw("error zero determinent");
	}
	if (nR >=3 && nC >= 3)
	{
	CMatrix z;
	double t1,t2;
	value1=0;
	int m=1;
	int iC;
	for ( iC = 0; iC<nC; iC++)
	{
	z=getCofactor(0, iC);
	t1=z.getDeterminant();
    t2=value1 +=m * values[0][iC] *t1;
		m*=-1;
	}
	if(value1==0)
        throw("error zero determinent");
	}
	return value1;
}
void CMatrix::operator/=(CMatrix& m)
{
	*this=div(m);
}

void CMatrix::operator/=(double d)
{
	for(int iR=0;iR<nR;iR++)
		for(int iC=0;iC<nC;iC++)
			values[iR][iC] *= 1/d;
}


CMatrix& CMatrix::operator/(CMatrix& m)
{
    return div(m);
}

CMatrix& CMatrix::operator/(double d)
{
CMatrix r(nR,nC);
    for(int iR=0;iR<nR;iR++)
		for(int iC=0;iC<nC;iC++)
			r.values[iR][iC]=values[iR][iC] * 1/d;
static CMatrix y;
y=r;
    return y;
}
CMatrix& CMatrix::add(CMatrix& m)
{
    CMatrix T(nR,nC);
	if (nR != m.nR || nC != m.nC)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		T.values[iR][iC]=values[iR][iC] + m.values[iR][iC];
    static CMatrix l;
    l=T;
    return l;
}
CMatrix& CMatrix::sub(CMatrix& m)
{
	CMatrix T(nR,nC);
	if (nR != m.nR || nC != m.nC)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR<nR; iR++)
	for (int iC = 0; iC<nC; iC++)
		T.values[iR][iC]=values[iR][iC] - m.values[iR][iC];
    static CMatrix g;
    g=T;
    return g;
}
CMatrix& CMatrix::mul(CMatrix& m)
{
    double d1,d2;
    CMatrix T(nR,m.nC,0);
	if (nC != m.nR)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR<T.nR; iR++)
	{
	for (int iC = 0; iC<T.nC; iC++)
	{
		for (int k = 0; k<m.nR; k++)
		{

            d1=values[iR][k];
            d2=m.values[k][iC];
			T.values[iR][iC] += d1 * d2;
        }
	}
	}
	static CMatrix r;
	r=T;
	return r;
}
CMatrix& CMatrix::getTranspose(){
    CMatrix T(nR,nC);
    for(int i=0;i<nR;i++)
    {
        for(int j=0;j<nC;j++)
        {
            T.values[j][i]=this->values[i][j];
        }
    }
    static CMatrix b ;
    b=T;
    return b;
}
CMatrix& CMatrix::getInverse(){
if(FastestDeterminant()==0)
    throw ("error zero determinent");
 CMatrix a(nR,nC);
 CMatrix A;
 double d ;
if(nC == 0||nR == 0) throw("Invalid matrix dimension");
else if(nC==1&&nR==1)
{
    a.values[0][0]=1/values[0][0];
}
else if(nC==2&&nR==2)
    {
    double d ;
    d= 1/getDeterminant();
    a.values[0][0]=this->values[1][1]*d;
    a.values[1][1]=this->values[0][0]*d;
    a.values[0][1]=-1*this->values[0][1]*d;
    a.values[1][0]=-1*this->values[1][0]*d;
    }
else{
        for(int ir=0;ir<nR;ir++)
        {
          for(int ic=0;ic<nC;ic++)
          {
            A=getCofactor(ir, ic);
            d=A.FastestDeterminant();
            a.values[ir][ic]=d*pow(-1,(ir+ic));
          }
        }
        a=a.getTranspose();
        d=1/FastestDeterminant();
        a*=d;
    }
    static CMatrix v ;
    v=a;
    return v;
}
CMatrix& CMatrix::div(CMatrix& m)
{
    CMatrix w(nR,m.nC);
	if(nC != m.nR)
		throw("Invalid matrix dimension");
    w=*this*m.getInverse();
    static CMatrix o;
    o=w;
    return o;
}
double CMatrix:: FastestDeterminant()
{
double ratio;
CMatrix d(nR,nC);
CMatrix t;
for(int i=0;i<nC;i++)
    {
        d.values[0][i]=values[0][i];
    }
for(int i=1;i<nR;i++)
{
    if(values[0][0]==0)
    throw(new string("error invalid division determinant equals to 0"));
    ratio=values[i][0]/values[0][0];
    for(int j=0;j<nC;j++)
    {
        d.values[i][j]=values[i][j]-(ratio*values[0][j]);
    }
}
double value,v ;
if(d.nC<=3&&d.nR<=3)
value=d.getCofactor(0,0).getDeterminant()*d.values[0][0];
if(d.nC>3&&d.nR>3)
{
t=d.getCofactor(0,0);
v=t.FastestDeterminant();
value=v*d.values[0][0];
//value=d.getCofactor(0,0).FastestDeterminant()*d.values[0][0];
}
return value;
}

