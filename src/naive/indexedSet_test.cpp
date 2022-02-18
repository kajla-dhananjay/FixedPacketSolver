#include<bits/stdc++.h>
#include "indexedSet.h"

int main()
{
    std::vector<int> v{4,1,3,2};
    indexedSet<int> st(v);
    std::cout << st.getMax() << std::endl;
    st.setVal(1,5);
    std::cout << st.getMax() << std::endl;
    std::cout << st.getVal(1) << std::endl;
    st.setVal(0,6);
    std::cout << st.getMax() << std::endl;
}