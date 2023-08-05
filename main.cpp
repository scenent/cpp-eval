#include <map>
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <cmath>
#include <sstream>

inline std::wstring erasedFront(std::wstring str, int index) {
    if (index >= str.size()) return L"";
    while (index--) {
        str.erase(str.begin());
    }
    return str;
}

inline bool startsWith(std::wstring str, std::wstring prefix) {
    if (prefix.size() > str.size()) return false;
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

inline std::wstring eraseSpace(std::wstring str) {
    std::wstring::iterator it = str.begin();
    while (it != str.end()){
    	if (*it == L' ') it = str.erase(it);
		else it++;
	}
	return str;
}

inline std::vector<std::wstring> split(std::wstring input, wchar_t delimiter) {
    std::vector<std::wstring> answer;
    std::wstringstream ss(input);
    std::wstring temp;
    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }
    return answer;
}

enum class CharType {
	Unknown,
	WhiteSpace,
	NumberLiteral,
	StringLiteral,
	IdentifierAndKeyword,
	OperatorAndPunctuator,
};

CharType getCharType(wchar_t c){
	if (L' ' == c || L'\t' == c || L'\r' == c || L'\n' == c) {
		return CharType::WhiteSpace;
	}
	if (L'0' <= c && c <= L'9') {
		return CharType::NumberLiteral;
	}
	if (c == L'\"') {
		return CharType::StringLiteral;
	}
	if (L'a' <= c && c <= L'z' || L'A' <= c && c <= L'Z' || L'[' == c || L']' == c) {
		return CharType::IdentifierAndKeyword;
	}
	if (33 <= c && c <= 47 && c != '\'' ||
		58 <= c && c <= 64 ||
		91 <= c && c <= 96 ||
		123 <= c && c <= 126) {
		return CharType::OperatorAndPunctuator;
	}
	return CharType::Unknown;
}

enum class TokenKind {
	Unknown,
	True, False,
	Int, Float,
	String,
	Parameter,
	
	Add, Sub, Mul, Div,
	Negative,
	IsEqual, IsNotEqual, IsGreat, IsLess, IsGreatEqual, IsLessEqual,
	LeftParent, RightParent,

	Or, And, Not,
	
	Sin, Cos, Tan,
	
	Pow
};

std::map<TokenKind, int> evalPriority = {
	{TokenKind::LeftParent, -2},
	{TokenKind::And, -1}, {TokenKind::Or, 0},
	{TokenKind::Not, 1},
	{TokenKind::IsEqual, 2}, {TokenKind::IsNotEqual, 2},
	{TokenKind::IsGreat, 2}, {TokenKind::IsLess, 2},
	{TokenKind::IsGreatEqual, 2}, {TokenKind::IsLessEqual, 2},
	{TokenKind::Add, 3}, {TokenKind::Sub, 3},
	{TokenKind::Mul, 4}, {TokenKind::Div, 4},
	{TokenKind::Negative, 5},
	{TokenKind::Sin, 6}, {TokenKind::Cos, 6}, {TokenKind::Tan, 6},
	{TokenKind::Pow, 6},
};


std::map<TokenKind, std::string> kind2String = {
	{TokenKind::Unknown, "Unknown"},
	{TokenKind::True, "True"},
	{TokenKind::False, "False"},
	{TokenKind::Int, "Int"},
	{TokenKind::Float,"Float"},
	{TokenKind::String, "String"},
	{TokenKind::Parameter, "Parameter"},
	{TokenKind::Add, "Add"},
	{TokenKind::Sub, "Sub"},
	{TokenKind::Mul, "Mul"},
	{TokenKind::Div, "Div"},
	{TokenKind::Negative, "Negative"},
	{TokenKind::IsEqual, "IsEqual"},
	{TokenKind::IsNotEqual, "IsNotEqual"},
	{TokenKind::IsGreat, "IsGreat"},
	{TokenKind::IsLess, "IsLess"},
	{TokenKind::IsGreatEqual, "IsGreatEqual"},
	{TokenKind::IsLessEqual, "IsLessEqual"},
	{TokenKind::Or, "Or"},
	{TokenKind::And, "And"},
	{TokenKind::Not, "Not"},
	{TokenKind::Sin, "Sin"},
	{TokenKind::Cos, "Cos"},
	{TokenKind::Tan, "Tan"},
	{TokenKind::Pow, "Pow"},
	
};

struct Token {
	TokenKind type;
	std::wstring data;
	void print() {
		std::cout << "[" << kind2String[type];
		std::wcout << ", \"" << data << L"\"]\n";
	}
};

