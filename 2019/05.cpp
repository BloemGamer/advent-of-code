#include <iostream>
#include <fstream>
#include <vector>

#define I3 5

int i=-1, j = 12, k = 2;
int p1, p2, p3;
void (*fn[100])(void);
std::vector<int> v;
std::string s;
int main()
{
    getline((std::ifstream("txt/05.txt")),s);

    fn[1]=[](){v[p3]=v[p1]+v[p2];i+=4;};
    fn[2]=[](){v[p3]=v[p1]*v[p2];i+=4;};
    fn[3]=[](){v[p1]=I3;i+=2;};
    fn[4]=[](){std::cout<<v[p1]<<"\n";i+=2;};

    fn[5]=[](){v[p1]?i=v[p2]:i+=3;};
    fn[6]=[](){v[p1]?i+=3:i=v[p2];};
    fn[7]=[](){v[p3]=v[p1]<v[p2];i+=4;};
    fn[8]=[](){v[p3]=v[p1]==v[p2];i+=4;};

    fn[99]=[](){i=-1;};

    int (*run)() = []() -> int
        {
            v.clear();
            for(;(i=s.find(",",i)+1)||(!v.size());v.push_back(atoi(s.c_str()+i)));
            for(i=0;i!=-1;fn[v[i]%100]()){p1=(v[i]/100%10?i+1:v[i+1]);p2=(v[i]/1000%10?i+2:v[i+2]);p3=(v[i]/10000%10?i+3:v[i+3]);}
            return v[0];
        };
    run();
}
