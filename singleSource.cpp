#include <bits/stdc++.h>

using namespace std;

ofstream pass_4, pass_3, path_count;

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
    	vector<Point> drop;
    	Point pickup;
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
    		
    		drop.push_back(D);
    		pickup=P;

    		for(int i=0;i<5;i++) {
    			for(int j=0;j<5;j++) {
    				this->D[i][j]=-1;
    			}
    		}

    		this->D[0][1]=pickup.distance(drop[0]);

    		//this->D[0][4]=pickup[0].distance(drop);

    		compute_costs();
    		compute_disutilities();
    	}

    	void add_passenger(Point P) {
    		drop.push_back(P);
    		int i;

    		for(i=0;i<drop.size()-1;i++) {
    			D[0][i+1]=pickup.distance(drop[i]);
    			D[i+1][i+2]=drop[i].distance(drop[i+1]);
    		}

    		D[0][i+1]=pickup.distance(drop[i]);

    		//D[i][4]=pickup[i].distance(drop);

    		compute_costs();
    		compute_disutilities();
    	}

    	void remove_passenger() {

    		int i=drop.size();
    		D[i-1][i]=-1;
    		D[0][i]=-1;
    		drop.pop_back();
    		du.pop_back();

    		compute_costs();
    	}

    	bool isSIRfeasible() {

    		for(int i=0;i<drop.size()-1;i++) {
    			if(du[drop.size()-2][i]<du[drop.size()-1][i]) return false;
    		}

    		// for(int j=2;j<=drop.size();j++) {
    		// 	float sum=0;
    		// 	for(int k=1;k<j;k++) sum+=D[k][k+1];
    		// 	if(2*D[0][j]-D[0][1]-D[j-1][j]<sum) return false;
    		// }
    		return true;

    	}

    	void print_path() {
    		pickup.print_point();
    		cout<<"    ";
    		for(int i=0;i<drop.size();i++) {
    			drop[i].print_point();
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
    		for(i=0;i<drop.size();i++) {
    			//D[i][4]=pickup[i].distance(drop);
    			if(D[i][i+1]!=-1) {
    				op_cost+=D[i][i+1];
    				final=i+1;
    			}
    		}

    		//if(D[final][4]!=-1) op_cost+=D[final][4];

			for(int i=0;i<drop.size();i++) {
				float cost=0;
				final=i;



				for(int j=0;j<=i;j++) {
					cost+=D[j][j+1];
					// if(D[j][j+1]!=-1) {
					// 	cost+=a[i]*D[j][j+1];
					// 	final=j+1;
					// }
				}
				//if(D[final][4]!=-1) cost+=a[i]*D[final][4];
				//cost-=a[i]*D[i][4];
				cost-=D[0][i+1];

				i_cost.push_back(cost);
			}

			f.clear();
			f1.clear();

			

			for(int i=0;i<drop.size();i++) {
				float cost_op=0, compens_given=0,compens_rec=0,net=0;
				int j=1;
				for(j=0;j<=i;j++) {
					cost_op+=a_op*D[j][j+1]/(drop.size()-j);
					//cost_op+=a_op*D[j-1][j]/j;
				}
				//cost_op+=a_op*D[j-1][4]/j;

				if(i<drop.size()-1) compens_given=a[i]*(D[0][i+1]+D[i+1][i+2]-D[0][i+2]);

				//if(i!=0) compens_given=a[i]*(D[i-1][i]+D[i][4]-D[i-1][4]);

				for(j=0;j<i;j++) {
					compens_rec+=a[j]*(D[0][j+1]+D[j+1][j+2]-D[0][j+2])/(drop.size()-j-1);
				}

				net=cost_op+compens_given-compens_rec;
				f1.push_back(cost_op);
				f.push_back(net);


				//du.push_back(f[i]+i_cost[i]);
			}

			//du.push_back(disutility);




    	}

    	void compute_disutilities() {

    		vector<float> disutility;
    		for(int i=0;i<drop.size();i++) {
    			disutility.push_back(f[i]+i_cost[i]);
    		}
    		du.push_back(disutility);

    	}

    	void write_to_file() {
            write_single_op();
            write_f();
            write_f1();
        }
        void write_single_op(){
            if(drop.size()==4){
                for(int i=0;i<drop.size();i++){
                    pass_4<<pickup.distance(drop[i]);
                    pass_4<<"\t";
                }
                pass_4<<endl;
            }
            else if(drop.size()==3){
                for(int i=0;i<drop.size();i++){
                pass_3<<pickup.distance(drop[i]);
                pass_3<<"\t";
            }
            pass_3<<endl;
            }
        }
        void write_f(){
            if(drop.size()==4){
                for(int i=0;i<drop.size();i++) {
                    pass_4<<f[i]<<"\t";
                }
                pass_4<<endl;
            }
            else if(drop.size()==3){
                for(int i=0;i<drop.size();i++) {
                    pass_3<<f[i]<<"\t";
                }
                pass_3<<endl;
            }
        }
        void write_f1(){
            if(drop.size()==4){
                for(int i=0;i<drop.size();i++) {
                    pass_4<<f1[i]<<"\t";
                }
                pass_4<<endl;
            }
            else if(drop.size()==3){
                for(int i=0;i<drop.size();i++) {
                    pass_3<<f1[i]<<"\t";
                }
                pass_3<<endl;
            }
        }

        void print() {
            // print_ic();
            // print_disutilities();
            print_single_op();
            print_f();
            print_f1();

        }

void print_single_op(){
            cout<<"single op:";
            for(int i=0;i<drop.size();i++){
                cout<<pickup.distance(drop[i]);
                cout<<"\t";
            }
            cout<<endl;
        }

        void print_ic() {
            cout<<"IC: ";
            for(int i=0;i<drop.size();i++) {
                //cout<<drop_order[i]<<"++++"<<endl;
               cout<<i_cost[i]<<"\t";
            }
            cout<<endl;

        }

        void print_f() {
            cout<<"F measure: ";
            //cout<<"----"<<pickup.size()<<endl;
            for(int i=0;i<drop.size();i++) {
                
                cout<<f[i]<<"\t";
                //cout<<endl;
            }
            cout<<endl;

        }

        void print_f1() {
            cout<<"F1 measure: ";
            for(int i=0;i<drop.size();i++) {
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
	return a.drop.size()>b.drop.size();
}


int main() 
{

	pass_3.open("3_pass_ss.txt",ios::app);
    pass_4.open("4_pass_ss.txt",ios::app);
    path_count.open("count_ss.txt",ios::app);

	//Point p1(0,0);
    // Point d2(9,0);
    // Point s(1,1);
    // Point d1(9,1);
    // Point p3(2,1);
    // Point d3(11,0);
    // Point p4(7,0);
    // Point d4(10,0);
    
    // Path p(s,d1);
    // p.add_passenger(d2);

    // cout<<p.i_cost[1]<<" "<<p.f[1]<<endl;
    
    // for(int i=0;i<p.du.size();i++) {
    //             for(int j=0;j<p.du[i].size();j++) {
    //                 cout<<p.du[i][j]<<"\t";    
    //             }
    //             cout<<endl;
                
    //         }
	vector<Point> drop;
	int count=0;
    int GRID_SIZE = 200;
    int NUM_PASS = 100;
    srand(time(NULL));
    
        int sx, sy, dx, dy;
        sx=rand()%GRID_SIZE;
        sy=rand()%GRID_SIZE;
      	Point pickup(sx,sy);

    for(int i=0;i<NUM_PASS;i++){
        dx=rand()%GRID_SIZE;
        dy=rand()%GRID_SIZE;

        
        Point d(dx,dy);
        
        // cout<<i<<"  ";
        // s.print_point();
        // cout<<"\t";
        // d.print_point();
        drop.push_back(d);
        //cout<<endl;
    }

    bool visited[NUM_PASS];
    memset(visited,false,NUM_PASS);
    vector<Path> routes;
    for(int i=0;i<NUM_PASS;i++){
        if(!visited[i]){
            visited[i]=true;
            Path p(pickup,drop[i]);
            int n=3;
            for(int j=0,counter=3;j<NUM_PASS && counter;j++){
                if(!visited[j]){
                    //cout<<j<<endl<<i<<endl;
                    p.add_passenger(drop[j]);
                    if(p.isSIRfeasible()){
                        counter--;
                        //cout<<counter<<endl;
                        visited[j]=true;
                    }
                    else p.remove_passenger();
                }
            }
            if(p.drop.size()==4) {
                //cout<<"+++"<<p.pickup.size()<<endl;
                //p.print();
                //pass_4<<"12341asdfsd"<<endl;
                p.write_to_file();
                count++;
               // p.print();
                routes.push_back(p);
            }
            
        }
    }
    sort(routes.begin(),routes.end(),compare);
    path_count<<count<<endl;


	return 0;
}
