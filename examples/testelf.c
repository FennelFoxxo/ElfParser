// Look for these symbols in the output ELF file!

static char mychar1;
static char mychar2;

static int myint1;
static int myint2;

static int myspecialint1 __attribute__((section(".myspecialsection1")));
static int myspecialint2 __attribute__((section(".myspecialsection2")));

int main() {
    return 0;
}

int myaddfunc (int a, int b) {
    return a + b;
}

int mysubfunc (int a, int b) {
    return a - b;
}

int mymulfunc (int a, int b) {
    return a * b;
}

int mydivfunc (int a, int b) {
    return a / b;
}