std::vector<Token> getParameter(std::wstring src);

std::vector<Token> tokenize(std::wstring src) {
	std::vector<Token> result;
	int index = 0;
	while (index < src.size()) {
		std::wstring temp = L"";
		if (src.size() > 0 && src[0] == L' ') break;
		switch (getCharType(src[index])) {
		case (CharType::WhiteSpace): {
			index++;
			break;
		}
		case (CharType::NumberLiteral): {
			while (getCharType(src[index]) == CharType::NumberLiteral || src[index] == L'.') { temp += src[index]; index++; }
			if (temp.find(L'.') != std::wstring::npos)
				result.push_back({TokenKind::Float, temp});
			else
				result.push_back({TokenKind::Int, temp});
			break;
		}
		case (CharType::StringLiteral): {
			index++;
			while (src[index] != L'\"') {
				temp += src[index]; index++;
			}
			index++;
			result.push_back({ TokenKind::String, temp });
			break;
		}
		case (CharType::IdentifierAndKeyword): {
			std::wstring rest = erasedFront(src, index);
			if (startsWith(rest, L"and")) { result.push_back({ TokenKind::And, L"and" }); index += 3; }
			else if (startsWith(rest, L"or")) { result.push_back({ TokenKind::Or, L"or" }); index += 2; }
			else if (startsWith(rest, L"not")) { result.push_back({ TokenKind::Not, L"not" }); index += 3; }
			else if (startsWith(rest, L"true")) { result.push_back({ TokenKind::True, L"true" }); index += 4; }
			else if (startsWith(rest, L"false")) { result.push_back({ TokenKind::False, L"false" }); index += 5; }
			else if (startsWith(rest, L"sin")) { result.push_back({ TokenKind::Sin, L"sin" }); index += 3; }
			else if (startsWith(rest, L"cos")) { result.push_back({ TokenKind::Cos, L"cos" }); index += 3; }
			else if (startsWith(rest, L"tan")) { result.push_back({ TokenKind::Tan, L"tan" }); index += 3; }
			else if (startsWith(rest,  L"pow")){
				result.push_back({TokenKind::Pow, L"pow"});
				index += 3;
				if (src[index] != L'(') assert(false);
				index++;
				int startIndex = index;
				int endIndex = 0;
				std::wstring parameterSource = L"";
				std::stack<wchar_t> parentStack;
				parentStack.push(L'(');
				while (index < src.size() && parentStack.empty() == false){
					if (src[index] == L'('){
						parentStack.push(L'(');
					}
					else if (src[index] == L')'){
						parentStack.pop();
					}
					index++;
				}
				endIndex = index;
				for (int i=startIndex; i<endIndex - 1; i++){
					parameterSource += src[i];
				}
				result.push_back({TokenKind::Parameter, parameterSource});
			}
			break;
		}
		case (CharType::OperatorAndPunctuator): {
			std::wstring rest = erasedFront(src, index);
			if (src[index] == L'+') { result.push_back({ TokenKind::Add, L"+" }); index++; }
			else if (src[index] == L'-') { 
				if (result.empty() || (result[result.size()-1].type != TokenKind::Int
						    && result[result.size() - 1].type != TokenKind::Float
				                    && result[result.size() - 1].type != TokenKind::RightParent)) {
					result.push_back({ TokenKind::Negative, L"-" });
					index++;
				}
				else {
					result.push_back({ TokenKind::Sub, L"-" });
					index++;
				}
			}
			else if (src[index] == L'*') { result.push_back({ TokenKind::Mul, L"*" }); index++;}
			else if (src[index] == L'/') {result.push_back({ TokenKind::Div, L"/" });index++;}
			else if (src[index] == L'(') {result.push_back({ TokenKind::LeftParent, L"(" });index++;}
			else if (src[index] == L')') {result.push_back({ TokenKind::RightParent, L")" });index++;}
			else if (index+1 < src.size() && src[index] == L'=' && src[index+1] == L'=') {result.push_back({ TokenKind::IsEqual, L"==" });index+=2;}
			else if (index + 1 < src.size() && src[index] == L'!' && src[index + 1] == L'=') {result.push_back({ TokenKind::IsNotEqual, L"!=" });index += 2;}
			else if (index + 1 < src.size() && src[index] == L'>' && src[index + 1] == L'=') {result.push_back({ TokenKind::IsGreatEqual, L">=" });index += 2;}
			else if (index + 1 < src.size() && src[index] == L'<' && src[index + 1] == L'=') {result.push_back({ TokenKind::IsLessEqual, L"<=" });index += 2;}
			else if (src[index] == L'>') {result.push_back({ TokenKind::IsGreat, L">" });index++;}
			else if (src[index] == L'<') {result.push_back({ TokenKind::IsLess, L"<" });index++;}
			else {
				std::cerr << "error\n";
			}
			break;
		}
		}
	}
	return result;
}

