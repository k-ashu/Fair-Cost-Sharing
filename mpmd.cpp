#include <bits/stdc++.h>

using namespace std;
ofstream pass_4, pass_3;
class Point
{
public:
	int x,y;

	Point() {
	}
	
    Point(int x, int y) {
		this->x = x;
		this->y = y;
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


class PathA{
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

    	PathA() {
    	}

    	PathA(Point P, Point D) {
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

    		//this->D[0][4]=pickup[0].distance(drop);

    		compute_costs();
    		compute_disutilities();
    	}

    	void add_passenger(Point P) {
    		pickup.push_back(P);
    		int i;
    		for(i=0;i<pickup.size()-1;i++) {
    			D[i][i+1]=pickup[i].distance(pickup[i+1]);
    		}

    		//D[i][4]=pickup[i].distance(drop);

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
  //  private:
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

                // this->print_distances();
                // cout<<i<<":cost_op "<<cost_op<<endl;
                // cout<<i<<":compens_rec "<<compens_rec<<"  :compens_given "<<compens_given<<endl;

				net=cost_op+compens_given-compens_rec;
               // cout<<"--->"<<net<<endl<<endl;
				f.push_back(net);
                f1.push_back(cost_op);


				//du.push_back(f[i]+i_cost[i]);
			}

			//du.push_back(disutility);




    	}

    	void compute_disutilities() {

    		vector<float> disutility;
    		for(int i=0;i<pickup.size();i++) {
    			disutility.push_back(f[i]+i_cost[i]);
    		}
    		du.push_back(disutility);

    	}
};

class PathB{
    public:
    	vector<Point> drop;
    	Point pickup;
    	float op_cost;
    	vector<float> i_cost;
    	vector<float> f;

        vector<float> f_final;

        vector<float> f1;
    	vector<vector<float> > du;
    	int D[5][5];
     	float a_op;
        //map <Point,int> seq;
    	vector<float> a;

    	//int d12, d23, d1D, d2D, d3D;

    	PathB() {
    		
    	}

    	PathB(Point P, Point D) {
    		//a=a1=a2=a3=1
    		a_op=1;

    		for(int i=0;i<4;i++) {
    			a.push_back(1);
    		}
    		
    		drop.push_back(D);
    		pickup=P;

    		for(int i=0;i<5;i++) {
    			for(int j=0;j<5;j++) 
    				this->D[i][j]=-1;
    			
    		}

    		this->D[0][1]=pickup.distance(drop[0]);

    		//this->D[0][4]=pickup[0].distance(drop);

    		compute_costs();
    		compute_disutilities();
    	}

