#include<iostream>
using namespace std;

int main (){
	double list[]={3.4, 2.0, 3.5, 5.5};
	cout<<sizeof(list)/sizeof(list[0])<<endl;
	cout<<list[sizeof(list)/sizeof(list[0])-1]<<endl;
	
	string a = "Saksham ", b = "Joshi";
	string c = a+b;
	cout<<c<<endl;

}
