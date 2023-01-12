#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <memory>

class Matrix;
class MatrixNode;

struct MatrixTerm
{
	int row = 0;
	int col = 0;
	int value = 0;
};

class MatrixNode : public std::enable_shared_from_this<MatrixNode>
{
	friend class Matrix;
private:
	bool ishead = false;
	std::shared_ptr<MatrixNode> down, right, next;
	MatrixTerm term;
public:
	MatrixNode() {}
	void set(bool mode, std::shared_ptr<MatrixTerm> term)
	{
		ishead = mode;
		if (mode)
			right = down = shared_from_this();
		else
			shared_from_this()->term = *term;
	}
	virtual ~MatrixNode() {}
};

class Matrix : public std::enable_shared_from_this<Matrix>
{
private:
	std::shared_ptr<MatrixNode> headNode;
public:
	Matrix(int row, int col)
	{
		std::shared_ptr<MatrixTerm> headTerm = std::make_shared<MatrixTerm>(row, col, 0);
		headNode = std::make_shared<MatrixNode>();
		headNode->set(false, headTerm);
		int headNodes = std::max(row, col);
		if (headNodes == 0)
		{
			headNode->next = headNode;
			return;
		}
		std::shared_ptr<MatrixNode> last = headNode;
		for (int i = 0; i < headNodes; i++)
		{
			std::shared_ptr<MatrixNode> new_node = std::make_shared<MatrixNode>();
			new_node->set(true, nullptr);
			last->next = new_node;
			last = new_node;
		}
		last->next = headNode;
	}
	virtual ~Matrix() {}
	void Store(int row, int col, int value)
	{
		if (row > headNode->term.row || col > headNode->term.col || row < 1 || col < 1)
			throw std::runtime_error("Out of range.");
		std::shared_ptr<MatrixTerm> new_term = std::make_shared<MatrixTerm>(row, col, value);
		std::shared_ptr<MatrixNode> new_node = std::make_shared<MatrixNode>();
		new_node->set(false, new_term);
		std::shared_ptr<MatrixNode> headIndex = headNode->next;
		for (int i = 1; i <= std::max(row, col); i++)
		{
			if (i == row)
			{
				std::shared_ptr<MatrixNode> rowIndex = headIndex;
				while (rowIndex->right != headIndex)
				{
					if (rowIndex->right->term.row > row)
						break;
					rowIndex = rowIndex->right;
				}
				if (rowIndex->term.row == row)
				{
					rowIndex->term.value += value;
					return;
				}
				else
				{
					new_node->right = rowIndex->right;
					rowIndex->right = new_node;
				}
			}
			else if (i == col)
			{
				std::shared_ptr<MatrixNode> colIndex = headIndex;
				while (colIndex->down != headIndex)
				{
					if (colIndex->down->term.col > col)
						break;
					colIndex = colIndex->down;
				}
				if (colIndex->term.col == col)
				{
					colIndex->term.value += value;
					return;
				}
				else
				{
					new_node->down = colIndex->down;
					colIndex->down = new_node;
				}
			}
			headIndex = headIndex->next;
		}
		headNode->term.value++;
	}
	Matrix& Transpose()
	{
		Matrix m1(headNode->term.col, headNode->term.row);
		std::shared_ptr<MatrixNode> headIndex = headNode->next, rowIndex;
		while (headIndex != headNode)
		{
			rowIndex = headIndex->right;
			while (rowIndex != headIndex)
			{
				m1.Store(rowIndex->term.col, rowIndex->term.row, rowIndex->term.value);
				rowIndex = rowIndex->right;
			}
			headIndex = headIndex->next;
		}
		return m1;
	}
	void clear()
	{
		std::shared_ptr<MatrixNode> headIndex = headNode->next;
		while (headIndex != headNode)
		{
			headIndex->right = headIndex->down = nullptr;
			headIndex = headIndex->next;
		}
		headNode = nullptr;
	}
	void operator=(Matrix& m1)
	{
		int new_row = m1.headNode->term.row, new_col = m1.headNode->term.col;
		clear();
		std::shared_ptr<MatrixTerm> headTerm = std::make_shared<MatrixTerm>(new_row, new_col, 0);
		headNode = std::make_shared<MatrixNode>();
		headNode->set(false, headTerm);
		int headNodes = std::max(new_row, new_col);
		if (headNodes == 0)
		{
			headNode->next = headNode;
			return;
		}
		std::shared_ptr<MatrixNode> last = headNode;
		for (int i = 0; i < headNodes; i++)
		{
			std::shared_ptr<MatrixNode> new_node = std::make_shared<MatrixNode>();
			new_node->set(true, nullptr);
			last->next = new_node;
			last = new_node;
		}
		last->next = headNode;
		std::shared_ptr<MatrixNode> headIndex = m1.headNode->next, rowIndex;
		while (headIndex != m1.headNode)
		{
			rowIndex = rowIndex->right;
			while (rowIndex != headIndex)
			{
				Store(rowIndex->term.row, rowIndex->term.col, rowIndex->term.value);
				rowIndex = rowIndex->right;
			}
			headIndex = headIndex->next;
		}
	}
	Matrix& operator+(Matrix& m1)
	{
		if (headNode->term.row != m1.headNode->term.row || headNode->term.col != m1.headNode->term.col)
			throw std::runtime_error("Matrix not match.");
		Matrix result(headNode->term.row, headNode->term.col);
		std::shared_ptr<MatrixNode> headIndex = headNode->next, rowIndex;
		while (headIndex != headNode)
		{
			rowIndex = headIndex->right;
			while (rowIndex != headIndex)
			{
				result.Store(rowIndex->term.row, rowIndex->term.col, rowIndex->term.value);
				rowIndex = rowIndex->right;
			}
			headIndex = headIndex->next;
		}
		headIndex = m1.headNode->next;
		while (headIndex != m1.headNode)
		{
			rowIndex = headIndex->right;
			while (rowIndex != headIndex)
			{
				result.Store(rowIndex->term.row, rowIndex->term.col, rowIndex->term.value);
				rowIndex = rowIndex->right;
			}
			headIndex = headIndex->next;
		}
	}
	void operator+=(Matrix m1)
	{
		std::shared_ptr<MatrixNode> headIndex = m1.headNode, rowIndex;
		while (headIndex != m1.headNode)
		{
			rowIndex = headIndex->right;
			while (rowIndex != headIndex)
			{
				Store(rowIndex->term.row, rowIndex->term.col, rowIndex->term.value);
				rowIndex = rowIndex->right;
			}
			headIndex = headIndex->next;
		}
	}
	Matrix& operator*(Matrix m1)
	{
		if (headNode->term.col != m1.headNode->term.row)
			throw std::runtime_error("Matrix not match.");
		Matrix result(headNode->term.row, m1.headNode->term.col);
		std::shared_ptr<MatrixNode> currentRow = headNode->next, currentCol, rowIndex, colIndex;
		while (currentRow != headNode)
		{
			currentCol = m1.headNode->next;
			while (currentCol != m1.headNode)
			{
				int dotProduct = 0;
				rowIndex = currentRow->right;
				colIndex = currentCol->down;
				while (rowIndex != currentRow && colIndex != currentCol)
				{
					if (rowIndex->term.col > colIndex->term.row)
						colIndex = colIndex->down;
					else if (rowIndex->term.col < colIndex->term.row)
						rowIndex = rowIndex->right;
					else if (rowIndex->term.col == colIndex->term.row)
					{
						dotProduct += (rowIndex->term.value * colIndex->term.value);
						rowIndex = rowIndex->right;
						colIndex = colIndex->down;
					}
				}
				result.Store(currentRow->right->term.row, currentCol->down->term.col, dotProduct);
				currentCol = currentCol->next;
			}
			currentRow = currentRow->next;
		}
	}
	void operator*=(Matrix m1)
	{
		*shared_from_this() = *shared_from_this() * m1;
	}
};

int main()
{
	return 0;
}