        void order(){
            Point temp=drop[0];
            //cout<<"pickup";
            //pickup.print_point();
            //cout<<endl;
            for(int i=1,min=1000000,ind=i;i<drop.size()-1;i++,min=1000000){
                for(int j=i;j<drop.size();j++){
                    if(temp.distance(drop[j])<min){
                        min=temp.distance(drop[j]);
                        // cout<<"-->"<<temp.distance(drop[j])<<"\t";
                        // // temp.print_point();
                        // drop[j].print_point();
                        // cout<<endl;
                        ind=j;
                    }
                }
                swap(drop[i],drop[ind]);
                // cout<<"===";
                // drop[i].print_point();
                // drop[ind].print_point();
                // cout<<endl;
                temp=drop[i];
            }
            // cout<<"---------";
            // for(int i=0;i<drop.size();i++)
            //     drop[i].print_point();
            // cout<<endl;

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
            f_final.clear();
            f1.clear();

			

			for(int i=0;i<drop.size();i++) {
				float cost_op=0,cost_op1=0, compens_given=0,compens_rec=0,net=0,net1=0;
				int j=1;
				for(j=0;j<=i;j++) {
					cost_op+=a_op*D[j][j+1]/(drop.size()-j);
					//cost_op+=a_op*D[j-1][j]/j;
				}
                //if(i==0) cost_op1=
                for(j=1;j<=i;j++) {
                    cost_op1+=a_op*D[j][j+1]/(drop.size()-j);
                    //cost_op+=a_op*D[j-1][j]/j;
                }

				//cost_op+=a_op*D[j-1][4]/j;

				if(i<drop.size()-1) compens_given=a[i]*(D[0][i+1]+D[i+1][i+2]-D[0][i+2]);

				//if(i!=0) compens_given=a[i]*(D[i-1][i]+D[i][4]-D[i-1][4]);


				for(j=0;j<i;j++) {
					compens_rec+=a[j]*(D[0][j+1]+D[j+1][j+2]-D[0][j+2])/(drop.size()-j-1);
				}

                // this->print_distances();
                // cout<<i<<":cost_op1 "<<cost_op1<<endl;
                // cout<<i<<":compens_rec "<<compens_rec<<"  :compens_given "<<compens_given<<endl;

				net=cost_op+compens_given-compens_rec;
                net1=cost_op1+compens_given-compens_rec;
                 //cout<<"--->"<<net1<<endl<<endl;

				f.push_back(net);
                f_final.push_back(net1);


                f1.push_back(cost_op1);


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
};





class Path{
    public:
    	vector<Point> pickup;
    	vector<Point> drop;
    	PathA * pA;
    	PathB * pB;
    	float op_cost;
    	vector<float> ic_pickup;
    	vector<float> ic_drop;
    	
        vector<float> f_pickup;
    	vector<float> f_drop;

        vector<float> f1_pickup;
        vector<float> f1_drop;

        int drop_order[4];
    	vector<vector<float> > du_pickup;
    	vector<vector<float> > du_drop;
    	int D[5][5];
    	//int D2[5][5];

    	float a_op;

    	vector<float> a;

    	Path(Point P, Point D) {
    		//a=a1=a2=a3=1
    		a_op=1;

    		for(int i=0;i<4;i++) {
    			a.push_back(1);
    		}
    		
    		pickup.push_back(P);
    		drop.push_back(D);
    		pA = new PathA(P,D);

    		//pB = new PathB();
            for(int i=0;i<4;i++)
                drop_order[i]=-1;
            drop_order[0]=1;

    	}

    	bool add_passenger(Point P, Point D) {

    		pA->add_passenger(P);
    		pB = new PathB(P,this->drop[0]);
    		for(int i=1;i<this->drop.size();i++) {
    			pB->add_passenger(this->drop[i]);
	   		}
	    	pB->add_passenger(D);
            //pB->order();
            
            ic_pickup.clear();
            ic_drop.clear();
            
            f_pickup.clear();
            f_drop.clear();

            f1_pickup.clear();
            f1_drop.clear();

            // du_pickup.clear();
            // du_drop.clear();


	    	if(pA->isSIRfeasible() && pB->isSIRfeasible()) {
	    		for(int i=0;i<pA->pickup.size();i++) {
	    			ic_pickup.push_back(pA->i_cost[i]);
	    			ic_drop.push_back(pB->i_cost[i]);
	    			
                    f_pickup.push_back(pA->f[i]);
	    			f_drop.push_back(pB->f_final[i]);

                    f1_pickup.push_back(pA->f1[i]);
                    f1_drop.push_back(pB->f1[i]);

	    			vector<float> disutility1;
	    			vector<float> disutility2;

	    			for(int j=0;j<pA->du[i].size();j++) {

	    				disutility1.push_back(pA->du[i][j]);
	    				disutility2.push_back(pB->du[i][j]);
	    			}
	    			du_pickup.push_back(disutility1);
	    			du_drop.push_back(disutility2);
	    		}
	    		this->pickup.push_back(P);
                this->drop.push_back(D);
                //cout<<"success"<<endl;
                drop_order[0]=1;
                for(int i=1;i<pB->drop.size();i++){
                    int index=-1;
                    for(int j=1;j<this->drop.size();j++) {
                        if(pB->drop[i]==this->drop[j]) {
                            index=j;
                            break;
                        }
                    }
                    drop_order[index]=i+1;
                }


                // for(int i=0;i<4;i++)
                //     cout<<"----"<<drop_order[i]<<"\t";

               // cout<<endl;
	    		
	    		return true;
	    	}
            pA->remove_passenger();
            

	    	return false;

    	}

        void print() {
            cout<<endl;
            cout<<endl;
            cout<<endl;
            print_path();
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
                    pass_4<<pickup[i].distance(drop[i]);
                    pass_4<<"\t";
                }
                pass_4<<endl;
            }
            else if(pickup.size()==3){
                for(int i=0;i<pickup.size();i++){
                pass_3<<pickup[i].distance(drop[i]);
                pass_3<<"\t";
            }
            pass_3<<endl;
            }
        }
        void write_f(){
            if(pickup.size()==4){
                for(int i=0;i<pickup.size();i++) {
                    pass_4<<f_pickup[i]+f_drop[drop_order[i]-1]<<"\t";
                }
                pass_4<<endl;
            }
            else if(pickup.size()==3){
                for(int i=0;i<pickup.size();i++) {
                    pass_3<<f_pickup[i]+f_drop[drop_order[i]-1]<<"\t";
                }
                pass_3<<endl;
            }
        }
        void write_f1(){
            if(pickup.size()==4){
                for(int i=0;i<pickup.size();i++) {
                    pass_4<<f1_pickup[i]+f1_drop[drop_order[i]-1]<<"\t";
                }
                pass_4<<endl;
            }
            else if(pickup.size()==3){
                for(int i=0;i<pickup.size();i++) {
                    pass_3<<f1_pickup[i]+f1_drop[drop_order[i]-1]<<"\t";
                }
                pass_3<<endl;
            }
        }

        

        void print_single_op(){
            cout<<"single op:";
            for(int i=0;i<pickup.size();i++){
                cout<<pickup[i].distance(drop[i]);
                cout<<"\t";
            }
            cout<<endl;
        }

