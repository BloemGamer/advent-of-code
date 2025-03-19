#include <cstdint>
#include <cstring>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "../libs/old/bloem.h"

struct Lan_Party
{
    union
    {
        uint16_t i;
        char c[2];
    }a, b, c;
    
};


struct Connection
{
    union
    {
        uint16_t i;
        char c[2];
    }a, b;
};



std::vector<Lan_Party> parties;

struct Connection* connections;

void part1(void);
void part2(void);
void fix_data(void);
void bubble_sort(uint16_t* arr, size_t n);
void swap(uint16_t* xp, uint16_t* yp);

filecontent file;

int main(int argc, char** argv)
{
    file.give_argv(argv);
    file.fix_file("M");
    fix_data();
    part1();
    part2();
}

void part1(void)
{
    long long answer = 0;

    for(size_t i = 0; i < parties.size(); i++)
    {
        if((parties[i].a.c[0] == 't' || parties[i].b.c[0] == 't' || parties[i].c.c[0] == 't'))
        {
            //std::cout << parties[i].a.c[0] << parties[i].a.c[1] << parties[i].b.c[0] << parties[i].b.c[1] << parties[i].c.c[0] << parties[i].c.c[1] << "\n";
            answer++;
        }
    }

    std::cout << "Part 1: " << answer << "\n";
}

void part2(void)
{
    std::vector<uint16_t> big_lan, current_lan;
    size_t i, j, k, l;
    for(i = 0; i < file.amount_lines * 2; i++)
    {
        current_lan.clear();
        current_lan.push_back(connections[i].a.i);
        for(bool added = true; added; added = false)
        {
            for(j = 0; j < file.amount_lines * 2; j++)
            {
                bool endl = false;
                for(k = 0; k < current_lan.size(); k++)
                {
                    if(connections[j].a.i == current_lan[k])
                        goto already_in_lan;
                }
                for(k = 0; k < current_lan.size(); k++)
                {
                    endl = true;
                    bool endk = false;
                    for(l = 0; l < file.amount_lines * 2; l++)
                    {
                        if(connections[j].a.i != connections[l].a.i)
                            continue;
                        if(connections[l].b.i == current_lan[k])
                        {
                            endk = true;
                            break;
                        }
                    }
                    if(!endk)
                    {
                        endl = false;
                        break;
                    }
                }
                if(endl)
                {
                    current_lan.push_back(connections[j].a.i);
                }

            already_in_lan:;
            }
            if(current_lan.size() > big_lan.size())
                std::swap(current_lan, big_lan);
        }
    }
    union
    {
        uint16_t i;
        char c[2];
    }tmp, tmp2;
    uint16_t* arr = (uint16_t*)alloca(big_lan.size() * sizeof(uint16_t));
    memset((void*)arr, 0, big_lan.size() * sizeof(uint16_t));

    for(i = 0; i < big_lan.size(); i++)
    {
        tmp.i = big_lan[i];
        tmp2.c[1] = tmp.c[0];
        tmp2.c[0] = tmp.c[1];
        arr[i] = tmp2.i;
    }

    bubble_sort(arr, big_lan.size());

    std::cout << "Part 2:";
    for(i = 0; i < big_lan.size(); i++)
    {
        tmp.i = arr[i];
        std::cout << tmp.c[1] << tmp.c[0] << ",";
        // std::cout << std::hex << tmp.i << ",";
    }
}

void fix_data(void)
{
    connections = new Connection[file.amount_lines * 2]();
    for(size_t i = 0; i < file.amount_lines; i++)
    {
        connections[i*2].a.c[0] = file.file[i][0];
        connections[i*2].a.c[1] = file.file[i][1];

        connections[i*2].b.c[0] = file.file[i][3];
        connections[i*2].b.c[1] = file.file[i][4];


        connections[(i*2)+1].b.c[0] = file.file[i][0];
        connections[(i*2)+1].b.c[1] = file.file[i][1];

        connections[(i*2)+1].a.c[0] = file.file[i][3];
        connections[(i*2)+1].a.c[1] = file.file[i][4];

    }
    struct Lan_Party tmp;
    uint16_t c0, c1;
    for(size_t i = 0; i < file.amount_lines * 2; i++)
    {
        for(size_t j = i + 1; j < file.amount_lines * 2; j++)
        {
            if(connections[i].a.i == connections[j].a.i)
            {
                tmp.a.i = connections[i].a.i;
                tmp.b.i = connections[i].b.i;
                tmp.c.i = connections[j].b.i;
                c0 = connections[i].b.i;
                c1 = connections[j].b.i;
            }
            else 
                continue;
            
            for(size_t k = j + 1; k < file.amount_lines * 2; k++)
            {
                    if((connections[k].a.i == c0 && connections[k].b.i == c1))  
                        parties.push_back(tmp);
            }
        }
    }

    return;
}

void bubble_sort(uint16_t* arr, size_t n)
{
    size_t i, j;
    bool swapped;
    for(i = 0; i < n - 1; i++)
    {
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }
}

void swap(uint16_t* xp, uint16_t* yp)
{
    uint16_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}
