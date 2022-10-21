#include <iostream>
#include <vector>
#include "query.h"
using namespace std;

int main(){
    database_t fake_data_base;
    student_t student1={43,"Philipe","Jorissen","physique",28/02/1960};
    student_t student2={127,"Marcel","Jorissen","math",10/05/1989};
    student_t student3={96,"Philipe","Boubakar","physique",02/01/2001};
    student_t student4={43,"michel","benzema","psycho",15/03/1970};
    student_t student5={127,"Adam","plustard","tourisme",18/11/2008};

    student_t student_test_list[]={student1,student2,student3,student4,student5};
    fake_data_base.data=student_test_list;
    fake_data_base.lsize=5;
    fake_data_base.psize=10;
    for (int i=0; i<4;i++ ){
        insert(student_test_list[i],&fake_data_base);
        cout<<fake_data_base<<endl;
        cout<<select("fname","Jorissen",&fake_data_base)<<endl;
        //delete("fname","Jorissen",&fake_data_base);
        //update();
    }
}