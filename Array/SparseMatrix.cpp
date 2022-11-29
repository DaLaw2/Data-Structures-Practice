#include<iostream>
#include<algorithm>
#include<stdexcept>

using namespace std;

class MatrixTerm
{
	friend class SparseMatrix;
private:
	int row = 0;
	int col = 0;
	int value = 0;
};

class SparseMatrix
{
private:
	int rows = 0;
	int cols = 0;
	int terms = 0;
	int capacity = 0;
	MatrixTerm* smArray;
public:
	SparseMatrix(int rows, int cols, int capacity)
	{
		this->rows = rows;
		this->cols = cols;
		if (capacity <= 0)
			this->capacity = 1;
		else
			this->capacity = capacity;
		smArray = new MatrixTerm[this->capacity];
	}
	SparseMatrix Transpose()
	{
		SparseMatrix temp(cols, rows, terms);
		temp.terms = terms;
		if (terms > 0)
		{
			int* rowSize = new int[cols];
			int* rowStart = new int[cols];
			fill(rowSize, rowSize + cols, 0);
			for (int i = 0; i < terms; i++)
				rowSize[smArray[i].col]++;
			rowStart[0] = 0;
			for (int i = 1; i < cols; i++)
				rowStart[i] = rowStart[i - 1] + rowSize[i - 1];
			for (int i = 0; i < terms; i++)
			{
				int j = rowStart[smArray[i].col];
				temp.smArray[j].row = smArray[i].col;
				temp.smArray[j].col = smArray[i].row;
				temp.smArray[j].value = smArray[i].value;
				rowStart[smArray[i].col]++;
			}
			delete[] rowSize;
			delete[] rowStart;
		}
		return temp;
	}
	SparseMatrix operator+(SparseMatrix m1)
	{
		int index1 = 0, index2 = 0;
		if (rows != m1.rows || cols != m1.cols)
			throw invalid_argument("Obj can't be added");
		SparseMatrix temp(rows, cols, capacity);
		while ((index1 < terms) && (index2 < m1.terms))
		{
			if ((smArray[index1].row == m1.smArray[index2].row) && (smArray[index1].col == m1.smArray[index2].col))
			{
				temp.Store(smArray[index1].row, smArray[index1].col, smArray[index1].value + m1.smArray[index2].value);
				index1++; index2++;
			}
			else if (smArray[index1].row != m1.smArray[index2].row)
			{
				if (smArray[index1].row < m1.smArray[index2].row)
				{
					temp.Store(smArray[index1].row, smArray[index1].col, smArray[index1].value);
					index1++;
				}
				else
				{
					temp.Store(m1.smArray[index2].row, m1.smArray[index2].col, m1.smArray[index2].value);
					index2++;
				}
			}
			else if (smArray[index1].col != m1.smArray[index2].col)
			{
				if (smArray[index1].col < m1.smArray[index2].col)
				{
					temp.Store(smArray[index1].row, smArray[index1].col, smArray[index1].value);
					index1++;
				}
				else
				{
					temp.Store(m1.smArray[index2].row, m1.smArray[index2].col, m1.smArray[index2].value);
					index2++;
				}
			}
		}
		for (; index1 < terms; index1++)
			temp.Store(smArray[index1].row, smArray[index1].col, smArray[index1].value);
		for (; index2 < m1.terms; index2++)
			temp.Store(m1.smArray[index2].row, m1.smArray[index2].col, m1.smArray[index2].value);
		return temp;
	}
	inline void operator+=(SparseMatrix m1)
	{
		*this = *this + m1;
	}
	SparseMatrix operator*(SparseMatrix m1)
	{
		if (cols != m1.rows)
			throw invalid_argument("Obj can't be multiplied");
		SparseMatrix result(rows, m1.cols, 0);
		SparseMatrix m1Xpos = m1.Transpose();
		ReSize(terms + 1);
		m1Xpos.ReSize(m1Xpos.terms + 1);
		smArray[terms].row = rows;
		m1Xpos.smArray[m1.terms].row = m1.cols;
		m1Xpos.smArray[m1.terms].col = -1;
		int sum = 0;
		int currRowIndex = 0, currRowBegin = 0, currRow = smArray[0].row;
		while (currRowIndex < terms)
		{
			int currCol = m1Xpos.smArray[0].row;
			int currColIndex = 0;
			while (currColIndex <= m1.terms)
			{
				if (smArray[currRowIndex].row != currRow)
				{
					result.Store(currRow, currCol, sum);
					sum = 0;
					currRowIndex = currRowBegin;
					while (m1Xpos.smArray[currColIndex].row == currCol)
						currColIndex++;
					currCol = m1Xpos.smArray[currColIndex].row;
				}
				else if (m1Xpos.smArray[currColIndex].row != currCol)
				{
					result.Store(currRow, currCol, sum);
					sum = 0;
					currRowIndex = currRowBegin;
					currCol = m1Xpos.smArray[currColIndex].row;
				}
				else if (smArray[currRowIndex].col == m1Xpos.smArray[currColIndex].col)
				{
					sum += smArray[currRowIndex].value * m1Xpos.smArray[currColIndex].value;
					currRowIndex++;
					currColIndex++;
				}
				else if (smArray[currRowIndex].col < m1Xpos.smArray[currColIndex].col)
					currRowIndex++;
				else
					currColIndex++;
			}
			while (smArray[currRowIndex].row == currRow)
				currRowIndex++;
			currRowBegin = currRowIndex;
			currRow = smArray[currRowIndex].row;
		}
		ReSize(terms);
		m1.ReSize(m1.terms);
		result.ReSize(result.terms);
		return result;
	}
	inline void operator*=(SparseMatrix m1)
	{
		*this = *this * m1;
	}
	void operator=(SparseMatrix m1)
	{
		rows = m1.rows;
		cols = m1.cols;
		terms = m1.terms;
		capacity = m1.capacity;
		delete[] smArray;
		smArray = new MatrixTerm[capacity];
		copy(m1.smArray, m1.smArray + m1.terms, smArray);
	}
	void Store(int row, int col, int value)
	{
		if (rows <= row || cols <= col)
			throw length_error("Out of range");
		if (value == 0)
			return;
		if (terms >= capacity)
			ReSize(2 * capacity);
		smArray[terms].row = row;
		smArray[terms].col = col;
		smArray[terms++].value = value;
	}
	void ReSize(int size)
	{
		MatrixTerm* temp = new MatrixTerm[size];
		copy(smArray, smArray + terms, temp);
		delete[] smArray;
		smArray = temp;
		capacity = size;
	}
};

int main()
{
	SparseMatrix m1(4, 5, 4);
	SparseMatrix m2(5, 4, 6);
	m1.Store(0, 1, 2);
	m1.Store(1, 2, 1);
	m1.Store(2, 3, 3);
	m1.Store(3, 0, 4);
	m2.Store(0, 1, 8);
	m2.Store(1, 3, 6);
	m2.Store(3, 1, 5);
	m2.Store(4, 0, 7);
	SparseMatrix m3 = m1 * m2;
	return 0;
}