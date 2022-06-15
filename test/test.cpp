#include<iostream>
#include<scip/scip.h>
#include <scip/scipdefplugins.h>
#include "objscip/objscip.h"
#include "objscip/objscipdefplugins.h"

/*export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ray/scip/scipoptsuite-7.0.0/build/lib*/
// in my env, I needed to directry link.

using namespace scip;

/*LP optimization

min
 3x1 + 2x2
subject to 
 x1 + x2 <=4
 2x1 + x2 <=5
 -x1 + x2 >=2
 x1,x2 >= 0

*/

int main(void){
    SCIP *solver;

    /* initialize SCIP environment */
    SCIP_CALL( SCIPcreate(&solver) );

    /* include default plugins */
    SCIP_CALL( SCIPincludeDefaultPlugins(solver) );

    SCIP_CALL( SCIPcreateProbBasic(solver, "SCIP_example"));

    /*you can change the objective by min or max this line.By default scip minimize the objective function*/
    //SCIP_CALL(SCIPsetObjsense(solver, SCIP_OBJSENSE_MAXIMIZE));

    SCIP_VAR* x1 = nullptr;
    
    SCIP_CALL( SCIPcreateVarBasic(solver,
    &x1,                       // reference to the variable
    "x1",                      // name
    0.0,                       // lower bound
    SCIPinfinity(solver),        // upper bound
    3.0,                       // objective coefficient.
    SCIP_VARTYPE_CONTINUOUS)); // variable type
                                
    SCIP_CALL( SCIPaddVar(solver, x1) );  //Adding the first variable to scip

    /*defining the other variable x2*/
    SCIP_VAR* x2 = nullptr;
    SCIP_CALL( SCIPcreateVarBasic(solver,
    &x2,                       // reference to the variable
    "x2",                      // name
    0.0,                       // lower bound
    SCIPinfinity(solver),        // upper bound
    2.0,                       // objective coefficient.
    SCIP_VARTYPE_CONTINUOUS)); // variable type
                                
    SCIP_CALL( SCIPaddVar(solver, x2) );  //Adding the first variable to scip

    //addding constraint
    SCIP_CONS* cons1 = nullptr;;

    SCIP_CALL(SCIPcreateConsBasicLinear(solver,                 // SCIP pointer
                                        &cons1,               // pointer to SCIP constraint
                                        "cons1",              // name of the constraint
                                        0,                    // How many variables are you adding  now
                                        nullptr,              // an array of pointers to various variables
                                        nullptr,              // an array of values of the coefficients of the corresponding vars
                                        -SCIPinfinity(solver),  // LHS of the constraint
                                        4));                  // RHS of the constraint

    SCIP_CALL( SCIPaddCoefLinear(solver, cons1, x1, 1.0));      //Adding the variable x1 with A matrix coeffient of 1.0

    SCIP_CALL( SCIPaddCoefLinear(solver, cons1, x2, 1.0));      //Adding the variable x2 with A matrix coeffient of 1.0

    SCIP_CALL( SCIPaddCons(solver, cons1));
    

    //adding second constraint 
     SCIP_CONS* cons2 = nullptr;;
    
    SCIP_CALL(SCIPcreateConsBasicLinear(solver,
                                        &cons2,
                                        "cons2",
                                        0,
                                        nullptr,
                                        nullptr,
                                        -SCIPinfinity(solver),
                                        5));
    
    SCIP_CALL( SCIPaddCoefLinear(solver, cons2, x1, 2.0));
    
    SCIP_CALL( SCIPaddCoefLinear(solver, cons2, x2, 1.0));
    
    SCIP_CALL( SCIPaddCons(solver, cons2));
    
    
    //adding the last constraint
    SCIP_CONS* cons3 = nullptr;;
    
    SCIP_CALL(SCIPcreateConsBasicLinear(solver,
                                        &cons3,
                                        "cons3",
                                        0,
                                        nullptr,
                                        nullptr,
                                        -SCIPinfinity(solver),
                                        -2.0));
    
    SCIP_CALL( SCIPaddCoefLinear(solver, cons3, x1, -1.0));
    
    SCIP_CALL( SCIPaddCoefLinear(solver, cons3, x2, 1.0));
    
    SCIP_CALL( SCIPaddCons(solver, cons3));


    //Scip releasing all the constraints
    SCIP_CALL( SCIPreleaseCons(solver, &cons1) );
    SCIP_CALL( SCIPreleaseCons(solver, &cons2) );
    SCIP_CALL( SCIPreleaseCons(solver, &cons3) );


     //Solving the problem
    SCIP_CALL( SCIPsolve(solver) );

    //solution object to take a look at the solution
    SCIP_SOL* sol;
    sol = SCIPgetBestSol(solver);
    std::cout << "x1: " << SCIPgetSolVal(solver, sol, x1) << " " << "x2: " << SCIPgetSolVal(solver, sol, x2) << "\n";
    
    SCIP_CALL( (SCIPwriteOrigProblem(solver, "scip_example.lp", nullptr, FALSE)));

    //Freeing the variables
    SCIP_CALL( SCIPreleaseVar(solver, &x1));
    SCIP_CALL( SCIPreleaseVar(solver, &x2));
    
    //Freeing the SCIP environment
    SCIP_CALL( SCIPfree(&solver) );
    

    
    return 0;
}