Token eval(std::wstring src) {
	if (src.empty()) return { TokenKind::Unknown, L"" };
	std::vector<Token> tokens = tokenize(src);
	std::vector<Token> postfix;
	std::stack<Token> stack;
	for (int n = 0; n < tokens.size(); n++) {
		if (tokens[n].type == TokenKind::Int || tokens[n].type == TokenKind::Float || tokens[n].type == TokenKind::String || tokens[n].type == TokenKind::True || tokens[n].type == TokenKind::False
			|| tokens[n].type == TokenKind::Parameter
		) {
			postfix.push_back(tokens[n]);
		}
		else if(tokens[n].type == TokenKind::LeftParent) {
			stack.push(tokens[n]);
		}
		else if (tokens[n].type == TokenKind::RightParent) {
			while (stack.top().type != TokenKind::LeftParent) {
				postfix.push_back(stack.top()); stack.pop();
			}
			stack.pop();
		}
		else {
			while (stack.empty() == false && evalPriority[tokens[n].type] <= evalPriority[stack.top().type]) {
				postfix.push_back(stack.top()); stack.pop();
			}
			stack.push(tokens[n]);
		}
	}
	while (stack.empty() == false) {
		postfix.push_back(stack.top()); stack.pop();
	}
	Token target1, target2;
	for (int i = 0; i < postfix.size(); i++) {
		if (postfix[i].type == TokenKind::Int || postfix[i].type == TokenKind::Float || postfix[i].type == TokenKind::String || postfix[i].type == TokenKind::True || postfix[i].type == TokenKind::False
			|| postfix[i].type == TokenKind::Parameter
		) {
			stack.push(postfix[i]);
		}
		else if (postfix[i].type == TokenKind::Negative) {
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float) { stack.push({ TokenKind::Float, L"-" + target1.data }); }
			else if (target1.type == TokenKind::Int) { stack.push({ TokenKind::Int, L"-" + target1.data }); }
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Sin) {
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float) { stack.push({ TokenKind::Float, std::to_wstring(sin(stoi(target1.data))) }); }
			else if (target1.type == TokenKind::Int) { stack.push({ TokenKind::Float, std::to_wstring(sinf(stof(target1.data))) }); }
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Cos) {
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float) { stack.push({ TokenKind::Float, std::to_wstring(cos(stoi(target1.data))) }); }
			else if (target1.type == TokenKind::Int) { stack.push({ TokenKind::Float, std::to_wstring(cosf(stof(target1.data))) }); }
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Tan) {
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float) { stack.push({ TokenKind::Float, std::to_wstring(tan(stoi(target1.data))) }); }
			else if (target1.type == TokenKind::Int) { stack.push({ TokenKind::Float, std::to_wstring(tanf(stof(target1.data))) }); }
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Add) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float && target2.type == TokenKind::Float)
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.data) + stof(target2.data)) });
			else if (target1.type == TokenKind::Int && target2.type == TokenKind::Int)
				stack.push({ TokenKind::Int, std::to_wstring(stoi(target1.data) + stoi(target2.data)) });
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float))
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.data) + stof(target2.data)) });
			else if (target1.type == TokenKind::String && target2.type == TokenKind::String)
				stack.push({ TokenKind::String, target1.data + target2.data });
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Sub) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float && target2.type == TokenKind::Float)
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.data) - stof(target2.data)) });
			else if (target1.type == TokenKind::Int && target2.type == TokenKind::Int)
				stack.push({ TokenKind::Int, std::to_wstring(stoi(target1.data) - stoi(target2.data)) });
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float))
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.data) - stof(target2.data)) });
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Mul) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float && target2.type == TokenKind::Float)
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.data) * stof(target2.data)) });
			else if (target1.type == TokenKind::Int && target2.type == TokenKind::Int)
				stack.push({ TokenKind::Int, std::to_wstring(stoi(target1.data) * stoi(target2.data)) });
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float))
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.data) * stof(target2.data)) });
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Div) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float && target2.type == TokenKind::Float)
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.data) / stof(target2.data)) });
			else if (target1.type == TokenKind::Int && target2.type == TokenKind::Int)
				stack.push({ TokenKind::Int, std::to_wstring(stoi(target1.data) / stoi(target2.data)) });
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float))
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.data) / stof(target2.data)) });
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsEqual) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)){
				if (stof(target1.data) == stof(target2.data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if (target1.type == TokenKind::String && target2.type == TokenKind::String) {
				if (target1.data == target2.data)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if ((target1.type == TokenKind::True || target1.type == TokenKind::False) && (target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type == target2.type)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsNotEqual) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)) {
				if (stof(target1.data) != stof(target2.data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if (target1.type == TokenKind::String && target2.type == TokenKind::String) {
				if (target1.data != target2.data)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if ((target1.type == TokenKind::True || target1.type == TokenKind::False) && (target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type != target2.type)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsGreat) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)){
				if (stof(target1.data) > stof(target2.data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsLess) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)){
				if (stof(target1.data) < stof(target2.data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsGreatEqual) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)){
				if (stof(target1.data) >=  stof(target2.data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsLessEqual) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)){
				if (stof(target1.data) <= stof(target2.data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Or) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::True || target1.type == TokenKind::False) && (target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type == TokenKind::True || target2.type == TokenKind::True)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::And) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::True || target1.type == TokenKind::False) && (target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type == TokenKind::True && target2.type == TokenKind::True)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Not) {
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::True) stack.push({ TokenKind::False, L"false" });
			else if (target1.type == TokenKind::False) stack.push({ TokenKind::True, L"true" });
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Pow){
			target1 = stack.top(); stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.data);
			if (params.size() != 2) assert(false);
			if (!(params[0].type == TokenKind::Int || params[0].type == TokenKind::Float)) assert(false);
			if (!(params[1].type == TokenKind::Int || params[1].type == TokenKind::Float)) assert(false);
			stack.push({TokenKind::Float, std::to_wstring(pow(stod(params[0].data), stod(params[1].data)))});
		}
		else {
			std::cout << "ERROR\n";
		}
	}
	if (stack.empty() == false) {
		return stack.top();
	}
	else {
		std::cerr << "FATAL ERROR!\n";
		assert(false);
	}
}

