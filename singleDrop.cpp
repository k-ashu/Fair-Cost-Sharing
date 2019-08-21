#include <bits/stdc++.h>

using namespace std;

ofstream pass_4, pass_3;

class Point
{
public:
	int x,y;
	int dest;

	Point() {

	}

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	Point(int x, int y, Point dest) {
		this->x = x;
		this->y = y;
		this->dest = abs(this->x-dest.x)+abs(this->y-dest.y);
	}

	void print_point() {
		cout<<"("<<x<<','<<y <<")";
	}

	int distance(Point P) {
		return abs(this->x-P.x)+abs(this->y-P.y);
	}

	inline bool operator==(const Point& a) {
		return (a.x==this->x && a.y==this->y);
	}
};

class Path{
    public:
    	vector<Point> pickup;
    	Point drop;
    	float op_cost;
    	vector<float> i_cost;
    	vector<float> f;
    	vector<float> f1;
    	vector<vector<float> > du;
    	int D[5][5];

    	float a_op;

    	vector<float> a;

    	//int d12, d23, d1D, d2D, d3D;

    	Path(Point P, Point D) {
    		//a=a1=a2=a3=1
    		a_op=1;

    		for(int i=0;i<4;i++) {
    			a.push_back(1);
    		}
    		
    		pickup.push_back(P);
    		drop=D;

    		for(int i=0;i<5;i++) {
    			for(int j=0;j<5;j++) {
    				this->D[i][j]=-1;
    			}
    		}

    		this->D[0][4]=pickup[0].distance(drop);

    		compute_costs();
    		compute_disutilities();
    	}

    	void add_passenger(Point P) {
    		pickup.push_back(P);
    		int i;
    		for(i=0;i<pickup.size()-1;i++) {
    			D[i][i+1]=pickup[i].distance(pickup[i+1]);
    		}

    		D[i][4]=pickup[i].distance(drop);

    		compute_costs();
    		compute_disutilities();
    	}

    	void remove_passenger() {

    		int i=pickup.size()-1;
    		D[i-1][i]=-1;
    		D[i][4]=-1;
    		pickup.pop_back();
    		du.pop_back();

    		compute_costs();
    	}

    	bool isSIRfeasible() {
    		
    		for(int i=0;i<pickup.size()-1;i++) {
    			if(du[pickup.size()-2][i]<du[pickup.size()-1][i]) return false;
    		}


    		// for(int i=1;i<pickup.size();i++) {
    		// 	float sum_a=0;
    		// 	for(int j=0;j<i;j++) sum_a+=a[j];
    		// 	if(D[i-1][i]+D[i][4]-D[i-1][4]>D[i][4]/(1+sum_a/a_op)) return false;
    		// }
    		return true;

    	}

    	void print_path() {
    		for(int i=0;i<pickup.size();i++) {
    			pickup[i].print_point();
    			cout<<"  ";
    		}
    		cout<<" "<<op_cost<<endl;
    	}

    	void print_distances() {
    		for(int i=0;i<5;i++) {
    			for(int j=0;j<5;j++) {
    				cout<<D[i][j]<<" ";
    			}
    			cout<<endl;
    		}
    	}
   // private:
    	void compute_costs() {

    		op_cost=0;
    		i_cost.clear();
    		int i,final=0;
    		for(i=0;i<pickup.size();i++) {
    			D[i][4]=pickup[i].distance(drop);
    			if(D[i][i+1]!=-1) {
    				op_cost+=D[i][i+1];
    				final=i+1;
    			}
    		}

    		if(D[final][4]!=-1) op_cost+=D[final][4];

			for(int i=0;i<pickup.size();i++) {
				float cost=0;
				final=i;
				for(int j=i;j<pickup.size();j++) {
					if(D[j][j+1]!=-1) {
						cost+=a[i]*D[j][j+1];
						final=j+1;
					}
				}
				if(D[final][4]!=-1) cost+=a[i]*D[final][4];
				cost-=a[i]*D[i][4];
				i_cost.push_back(cost);
			}

			f.clear();
			f1.clear();

			

			for(int i=0;i<pickup.size();i++) {
				float cost_op=0, compens_given=0,compens_rec=0,net=0;
				int j=1;
				for(j=i+1;j<pickup.size();j++) {
					cost_op+=a_op*D[j-1][j]/j;
				}
				cost_op+=a_op*D[j-1][4]/j;


				if(i!=0) compens_given=a[i]*(D[i-1][i]+D[i][4]-D[i-1][4]);

				for(j=i+1;j<pickup.size();j++) {
					compens_rec+=a[j]*(D[j-1][j]+D[j][4]-D[j-1][4])/j;
				}

				net=cost_op+compens_given-compens_rec;
				f.push_back(net);
				f1.push_back(cost_op);
				//cout<<cost_op<<endl;

				//du.push_back(f[i]+i_cost[i]);
			}

			//du.push_back(disutility);




    	}

