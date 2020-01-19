#include<bits/stdc++.h>
using namespace std;

class client//client object
{
    int x,y;
    int cid;
public:
    static int scid;

    friend bool operator< (const client &c1, const client &c2);
    client (const client &obj)
    {
        this->x=obj.getX();
        this->y=obj.getY();
        this->cid=obj.getID();
    }
    client(int a=0,int b=0)
    {
        x=a;
        y=b;
        scid=scid+1;
        cid=scid;
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

int client::scid=0;

bool operator< (const client &c1, const client &c2)
{
    return (c1.getID()<c2.getID());
}
class facility//facility object
{
    int x,y;
    int fid;
    double cost;
public:
    static int sfid;

    bool truth;
     friend bool operator<(const facility&f1,const facility &f2);
    facility(int a=0,int b=0,int cst=0)
    {
        x=a;
        y=b;
        cost=cst;
        truth=false;
        sfid=sfid+1;
        fid=sfid;
    }
    facility (const facility &obj)
    {
        this->x=obj.getX();
        this->y=obj.getY();
        this->fid=obj.getID();
        this->truth=obj.truth;
        this->cost=obj.getCost();
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
        return fid;
    }
    double getCost() const
    {
        return cost;
    }
};

int facility::sfid=0;

bool operator< (const facility &f1, const facility &f2)
{
    return (f1.getID()<f2.getID());
}
double assign_matrix[10005][10005];
double cal_distance(client c,facility f,int demand_d)//function to calculate distance between a facility and a client
{
    double x_diff=c.getX()-f.getX();
    double y_diff=c.getY()-f.getY();

    double x_diffs=x_diff*x_diff;
    double y_diffs=y_diff*y_diff;
    
    double result=sqrt(x_diffs+y_diffs);
    
    return (result*demand_d);
}
vector<pair<int,int> > ufl_fun(int obj_n , vector<pair<int,int> >v_obj , vector<int> co_obj, int cli_n, vector<pair<int,int> >v_cli, vector<int>vec_demand)
{
  facility::sfid=0;
  client::scid=0;	
    vector<pair<int,int> >v_ufl;
    int n_clie,n_faci;
    double facility_cost,assign_cost,ct;
    int number_of_open_facilities=0;
    
    facility_cost=0;
    assign_cost=0;
    n_clie=cli_n;
    n_faci=obj_n;

    vector<client>c;
    vector<facility>f;
    vector<facility>f_assign;

    int ax,ay;
     for(int i=1;i<=n_clie;i++)
     {
         ax=v_cli[i-1].first;
         ay=v_cli[i-1].second;
         client temp(ax,ay);
         c.push_back(temp);
     }
      for(int i=1;i<=n_faci;i++)
     {
         ax=v_obj[i-1].first;
         ay=v_obj[i-1].second;
         ct=co_obj[i-1];
         facility temp(ax,ay,ct);
         f.push_back(temp);
         f_assign.push_back(temp);
     }

     vector<client>::iterator itc;
     itc=c.begin();    
     vector<facility>::iterator itf;
     itf=f.begin();    
     int n_faci_left=n_faci;
     vector<facility>::iterator fit,fiit;
     map<client,facility>mp;//mapping clients to facilities
      
     int r_var;
     srand(time(NULL));
     int f_open=0;
     while(true)
     {
       f_open=0; 
       for(int i=1;i<=n_faci;i++)
          {
          r_var=rand()%2;
          bool close_fa=false;
        if(r_var==1)
		  {
              f_open++;
              f_assign[i-1].truth=true;
              facility_cost+=f_assign[i-1].getCost();
            number_of_open_facilities++;
        }
      }
      if(f_open!=0)//If no facility is opened randomly, open again
      {
          break;
      }
       }
      double dist_min;
      double fac_min;
     for(int i=1;i<=n_clie;i++)//Assigning all the clients to the nearest open facility
     {
            dist_min=numeric_limits<double>::max();
            for(int j=1;j<=n_faci;j++)
            {
                if((f_assign[j-1].truth==true)&&(cal_distance(c[i-1],f_assign[j-1],vec_demand[i-1])<dist_min))
                {
                    dist_min=cal_distance(c[i-1],f_assign[j-1],vec_demand[i-1]);
                    fac_min=j;
                }
            }
             mp.insert(pair<client,facility>(c[i-1],f_assign[fac_min-1]));
             
             assign_cost+=cal_distance(c[i-1],f_assign[fac_min-1],vec_demand[i-1]);
     }
     for(int i=1;i<=n_clie;i++)//Displaying the assignment of clients to facilities
     {
         v_ufl.push_back(make_pair<int,int> (mp.find(c[i-1])->second.getID(),i));
     }
     double max_benefit_all=-numeric_limits<double>::max();//Max benefit to check max benefit from add,delete or swap
     char max_benefit_task;//To keep an account of which one(add,delete or swap) gave max benefit
     int times=0;//To keep track of number of passes of local search
     while(true)//Keep iterating until there is no reduction in cost
     {
         times++;
     max_benefit_all=-numeric_limits<double>::max();
     double swap_assign_benefit=0;//To hold the maximum benefit that can be obtained from swap move
     double benefit_max=0;
    
     double temp_swap_assign_benefit=0;//To hold the max swap benefit that can be obtained from opening a particular closed facility among the ones already considered
       bool conti=false;//to check atleast 1 facility is close,we do not need to check if 1 facilty is opened because it is always true

     double temp_inner_swap_benefit=0;//To hold the benefit to all the clients from a particular facility being closed and a particlar facility being open,under consideration
     
    
     int counter_del=0;

     int inner_swap_close_faci=-1;
     
     double red_assign_cost;
     double red_fac_cost;
     
     double inner_benefit;//To hold the benefit in cost for a particular client from a particular facility being closed and a particlar facility being open,under consideration
      double temp_benefit=0;//To hold the negative of the cost of the facility being open for a particular swap move
    
    
	int benefit_max_fac;
	
	 int swap_open_faci;
     int swap_close_faci;
     
     double temp_facility_cost_del=facility_cost;
    double temp_assign_cost_del=assign_cost;
    
    double del_cost=0;
    double red_assign_cost_del=0;
    double benefit=0;
    
    
    double temp_red_cost=-numeric_limits<double>::max();
     
     
	 
     bool counter_add=false;//Add only when at least 1 facilty is not opened
     for(int i=1;i<=n_faci;i++)//Checking for at least 1 closed facility
     {
         if(f_assign[i-1].truth==false)
         {
             counter_add=true;
             break;
        }
     }
     double temp_facility_cost_add=facility_cost;
     double temp_assign_cost_add=assign_cost;
     
     double open_cost=0;
     double red_assign_cost_add=0;
     double benefit_add=0;
     double benefit_max_add=0;
     int benefit_max_fac_add;
     
     if(counter_add==true)//Add when at least 1 facility is closed
     {
     for(int i=1;i<=n_faci;i++)
     {
         if(f_assign[i-1].truth==false)//If facility is closed,open and check the benefit
         {
             open_cost=f_assign[i-1].getCost();
             red_assign_cost_add=0;
             for(int j=1;j<=n_clie;j++)
             {
                 if(cal_distance(c[j-1],f_assign[i-1],vec_demand[j-1]) < cal_distance(c[j-1],mp.find(c[j-1])->second,vec_demand[j-1]) )
                 {
                     red_assign_cost_add+=(cal_distance(c[j-1],mp.find(c[j-1])->second,vec_demand[j-1])-cal_distance(c[j-1],f_assign[i-1],vec_demand[j-1]));
                 }
            }
            benefit_add=red_assign_cost_add-open_cost;
            if(benefit_add>0)//If benefit is greater than 0 then only consider opening the facility
            {
                if(benefit_add>benefit_max_add)//Finding Max benefit from opening all of the closed facilities separately
                {
                    benefit_max_add=benefit_add;
                    benefit_max_fac_add=i;
                }
            }
        }
     }
       }
     if(benefit_max_add>0)
     {
     	goto label;
	 }

     //Delete move
     for(int i=1;i<=n_faci;i++)//Check at least 2 facilites are opened or not
     {
         if(f_assign[i-1].truth==true)
         {
             counter_del++;
        }
     }
     
    
     if(counter_del>1)
     {
         //If more than 1 facility is opened, only then delete move
     for(int i=1;i<=n_faci;i++)
     {
         if(f_assign[i-1].truth==true)//If facility is opened ,consider closing it and find the reduction in total cost
         {
             del_cost=f_assign[i-1].getCost();
             red_assign_cost_del=0;
             for(int j=1;j<=n_clie;j++)
             {
                 if(mp.find(c[j-1])->second.getID()==f_assign[i-1].getID())
                 {
                 temp_red_cost=-numeric_limits<double>::max();
                 for(int k=1;k<=n_faci;k++)
                 {
                       if((f_assign[k-1].truth==true)&&(f_assign[k-1].getID()!=f_assign[i-1].getID()))
                       {
                          if((mp.find(c[j-1])->second.getID()==f_assign[i-1].getID())||(cal_distance(c[j-1],f_assign[k-1],vec_demand[j-1]) < cal_distance(c[j-1],mp.find(c[j-1])->second,vec_demand[j-1]) ))
                             {
                                 temp_red_cost=max((cal_distance(c[j-1],mp.find(c[j-1])->second,vec_demand[j-1])-cal_distance(c[j-1],f_assign[k-1],vec_demand[j-1])),temp_red_cost);
                            }             
                       }     
                 }
                 red_assign_cost_del+=temp_red_cost;
                }
            }
            
            benefit=red_assign_cost_del+del_cost;
            if(benefit>0)//If benefit is positive ,then only consider
            {
                if(benefit>benefit_max)//Find the maximum benefit from closing all the opened facilities separately
                {
                    benefit_max=benefit;
                    benefit_max_fac=i;
                }
            }
         }
     }
        }
     
     if(benefit_max>0)
     {
     	goto label;
	 }
     
    
     
     //swap;
   
     for(int i=1;i<=n_faci;i++)
     {
         if(f_assign[i-1].truth==false)
         {
         conti=true;
         break;    
        }
     }
     
     
     
     
    
     if(conti==true)//If atleast 1 facilty is closed,then only consider swap move
     {
         swap_assign_benefit=-numeric_limits<double>::max();
         for(int i=1;i<=n_faci;i++)
         {
             temp_swap_assign_benefit=-numeric_limits<double>::max();
            if(f_assign[i-1].truth==false)//If facility is closed then open it and then check for replacement with all opened facilities
            {
                temp_benefit=-(f_assign[i-1].getCost());
                for(int j=1;j<=n_faci;j++)
                {
                    if(f_assign[j-1].truth==true)//Open facility,consider closing it and find the benefit
                    {
                       temp_inner_swap_benefit=temp_benefit+f_assign[j-1].getCost();
                       for(int k=1;k<=n_clie;k++)
                       {
                           if(mp.find(c[k-1])->second.getID()==f_assign[j-1].getID())//If client is assigned to the facility which is to be closed
                           {
                               inner_benefit=-numeric_limits<double>::max();
                            for(int l=1;l<=n_faci;l++)
                            {
                                  if(((f_assign[l-1].truth==true)&&(f_assign[l-1].getID()!=f_assign[j-1].getID() ))||(f_assign[l-1].getID()==f_assign[i-1].getID()))
                                  {
                                               inner_benefit=max(inner_benefit,cal_distance(c[k-1],mp.find(c[k-1])->second,vec_demand[k-1])-cal_distance(c[k-1],f_assign[l-1],vec_demand[k-1]));
                                  
                                  }        
                            }
                                
                        }else if(cal_distance(c[k-1],f_assign[i-1],vec_demand[k-1])<cal_distance(c[k-1],mp.find(c[k-1])->second,vec_demand[k-1]))//If client is not assigned to the facilty being closed but assigning it to the newly opened facility gives reduction in cost                
                        {
                            inner_benefit=cal_distance(c[k-1],mp.find(c[k-1])->second,vec_demand[k-1])-cal_distance(c[k-1],f_assign[i-1],vec_demand[k-1]);
                        }else//If client is not assigned to the facility being closed and assigning it to newly opened facility does not give reduction in cost
                        {
                            inner_benefit=0;
                        }
                        temp_inner_swap_benefit+=inner_benefit;//Add the benefit from all the clients
                       }
                       if(temp_inner_swap_benefit>0)//If total benefit is positive,then only consider it
                       {
                           
                           inner_swap_close_faci=j;
                       }
                       temp_swap_assign_benefit=max(temp_swap_assign_benefit,temp_inner_swap_benefit);//Find maximum benefit possible from all the combinations of just closed and all opened facilities
                      }
            }
            if(temp_swap_assign_benefit>swap_assign_benefit)//Find max benefit from all combinations of all closed and all opened facilities
            {
                swap_open_faci=i;//Record the opened facilty
                swap_close_faci=inner_swap_close_faci;//Record the closed facility
            }
            swap_assign_benefit=max(swap_assign_benefit,temp_swap_assign_benefit);//Find max benefit from all combinations of all closed and all opened facilities
           }
        }
            
     }
     
     
     label:
     //Checking max benefit from add,delete and swap
     
     if((counter_add==true)&&(benefit_max_add>max_benefit_all))//If add gives max benefit,record it
     {
         max_benefit_all=benefit_max_add;
         max_benefit_task='a';
     }
     if((counter_del>1)&&(benefit_max>max_benefit_all))//If delete gives max benefit,record it
     {
         max_benefit_all=benefit_max;
         max_benefit_task='d';
     }
     if((conti==true)&&(swap_assign_benefit>max_benefit_all)&&(swap_assign_benefit>0))//If swap gives max benefit,record it
     {
         max_benefit_all=swap_assign_benefit;
         max_benefit_task='s';
     }
     if(max_benefit_all>0)//If max benefit from add,delete and swap is positive the incorporate the change
     {
         switch(max_benefit_task){
             
             case 'a'://Add move
                     f_assign[benefit_max_fac_add-1].truth=true;
                     red_assign_cost_add=0;
                     for(int j=1;j<=n_clie;j++)
                         {
                             if(cal_distance(c[j-1],f_assign[benefit_max_fac_add-1],vec_demand[j-1]) < cal_distance(c[j-1],mp.find(c[j-1])->second,vec_demand[j-1]) )
                             {
                                 red_assign_cost_add+=(cal_distance(c[j-1],mp.find(c[j-1])->second,vec_demand[j-1])-cal_distance(c[j-1],f_assign[benefit_max_fac_add-1],vec_demand[j-1]));
                                 mp.erase(c[j-1]);
                                 mp.insert(pair<client,facility> (c[j-1],f_assign[benefit_max_fac_add-1]));
                            }
                         }
                    temp_assign_cost_add-=red_assign_cost_add;
                    temp_facility_cost_add+=(f_assign[benefit_max_fac_add-1].getCost());
                    v_ufl.clear();
                    for(int i=1;i<=n_clie;i++)
                     {
                        v_ufl.push_back(make_pair<int,int> (mp.find(c[i-1])->second.getID(),c[i-1].getID()));
                     }
                     facility_cost=temp_facility_cost_add;//Change the current facilty cost to the new facilty cost
                     assign_cost=temp_assign_cost_add;//Change the current assignment cost to the new cost
                    break;
            case 'd'://Delete move
                    f_assign[benefit_max_fac-1].truth=false;
                     del_cost=f_assign[benefit_max_fac-1].getCost();
                         red_assign_cost_del=0;
                     for(int j=1;j<=n_clie;j++)
                         {
                             if(mp.find(c[j-1])->second.getID()==f_assign[benefit_max_fac-1].getID())
                             { 
                             double red_cost_del_err=-numeric_limits<double>::max();
                             	int which_one=0;
                             for(int k=1;k<=n_faci;k++)
                             {
                                      	
                                   if((f_assign[k-1].truth==true))
                                   {
                                   	          if((cal_distance(c[j-1],mp.find(c[j-1])->second,vec_demand[j-1])-cal_distance(c[j-1],f_assign[k-1],vec_demand[j-1]))>red_cost_del_err)
                                   	          {
                                   	          	which_one=k;
                                   	          	red_cost_del_err=(cal_distance(c[j-1],mp.find(c[j-1])->second,vec_demand[j-1])-cal_distance(c[j-1],f_assign[k-1],vec_demand[j-1]));
											 }
                                      }     
                             }
                             mp.erase(c[j-1]);
                             mp.insert(pair<client,facility> (c[j-1],f_assign[which_one-1]));
                             red_assign_cost_del+= red_cost_del_err;
                             
                            }
                        }
                    temp_assign_cost_del-=red_assign_cost_del;
                    temp_facility_cost_del-=(f_assign[benefit_max_fac-1].getCost());
                    v_ufl.clear();
                    for(int i=1;i<=n_clie;i++)
                     {
			    v_ufl.push_back(make_pair<int,int> (mp.find(c[i-1])->second.getID(),c[i-1].getID()));
                     }
		     facility_cost=temp_facility_cost_del;//Change the current facilty cost to the new facilty cost
                     assign_cost=temp_assign_cost_del;//Change the current assignment cost to the new cost
                     break;
            case 's'://Swap move
                    f_assign[swap_open_faci-1].truth=true;
                    f_assign[swap_close_faci-1].truth=false;
                    red_fac_cost=f_assign[swap_close_faci-1].getCost()-f_assign[swap_open_faci-1].getCost();
                    red_assign_cost=0;
                    for(int i=1;i<=n_clie;i++)
                    {
                        if(mp.find(c[i-1])->second.getID()==f_assign[swap_close_faci-1].getID())
                        {
                        	double red_cost_swa_err=-numeric_limits<double>::max();
                             	int which_one_swap=0;
                            for(int j=1;j<=n_faci;j++)
                            {
                                if(f_assign[j-1].truth==true)
                                {
					if((cal_distance(c[i-1],mp.find(c[i-1])->second,vec_demand[i-1])-cal_distance(c[i-1],f_assign[j-1],vec_demand[i-1]))>red_cost_swa_err)
                                   	          {
                                   	          	which_one_swap=j;
                                   	          	red_cost_swa_err=((cal_distance(c[i-1],mp.find(c[i-1])->second,vec_demand[i-1])-cal_distance(c[i-1],f_assign[j-1],vec_demand[i-1])));
											 }
                                     
                                }
                            }
                             mp.erase(c[i-1]);
                             mp.insert(pair<client,facility> (c[i-1],f_assign[which_one_swap-1]));
                             red_assign_cost+= red_cost_swa_err;
                        }else if(cal_distance(c[i-1],f_assign[swap_open_faci-1],vec_demand[i-1])<cal_distance(c[i-1],mp.find(c[i-1])->second,vec_demand[i-1]))
                        {
                            red_assign_cost+=cal_distance(c[i-1],mp.find(c[i-1])->second,vec_demand[i-1])-cal_distance(c[i-1],f_assign[swap_open_faci-1],vec_demand[i-1]);
                            mp.erase(c[i-1]);
                             mp.insert(pair<client,facility> (c[i-1],f_assign[swap_open_faci-1]));
                        }
                    }
                    
                    double assignment_cost=assign_cost-red_assign_cost;
                    double facility_cost_swap=facility_cost-red_fac_cost;
                    v_ufl.clear();
                    for(int i=1;i<=n_clie;i++)
                     {
			v_ufl.push_back(make_pair<int,int> (mp.find(c[i-1])->second.getID(),c[i-1].getID()));
                     }
                     facility_cost=facility_cost_swap;//Change the current facilty cost to the new facilty cost
                     assign_cost=assignment_cost;//Change the current assignment cost to the new cost
                             break;
                                 }
     }else//If no reduction is total cost is possible ,then stop the local search
     {
         break;
     }
        }
    return v_ufl;
}
