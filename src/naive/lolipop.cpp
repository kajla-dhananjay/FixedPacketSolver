#include<bits/stdc++.h>
using namespace std;

int main()
{
    int N;
    std::cin >> N;
    std::cout << "sym unweighted" << std::endl;
    for(int i = 1; i <= N; i++)
    {
        for(int j = i+1; j <= N; j++)
        {
            std::cout << i << ' ' << j << std::endl;
        }
    }
    for(int i = N+1; i <= 2 * N; i++)
    {
        std::cout << i << ' ' << i-1 << std::endl;
    }
}