    	void compute_disutilities() {

    		vector<float> disutility;
    		for(int i=0;i<pickup.size();i++) {
    			// cout<<endl;
       //          cout<<f[i]<<" "<<i_cost[i]<<"******";
       //          cout<<endl;
    			disutility.push_back(f[i]+i_cost[i]);
    		}
    		du.push_back(disutility);

    	}

    	 void print() {
             print_ic();
             print_disutilities();
            print_single_op();
            print_f();
            print_f1();

        }
        void write_to_file() {
            write_single_op();
            write_f();
            write_f1();
        }
        void write_single_op(){
            if(pickup.size()==4){
                for(int i=0;i<pickup.size();i++){
                    pass_4<<pickup[i].distance(drop);
                    pass_4<<"\t";
                }
                pass_4<<endl;
            }
            else if(pickup.size()==3){
                for(int i=0;i<pickup.size();i++){
                pass_3<<pickup[i].distance(drop);
                pass_3<<"\t";
            }
            pass_3<<endl;
            }
        }
        void write_f(){
            if(pickup.size()==4){
                for(int i=0;i<pickup.size();i++) {
                    pass_4<<f[i]<<"\t";
                }
                pass_4<<endl;
            }
            else if(pickup.size()==3){
                for(int i=0;i<pickup.size();i++) {
                    pass_3<<f[i]<<"\t";
                }
                pass_3<<endl;
            }
        }
        void write_f1(){
            if(pickup.size()==4){
                for(int i=0;i<pickup.size();i++) {
                    pass_4<<f1[i]<<"\t";
                }
                pass_4<<endl;
            }
            else if(pickup.size()==3){
                for(int i=0;i<pickup.size();i++) {
                    pass_3<<f1[i]<<"\t";
                }
                pass_3<<endl;
            }
        }

        

        void print_single_op(){
            cout<<"single op:";
            for(int i=0;i<pickup.size();i++){
                cout<<pickup[i].distance(drop);
                cout<<"\t";
            }
            cout<<endl;
        }

        void print_ic() {
            cout<<"IC: ";
            for(int i=0;i<pickup.size();i++) {
                //cout<<drop_order[i]<<"++++"<<endl;
               cout<<i_cost[i]<<"\t";
            }
            cout<<endl;

        }

        void print_f() {
            cout<<"F measure: ";
            //cout<<"----"<<pickup.size()<<endl;
            for(int i=0;i<pickup.size();i++) {
                //cout<<f_pickup[i]<<"\t"<<f_drop[i]<<endl;
                cout<<f[i]<<"\t";
                //cout<<endl;
            }
            cout<<endl;

        }

        void print_f1() {
            cout<<"F1 measure: ";
            for(int i=0;i<pickup.size();i++) {
                cout<<f1[i]<<"\t";
            }
            cout<<endl;

        }

