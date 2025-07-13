#include <iostream>
#include <fstream>
#include <vector>

int i=-1, j = 12, k = 2;
void (*fn[100])(void);
std::vector<int> v;
std::string s;
int main()
{
    getline((std::ifstream("txt/02.txt")),s);
    fn[1]=[](){v[v[i+3]]=v[v[i+1]]+v[v[i+2]];i+=4;};
    fn[2]=[](){v[v[i+3]]=v[v[i+1]]*v[v[i+2]];i+=4;};
    fn[99]=[](){i=-1;};

    int (*run2)() = []() -> int
    {
        v.clear();
        for(;(i=s.find(",",i)+1)||(!v.size());v.push_back(atoi(s.c_str()+i)));
        v[1]=j;
        v[2]=k;
        for(i=0;i!=-1;fn[v[i]]()){}
        return v[0];
    };
    
    std::cout << run2() << "\n";
    for(j=0;j<100;j++)
        for(k=0;k<100;k++)
            if(run2()==19690720) std::cout << 100*j+k<<"\n";
}
