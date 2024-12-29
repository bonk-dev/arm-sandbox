#include <cstdio>

int main() {
	FILE* test_file = fopen("/tmp/test2.txt", "r");
	if (test_file == nullptr) {
		puts("Error while reading file");
	}
	else {
		char some_string[100];
		fscanf(test_file, "%s", some_string);

		puts(some_string);
	}
}