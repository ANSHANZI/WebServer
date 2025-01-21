#include <iostream>
#include <vector>

void foo(std::vector<int>& v) {  // 左值引用
    v.push_back(1);
    std::cout << "foo: ";
    for (int i : v) std::cout << i << " ";
    std::cout << std::endl;
}

void bar(std::vector<int>&& v) { // 右值引用
    v.push_back(2);
    std::cout << "bar: ";
    for (int i : v) std::cout << i << " ";
    std::cout << std::endl;
}

int main() {
    std::vector<int> v1 = {1, 2, 3};
    foo(v1);  // 左值传递
    bar(std::vector<int>{4, 5, 6});  // 右值传递
}
