struct GC {
	char buf[1 << 16];
	size_t bc = 0, be = 0;
	char operator()() {
		if (bc >= be) {
			buf[0] = 0, bc = 0;
			be = fread(buf, 1, sizeof(buf), stdin);
		}
		return buf[bc++]; // returns 0 on EOF
	}
} gc;
int readInt() {
	int a, c;
	while ((a = gc()) < 40);
	while ((c = gc()) >= 48) a = a * 10 + c - 480;
	return a - 48;
}
