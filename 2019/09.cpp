#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

#define I3 2

long long i=-1, p1, p2, p3, b=0;
void (*fn[100])(void);
std::vector<long long> v;
std::string s;
int main()
{
    getline((std::ifstream("txt/09.txt")),s);

    fn[1]=[](){v[p3]=v[p1]+v[p2];i+=4;};
    fn[2]=[](){v[p3]=v[p1]*v[p2];i+=4;};
    fn[3]=[](){v[p1]=I3;i+=2;};
    fn[4]=[](){std::cout<<v[p1];i+=2;};
    fn[5]=[](){v[p1]?i=v[p2]:i+=3;};
    fn[6]=[](){v[p1]?i+=3:i=v[p2];};
    fn[7]=[](){v[p3]=v[p1]<v[p2];i+=4;};
    fn[8]=[](){v[p3]=v[p1]==v[p2];i+=4;};
    fn[9]=[](){b+=v[p1];i+=2;};
    fn[99]=[](){i=-1;};
    for(;(i=s.find(",",i)+1)||(v.empty());v.push_back(atoll(s.c_str()+i)));
    for(size_t j = v.size(); ++j < 9876543; v.push_back(0));
    for(i=0;i!=-1;fn[v[i]%100]())
    {
        p1=(v[i]/100%10?(v[i]/100%10-1?b+v[i+1]:i+1):v[i+1]);
        p2=(v[i]/1000%10?(v[i]/1000%10-1?b+v[i+2]:i+2):v[i+2]);
        p3=(v[i]/10000%10?(v[i]/10000%10-1?b+v[i+3]:i+3):v[i+3]);
    }
}
