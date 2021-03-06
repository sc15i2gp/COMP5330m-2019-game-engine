#include "expression_evaluator.h"

float op_add(float a, float b) { return a + b; }
float op_subtract(float a, float b) { return a - b; }
float op_multiply(float a, float b) { return a * b; }
float op_divide(float a, float b) { return a / b; }
float op_exponent(float a, float b) { return pow(a, b); }
float op_less_than(float a, float b) { return (a < b) ? 1.0 : 0.0; }
float op_greater_than(float a, float b) { return (a > b) ? 1.0 : 0.0; }
float op_less_than_eq(float a, float b) { return (a <= b) ? 1.0 : 0.0; }
float op_greater_than_eq(float a, float b) { return (a >= b) ? 1.0 : 0.0; }
float op_equal(float a, float b) { return (a == b) ? 1.0 : 0.0; }
float op_not_equal(float a, float b) { return (a != b) ? 1.0 : 0.0; }
float op_and(float a, float b) { return (a == 1.0 && b == 1.0) ? 1.0 : 0.0; }
float op_or(float a, float b) { return (a == 1.0 || b == 1.0) ? 1.0 : 0.0; }

typedef float(*operation)(float, float);

struct op_mapping
{
	char* op_symbol;
	int precedence;
	operation op;
};

op_mapping operation_table[13] =
{
	{"^", 8, &op_exponent},
	{"*", 7, &op_multiply},
	{"/", 7, &op_divide},
	{"+", 6, &op_add},
	{"-", 6, &op_subtract},
	{">", 4, &op_greater_than},
	{"<", 4, &op_less_than},
	{">=", 4, &op_greater_than_eq},
	{"<=", 4, &op_less_than_eq},
	{"==", 4, &op_equal},
	{"!=", 4, &op_not_equal},
	{"&&", 3, &op_and},
	{"||", 2, &op_or}
};

op_mapping get_op_mapping(char* op)
{
	for (int i = 0; i < 13; i++) if (are_operators_equal(operation_table[i].op_symbol, op)) return operation_table[i];
}

operation find_operation(char* op)
{
	return get_op_mapping(op).op;
}

int get_operator_precedence(char* op)
{
	return get_op_mapping(op).precedence;
}

bool has_greater_precedence(char* x, char* y)
{
	int p_x = get_operator_precedence(x);
	int p_y = get_operator_precedence(y);

	return p_x > p_y;
}

bool has_equal_precedence(char* x, char* y)
{
	int p_x = get_operator_precedence(x);
	int p_y = get_operator_precedence(y);

	return p_x == p_y;
}

float compute_postfix_string_result(char* str, int str_length)
{
	STACK(float) s;
	arithmetic_tokeniser tokeniser = { str, str_length };

	for (arithmetic_token token = tokeniser.get_next_postfix_token(); token.type != token_end && token.type != token_invalid; token = tokeniser.get_next_postfix_token())
	{//For each token in input rpn string
		if (token.type == token_operator)
		{
			float b = s.pop_state();
			float a = s.pop_state();
			float r = 0.0;
			operation o = find_operation(token.str);
			r = o(a, b);
			s.push_state(r);
		}
		else if (token.type == token_real)
		{
			float f = read_real(token.str);
			s.push_state(f);
		}
	}
	return s.pop_state();
}

bool is_left_associative(char c)
{
	return c != '^';
}

void pop_operator(STACK(arithmetic_token)* s, char_queue* output_queue)
{
	arithmetic_token op = s->pop_state();
	if (op.str[0] != '(')
	{
		output_queue->push(op.str, op.length);
		output_queue->push(" ", 1);
	}
}

char top_operator(STACK(arithmetic_token)* s)
{
	return *s->read_state().str;
}

bool is_valid_infix_expr(char* input, int input_length)
{
	arithmetic_tokeniser tokeniser = { input, input_length };
	arithmetic_token token;
	for (token = tokeniser.get_next_infix_token(); token.type != token_end && token.type != token_invalid; token = tokeniser.get_next_infix_token());
	return token.type == token_end;
}

//NOTE: Imlpementation of shunting yard algorithm
//NOTE: This function cannot tell if a string is already postfix, if the input string is already postfix, then result could be 
//	an incorrectly formatted string
void convert_expression_to_postfix(char* input, int input_length)
{
	STACK(arithmetic_token) s;

	char_queue output_queue;
	arithmetic_tokeniser tokeniser = { input, input_length };

	for (arithmetic_token token = tokeniser.get_next_infix_token(); token.type != token_end && token.type != token_invalid; token = tokeniser.get_next_infix_token())
	{//For each token in input str
		if (token.type == token_real)
		{
			output_queue.push(token.str, token.length);
			output_queue.push(" ", 1);
		}
		else if (token.type == token_operator)
		{
			while (!s.is_empty() && (has_greater_precedence(s.read_state().str, token.str) ||
				(has_equal_precedence(s.read_state().str, token.str) &&
					is_left_associative(*s.read_state().str))) &&
				top_operator(&s) != '(')
			{
				//Pop operator from token stack until stack's top token has: lower precedence than current token's;
				//equal precedence as current token's and current token is right associative (e.g. exponent)
				pop_operator(&s, &output_queue);
			}
			s.push_state(token);
		}
		if (token.type == token_open_paren) s.push_state(token);
		if (token.type == token_close_paren)
		{
			while (*s.read_state().str != '(')
			{
				pop_operator(&s, &output_queue);
			}
			s.pop_state();
		}
	}
	while (!s.is_empty())
	{//Pop remaining operator tokens from stack to output queue
		pop_operator(&s, &output_queue);
	}
	overwrite_string(input, output_queue.char_array, 0, input_length);
}