        void print_disutilities() {
            cout<<"Disutilities: "<<endl;
           // cout<<du_pickup.size()<<endl;
            for(int i=0;i<du.size();i++) {
             
                for(int j=0;j<du[i].size();j++) {
                    //cout<<".";
                    cout<<du[i][j]<<"\t";    
                }
                cout<<endl;
                
            }
            cout<<endl;

        }

};



bool compare(Path a, Path b) {
	return a.pickup.size()>b.pickup.size();
}


int main() 
{

	pass_3.open("3_pass_sd.txt",ios::app);
    pass_4.open("4_pass_sd.txt",ios::app);


	// Point p1(0,0);
 //    //Point p1(9,0);
 //    Point p2(1,1);
 //    Point d(9,1);
 //    Point p3(2,1);
 //    Point d3(11,0);
 //    Point p4(7,0);
 //    Point d4(10,0);
    
 //    Path p(p1,d);
 //    p.add_passenger(p2);

 //    cout<<p.i_cost[0]<<" "<<p.f[0]<<endl;
    
 //    for(int i=0;i<p.du.size();i++) {
 //                for(int j=0;j<p.du[i].size();j++) {
 //                    cout<<p.du[i][j]<<"\t";    
 //                }
 //                cout<<endl;
                
 //            }
 //            //cout<<endl;

 //            p.print_distances();


    
    

	// Point D(10,0);

	// Point p1(0,0,D);

	// Path path(p1,D);

	// //path.print_path();

	

	// Point p2(2,1);
	// Point p3(5,0);
	// Point p4(8,0);

	// path.add_passenger(p2);
	// path.add_passenger(p3);
	// path.add_passenger(p4);
	// path.print_distances();
	// cout<<path.op_cost<<endl;

	// for(int i=0;i<path.i_cost.size();i++) {
	// 	cout<<path.i_cost[i]<<" ";
	// }

	// cout<<endl;

	// for(int i=0;i<path.i_cost.size();i++) {
	// 	cout<<path.f[i]<<" ";
	// }

	// cout<<endl;

	// if(path.isSIRfeasible()) cout<<"YES"<<endl;
	// else cout<<"NO"<<endl;

	// cout<<endl;
	
	
	/*

int l=0;

	vector<Point> P;
	int GRID_SIZE = 500;
	int NUM_PASS = 50;
	srand(time(NULL));

	Point D(rand()%GRID_SIZE,rand()%GRID_SIZE);
	D.dest=0;

	cout << "Destination:" <<endl;
	cout<<"("<<D.x<<','<<D.y <<")"<<'\t'<<D.dest<< endl;
	cout<<endl<<"Passengers"<<endl;

	for(int i=0;i<NUM_PASS;i++){
		int x, y;
		x=rand()%GRID_SIZE;
		y=rand()%GRID_SIZE;

		Point p(x,y,D);

		P.push_back(p);
	}

	for(vector<Point>::iterator it=P.begin();it!=P.end();it++) {
		cout<<"("<<it->x<<','<<it->y <<")"<<'\t'<<it->dest<< endl;
	}

	vector<Path> sir;
	vector<Path> sir_dist;

	while(true) {
		sir.clear();
	for(int i=0;i<NUM_PASS;i++) {
		Path path(P[i],D);
		int temp=-1,n=3;
		bool vis[NUM_PASS];
		memset(vis,false,NUM_PASS);
		vis[i]=true;
		while(n--){
			temp=-1;
			double min_ic=1000000000;
			for(int j=0;j<P.size();j++){
	        	if(vis[j])
	            	continue;
	        	else {
	        		path.add_passenger(P[j]);
		        	if(path.isSIRfeasible() && path.i_cost[2-n]<min_ic) {
		        		temp=j;
	    	    		min_ic=path.i_cost[2-n];
	        		}
	        		path.remove_passenger();
	        	}
	   		}	
	    	if(temp!=-1){
	    		path.add_passenger(P[temp]);
	    		vis[temp]=true;	
	    	}
	    	else break;
		}

		sir.push_back(path);
	}

	
	sort(sir.begin(),sir.end(),compare);

	int nu=0;
	for(int i=0;i<sir.size();i++) {
		nu+=sir[i].pickup.size();
	}
	cout<<sir[0].pickup.size()<<"  "<<nu<<endl;

	if(sir[0].pickup.size()!=1) sir_dist.push_back(sir[0]);
	else break;
	
	//cout<<l++<<endl;

	for(int i=0;i<sir[0].pickup.size();i++) {

		// for(int j=0;j<P.size();j++) {
		// 	if(sir[0].pickup[i]==P[j]) cout<<j<<endl;
		// }


		// auto it= find(P.begin(),P.end(),sir[0].pickup[i]);
		// if(it!=P.end()) {
		// 	P.erase(it);
		// }
		P.erase(remove(P.begin(),P.end(),sir[0].pickup[i]),P.end());
	}



	}









	cout<<endl<<sir_dist.size()<<endl;
	for(int i=0;i<sir_dist.size();i++) {
		if(sir_dist[i].pickup.size()>3) {
			for(int j=0;j<sir_dist[i].pickup.size();j++) {
				sir_dist[i].pickup[j].print_point();
				cout<<"  ";
			}
			cout<<endl;

			sir_dist[i].print_distances();


			cout<<endl<<"op_cost: "<<sir_dist[i].op_cost<<endl;

			cout<<"i_costs:  ";

			for(int j=0;j<sir_dist[i].i_cost.size();j++) {
				cout<<sir_dist[i].i_cost[j]<<"  ";
			}

			cout<<endl;

			cout<<"f:  ";

			for(int j=0;j<sir_dist[i].f.size();j++) {
				cout<<sir_dist[i].f[j]<<"  ";
			}

			cout<<endl;

			cout<<"disutilities:  "<<endl;

			//cout<<sir[i].du[3].size()<<endl;

			for(int j=0;j<sir_dist[i].du.size();j++) {
				for(int k=0;k<sir_dist[i].du[j].size();k++) {
					cout<<sir_dist[i].du[j][k]<<"  ";
				}
				cout<<endl;
			}

			cout<<endl;


			//break;
		}


	}

	int num=0;
	for(int i=0;i<sir_dist.size();i++) {
		num+=sir_dist[i].pickup.size();
	}
	cout<<num<<endl;



*/

vector<Point> pickup;

    int GRID_SIZE = 200;
    int NUM_PASS = 500;
    srand(time(NULL));
    
        int sx, sy, dx, dy;
        dx=rand()%GRID_SIZE;
        dy=rand()%GRID_SIZE;
      	Point drop(dx,dy);

    for(int i=0;i<NUM_PASS;i++){
        sx=rand()%GRID_SIZE;
        sy=rand()%GRID_SIZE;

        
        Point s(sx,sy);
        
        // cout<<i<<"  ";
        // s.print_point();
        // cout<<"\t";
        // d.print_point();
        pickup.push_back(s);
        //cout<<endl;
    }

    bool visited[NUM_PASS];
    memset(visited,false,NUM_PASS);
    vector<Path> routes;
    for(int i=0;i<NUM_PASS;i++){
        if(!visited[i]){
            visited[i]=true;
            Path p(pickup[i],drop);
            int n=3;
            for(int j=0,counter=3;j<NUM_PASS && counter;j++){
                if(!visited[j]){
                    //cout<<j<<endl<<i<<endl;
                    p.add_passenger(pickup[j]);
                    if(p.isSIRfeasible()){
                        counter--;
                        //cout<<counter<<endl;
                        visited[j]=true;
                    }
                    else p.remove_passenger();
                }
            }
            if(p.pickup.size()>2) {
                //cout<<"+++"<<p.pickup.size()<<endl;
                //p.print();
                //pass_4<<"12341asdfsd"<<endl;
              p.write_to_file();
              // p.print();
                routes.push_back(p);
            }
            
        }
    }
    sort(routes.begin(),routes.end(),compare);



	return 0;
}
