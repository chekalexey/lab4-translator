#ifndef __TRANSLATOR__
#define __TRANSLATOR__

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include "stack.h"

class term {
	term *obj;
public:
	virtual ~term(){
		delete obj;
	}
	virtual std::string GetType() {
		return "term";
	}
};

class operation : public term {
	int pr = 0;
public:
	std::string GetType() {
		return "operation";
	}
	virtual int priority() {
		return this->pr;
	}
	virtual double calc(double x, double y) = 0;
};

class operand : public term{
	double val = 0;
public:
	virtual std::string GetType() {
		return "operand";
	}
	virtual double value() {
		return val;
	}
	
};
class number : public operand{
	double val = 0;
public:
	number(double x) {
		val = x;
	}
	void setval(double x){
		val *= x;
	}
	double value() override {
		return val;
	}
};

class openbracket : public term {
public:
	std::string GetType() override{
		return "openbracket";
	}
};

class closebracket : public term {
public:
	std::string GetType() override{
		return "closebracket";
	}
};

extern std::map<std::string, double> vars;
class var : public operand {
	double val = 0;
	std::string name;
public:
	var(std::string n) {
		name = n;
	}
	double value() override {
		if (vars.find(name) != vars.end())
			return vars[name];
		else
			throw "the variable has not been initialized";
	}
	std::string GetName(){
		return name;
	}
	std::string GetType() override{
		return "var";
	}
};

class add : public operation {
	int pr = 1;
public:
	int priority() {
		return this->pr;
	}
	double calc(double x, double y) override{
		return x + y;
	}
};

class divn : public operation {
	int pr = 2;
public:
	int priority() {
		return this->pr;
	}
	double calc(double x, double y) override {
		if (abs(y) < 0.0000000001) throw "division on zero";
		return x / y;
	}
};

class sub : public operation {
	int pr = 1;
public:
	int priority() {
		return this->pr;
	}
	std::string GetType() override {
		return "sub";
	}
	double calc(double x, double y) override{
		return x - y;
	}
};

class mul : public operation {
	int pr = 2;
public:
	int priority() {
		return this->pr;
	}
	double calc(double x, double y) override {
		return x * y;
	}
};

class ex : public operation {
	int pr = 3;
public:
	int priority() {
		return this->pr;
	}
	double calc(double x, double y) override {
		return pow(x, y);
	}
};

class eq : public term {
	int pr = 0;
public:
	int priority() {
		return this->pr;
	}
	void assign(std::string name, double y) {
		vars[name] = y;
	}
	std::string GetType() {
		return "eq";
	}
};

class functions : public term {
public:
	std::string GetType() {
		return "function";
	}
	virtual double calc(double x) = 0;
};

class sine : public functions {
public:
	double calc(double x) override {
		return sin(x);
	}
};

class cosine : public functions {
public:
	double calc(double x) override {
		return cos(x);
	}
};

class module : public functions {
public:
	double calc(double x)  override {
		return abs(x);
	}
};

class tangent : public functions {
public:
	double calc(double x)  override {
		return tan(x);
	}
};

class arctangent : public functions {
public:
	double calc(double x) override {
		return atan(x);
	}
};

static std::vector<term*> LexicalAnalyser(std::string str) {
	std::vector<term*> res;

	static const double pi = 3.141592653589793;
	static const double e  = 2.718281828459045;

	for (int i = 0; i < str.size(); i++) {

		if (str[i] >= '0' && str[i] <= '9') {
			std::string numstr;
			int f = 1; 

			while (str[i] >= '0' && str[i] <= '9' || str[i]== ',' || str[i] == '.') {
				numstr.push_back(str[i]);
				if (str[i] == '.' || str[i] == ',') f--;
				i++;
			}

			if (f >= 0){
				res.push_back(new number(stod(numstr)));
			}

			else throw "Error";
		}

		if (str[i] >= 'a' && str[i] <= 'z' || str[i] >= 'A' && str[i] <= 'Z') {
			std::string name;

			while (str.size() > i && str[i] != '+' && str[i] != '-' && 
					str[i] != '*' && str[i] != ')' && str[i] != '/' &&
					str[i] != '=' && str[i] != ' ' && str[i] != '^' && str[i] != '(') {
				name.push_back(str[i]);
				i++;
			}

			if (name == "e")
				res.push_back(new number(e));

			else if (name == "pi")
				res.push_back(new number(pi));

			else if (name == "sin")
				res.push_back(new sine);

			else if (name == "abs")
				res.push_back(new module);

			else if (name == "cos")
				res.push_back(new cosine);

			else if (name == "tan")
				res.push_back(new tangent);

			else if (name == "atg")
				res.push_back(new arctangent);

			else
				res.push_back(new var(name));
		}

		if (str[i] == '+')
			res.push_back(new add);

		if (str[i] == '-')
			res.push_back(new sub);

		if (str[i] == '/')
			res.push_back(new divn);

		if (str[i] == '*')
			res.push_back(new mul);

		if (str[i] == '^')
			res.push_back(new ex);

		if (str[i] == '(')
			res.push_back(new openbracket);

		if (str[i] == ')')
			res.push_back(new closebracket);
		
		if (str[i] == '=')
			res.push_back(new eq);
	}

	return res;
}

