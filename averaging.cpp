#include <bits/stdc++.h>

using namespace std;


int main() {

	//vector<float> avg_distance(3);
	vector<long double> avgf_op(3);
	vector<long double> avgf_f1(3);
	vector<long double> avgf1_op(3);

	int n=0;

	for(int i=0;i<50000;i++) {

		long double op[3];
		long double f[3];
		long double f1[3];
		for(int j=0;j<3;j++) {
			cin>>op[j];
		}

		for(int j=0;j<3;j++) {
			cin>>f[j];
			avgf_op[j]+=f[j]/op[j];
		}

		for(int j=0;j<3;j++) {
			cin>>f1[j];
			avgf_f1[j]+=f[j]/f1[j];
			avgf1_op[j]+=f1[j]/op[j];
		}
		n++;
	}

	for(int i=0;i<3;i++) {
		avgf_op[i]=avgf_op[i]/n;
		avgf1_op[i]=avgf1_op[i]/n;
		avgf_f1[i]=avgf_f1[i]/n;
	}

	cout<<"Avg (F/op): "<<endl;
	for(int i=0;i<3;i++) {

		cout<<avgf_op[i]<<"\t";
	}
	cout<<endl;

	cout<<"Avg (F1/op): "<<endl;
	for(int i=0;i<3;i++) {

		cout<<avgf1_op[i]<<"\t";
	}
	cout<<endl;

	cout<<"Avg (F/F1): "<<endl;
	for(int i=0;i<3;i++) {

		cout<<avgf_f1[i]<<"\t";
	}
	cout<<endl;

return 0;

}
