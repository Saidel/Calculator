#ifndef TEST_1_H
#define TEST_1_H

#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;



double RandD()
{
    double f = (double)rand() / RAND_MAX;
    return roundf((f * (20)-10)*100)/100;
}
string Double_To_String(double d){
    string Res = to_string(roundf(d*100.0)/100.0);
    while(Res.back() == '0') Res.pop_back();
    if(Res.back() == '.') Res.pop_back();
    if(Res.compare("-0") == 0) Res = "0";

    return Res;
}


string GenRandomExpression(double & Res, int brlvl = 0){
    Res = RandD();
    double Last_Num = Res;
    string out = Double_To_String(Res);
    int n = rand()%3+1;

    for(int i = 0; i<n; i++){
        int operation = rand()%4;
        double D1;
        int is_expr = rand()%2;
        string S;
        if(is_expr && brlvl<3) S = GenRandomExpression(D1, brlvl+1);
        else{
            D1 = RandD();
            S = Double_To_String(D1);
        }

        if(operation == 0){
            out += "+";

            Res += D1;
            Last_Num = D1;
        }else if(operation == 1){
            out += "-";

            Res -= D1;
            Last_Num = -D1;
        }else if(operation == 2){
            out += "*";

            Res -= Last_Num;
            Last_Num *= D1;
            Res += Last_Num;

        }else if(operation == 3){
            out += "/";

            Res -= Last_Num;
            Last_Num /= D1;
            Res += Last_Num;
        }

        if(is_expr && brlvl<5) out += "("+S+")";
        else out += S;
    }

    return out;
}

string GenExpression(double & Res){
    string out = GenRandomExpression(Res);

    Res = roundf(Res*100)/100;
    return out;
}

#define BOOST_TEST_MODULE boost_test_macro_overview
#include <boost/test/included/unit_test.hpp>
#include "calculator.h"


BOOST_AUTO_TEST_CASE( test_macro_overview )
{
    srand(time(0));
    for(int i = 0; i<100000; i++){
        double Res;
        string out = GenExpression(Res);
        string R = Double_To_String(Res);
        string Calc = Calculate(out);
        out+= " | "+Calc+"|"+R;

        BOOST_TEST((R.compare(Calc) == 0 || Calc == "ошибка деления на ноль" || Calc == "по модулю не меньше 10000000"), "\n"+out);
    }
}


#endif
