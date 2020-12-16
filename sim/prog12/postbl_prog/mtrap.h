#define die(str, ...) { printm("%s:%d: " str "\n", __FILE__, __LINE__, ##__VA_ARGS__); sysexit(1); }

#define assert(x) { if (!(x)) { die("assertion fail: %s", #x ); } }
