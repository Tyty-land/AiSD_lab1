#include <iostream>
#include <complex>
#include <cmath>
#include <typeinfo>

using namespace std;

template <typename TYPE>
class Polynomials
{
	TYPE* ratio_array;
	int _size;
	int accuracy = 0.00001;
public:
	Polynomials(const TYPE* other_ratio_array, const int size)
	{
		if (this->ratio_array != nullptr)
			delete[] this->ratio_array;
		this->_size = size;
		this->ratio_array = new TYPE[size];
		for (int i = 0; i < size; i++)
		{
			this->ratio_array[i] = other_ratio_array[i];
		}
	}
	Polynomials(const int degree)
	{
		if (degree >= 0)
		{
			if (this->ratio_array != nullptr)
				delete[] this->ratio_array;
			this->_size = degree + 1;
			this->ratio_array = new TYPE[_size]{ 0 };
		}
		else
			throw "(Polynomials(const int degree) - Максимальная степень не может быть отрицательной)";
	}
	//Конструктор копирования
	Polynomials(const Polynomials& other)
	{
		this->_size = other.getDegree() + 1;
		this->ratio_array = new TYPE[this->_size];
		for (int i = 0; i < this->_size; i++)
		{
			this->ratio_array[i] = other[i];
		}

	}
	~Polynomials() 
	{
		if (this->ratio_array != nullptr)
		{
			delete[] this->ratio_array;
			this->ratio_array = nullptr;
		}
	}
	//Методы
	void setRatio(const TYPE& ratio, const int degree)
	{
		if (degree >= 0)
		{
			if (degree >= this->_size)
			{
				this->expand(degree - this->getDegree());
			}
			this->ratio_array[degree] = ratio;
		}
		else
			throw "(.setRatio() - Степень в полиноме не отрицательна)";
	}
	int getDegree() const
	{
		return this->_size - 1;
	}
	TYPE f(const TYPE& x)
	{
		TYPE f_x = 0;
		for (int i = 0; i < this->_size; i++)
		{
			f_x += (this->ratio_array[i] * TYPE(pow(x, i)));
		}
		return f_x;
	}
	void shrink_to_fit()
	{
		if (this->ratio_array != nullptr)
		{
			int new_size = this->_size;
			while (this->ratio_array[new_size - 1] == TYPE(0) && new_size != 1)
			{
				new_size -= 1;
			}
			TYPE* temp = new TYPE[new_size];
			for (int i = 0; i < new_size; i++)
			{
				temp[i] = this->ratio_array[i];
			}
			delete[] this->ratio_array;
			this->ratio_array = temp;
			this->_size = new_size;
		}
	}
	void expand(const int count)
	{
		if (count >= 0)
		{
			if (this->ratio_array != nullptr)
			{
				int new_size = this->_size + count;
				TYPE* temp = new TYPE[new_size]{0};
				for (int i = 0; i < this->_size; i++)
				{
					temp[i] = this->ratio_array[i];
				}
				delete[] this->ratio_array;
				this->ratio_array = temp;
				this->_size = new_size;
			}
		}
		else
			throw "(.expand(const int count) - Кол-во может быть только положительным числом)";
	}
	void operator=(const Polynomials<TYPE>& other)
	{
		if (this->ratio_array != nullptr)
		{
			delete[] this->ratio_array;
			this->ratio_array = nullptr;
		}
		this->_size = other.getDegree() + 1;
		this->ratio_array = new TYPE[this->_size];
		for (int i = 0; i < this->_size; i++)
		{
			this->ratio_array[i] = other[i];
		}
	}

	template <typename TYPE>
	bool operator==(const Polynomials<TYPE>& other)
	{
		if (this->_size == other.getDegree() + 1)
		{
			for (int i = 0; i < this->_size; i++)
			{
				if (this->ratio_array[i] - other[i] > this->accuracy || this->ratio_array[i] - other[i] < -this->accuracy)
					return false;
			}
			return true;
		}
		return false;
	}

	template <typename TYPE_COMPLEX>
	bool operator==(const Polynomials<std::complex<TYPE_COMPLEX>>& other)
	{
		if (this->_size == other.getDegree() + 1)
		{
			for (int i = 0; i < this->_size; i++)
			{
				if (this->ratio_array[i].real() - other[i].real() > this->accuracy || this->ratio_array[i].real() - other[i].real() < -this->accuracy ||
					this->ratio_array[i].imag() - other[i].imag() > this->accuracy || this->ratio_array[i].imag() - other[i].imag() < -this->accuracy)
					return false;
			}
			return true;
		}
		return false;
	}

	template <typename TYPE>
	bool operator!=(const Polynomials<TYPE>& other)
	{
		return other == this;
	}

