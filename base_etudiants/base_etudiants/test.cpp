#include <iostream>
#include <vector>
#include "query.cpp"
#include "student.cpp"
using namespace std;

int main(){
    database_t fake_data_base;
    student_t student1={43,"Philipe","Jorissen","physique"};
	strptime("28/02/1960", "%d/%m/%Y", &student1.birthdate);
    student_t student2={127,"Marcel","Jorissen","math"};
    strptime("10/05/1989", "%d/%m/%Y", &student2.birthdate);
	student_t student3={96,"Philipe","Boubakar","physique"};
    strptime("02/01/2001", "%d/%m/%Y", &student3.birthdate);

	student_t student6={88, "Jules", "Cesar", "youyou"};
	strptime("19/02/2182", "%d/%m/%Y", &student6.birthdate);

	char b[1000];
	student_to_str(b, &student1);
	cout<<b;


    student_t student_test_list[3]={student1,student2,student3};
    fake_data_base.data=student_test_list;
    fake_data_base.lsize=3;
    fake_data_base.psize=10*sizeof(student_t);
	insert(&student6,&fake_data_base);
		cout<<"insert passé"<<endl;
		cout<<fake_data_base.lsize<<endl;
		for (int i=0; i<fake_data_base.lsize; i++){
			cout<<i<<"\n";
			char buffer[1000];
			student_to_str(buffer, &fake_data_base.data[i]);
			cout<<buffer;}
        //cout<<select("fname","Jorissen",&fake_data_base)<<endl;
        //delete("fname","Jorissen",&fake_data_base);
        //update();
}
