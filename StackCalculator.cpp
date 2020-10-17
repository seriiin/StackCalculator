//2018203043최세린
#include <iostream>
#include <stack>
#include <string>
using namespace std;

//number스택, operations스택을 전달 받으면 이에 따른 연산을 하는 함수
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
	
	//아직 예외 처리 안함!!!!
	while (ins && ins.peek() != '\n')
	{//숫자일 경우에는 모두 number에 넣어준다
		if (isdigit(ins.peek()) || ins.peek() == DECIMAL) {
			ins >> result;
			number.push(result);
		}
		//연산자일때
		else if (strchr("+-*/", ins.peek()) != NULL) {
			ins >> symbol;
			//operations스택 안이 비어있지 않은 경우
			if (operations.empty() == false) {
				switch (symbol) {
				case '+':
					//'*'연산자와 '/'연산자는 '+'연산자보다 우선순위가 높으므로
					if (operations.top() == '*' || operations.top() == '/'){
						calculate(number, operations);
						operations.push('+');
					}
					else
						operations.push('+');
					break;
				case '-':
					//'*'연산자와 '/'연산자는 '+'연산자보다 우선순위가 높으므로
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

		//'('괄호일 때
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
			
		}//']'괄호일 경우
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
		}//'}'괄호일 경우
		else if (ins.peek() == '}') {
			ins >> symbol;
			int plz = 0;//괄호를 한개만 처리할 수 있도록
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
				if (plz == 1)//세트인 괄호를 만나면 와일문도 빠져나온다.
					break;
			}
		}//왼쪽 괄호를 입력받으면 스택 operations에 push
		else if (strchr("({[", ins.peek()) != NULL) {
			ins >> symbol;
			operations.push(symbol);
		}
		//"EOI"를 입력받으면 종료
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
		//연산자, 숫자가 아닐때
		else
			ins.ignore();
	}
	while (!operations.empty()) {
		switch (operations.top()) {
			//오류 처리를 하기 전에, 버퍼를 비워주어야한다.
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