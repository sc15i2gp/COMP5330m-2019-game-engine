#pragma once
#define STACK_DEFAULT(type) type##_stack_32
#define STACK_WITH_LIMIT(type, limit) type##_stack_##limit

#define GET_3RD_ARG(a1, a2, a3, ...) a3
#define CHOOSE_STACK_MACRO(...) GET_3RD_ARG(__VA_ARGS__, STACK_WITH_LIMIT, STACK_DEFAULT)

#define STACK(...) CHOOSE_STACK_MACRO(__VA_ARGS__)(__VA_ARGS__)
struct turtle_stack_32
{
	turtle states[32] = {};
	int top = 0;

	bool is_empty()
	{
		return this->top == 0;
	}

	inline
		turtle* top_state()
	{
		return this->states + this->top;
	}

	turtle read_state()
	{
		return *(this->top_state() - 1);
	}

	void push_state(turtle state)
	{
		*(this->top_state()) = state;
		this->top++;
	}

	turtle pop_state()
	{
		this->top--;
		return *this->top_state();
	}
};

