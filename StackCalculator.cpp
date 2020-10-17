//2018203043�ּ���
#include <iostream>
#include <stack>
#include <string>
using namespace std;

//number����, operations������ ���� ������ �̿� ���� ������ �ϴ� �Լ�
void calculate(stack<double>& number, stack<char>& operations) {
	double opd1; double opd2;
	opd2 = number.top();
	number.pop();
	opd1 = number.top();
	number.pop();
	switch (operations.top()) {
	case '+':number.push(opd1 + opd2); break;
	case '-':number.push(opd1 - opd2); break;
	case '*':number.push(opd1 * opd2); break;
	case '/':
		if (opd2 == 0) throw opd2;
		number.push(opd1 / opd2); break;
	}
	operations.pop();
};

double read_and_postFix(istream& ins) {
	const char DECIMAL = '.';
	stack<double> number;
	stack<char> operations;
	double result;
	char symbol;
	char paren = 'x';
	
	//���� ���� ó�� ����!!!!
	while (ins && ins.peek() != '\n')
	{//������ ��쿡�� ��� number�� �־��ش�
		if (isdigit(ins.peek()) || ins.peek() == DECIMAL) {
			ins >> result;
			number.push(result);
		}
		//�������϶�
		else if (strchr("+-*/", ins.peek()) != NULL) {
			ins >> symbol;
			//operations���� ���� ������� ���� ���
			if (operations.empty() == false) {
				switch (symbol) {
				case '+':
					//'*'�����ڿ� '/'�����ڴ� '+'�����ں��� �켱������ �����Ƿ�
					if (operations.top() == '*' || operations.top() == '/'){
						calculate(number, operations);
						operations.push('+');
					}
					else
						operations.push('+');
					break;
				case '-':
					//'*'�����ڿ� '/'�����ڴ� '+'�����ں��� �켱������ �����Ƿ�
					if (operations.top() == '*' || operations.top() == '/'){
						calculate(number, operations);
						operations.push('-');
					}
					else
						operations.push('-');
					break;
				case '*':case '/':
					operations.push(symbol);
					break;
				}
			}
			else
				operations.push(symbol);

		}

		//'('��ȣ�� ��
		else if (ins.peek() == ')') {
			ins >> symbol;
			int plz = 0;
			if (operations.empty())
				throw paren;
			while (!operations.empty()) {
				switch (operations.top()) {
				case '(':
					operations.pop();
					++plz;
					break;
				case '{': case '[':
					while (!number.empty())number.pop();
					while (!operations.empty())operations.pop();
					while (ins.peek() != '\n')ins.ignore();
					throw paren;
				default:
					calculate(number, operations);
				}
				if (plz == 1)
					break;
			}
			
		}//']'��ȣ�� ���
		else if (ins.peek() == ']') {
			ins >> symbol;
			int plz = 0;
			if (operations.empty())
				throw paren;
			while (!operations.empty()) {
				switch (operations.top()) {
				case '[':
					operations.pop();
					++plz;
					break;
				case '{': case '(':
					while (!number.empty())number.pop();
					while (!operations.empty())operations.pop();
					while (ins.peek() != '\n')ins.ignore();
					throw paren;
				default:
					calculate(number, operations); 
				}
				if (plz == 1)
					break;
			}
		}//'}'��ȣ�� ���
		else if (ins.peek() == '}') {
			ins >> symbol;
			int plz = 0;//��ȣ�� �Ѱ��� ó���� �� �ֵ���
			if (operations.empty())
				throw paren;
			while (!operations.empty()) {
				switch (operations.top()) {
				case '{':
					operations.pop();
					++plz;
					break;
				case '(': case '[':
						while (!number.empty())number.pop();
						while (!operations.empty())operations.pop();
						while (ins.peek() != '\n')ins.ignore();
						throw paren;
				default:
					calculate(number, operations);
				}
				if (plz == 1)//��Ʈ�� ��ȣ�� ������ ���Ϲ��� �������´�.
					break;
			}
		}//���� ��ȣ�� �Է¹����� ���� operations�� push
		else if (strchr("({[", ins.peek()) != NULL) {
			ins >> symbol;
			operations.push(symbol);
		}
		//"EOI"�� �Է¹����� ����
		else if (ins.peek() == 'E') {
			ins.ignore();
			if (ins.peek() == 'O') {
				ins.ignore();
				if (ins.peek() == 'I') {
					ins.ignore();
					if (ins.peek() == '\n')
						exit(1);
				}
			}
		}
		//������, ���ڰ� �ƴҶ�
		else
			ins.ignore();
	}
	while (!operations.empty()) {
		switch (operations.top()) {
			//���� ó���� �ϱ� ����, ���۸� ����־���Ѵ�.
		case '(':
		case '{':
		case '[':
			while (!number.empty())number.pop();
			while (!operations.empty())operations.pop();
			while (ins.peek() != '\n')ins.ignore();
			throw paren;
		default:
			calculate(number, operations); break;
		}
	}
	return number.top();
}

int main(void) {
	double result;
	string reason;
	while (1) {
		try {
			result = read_and_postFix(cin);
			cout << fixed;
			cout.precision(3);
			cout << result << endl;
		}
		catch (double except) {
			reason = "divided by zero";
			cout << "Error!: " << reason << endl;
		}
		catch (char expt) {
			reason = "unbalanced parentheses";
			cout << "Error!: " << reason << endl;
		}
		cin.ignore();
	}
}