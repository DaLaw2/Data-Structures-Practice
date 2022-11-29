#include<iostream>
#include<algorithm>
#include<cmath>

using namespace std;

class Term
{
	friend class Polynomial;
private:
	double coef = 0;
	int exp = 0;
};

class Polynomial
{
private:
	int terms = 0;
	int capacity = 0;
	Term* termArray = nullptr;
public:
	Polynomial()
	{
		termArray = new Term[1];
		capacity = 1;
	}
	Polynomial operator+(Polynomial p1)
	{
		Polynomial result;
		int aPos = 0, bPos = 0;
		while ((aPos < terms) && (bPos < p1.terms))
		{
			if (termArray[aPos].exp == p1.termArray[bPos].exp)
			{
				double temp = termArray[aPos].coef + p1.termArray[bPos].coef;
				if (temp)
					result.NewTerm(temp, termArray[aPos].exp);
				aPos++; bPos++;
			}
			else if (termArray[aPos].exp > p1.termArray[bPos].exp)
			{
				result.NewTerm(p1.termArray[bPos].coef, p1.termArray[bPos].exp);
				bPos++;
			}
			else
			{
				result.NewTerm(termArray[aPos].coef,termArray[aPos].exp);
				aPos++;
			}
		}
		for (; aPos < terms; aPos++)
			result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
		for (; bPos < p1.terms; bPos++)
			result.NewTerm(p1.termArray[bPos].coef, p1.termArray[bPos].exp);
		return result;
	}
	void operator+=(Polynomial p1)
	{
		*this = *this + p1;
	}
	Polynomial operator*(Polynomial p1)
	{
		Polynomial result;
		for (int aPos = 0; aPos < terms; aPos++)
		{
			Polynomial temp;
			for (int bPos = 0; bPos < p1.terms; bPos++)
				temp.NewTerm(termArray[aPos].coef * p1.termArray[bPos].coef, termArray[aPos].exp + p1.termArray[bPos].exp);
			result += temp;
		}
		return result;
	}
	void operator*=(Polynomial p1)
	{
		*this = *this * p1;
	}
	void operator=(Polynomial p1)
	{
		terms = p1.terms;
		capacity = p1.capacity;
		delete[] termArray;
		termArray = new Term[capacity];
		copy(p1.termArray, p1.termArray + p1.terms, termArray);
	}
	double Eval(double value)
	{
		double result = 0;
		for (int pos = 0; pos < terms; pos++)
			result += (termArray[pos].coef * pow(value, termArray[pos].exp));
		return result;
	}
	void NewTerm(const double theCoef, const int theExp)
	{
		if (capacity == terms)
		{
			capacity *= 2;
			Term* temp = new Term[capacity];
			copy(termArray, termArray + terms, temp);
			delete[] termArray;
			termArray = temp;
		}
		termArray[terms].coef = theCoef;
		termArray[terms++].exp = theExp;
	}
	void Print()
	{
		for (int pos = terms - 1; pos >= 0; pos--)
		{
			if (pos == 0)
			{
				if (termArray[0].exp == 0)
					cout << termArray[0].coef << endl;
				else if (termArray[0].exp == 1)
					cout << termArray[pos].coef << "x" << endl;
				else
					cout << termArray[pos].coef << "x^" << termArray[pos].exp << endl;
			}
			else if (termArray[pos].exp == 1)
				cout << termArray[pos].coef << "x+";
			else
				cout << termArray[pos].coef << "x^" << termArray[pos].exp << "+";
		}
	}
};

int main()
{
	Polynomial P1, P2, addResult, multResult;
	P1.NewTerm(3, 0);P1.NewTerm(9, 1);P1.NewTerm(5, 2);P1.NewTerm(3, 3);P1.NewTerm(8, 4);
	P1.Print();
	P2.NewTerm(6, 1);P2.NewTerm(4, 3);
	P2.Print();
	addResult = P1 + P2;
	addResult.Print();
	multResult = P1 * P2;
	multResult.Print();

	addResult = P1;
	addResult += P2;
	addResult.Print();
	multResult = P1;
	multResult *= P2;
	multResult.Print();
	cout << multResult.Eval(1) << endl;
	return 0;
}