std::vector<Token> getParameter(std::wstring src){
	std::vector<std::wstring> elements = split(eraseSpace(src), L',');
	std::vector<Token> result;
	for (int i=0; i<elements.size(); i++){
		Token t = eval(elements[i]);
		if (startsWith(t.data, L" ")){
			t.data.erase(t.data.begin());
		}
		result.push_back(t);
	}
	return result;
}


int main(){
	std::cout << "1 + 2 -> ";
	eval(L"1 + 2").print();
	std::cout << "1.0 + 2.0 -> ";
	eval(L"1.0 + 2.0").print();
	std::cout << "1 + 2 * 3 -> ";
	eval(L"1 + 2 * 3").print();
	std::cout << "(1 + 2) * 3 -> ";
	eval(L"(1 + 2) * 3").print();
	std::cout << "1 + 2 == 3 -> ";
	eval(L"1 + 2 == 3").print();
	std::cout << "1 + 2 == 3 and 2 * 3 == 6 -> ";
	eval(L"1 + 2 == 3 and 2 * 3 == 6").print();
	std::cout << "0 == 1 or 1 == 1 -> ";
	eval(L"0 == 1 or 1 == 1").print();
	std::cout << "1 * 2 == 2 or 0 == 1 and 1 * 3 == 3 or 0 == 1 -> ";
	eval(L"1 * 2 == 2 or 0 == 1 and 1 * 3 == 3 or 0 == 1").print();
	std::cout << "1 == 1 and 2 == 2 and 3 != 3 -> ";
	eval(L"1 == 1 and 2 == 2 and 3 != 3").print();
	std::cout << "\"Hello\" + \" \" + \"World!\" -> ";
	eval(L"\"Hello\" + \" \" + \"World!\"").print();
	std::cout << "true and false -> ";
	eval(L"true and false").print();
	std::cout << "-(1 + 2 * 3) -> ";
	eval(L"-(1 + 2 * 3)").print();
	std::cout << "not 1 == 1 or not 1 == 2 -> ";
	eval(L"not 1 == 1 or not 1 == 2").print();
	std::cout << "-cos(0) + sin(0) + tan(0) -> ";
	eval(L"-cos(0) + sin(0) + tan(0)").print();
	std::cout << "sin 1 -> ";
	eval(L"sin 1").print();
	std::cout << "pow((1 * 4), 2) + 4 -> ";
	eval(L"pow((1 * 4), 2) + 4").print();
}