static bool Parser(std::vector<term*> vcr) {
	enum class State{ S0, Q1, Q2, Q3, Q4, V, U, Eq, F, Error};
	State ka = State::S0;

	for (int i = 0; i < vcr.size(); i++) {
		switch (ka) {

		case State::S0:
			if (vcr[i]->GetType() == "operand")
				ka = State::Q1;
			else if (vcr[i]->GetType() == "var")
				ka = State::V;
			else if (vcr[i]->GetType() == "openbracket")
				ka = State::Q2;
			else if (vcr[i]->GetType() == "sub")
				ka = State::U;
			else if (vcr[i]->GetType() == "function")
				ka = State::F;
			else
				ka = State::Error;
			continue;

		case State::Q1:
			if (vcr[i]->GetType() == "operation" || vcr[i]->GetType() == "sub")
				ka = State::Q3;
			else if (vcr[i]->GetType() == "closebracket")
				ka = State::Q4;
			else
				ka = State::Error;
			continue;

		case State::V:
			if (vcr[i]->GetType() == "operation" || vcr[i]->GetType() == "sub")
				ka = State::Q3;
			else if (vcr[i]->GetType() == "closebracket")
				ka = State::Q4;
			else if (vcr[i]->GetType() == "eq")
				ka = State::Eq;
			else
				ka = State::Error;
			continue;

		case State::Q2:
			if (vcr[i]->GetType() == "openbracket")
				ka = State::Q2;
			else if (vcr[i]->GetType() == "closebracket")
				ka = State::Q4;
			else if (vcr[i]->GetType() == "operand")
				ka = State::Q1;
			else if (vcr[i]->GetType() == "var")
				ka = State::V;
			else if (vcr[i]->GetType() == "sub")
				ka = State::U;
			else if (vcr[i]->GetType() == "function")
				ka = State::F;
			else
				ka = State::Error;
			continue;

		case State::Q3:
			if (vcr[i]->GetType() == "openbracket")
				ka = State::Q2;
			else if (vcr[i]->GetType() == "operand")
				ka = State::Q1;
			else if (vcr[i]->GetType() == "var")
				ka = State::V;
			else if (vcr[i]->GetType() == "function")
				ka = State::F;
			else
				ka = State::Error;
			continue;

		case State::Q4:
			if (vcr[i]->GetType() == "operation" || vcr[i]->GetType() == "sub")
				ka = State::Q3;
			else if (vcr[i]->GetType() == "closebracket")
				ka = State::Q4;
			else
				ka = State::Error;
			continue;

		case State::Eq:
			if (vcr[i]->GetType() == "operand")
				ka = State::Q1;
			else if (vcr[i]->GetType() == "openbracket")
				ka = State::Q2;
			else if (vcr[i]->GetType() == "var")
				ka = State::V;
			else if (vcr[i]->GetType() == "sub")
				ka = State::U;
			else if (vcr[i]->GetType() == "function")
				ka = State::F;
			else
				ka = State::Error;
			continue;

		case State::U:
			if (vcr[i]->GetType() == "operand")
				ka = State::Q1;
			else if (vcr[i]->GetType() == "var")
				ka = State::V;
			else if (vcr[i]->GetType() == "openbracket")
				ka = State::Q2;
			else if (vcr[i]->GetType() == "function")
				ka = State::F;
			else
				ka = State::Error;
			continue;

		case State::F:
			if (vcr[i]->GetType() == "openbracket")
				ka = State::Q2;
			else
				ka = State::Error;
			continue;

		case State::Error:
			break;

		default: ka = State::Error;
		}
	}

	int k = 0;

	for (int i = 0; i < vcr.size(); i++) {
		if (vcr[i]->GetType() == "openbracket")
			k++;
		if (vcr[i]->GetType() == "closebracket")
			k--;
		if (k < 0)
			ka = State::Error;
	}

	if ((ka == State::S0 || ka == State::Q1 || ka == State::Q4 || ka == State::V) && k == 0)
		return true;
	else
		return false;
}