	template <typename TYPE_COMPLEX>
	bool operator!=(const Polynomials<std::complex<TYPE_COMPLEX>>& other)
	{
		return other == this;
	}
	TYPE operator[](const int index) const
	{
		if (index >= 0 && index < this->_size)
		{
			return this->ratio_array[index];
		}
		else if (index >= this->_size)
			return 0;
		else
			throw "(operator[] - Индекс вышел за пределы массива)";
	}
	Polynomials<TYPE> operator+(const Polynomials<TYPE>& other)
	{
		if (this->_size <= other.getDegree() + 1)
		{	
			Polynomials<TYPE> temp(other.getDegree());
			for (int i = 0; i < other.getDegree() + 1; i++)
			{
				if (i < this->_size)
					temp.setRatio(this->ratio_array[i] + other[i], i);
				else
					temp.setRatio(other[i], i);
			}
			return temp;
		}
		else
		{
			Polynomials<TYPE> temp(this->_size - 1);
			for (int i = 0; i < this->_size; i++)
			{
				if (i < other.getDegree() + 1)
					temp.setRatio(this->ratio_array[i] + other[i], i);
				else
					temp.setRatio(this->ratio_array[i], i);
			}
			return temp;
		}
	}
	Polynomials<TYPE> operator-(const Polynomials<TYPE>& other)
	{
		if (this->_size <= other.getDegree() + 1)
		{
			Polynomials<TYPE> temp(other.getDegree());
			for (int i = 0; i < other.getDegree() + 1; i++)
			{
				if (i < this->_size)
					temp.setRatio(this->ratio_array[i] - other[i], i);
				else
					temp.setRatio(other[i], i);
			}
			return temp;
		}
		else
		{
			Polynomials<TYPE> temp(this->_size - 1);
			for (int i = 0; i < this->_size; i++)
			{
				if (i < other.getDegree() + 1)
					temp.setRatio(this->ratio_array[i] - other[i], i);
				else
					temp.setRatio(this->ratio_array[i], i);
			}
			return temp;
		}
	}
};
//Операторы в не класса
template <typename TYPE>
Polynomials<TYPE> operator*(const TYPE& value, const Polynomials<TYPE>& pol)
{
	Polynomials<TYPE> temp(pol.getDegree());
	for (int i = 0; i <= pol.getDegree(); i++)
	{
		temp.setRatio(pol[i] * value, i);
	}
	return temp;
}
template <typename TYPE>
Polynomials<TYPE> operator*(const Polynomials<TYPE>& pol, const TYPE& value)
{
	Polynomials<TYPE> temp(pol.getDegree());
	for (int i = 0; i <= pol.getDegree(); i++)
	{
		temp.setRatio(pol[i] * value, i);
	}
	return temp;
}
template<typename TYPE>
ostream& operator<<(ostream& out, const Polynomials<TYPE>& object)
{
	out << " f(x) =";
	for (int i = object.getDegree(); i >= 0; i--)
	{
		if (object[i] != TYPE(0) && i != 0)
			out << " (" << object[i] << ")*x^" << i << " +";
		else if (i == 0)
		{
			if (object[i] >= 0)
				out << " " << object[i] << endl;
			else
				out << " (" << object[i] << ")" << endl;
		}
	}
	return out;
}
template<typename TYPE_COMPLEX>
ostream& operator<<(ostream& out, const Polynomials<std::complex<TYPE_COMPLEX>>& object)
{
	out << " f(x) =";
	for (int i = object.getDegree(); i >= 0; i--)
	{
		if (object[i] != std::complex<TYPE_COMPLEX>(0) && i != 0)
			out << " (" << object[i].real() << " + " << object[i].imag() << "i)*x^" << i << " +";
		else if (i == 0)
		{
			out << " (" << object[i].real() << " + " << object[i].imag() << "i)" << endl;
		}
	}
	return out;
}
template <typename TYPE, typename T2>
double find_integral(const Polynomials<TYPE>& pol, T2 a, T2 b)
{
	Polynomials<TYPE> temp(pol.getDegree() + 1);
	for (int i = 0; i < pol.getDegree() + 1; i++)
	{
		temp.setRatio(pol[i] / (i + 1), i + 1);
	}
	cout << temp << endl;
	return temp.f(b) - temp.f(a);
}
template <typename TYPE_COMPLEX, typename T2>
std::complex<TYPE_COMPLEX> find_integral(const Polynomials<std::complex<TYPE_COMPLEX>>& pol, T2 a, T2 b)
{
	Polynomials<std::complex<TYPE_COMPLEX>> temp(pol.getDegree() + 1);
	for (int i = 0; i < pol.getDegree() + 1; i++)
	{
		temp.setRatio(pol[i] / TYPE_COMPLEX(i + 1), i + 1);
	}
	cout << temp;
	return temp.f(b) - temp.f(a);
}
int main()
{
	try
	{
		setlocale(LC_ALL, "ru");
		std::complex<float> Pol3[4]{15.0f + 5if, 5.3f + 1if, 6.0f + 3if, 15.6f + 4if};
		Polynomials<std::complex<float>> pol3(Pol3, 4);
		pol3.setRatio(4, 10);
		pol3 = std::complex<float>(2) * pol3 * std::complex<float>(2);
		double Pol4[4]{ 1.1, 2.2, 3.3, 4.4 };
		Polynomials<double> pol4(Pol4, 4);
		pol4.setRatio(5.5, 100);
		pol4 = 2.0 * pol4 * 2.0;
		cout << pol3 << pol4;
		cout << find_integral(pol3, 15.5f + 1if, 20.0f + 2.5if) << endl << find_integral(pol4, 4.5, 5.0);
	}
	catch (const char* ex)
	{
		cout << "[!] - Исключение: " << ex << endl;
	}

}
