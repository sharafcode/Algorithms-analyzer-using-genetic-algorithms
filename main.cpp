/// ============================================================================
/// Name        : Genetic-algorithm.cpp
/// Author      : Abd L-Rahman Sharaf
/// Email       : abdlrahman.sharaf@gmail.com
/// Copyright   : MIT license
/// Description : Hello World in C++, Ansi-style
/// ============================================================================

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <time.h>
#include <math.h>
#include <sstream>
#include <cstdlib>

using namespace std;

string max_gene, min_gene; //Global variables to use through all generations.
int avg_gene,sum_gene;
int n,m;

typedef vector<string> vec;      //Creating maps and vectors typedef to use in every scope in the code.
typedef map <double,string> ms;
typedef unsigned int ui;


int bin_to_dec(string str0){       //Function to change the binary numbers to decimal numbers used for the first entry generation only.
    int dec = 0, rem, base = 1;
    char s[str0.length()];
    for(ui i=0; i<str0.length(); i++){ // Convert string to array of characters.
            s[i]=str0[i];
    }
     const char *str1=s;
     int num= atoi(str1);
     while (num > 0){
        rem = num % 10;
        dec = dec + rem * base;
        base = base * 2;
        num = num / 10;
     }
     return dec;
}

string dec_to_bin(int n){  //Representation function to change the decimal numbers to binary numbers to be evaluated in the fitness function.
    string gene;
    int arr[64];
    int i=0,r;
    while(n!=0){
            r = n%2;
            arr[i++] = r;
            n /= 2;
            }
    for(int j=i-1; j>=0; j--){
            stringstream ss;
            ss<< arr[j];
            gene=gene+ss.str();
            }
    if(gene.length()<16){  //Make all genes with the same length of 16 bit in binary representation.
        string temp;
        for(int i=gene.length();i<16;i++)
        {
            temp=temp+"0";
        }
        temp=temp+gene;
        return temp;
        }
    else
        return gene;
}
//Macro to print all vector elements for checking only.
#define print_vector(v) for(vec:: iterator it=v.begin(); it!=v.end(); ++it){cout<<*it<<endl;};
#define print_map(mp) for(ms::iterator it=mp.begin();it!=mp.end();it++){cout<<bin_to_dec(it->second)<<"  =>  "<<it->first<<"  =>  "<<it->second<<endl;};
//Macro to print all map elements with its fitness evaluation.


//Fitness function
double fitness(string str){
     int gene= bin_to_dec(str);
     long double cmp;
     cmp=pow(gene,2)-(gene*log2(gene)); //Function to take the difference between running times (Bubble sort) & (Merge sort) algorithms.
    return cmp;
}


//Mutation function
string mutation(string gene){    //Mutation only for the worst gene with ratio 1:m
    srand(time(NULL));
    int mut=(rand()%gene.length());
    if(gene[mut]=='1')
        gene[mut]='0';
    else
        gene[mut]='1';

    return gene;
}

