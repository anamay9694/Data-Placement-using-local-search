#include<bits/stdc++.h>
#include"ufl_n_cache_local_first_come.h"
using namespace std;
class cache_ele//client object
{
	public:
    int x,y;
    int cid;

    static int scid;
int oid;
    friend bool operator< (const cache_ele &c1, const cache_ele &c2);
    cache_ele (const cache_ele &obj)
    {
        this->x=obj.getX();
        this->y=obj.getY();
        this->cid=obj.getID();
        this->oid=obj.oid;
    }
    cache_ele(int a=0,int b=0)
    {
        x=a;
        y=b;
        scid=scid+1;
        cid=scid;
        oid=0;
    }
    int getX() const
    {
         return x;
    }
    int getY() const
    {
        return y;
    }
    int getID() const
    {
        return cid;
    }
};

int cache_ele::scid=0;

bool operator< (const cache_ele &c1, const cache_ele &c2)
{
    return (c1.getID()<c2.getID());
}
double cal_assign_cost(pair<int,int>cac_co,pair<int,int>cli_co)//cij calculate
{
	double xd=cac_co.first-cli_co.first;
	double yd=cac_co.second-cli_co.second;
	double result =sqrt((xd*xd)+(yd*yd));
	return result;
}

bool object_arr[10000];//to test feasiblity of solution
vector<int>object_vec;//To arrange the feasible solution in increasing order
int main()
{
	clock_t t;
		clock_t startTime = clock();
//	clock_t tStart = clock();
	t = clock();

    /* Do your stuff here */
    
	
	freopen("input.txt","r",stdin);				
    //freopen("testing.txt","r",stdin);
    //freopen("out_cac_n.txt","w",stdout);
	int n_n;//For cache capacity n
	
	int n_cac_,n_obj_,n_cli_;//number of caches,objects and clients
	cin>>n_n;
	
	cin>>n_cac_>>n_obj_>>n_cli_;	
	vector<pair<int, pair<int,int> > >ve_obj;//obj number,cache number,cost
	vector<pair<int, int > >ve_ca_coor;//co ordinate of cache
	vector<pair<int, pair<int,int> > >ve_cli;//cli number,obj number,demand
	vector<pair<int,int> >ve_cli_cor;//client coordinate
	
	int fea_counter=0;//Counting the number of objects to open for feasible solution
	
	///////////////Initially object_arr is false for all the objects////////////////////
	for(int v=1;v<=n_obj_;v++)
	{
		object_arr[v]=false;
	}
	
	//////////////////////Taking input for cache co-ordinates///////////////////////
	for(int i=1;i<=n_cac_;i++)
	{
		int x,y;
		cin>>x>>y;
		ve_ca_coor.push_back(make_pair<int,int> (x,y));
	}
	/////////////////Taking input for ve_obj/////////////
	///////////It is of the form/////////////////
	////index Object_number Cache_number Cost
	////0        1            1           x
	////1        1            2           y
	////2        1            3           z
	////3        2            1           u
	////4        2            2           v
	////5        2            3           w
	for(int i=1;i<=n_obj_;i++)
	{
		for(int j=1;j<=n_cac_;j++)
		{
		    int x;
		    cin>>x;
		    ve_obj.push_back(make_pair(i,make_pair(j,x)));
		}
	}
	
	/////////////Taking input for ve_cli and ve_cli_cor and setting the object demanded to true///////////
	//////////ve_cli is of the form////////////
	////Index  client_number Object_number Demand
	////0       1             1             4
	////1       2             3             5
	////2       3             2             10
  	////3       4             3             6
	
	/////////////////////object_arr is of the form///////
	////Index Object_number  Truth
	////0        1            true
 	////1        2            false 
		for(int i=1;i<=n_cli_;i++)
	{
		    int x,d,x1,x2;
		    cin>>x>>d>>x1>>x2;
		    
		    object_arr[x]=true;
		    
		    ve_cli.push_back(make_pair(i,make_pair(x,d)));
		    ve_cli_cor.push_back(make_pair(x1,x2));
		
	}
	
	
	///////////////////counting objects which are demanded and pushing them in object_vec in ascending order////////////////////////
	//Object_vec is of the form////
	//Index Object_demanded
	//0         2
	//1         3
	//2         5
	for(int v=1;v<=n_obj_;v++)
	{
		if(object_arr[v]==true)
		{
			object_vec.push_back(v);
			fea_counter++;
		}
	}
	
	//////////////////////////c is of the form//////////////////
	//when n_n=2 we have 
	//Index cache_element co-ordinate_of_cache oid
	//0  	1             	x1,y1               0             
	//1  	2             	x1,y1               0
 	//2  	3             	x2,y2               0
	//3  	4             	x2,y2               0
	vector<cache_ele>c;
	for(int i=1;i<=n_cac_;i++)
	{
		for(int y=1;y<=n_n;y++)
		{
		cache_ele temp1(ve_ca_coor[i-1].first,ve_ca_coor[i-1].second);
         c.push_back(temp1);
        }
	}
	
	//////////////////Initially setting up a feasible solution in the cache element///////////////////////////
    srand(time(NULL));///Setting random values based on time
	int ran_;//random variable
	for(int i=1;i<=fea_counter;i++)//Assigning required objects to cache elements serially
	{
		c[i-1].oid=object_vec[i-1];
//		cout<<"ou\n";
//		cout<<c[i-1].getID()<<"\t";
//		cout<<c[i-1].oid<<"\n";
	}
	//////////////Till now we have c filled with required objects serially//////////////////
	///Index cache_element_number(cid)	 Object_in_that_cache_element(oid)
	///0             1              		1
	///1             2              		3(say object number 2 is not needed by any client)
	///2             3              		4 
	
	object_vec.push_back(0);//To ensure that when we choose randomly from demanded object,we have the case that a cache element is free
	/////object_vec is of the form now///
	//Index Object
	//0     1
	//1     3
	//2     4
	//3     0
	 
	for(int i=fea_counter+1;i<=(n_n*n_cac_);i++)
	{
		ran_=rand()%(fea_counter+1);
		c[i-1].oid=object_vec[ran_];//index based computation of random elements to open in the blank spaces
//		cout<<"ou\n";
//		cout<<c[i-1].getID()<<"\t";
//		cout<<c[i-1].oid<<"\n";
	}
	//Now we have c of the form//////
	//Index Cache_element  Object
	//0         1          1
	//1         2          3
	//2         3          4
	//3         4          1
	//4         5          0
	//5         6          3
	   
	map<int,int>mp;//To map free_cache_number(serially from 1) to actual free_cache_element cid
	map<int,int>mp_cli;//To map client_number(serially) demanding object i to the actual client number
	
	double assign_local_cost=0;//To calculate assignment cost
      double open_local_cost=0;//To calculate Opening cost
    
	//////////Keep iterating until there is no change in c,ie-  (cache_element) (Object)  
	while(true)
	{
		assign_local_cost=0;//To calculate assignment cost in that iteration
		
	vector<cache_ele>vce_copy;//To maintain a copy of c to check whether there has been change since the last iteration
	
	vector<cache_ele>::iterator itc,itc2;//To iterate through c and vce_copy respectively
	
	////////Copying previous content of c to vce_copy
	itc=c.begin();
	while(itc!=c.end())
	{
		vce_copy.push_back((*itc));
		itc++;
	}
	
	open_local_cost=0;//Initializing opening cost of facilities to 0 for the current iteration
	
	////Considering one object at a time,ie picking up one object and performing UFL with free locations and clients demanding that object
	for(int ii=1;ii<=fea_counter;ii++)
	{
		int i=object_vec[ii-1];
		mp.clear();//Clearing map for the present object
		mp_cli.clear();//Clearing mp_cli for the present object
		
		vector<cache_ele>::iterator it;//It is an iterator of c
		it=c.begin();//Setting it to first element of c
		
		int count_zeros=0;//For Counting the number of free location after picking up the current object
		vector<pair<int,int> >vec_coor;//For holding the co-ordinates of the free cache element locations
		vector<int>vec_cost;//To hold the cost of assigning of the i'th object to the free cache element locations
		while(it!=c.end())
		{
			if((*it).oid==i)////If the c element has that object i ,pick it up
			{
				count_zeros++;//Increment the counter counting the number of free locations
				(*it).oid=0;//Free the client element
				vec_coor.push_back(make_pair<int,int> ((*it).x,(*it).y));//vec_coor holding the co-ordinate of the free cache element
				vec_cost.push_back( ve_obj[n_cac_*(i-1)+(((*it).cid)+(n_n-1))/n_n-1].second.second);//Holding the opening cost of object i in the cache element location
				(*it).oid==0;
			     mp.insert(pair<int,int> (count_zeros,(*it).cid));//Mapping the free_cache_element_number to the actual cid of the cache_element location
			}else if((*it).oid==0)//If the cache_element location is already free then count it
			{
				count_zeros++;//Increment the counter counting the number of free locations
				vec_coor.push_back(make_pair<int,int>((*it).x,(*it).y));//vec_coor holding the co-ordinate of the free cache element
				vec_cost.push_back( ve_obj[n_cac_*(i-1)+(((*it).cid)+(n_n-1))/n_n-1].second.second);//Holding the opening cost of object i in the cache element location
				mp.insert(pair<int,int> (count_zeros,(*it).cid));//Mapping the free_cache_element_number to the actual cid of the cache_element location
			}
			it++;
		}
		//Example of mp for 3 locations with cid 4,5,7 is as follows//
		//mp.first        mp.second
		//1                4
		//2                5
		//3                7
		///////////vec_coor for object i with free cache_element_locations 1,5,6 will be///
		////Index    x      y
		////0       x1     y1
		////1       x5     y5
  		////2       x6     y6
  		/////////vec_cost is of the form for object i///
  		///Index  Cost
  		///0       x
  		///1       y
  		///2       z
  		
  		
		vector<pair<int,int> >::iterator ic;//Iterator for free cache_element locations co-ordinates
		vector<int>::iterator icos;//iterator for vec_cost
		
		icos=vec_cost.begin();//Setting icos to the first cost,ie cost of placing the object i in the 1st free cache element location
		
		ic=vec_coor.begin();//Setting ic to the first free cache_element_location co-ordinates
//		cout<<"ffff\n";
		
		/////////////////////Displaying The Free locations co-ordinates and the cost of placing object i in that location
	//	while(ic!=vec_coor.end())
	//	{
//			cout<<(*ic).first<<" "<<(*ic).second<<"\n";
//			cout<<(*icos)<<"\n";
	//		ic++;
	//		icos++;
	//	}
		
		///////////////Finding the clients interested in object i//////////
		vector<int>vec_n_cli;//To hold actual client number of client demanding the object i
		vector<pair<int,int> >vec_cli_cor;//To hold the co-ordinate of the clients demanding object i
		
		int count_z=0;//To count the client demanding object i in serial order from 1.
		
		vector<int>vec_demand;
		//Checking all the clients whether they demand object i or not
		for(int j=1;j<=n_cli_;j++)
		{
			if(ve_cli[j-1].second.first==i)//If the client j demand object i
			{ 
			    vec_demand.push_back(ve_cli[j-1].second.second);
				count_z++;//Increment the number of clients demanding object i by 1
				vec_n_cli.push_back(j);//Push the client number of the client to vec_n_cli
				mp_cli.insert(pair<int,int> (count_z,j));//map the serial_number_of_the_client demanding object i to actual client number
				vec_cli_cor.push_back(make_pair<int,int>(ve_cli_cor[j-1].first,ve_cli_cor[j-1].second));///Pushing the Co-ordinate of the client demanding the object i
			}
		}
		///////vec_n_cli is of the form when client 1,3,5 demand object i
		///Index Client_number
		///0      1
		///1      3
		///2      5
		//////mp_cli is of the form for clients 1,3,5 demanding object i
		//mp_cli.first    mp_cli.second
		// 1               1
		// 2               3
		// 3               5             
		///////vec_cli_cor is of the form for clients 1,3,5 demanding object i
		//Index x   y
		//0     x1  y1
		//1     x3  y3
		//2     x5  y5
		
		//Displaying the client element co-ordinates of the clients demanding object i
		ic=vec_cli_cor.begin();
	//	while(ic!=vec_cli_cor.end())
	//	{
	//		cout<<(*ic).first<<"\t"<<(*ic).second<<"\n";
	//		ic++;
	//	}
		vector<pair<int,int> >vp;//Will hold the facilty location(ie cache element number) and the client assigned to that location
	
	vp=ufl_fun(count_zeros,vec_coor,vec_cost,count_z,vec_cli_cor,vec_demand);//Calling UFL with number_of_facilty_locations,co-ordinates of the facilty location,
       	
	//cost of opening the respective facilities,number of clients demanding the object i and co-ordinates of the clients demanding the object i 
		
//vp is of the form
//Index facility_number client_number
//0          2             1
//1          1             2
//1          1             3  		
		
		vector<pair<int,int> >::iterator ittt;//Iterator for the assignment of facilty location to client,returned by UFL
	    ittt=vp.begin();//setting it to the first pair returned by UFL
	    
	while(ittt!=vp.end())//Iterating over all the pairs returned by UFL
	{
		c[mp.find((*ittt).first)->second-1].oid=i;//Setting the actual client_element(cid)'s oid with i,using the mapping from mp
//		cout<<(*ittt).first<<"\t"<<(*ittt).second<<"\n";//Displaying the facilty number and client number pair returned by UFL
		
		pair<int,int>pa_ca=make_pair(c[mp.find((*ittt).first)->second-1].x,c[mp.find((*ittt).first)->second-1].y);//Making pair of co-ordinates of the actual cache_element
//locations' x and y co-ordinates		
//		cout<<pa_ca.first<<" ddd "<<pa_ca.second<<"\n";//Displaying the pair of x and y co-ordinates of the client_location(facilty location,returned by UFL)
		
		pair<int,int>pa_cli=make_pair(ve_cli_cor[(mp_cli.find((*ittt).second)->second)-1].first,ve_cli_cor[(mp_cli.find((*ittt).second)->second)-1].second);
//Making pair of actual client co-ordinates served by the current open facility,from the mp_cli map prepared earlier
//		cout<<(*ittt).second<<" "<<mp_cli.find((*ittt).second)->second-1<<"\n";//Displaying the map of client number returned by UFl and the actual client number
		
//		cout<<pa_cli.first<<" ddd "<<pa_cli.second<<"\n";//Displaying the client's actual co-ordinates
		double dis_h=cal_assign_cost(pa_ca,pa_cli)*(ve_cli[mp_cli.find((*ittt).second)->second-1].second.second);//2nd value is demand
	//	double diss_h=(ve_cli[mp_cli.find((*ittt).second)->second-1].second.second)*cal_assign_cost(pa_ca,pa_cli)*(ve_cli[mp_cli.find((*ittt).second)->second-1].second.second);
	//	cout<<"ggg"<<"\n";
	//	cout<<dis_h<<"\n";
		
	//	cout<<"mul\n";
	//	cout<<(ve_cli[mp_cli.find((*ittt).second)->second-1].second.second)<<"\n";
	//	cout<<"ttt\n";
//		cout<<dis_h<<"\n";
//Calculating the assignment cost by using cal_assign_cost function and then multiplying it with the corresponding demand
		assign_local_cost+=dis_h;//Increment the assignment cost for the current iteration by dis_h
		ittt++;//Increment the iterator to the next pair of location and client
	}
//	cout<<"mmmmmmm\n";
//	cout<<i<<"\n";
//	cout<<assign_local_cost<<"\n";
	ittt=vp.begin();//Setting ittt to the first pair returned by UFl
	int xz=-1;//Setting xz to -1 initially
	//facilty locations returned by UFL are in non-decreasing order
	//So when in a pair returned by UFl,the facilty number changes ,it's actual opening cost will be added to the opening cost for the current iteration
	while(ittt!=vp.end())//Iterating over all the pairs returned by UFL
	{
		if(c[mp.find((*ittt).first)->second-1].cid!=xz)//If there is a new open facilty encountered
		{
			xz=c[mp.find((*ittt).first)->second-1].cid;//Set xz to the new open facilty
			open_local_cost+=(ve_obj[ ((c[mp.find((*ittt).first)->second-1].oid-1)*n_cac_)+(c[mp.find((*ittt).first)->second-1].cid+(n_n-1))/n_n -1].second.second);
		//Set open_local_cost to the cost of opening the facilty,ie.placing object i in the cache_element
		}
		ittt++;//Increment the iterator
	}
	
	/*	for(int iy=1;iy<=(n_n*n_cac_);iy++)
	{
		//ran_=rand()%(fea_counter+1);
		//c[i-1].oid=object_vec[ran_];//index based computation of random elements to open in the blank spaces
		cout<<"ou\n";
		cout<<c[iy-1].getID()<<"\t";
		cout<<c[iy-1].oid<<"\n";
	}
	
	*/
	
	}
	
	
	//Once all the object are considered one after the another, check whether assignment of cache_element locations to objects have changed or not.
	//If not changed then stop
	
	bool tru=true;//Set a variable to true 
	itc=c.begin();//Iterator for c
	itc2=vce_copy.begin();//Iterator for vce_copy(the previous copy of c)
	
	while(itc!=c.end())//Check all the elements of c
	{
		if((*itc).oid!=(*itc2).oid)//If there is a change in the assignment of cache_element_location to object
		{
			tru=false;//set tru to false and break
			break;
		}
		itc2++;//Increment the iterator for c's copy
		itc++;//Increment the iterator for c
	}
	
	if(tru==true)//If there is no change in c since previous iteration
	{
/////////////Displaying the content of the cache_element in the solution///////
		itc=c.begin();//Set itc to the first cache element
		int cv=0;//A counter to display the cache_element number
		while(itc!=c.end())//Iterate over all the cache_element locations
	{
		cv++;//Increment the counter for cache_element number
//		cout<<cv<<"\t"<<(*itc).oid<<"\n";//Display cache_element number and the object in it
		itc++;//Increment the iterator
	}
		
		
		break;//Break out of while loop
	}
//	cout<<assign_local_cost<<"\n";//Display the assignment cost in the current iteration
      }
      
      cout<<assign_local_cost<<"\n";//Display the final assignment cost
      cout<<open_local_cost<<"\n";//Display the final opening cost
      cout<<assign_local_cost+open_local_cost<<"\n";//Display the final total cost
      t = clock() - t;
 // printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
  //printf("Time taken: %.6fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  cout.precision(8);
  cout << fixed<<double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
	return 0;
}