        void print_ic() {
            cout<<"IC: ";
            for(int i=0;i<pickup.size();i++) {
                //cout<<drop_order[i]<<"++++"<<endl;
               cout<<(ic_pickup[i]+ic_drop[drop_order[i]-1])/2<<"\t";
            }
            cout<<endl;

        }

        void print_f() {
            cout<<"F measure: ";
            //cout<<"----"<<pickup.size()<<endl;
            for(int i=0;i<pickup.size();i++) {
                //cout<<f_pickup[i]<<"\t"<<f_drop[i]<<endl;
                cout<<f_pickup[i]+f_drop[drop_order[i]-1]<<"\t";
                //cout<<endl;
            }
            cout<<endl;

        }

        void print_f1() {
            cout<<"F1 measure: ";
            for(int i=0;i<pickup.size();i++) {
                cout<<f1_pickup[i]+f1_drop[drop_order[i]-1]<<"\t";
            }
            cout<<endl;

        }

        void print_disutilities() {
            cout<<"Disutilities: "<<endl;
           // cout<<du_pickup.size()<<endl;
            for(int i=0;i<du_pickup.size();i++) {
             
                for(int j=0;j<du_pickup[i].size();j++) {
                    //cout<<".";
                    cout<<du_pickup[i][j]+du_drop[i][drop_order[j]-1]<<"\t";    
                }
                cout<<endl;
                
            }
            cout<<endl;

        }
        void print_path() {




            for(int i=0;i<this->pickup.size();i++)
                cout<<" P"<<i+1<<"\t"<<"\t";
            for(int i=0;i<this->drop.size();i++)
                cout<<" D"<<this->drop_order[i]<<"\t"<<"\t";
            cout<<endl;
            for(int i=0;i<this->pickup.size();i++){
                this->pickup[i].print_point();
                cout<<"\t";
            }
            for(int i=0;i<this->drop.size();i++){
                this->drop[drop_order[i]-1].print_point();
                cout<<"\t";
            }
            cout<<endl;
        }

        // void print_f(){
        //     cout<<"f_pickup :   ";
        //     for(int i=0;i<this->f_pickup.size();i++)
        //         cout<<this->f_pickup[i]<<"  ";
        //     cout<<endl<<"f_drop   :   ";
        //     for(int i=0;i<this->f_drop.size();i++)
        //         cout<<this->f_drop[i]<<"  ";        
        //     cout<<endl;
        // }
};



bool compare(Path a, Path b) {
	return a.pickup.size()>b.pickup.size();
}
bool size_compare(Path a, Path b) {
    return a.pickup.size()>b.pickup.size();
}

int main() 
{
    
    pass_3.open("3_pass_n.txt",ios::app);
    pass_4.open("4_pass_n.txt",ios::app);
    Point p1(0,0);
    Point d1(9,0);
    Point p2(1,1);
    Point d2(9,1);
    Point p3(2,1);
    Point d3(11,0);
    Point p4(7,0);
    Point d4(10,0);
    Path p(p1,d1);
    if(p.add_passenger(p2,d2)) cout<<"YES"<<endl;
    else cout<<"NO"<<endl;
    
    // p.print_path();
    // p.print();

    if(p.add_passenger(p3,d3)) cout<<"YES"<<endl;
    else cout<<"NO"<<endl;
    // p.print_path();
    // p.print();

    if(p.add_passenger(p4,d4)) cout<<"YES"<<endl;
    else cout<<"NO"<<endl;
    // p.print_path();
    // p.print();
    

    vector<Point> pickup,drop;
    int GRID_SIZE = 200;
    int NUM_PASS = 500;
    srand(time(NULL));
    for(int i=0;i<NUM_PASS;i++){
        int sx, sy, dx, dy;
        sx=rand()%GRID_SIZE;
        sy=rand()%GRID_SIZE;
        dx=rand()%GRID_SIZE;
        dy=rand()%GRID_SIZE;

        Point s(sx,sy);
        Point d(dx,dy);
        
        // cout<<i<<"  ";
        // s.print_point();
        // cout<<"\t";
        // d.print_point();
        pickup.push_back(s);
        drop.push_back(d);
        //cout<<endl;
    }

    bool visited[NUM_PASS];
    memset(visited,false,NUM_PASS);
    vector<Path> routes;
    for(int i=0;i<NUM_PASS;i++){
        if(!visited[i]){
            visited[i]=true;
            Path p(pickup[i],drop[i]);
            int n=3;
            for(int j=0,counter=4;j<NUM_PASS && counter;j++){
                if(!visited[j]){
                    //cout<<j<<endl<<i<<endl;
                    if(p.add_passenger(pickup[j],drop[j])){
                        counter--;
                        //cout<<counter<<endl;
                        visited[j]=true;
                    }
                }
            }
            if(p.pickup.size()==4) {
                //cout<<"+++"<<p.pickup.size()<<endl;
                p.print();
                //pass_4<<"12341asdfsd"<<endl;
               // p.write_to_file();
                routes.push_back(p);
            }
            
        }
    }
    sort(routes.begin(),routes.end(),size_compare);

    
return 0;
}