//Crossover function
string* crossover(string male, string female){
    string* childs = new string[2];
    char gene1[male.size()];
    char gene2[female.size()];
    ui i,j;
//First child crossover
    for(i=0; i<(male.size()/2.0); i++){
        gene1[i]= male[i];
    }

    for(j=i; j<=female.size(); j++){
        gene1[j]=female[j];
    }
    childs[0]=gene1;
//Second child crossover
    for(i=0; i<(female.size()/2.0); i++){
        gene2[i]= female[i];
    }

    for(j=i; j<=male.size(); j++){
        gene2[j]=male[j];
    }
    childs[1]=gene2;

    return childs; //Returns the array of strings of the two children as a pointer.
}
int main()
{
    vec v;
    ms fit_map;
    cout <<"Enter the number of the generations: \n";
    cin>>n;
    cout<<"Enter the number of the genes : \n";
    cin>>m;
    string gen[n][m];
    int arr[m]; //Array for the user decimal inputs first generation.
    int avg[n], sum[n];

    //Get the first population from the user as the first generation in the algorithm.
    cout<<"\t\t ********************************\n";
    cout<<"\t\t Generation "<<0<<" is : "<<endl;
    cout<<"\t\t ********************************\n";
    cout<<"(Enter a decimal numbers for initial population)\n\n";
    for(int l=0; l<m; l++){
        cout<<"Enter the "<<l<<" gene: "<<endl;   // Get all first initial population from user as decimal then convert it to binary.
        cin>>arr[l];
        gen[0][l]=dec_to_bin(arr[l]);
        fit_map[fitness(gen[0][l])]= gen[0][l];
    }
    //delete arr[m];
    string worst;
    string best;
    worst=gen[0][0];
    best=gen[0][0];
    //Get worst and best gene for the first generation.
    for (int in=0; in<m; in++){
        sum[0]+=floor(fitness(gen[0][in]));

        if (fitness(worst)>fitness(gen[0][in])){
            worst= gen[0][in];
        }
        if(fitness(best)<fitness(gen[0][in])){
            best= gen[0][in];
        }
    }
    avg[0]=sum[0]/m;
    //Initialization for the best and worst gene in all generations.
    min_gene=worst;
    max_gene=best;

    //Start evolution next generations from here.
    for(int k=1; k<n; k++){
            int ctr=0, flag=0;
            string ml,fl;

            //Get the crossover elements by iterating through a sorted map and get the two middle elements in it.
            for (ms:: iterator it= fit_map.begin(); it!= fit_map.end(); it++){
                    if(ctr == floor(fit_map.size()/2.0) || ctr == floor(fit_map.size()/2.0)+1){
                        if(flag==0){
                            ml=(it->second);
                            flag++;
                            }
                        else{
                            fl=(it->second);
                            }
                    }
                    ctr++;
            }

            //Print all fitness evaluations in the map with its string.
            cout<<"\n Fitness evaluations => String gene \n";
            print_map(fit_map);
            cout<<"\n\n Average running time through the generation = \t\n"<<avg[k-1]<<endl;
            fit_map.clear();

            //Start to make the genetics operations for the generation.
            ctr=0, flag=0;
            cout<<"Crossover elements are : "<<ml<<"\t"<<fl<<endl;
            string *st= crossover(ml,fl);  //Applying crossover for two genes with average fitness evaluation.
            string child1=(*st);
            st++;
            string child2=(*st);
            cout<<"Crossover 1st child : "<<child1<<"\t\t 2nd child : "<<child2<<endl;
            cout<<"The worst gene in the generation : "<<worst<<endl;
            cout<<"The best gene in the generation : "<<best<<endl;
            string mut_child=mutation(worst);
            cout<<"Mutation for the worst gene : "<<mut_child<<endl;
            gen[k][0]= mut_child;       //Pushing the new comers from the previous generation to the following generation
            gen[k][1]= child1;
            gen[k][2]= child2;
            gen[k][3]= best;
            v.clear();

            //After clearing the vector pushing the new remainder genes into these vector for reproduction.
            for(int j=0; j<m; j++){
                    if((gen[k-1][j]!=worst) && (gen[k-1][j]!=best) && (gen[k-1][j]!=ml) && (gen[k-1][j]!=fl)){
                         v.push_back(gen[k-1][j]);
                    }
            }
            /*
            cout<<"\nVector Elements are :\n"; //If you want to check the remainder elements print all vector elements.
            print_vector(v);
            cout<<endl;
            */

            //Reproduction all the remainder genes into the following generation.
            for(ui j=0; j<v.size(); j++){
                    gen[k][j+4]=v.at(j);
            }
            //Print the genes in the same generation.
            cout<<"\t\t ********************************\n";
            cout<<"\t\t Generation "<<k<<" is : "<<endl;
            cout<<"\t\t ********************************\n";
            for(int f=0; f<m; f++){
                    cout<<"\t\t Gene "<<f<<" is : "<<gen[k][f]<<endl;
                    fit_map[fitness(gen[k][f])]= gen[k][f];
            }
            //Get the worst and the best genes in the generation.
            for (int in=0; in<m; in++){
                    sum[k]+=floor(fitness(gen[0][in]));

                    if (fitness(worst)>fitness(gen[k][in])){
                        worst= gen[k][in];
                        }
            if(fitness(best)<fitness(gen[k][in])){
                    best= gen[k][in];
                    }
            }
            avg[k]=sum[k]/m;

            //Check every gene is the best gene came out of all the generations.
            if(fitness(max_gene)<fitness(best))
                max_gene=best;
            if(fitness(min_gene)>fitness(worst))
                min_gene=worst;

            sum_gene+=avg[k];
            char c;
            cout<<"\t\t\t\t Do you want to continue next generation ('Y' for continue or 'N' for stop)\n";
            cin>>c;
            if(c=='y' || c=='Y' )
                continue;
            if (c=='N' || c=='n')
                break;
    }
    avg_gene= sum_gene/n;
    //Print after all iterations the best and worst gene came out from all the generations.
    cout<<" \t\t *******************************************  \n";
    cout<<"\n\t\t The best running time gene through all generations is :   \n";
    cout<<" \t\t x= "<<bin_to_dec(min_gene)<<"\t Binary representation =  "<<min_gene<<"\t with fitness evaluation =  "<<fitness(min_gene)<<endl;
    cout<<"\n\t\t The worst running time gene through all generations is :   \n";
    cout<<" \t\t x= "<<bin_to_dec(max_gene)<<"\t Binary representation =  "<<max_gene<<"\t with fitness evaluation =  "<<fitness(max_gene)<<endl;
    cout<<"\n\t\t The average running time through all generations is =   \n";
    cout<<" \t\t x= "<<avg_gene<<endl;
    cout<<" \t\t *******************************************  \n";
    return 0;
}
