#pragma once
#include "translator.h"
#include <gtest.h>

TEST(Translator, NoThrowWhenEmptyInput) {
	std::string s = " ";
	ASSERT_NO_THROW(Translator(s));
}

TEST(Translator, OneNumberInput) {
	std::string s = "1";
	EXPECT_EQ(1,Translator(s));
}

TEST(Translator, NumberWithPointInput) {
	std::string s = "1.5";
	EXPECT_EQ(1.5,Translator(s));
}

TEST(Translator, ThrowWhenNumberWithTwoPointInput) {
	std::string s = "1.54.16";
	ASSERT_ANY_THROW(Translator(s));
}

TEST(Translator, AddTwoNumber) {
	std::string s = "1 + 1";
	EXPECT_EQ(2,Translator(s));
}

TEST(Translator, SubTwoNumber) {
	std::string s = "2 - 1";
	EXPECT_EQ(1,Translator(s));
}

TEST(Translator, MulTwoNumber) {
	std::string s = "2 * 2";
	EXPECT_EQ(4,Translator(s));
}

TEST(Translator, Exponent) {
	std::string s = "2 ^ 3";
	EXPECT_EQ(8,Translator(s));
}

TEST(Translator, AnyThrowWhenDivByZero) {
	std::string s = "2 / 0";
	ASSERT_ANY_THROW(Translator(s));
}

TEST(Translator, NoThrowWhenDiv) {
	std::string s = "2 / 2";
	EXPECT_EQ(1,Translator(s));
}

TEST(Translator, CorrectOrderOfOperation) {
	std::string s = "2 * 2 - 9 / 3";
	EXPECT_EQ(1,Translator(s));
}

TEST(Translator, CorrectOrderOfOperationWithBracket) {
	std::string s = "2 * (3 - 9) / 3";
	EXPECT_EQ(-4,Translator(s));
}

TEST(Translator, IncorrectOrderBracket) {
	std::string s = "(2+3))(1-4";
	ASSERT_ANY_THROW(Translator(s));
}

TEST(Translator, ThrowWhenDifferentNumberOfBracket) {
	std::string s = "((2+3)";
	ASSERT_ANY_THROW(Translator(s));
}

TEST(Translator, NoThrowWhenSeveralBracket) {
	std::string s = "(((1+1)))";
	EXPECT_EQ(2,Translator(s));
}

TEST(Translator, NoThrowWhenOnlyNumberInBracket) {
	std::string s = "1+(1)";
	EXPECT_EQ(2,Translator(s));
}

TEST(Translator, CorrectOrderOfOperationWithExponent) {
	std::string s = "19 - 2*3^2";
	EXPECT_EQ(1,Translator(s));
}

TEST(Translator, NumberWithUnaryMinus) {
	std::string s = "-1";
	EXPECT_EQ(-1,Translator(s));
}

TEST(Translator, AddWithUnaryMinus) {
	std::string s = "-1+2";
	EXPECT_EQ(1,Translator(s));
}

TEST(Translator, SubWithUnaryMinus) {
	std::string s = "-1-2";
	EXPECT_EQ(-3,Translator(s));
}

TEST(Translator, MulWithUnaryMinus) {
	std::string s = "-2*2";
	EXPECT_EQ(-4,Translator(s));
}

std::map<std::string, double> vars;
TEST(Translator, InitialiseVariable) {
	std::string s = "a = 1";
	Translator(s);
	EXPECT_EQ(1,Translator("a"));
}

TEST(Translator, ThrowWhenUseNotInitialiseVariable) {
	std::string s = "1 + d";
	ASSERT_ANY_THROW(Translator(s));
}

TEST(Translator, InitialiseVariableWithLenGreaterThanOne) {
	std::string s = "A1_t = 1";
	Translator(s);
	EXPECT_EQ(1,Translator("A1_t"));
}

TEST(Translator, InitialiseVariableOfExpression) {
	std::string s = "a = 2-1";
	Translator(s);
	EXPECT_EQ(1,Translator("a"));
}

TEST(Translator, InitialiseSeveralVariables) {
	std::string s = "a = b = 1";
	Translator(s);
	EXPECT_EQ(1,Translator("a"));
	EXPECT_EQ(1,Translator("b"));
}

TEST(Translator, InitialiseVariableOfExpressionWithThisVariable) {
	std::string s0 = "a = 1";
	std::string s1 = "a = a + 1";
	Translator(s0);
	Translator(s1);
	EXPECT_EQ(2,Translator("a"));
}

TEST(Translator, ConstNumberPI) {
	std::string s = "pi";
	EXPECT_EQ(3.141592653589793, Translator(s));
}

TEST(Translator, ConstNumberE) {
	std::string s = "e";
	EXPECT_EQ(2.718281828459045, Translator(s));
}

TEST(Translator, ThrowWhenInitialiseVariableWithNameConst) {
	std::string s = "pi = 1";
	ASSERT_ANY_THROW(Translator(s));
}

TEST(Translator, ThrowWhenInitialiseNumber) {
	std::string s = "1 = 2";
	ASSERT_ANY_THROW(Translator(s));
}

TEST(Translator, FunctionSine) {
	std::string s = "sin(pi/2)";
	EXPECT_EQ(1,Translator(s));
}

TEST(Translator, FunctionCosine) {
	std::string s = "cos(pi)";
	EXPECT_EQ(-1,Translator(s));
}

TEST(Translator, FunctionModule) {
	std::string s = "abs(-1)";
	EXPECT_EQ(1,Translator(s));
}

TEST(Translator, FunctionTangent) {
	std::string s = "tan(0)";
	EXPECT_EQ(0,Translator(s));
}

TEST(Translator, FunctionArctangent) {
	std::string s = "atg(0)";
	EXPECT_EQ(0,Translator(s));
}

TEST(Translator, FunctionWithExpression) {
	std::string s = "abs(-1+(4-5))";
	EXPECT_EQ(2,Translator(s));
}

TEST(Translator, FunctionInFunction) {
	std::string s = "sin(sin(pi/2))";
	ASSERT_NO_THROW(Translator(s));
}

TEST(Translator, FunctionInExpression) {
	std::string s = "7-(abs(1-(4+1))+2)";
	EXPECT_EQ(1,Translator(s));
}
