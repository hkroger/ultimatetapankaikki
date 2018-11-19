#ifndef __TK__KEYB__
#define __TK__KEYB__

struct KeyboardHandler
{
	char state[100];
	struct
	{
		int Non[100];
	} tt;

	void init(int) {}
	void deinit() {}
	void clear_stack() {}
	int kbhit() { return 0; }
	int getch() { return 0; }
};

#endif
