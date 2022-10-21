#include <iostream>
#include <vector>
#include "query.cpp"
#include "student.cpp"
using namespace std;

int main(){
    database_t fake_data_base;
    student_t student1={43,"Philipe","Jorissen","physique",28/02/1960};
    student_t student2={127,"Marcel","Jorissen","math",10/05/1989};
    student_t student3={96,"Philipe","Boubakar","physique",02/01/2001};
    student_t student4={43,"michel","benzema","psycho",15/03/1970};
    student_t student5={127,"Adam","plustard","tourisme",18/11/2008};
	
	student_t student6={88, "Jules", "César", "youyou", 19/02/2182};

    student_t student_test_list[5]={student1,student2,student3,student4,student5};
    fake_data_base.data=student_test_list;
    fake_data_base.lsize=5;
    fake_data_base.psize=10*sizeof(student_t);
        insert(&student6,&fake_data_base);
		cout<<"insert passé"<<endl;
		for (int i=0; i<fake_data_base.lsize; i++){
			char buffer[1000];
			student_to_str(buffer, &fake_data_base.data[i]);
			cout<<buffer<<endl;}
        //cout<<select("fname","Jorissen",&fake_data_base)<<endl;
        //delete("fname","Jorissen",&fake_data_base);
        //update();
}
