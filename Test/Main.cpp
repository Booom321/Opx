#include "Framework/Framework.hpp"

void Run() {
    TestExecutor::Get()->Execute();
    printf("\n");
    system("pause");
}

int main() {
    Run();
    return 0;
}
