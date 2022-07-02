#include<iostream>
#include<scip/scip.h>
#include <scip/scipdefplugins.h>
#include "objscip/objscip.h"
#include "objscip/objscipdefplugins.h"
#include <vector>
#include <random>
#include <stdlib.h>


#include "scip/def.h"
#include "scip/type_cons.h"
#include "scip/type_lp.h"
#include "scip/type_retcode.h"
#include "scip/type_scip.h"
#include "scip/type_sepa.h"
#include "scip/type_sol.h"
#include "scip/type_var.h"
/*export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ray/scip/scipoptsuite-7.0.0/build/lib*/
// in my env, I needed to directry link.

using namespace scip;


// question producer
void Q_prod(std::vector<int>& v,std::vector<int> w,int *n);

int main(void){

    int n;      // package num
    int max_w = 100;      // max weight
    std::random_device rnd;     // 非決定的な乱数生成器を生成
    std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    std::uniform_int_distribution<> rand100(0, 99);        // [0, 99] 範囲の一様乱数
    std::uniform_int_distribution<> rand(10, 30);         // produce a randam integer from 100 to 1000
    std::uniform_int_distribution<> rand_w(1,20);
    n = rand(mt); 
    std::cout << n << std::endl;

    std::vector<long long int> v;
    std::vector<long long int> w;

    // prod problem
    for(int i = 0;i < n;i++){
        w.push_back(rand_w(mt));
        v.push_back(rand_w(mt));
        std::cout << w[i] << " " << v[i] << std::endl;
    }

    // dp solution
    long long int dp[n+1][max_w+1];     //dp[i][j] := max val when using package 0 to i on constraint weight <= j
    
    // init 
    for(int j=0;j<=max_w;j++){
        dp[0][j] = 0;
    }

    // dp (just be cautious that v and w's index is small by  one)
    for(int i = 1;i <= n;i++){
        for(int j = 0;j <= max_w;j++){
            if(j - w[i-1] >= 0){
                dp[i][j] = std::max(dp[i-1][j-w[i-1]] + v[i-1],dp[i-1][j]);
            }
            else{
                dp[i][j] = dp[i-1][j];
            }
        }
    }

    std::cout << dp[n][max_w] << std::endl;

    // scip solution
    SCIP *solver;
    SCIP *subscip;
    // initialize SCIP environment
    SCIP_CALL( SCIPcreate(&solver) );

    SCIP_CONS** conss;
    SCIP_Longint* vals;// = &v[0];
    SCIP_CONS* cons;
    SCIP_VAR* var;
    SCIP_Longint* weights;
    SCIP_Longint capacity;
    SCIP_Real dual;
    SCIP_VAR**  vars;                // variable array for the items

    int nitems;
    int nvars;
    int c;

    nitems = n;
    conss = (SCIP_CONS**)calloc(sizeof(SCIP_CONS*),n);
    weights = &w[0];
    capacity = max_w;
    nvars = 0;

    std::cout << "1" << std::endl;

    SCIP_CALL( SCIPallocBufferArray(subscip, &vals, nitems) );
        std::cout << "4" << std::endl;
    /* create for each order, which is not assigned yet, a variable with objective coefficient */
    for( c = 0; c < nitems; ++c )
    {
        cons = conss[c];
        std::cout << "5" << std::endl;
        /* check if each constraint is setppc constraint */
        assert( !strncmp( SCIPconshdlrGetName( SCIPconsGetHdlr(cons) ), "setppc", 6) );

        /* constraints which are (locally) disabled/redundant are not of
        * interest since the corresponding job is assigned to a packing
        */
        if( !SCIPconsIsEnabled(cons) )
            continue;

        if( SCIPgetNFixedonesSetppc(solver, cons) == 1 )
        {
            /* disable constraint locally */
            SCIP_CALL( SCIPdelConsLocal(solver, cons) );
            continue;
        }

        /* dual value in original SCIP */
        dual = SCIPgetDualsolSetppc(solver, cons);
        
        SCIP_CALL( SCIPcreateVarBasic(subscip, &var, SCIPconsGetName(cons), 0.0, 1.0, dual, SCIP_VARTYPE_BINARY) );
        SCIP_CALL( SCIPaddVar(subscip, var) );
        std::cout << "3" << std::endl;
        vals[nvars] = weights[c];
        vars[nvars] = var;
        nvars++;

        /* release variable */
        SCIP_CALL( SCIPreleaseVar(subscip, &var) );
    }

       std::cout << "2" << std::endl;

    /* create capacity constraint */
    SCIP_CALL( SCIPcreateConsBasicKnapsack(subscip, &cons, "capacity", nvars, vars, vals,
            capacity) );
    
    SCIP_CALL( SCIPaddCons(subscip, cons) );
    SCIP_CALL( SCIPreleaseCons(subscip, &cons) );


    // Solving the problem
    SCIP_CALL( SCIPsolve(solver) );


    SCIPfreeBufferArray(subscip, &vals);

    }