static double Calculate(std::vector<term*> t) {
	MyStack<term*, std::vector> st;

	for (int i = 0; i < t.size(); i++) {

		if (t[i]->GetType() == "operand") {
			st.push(t[i]);
		}

		if (t[i]->GetType() == "operation" || t[i]->GetType() == "sub") {
			operation* op  =  dynamic_cast<operation*>(t[i]);
			number*    y   =  dynamic_cast<number*>(st.top());
			st.pop();
			number* x = dynamic_cast<number*>(st.top());
			st.pop();

			double nval = op->calc(x->value(), y->value());

			st.push(new number(nval));
		}
	}
	if (t.size() > 0) {
		number* res = dynamic_cast<number*>(st.top());
		return res->value();
	}
	return 0;
}

static std::vector<term*> ToPostfix(std::vector<term*> vr) {

	MyStack<term*, std::vector> st;
	std::vector<term*> PstForm;

	if (vr.size() == 0)
		return PstForm;

	if (vr[0]->GetType() == "sub")
		PstForm.push_back(new number(0));

	if (vr.size() > 1 && vr[0]->GetType() == "var" && vr[1]->GetType() == "eq") {
		std::vector<term*> tmp(vr.size() - 2);

		for (int i = 2; i < vr.size(); i++) 
			tmp[i - 2] = vr[i];

		var* v = dynamic_cast<var*>(vr[0]);
		vars[v->GetName()] = Calculate(ToPostfix(tmp));
	}

	for (int i = 0; i < vr.size(); i++) {

		if (vr[i]->GetType() == "openbracket") {
			st.push(vr[i]);
		}

		if (i + 1 < vr.size() && vr[i]->GetType() == "openbracket" && vr[i + 1]->GetType() == "sub") {
			PstForm.push_back(new number(0));
		}

		if (vr[i]->GetType() == "closebracket") {
			while (st.top()->GetType() != "openbracket") {
				PstForm.push_back(st.top());
				st.pop();
			}
			st.pop();
		}

		if (vr[i]->GetType() == "function") {
			int j = i + 1;
			int k = 1;
			std::vector<term*> arg;
			arg.push_back(vr[j]);

			while (k > 0 && j < vr.size()) {
				j++;
				arg.push_back(vr[j]);

				if (vr[j]->GetType() == "openbracket")
					k++;
				if (vr[j]->GetType() == "closebracket")
					k--;
			}

			functions* fun = dynamic_cast<functions*>(vr[i]);
			PstForm.push_back(new number(fun->calc(Calculate(ToPostfix(arg)))));

			i = j;
			continue;
		}

		if (vr[i]->GetType() == "operand") {
			PstForm.push_back(vr[i]);
		}

		if (vr[i]->GetType() == "var") {
			var* v = dynamic_cast<var*>(vr[i]);
			PstForm.push_back(new number(v->value()));
		}

		if ((vr[i]->GetType()    == "operation" || vr[i]->GetType()    == "sub") && !st.empty() && 
			(st.top()->GetType() == "operation" || st.top()->GetType() == "sub")) {

			operation* OpFromStack   =  dynamic_cast<operation*>(st.top());
			operation* OpFromVector  =  dynamic_cast<operation*>(vr[i]);

			while (!st.empty() && OpFromStack->priority() >= OpFromVector->priority()) {
				PstForm.push_back(st.top());
				st.pop();

				if (!st.empty() && (st.top()->GetType() == "operation" || 
					st.top()->GetType() == "sub"))
					OpFromStack = dynamic_cast<operation*>(st.top());
				else
					break;
			}

			st.push(OpFromVector);
			continue;
		}

		if (vr[i]->GetType() == "operation" || vr[i]->GetType() == "sub") {
			st.push(vr[i]);
		}
	}
	
	while (!st.empty()) {
		PstForm.push_back(st.top());
		st.pop();
	}
	
	return PstForm;
}

static double Translator(std::string str) {
	std::vector<term*> tmp = LexicalAnalyser(str);
	if (Parser(tmp) == false)
		throw "incorrect input";

	if (tmp.size() > 0)
		return Calculate(ToPostfix(tmp));
}

static void GetResult(std::string str) {
	std::vector<term*> tmp = LexicalAnalyser(str);
	if (Parser(tmp) == false)
		throw "incorrect input";

	if (tmp.size() > 1 && tmp[1]->GetType() == "eq")
		Calculate(ToPostfix(tmp));

	else if (tmp.size() > 0)
		std::cout<<Calculate(ToPostfix(tmp))<<std::endl;
}

#endif