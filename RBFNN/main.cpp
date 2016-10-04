//
//  main.cpp
//  RBFNN
//
//  Created by Sankarsan Seal on 30/09/16.
//  Copyright © 2016 Sankarsan Seal. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cmath>
#include <cfloat>

using namespace std;

const char PATH[1024]="/Users/sankarsanseal/Documents/RBFNN/RBFNN/";
void rbfnn(int noOfDimensions, int noOfClasses, int k, FILE * train, FILE * test);

double euclidean( double *centroiddim, double * patterndim, int noOfDimension)
{
    double result=0;
    int i;
    for(i=0;i<noOfDimension;i++)
    {
        result+=pow((centroiddim[i]-patterndim[i]),2);
    }
    return sqrt(result);
}

double rbf(double * centroid, double * pattern, int noOfDimension)
{
    double result=0;
    result=exp(-1*centroid[noOfDimension]*pow(euclidean(centroid, pattern, noOfDimension),2));
    return result;
}

double sigmoid(double * hidden, double ** weight, int noOfHidden, int neuron_ind)
{
    int i;
    double result=0;
    double induced_local=0;
    for(i=0;i<noOfHidden;i++)
    {
        induced_local+=hidden[i]*weight[i][neuron_ind];
    }
    
    result=(1/(1+exp(-induced_local)));
    
    
    return result;
    
}

double ** kmean(double ***dcluster, int *dclass_count,int k,int noOfDimensions, int noOfClasses)
{
    int i,j,l,m;
    double **centroids=NULL;
    double **tempcentroids=NULL;
    centroids=(double **)malloc(sizeof(double *)*k*noOfClasses);
    tempcentroids=(double **)malloc(sizeof(double*)*k*noOfClasses);
    bool changed=true;
    int count[k*noOfClasses];
    
    //cout<<"no of Classes:"<<noOfClasses<<endl;
    
    for(i=0;i<k*noOfClasses;i++)
    {
        centroids[i]=(double *)malloc(sizeof(double)*(noOfDimensions+1));
        tempcentroids[i]=(double *)malloc(sizeof(double)*(noOfDimensions+1));
        count[i]=0;
    }
    
    srand((unsigned)time(NULL));
    
    if(centroids!=NULL)
    {
        for(i=0;i<noOfClasses;i++)
        {
            int base_index=(i*k);
            for(l=0;l<k;l++)
            {
                int index=-1;
               // cout<<i<<" : dclass_count"<<dclass_count[i]<<endl;
                index=rand()%dclass_count[i];
                //cout<<index<<endl;
                for(m=0;m<noOfDimensions;m++)
                {
                    centroids[base_index+l][m]=dcluster[i][index][m];
                }
                centroids[base_index+l][noOfDimensions]=0;
                
            }
        }
    }
    /*
     for(i=0;i<noOfClasses;i++)
     {
     for(j=0;j<k;j++)
     {
     for(l=0;l<noOfDimensions+1;l++)
     {
     cout<<centroids[(i*k)+j][l]<<" ";
     }
     cout<<endl;
     }
     cout<<"***"<<endl;
     }
     */
    while(changed)
    {
        changed=false;
        for(i=0;i<noOfClasses;i++)
        {
            int start_index=(i*k);
            int end_index=(i*k)+k;
            for(l=0;l<dclass_count[i];l++)
            {
                
                double min=DBL_MAX;
                double distance=0;
                int minindex=-1;
                for(j=start_index;j<end_index;j++)
                {
                    
                    distance=euclidean( centroids[j],dcluster[i][l],noOfDimensions);
                    if(min>distance)
                    {
                        min=distance;
                        minindex=j;
                    }
                }
                if(dcluster[i][l][noOfDimensions]!=(double)minindex)
                {
                    dcluster[i][l][noOfDimensions]=(double)minindex;
                    // cout<<"minindex "<<dcluster[i][l][noOfDimensions]<<" : "<<minindex<<endl;
                    changed=true;
                }
            }
            
            
        }
        if(changed)
        {
            
            for(i=0;i<k*noOfClasses;i++)
            {
                count[i]=0;
                for(j=0;j<=noOfDimensions;j++)
                {
                    tempcentroids[i][j]=0;
                }
            }
            for(i=0;i<noOfClasses;i++)
            {
                int base_index=i*k;
                for(j=0;j<dclass_count[i];j++)
                {
                    
                    for(l=base_index;l<base_index + k;l++)
                    {
                        //cout<<dcluster[i][j][noOfDimensions]<<" : " <<l<<endl;
                        if(dcluster[i][j][noOfDimensions]==(double)l)
                        {
                            // cout<<"here"<<endl;
                            count[l]++;
                            for(m=0;m<noOfDimensions;m++)
                                tempcentroids[l][m]+=dcluster[i][j][m];
                            
                            // for(m=0;m<=noOfDimensions;m++)
                            //  cout<<" l:"<<l<<" m:"<<m<<" "<<tempcentroids[l][m]<<" ";
                        }
                        //cout<<endl;
                    }
                    
                }
                
                int start_index=i*k;
                int end_index=(i*k)+k;
                for(j=start_index;j<end_index;j++)
                {
                    if(count[j]!=0)
                    {
                        
                        for(l=0;l<noOfDimensions;l++)
                        {   //cout<<"j:"<<j<<" l:"<<l<<" temp:"<<tempcentroids[j][l]<<" ";
                            
                            tempcentroids[j][l]/=count[j];
                            // cout<<"temp avg"<<tempcentroids[j][l]<<" ";
                        }
                        // cout<<endl;
                        
                        
                    }
                }
                
                for(j=0;j<dclass_count[i];j++)
                {
                    int tempindex=(int)dcluster[i][j][noOfDimensions];
                tempcentroids[tempindex][noOfDimensions]+=euclidean(tempcentroids[tempindex], dcluster[i][j], noOfDimensions);
                }
                
                for(j=start_index;j<end_index;j++)
                {
                    if(tempcentroids[j][noOfDimensions]!=0)
                    {
                        tempcentroids[j][noOfDimensions]/=count[j];
                    }
                }
            }
            
            
            for(i=0;i<k*noOfClasses;i++)
            {

                if(count[i]!=0 && tempcentroids[i][noOfDimensions]!=0)
                {
                    for(j=0;j<=noOfDimensions;j++)
                    {
                        
                        centroids[i][j] = tempcentroids[i][j];
                        
                    }
                   // if(tempcentroids[i][j]==0)
                     //   cout<<i<<":"<<j<<":"<<tempcentroids[i][j]<<endl;
                }
            }
            
            
        }
    }

    for(i=0;i<k*noOfClasses;i++)
    {
        if(centroids[i][noOfDimensions]!=0)
        centroids[i][noOfDimensions]=1/(2*pow(centroids[i][noOfDimensions],2));
        else
            centroids[i][noOfDimensions]=DBL_EPSILON;
    }
    
    return centroids;
}

void wine()
{
    char temp[1024];
    strcpy(temp,PATH);
    int noOfDimensions=13;
    int noOfClasses=3;
    //int noOfNodesHidden=0;
    int k=0;
    
    FILE * fp;
    FILE * fpt;
    
    
    fp=fopen(strcat(temp,"wine.train.txt"),"r");
    
    if(fp!=NULL)
    {
        cout<<"Enter the number of cluster per class:";
        cin>>k;
        //noOfNodesHidden=k;
        strcpy(temp,PATH);
        
        fpt=fopen(strcat(temp,"wine.test.txt"),"r");
        
        if(fpt!=NULL)
        {
            rbfnn(noOfDimensions, k , noOfClasses, fp, fpt);
            fclose(fp);
            fclose(fpt);
        }
        else
        {
            cout<<"Problem with Testing File opening."<<endl;
        }
        
        
    }
    else
    {
        cout<<"Problem with Training File opening."<<endl;
    }
    
    
}

void iris()
{
    char temp[1024];
    strcpy(temp,PATH);
    //int noOfNodesHidden=0;
    int noOfDimensions=4;
    int noOfClasses=3;
    int k=0;
    
    FILE * fp;
    FILE * fpt;
    
    
    fp=fopen(strcat(temp,"iris.train.txt"),"r");
    
    if(fp!=NULL)
    {
        cout<<"Enter the number of cluster per class:";
        cin>>k;
        
        strcpy(temp,PATH);
        
        fpt=fopen(strcat(temp,"iris.train.txt"),"r");
        
        if(fpt!=NULL)
        {
            rbfnn(noOfDimensions, k, noOfClasses, fp, fpt);
            fclose(fp);
            fclose(fpt);
        }
        else
        {
            cout<<"Problem with Testing File opening."<<endl;
        }
        
    }
    else
    {
        cout<<"Problem with Training File opening."<<endl;
    }
    
    
}


void rbfnn(int noOfDimensions, int k, int noOfClasses,  FILE * train, FILE * test)
{
    //char temp[1024];
    //strcpy(temp,PATH);
    int lines=0;
    FILE * fp;
    FILE *fpt;
   // int noOfDimension=4;
    //int noOfClasses=3;
    int i,j,l,m;
    int noOfNodesHidden=0;
    int noOfIterations=2000;
    double eta=0.1;
    
    double **dinput;
    double *testinput;
    double *testoutput;
    double **doutput;
    double **centroids;
    double ***dcluster;
    double *hidden;
    double **w1;
    double **w1delta;
    double *poutput;
    int *dclass_count;
    int ylabel;
    double *error;
    int confusion[noOfClasses][noOfClasses];
    
    //int count=0;
    
    fp=train;
    //fopen(strcat(temp,"iris.train.txt"),"r");
    lines=0;
    if(fp!=NULL)
    {

        noOfNodesHidden=k*noOfClasses;
        
        while(!feof(fp))
        {
            for(i=0;i<=noOfDimensions;i++)
            fscanf(fp,"%*lf");
            if(!feof(fp))
                lines++;
        }
        
        
        dinput=(double **)malloc(sizeof(double *)*lines);
        doutput=(double **)malloc(sizeof(double *)*lines);
        dclass_count=(int *)malloc(sizeof(int)*noOfClasses);
        dcluster=(double ***)malloc(sizeof(double *)*noOfClasses);
        
        
        for(i=0;i<noOfClasses;i++)
            dclass_count[i]=0;
        
        
        fseek(fp,0,SEEK_SET);
        
        for(i=0;i<lines;i++)
        {
            if((dinput[i]=(double *)malloc(sizeof(double)*noOfDimensions))
               &&
               (doutput[i]=(double *)malloc(sizeof(double)*noOfClasses))
               )
            {
                
                for(j=0;j<noOfDimensions;j++)
                {
                    fscanf(fp,"%lf",&dinput[i][j]);
                }
                fscanf(fp,"%d", &ylabel);
                
                for(j=0;j<noOfClasses;j++)
                {
                    if(j==(ylabel-1))
                    {
                        doutput[i][j]=1;
                        dclass_count[j]++;
                    }
                    else
                        doutput[i][j]=0;
                }
            }
            
        }
        for(i=0;i<noOfClasses;i++)
        {
            dcluster[i]=(double **)malloc(sizeof(double *)*dclass_count[i]);
            for(j=0;j<dclass_count[i];j++)
            {
                dcluster[i][j]=(double *)malloc(sizeof(double)*(noOfDimensions+1));
            }
        }
        
        for(i=0;i<noOfClasses;i++)
            dclass_count[i]=0;
        
        for(i=0;i<lines;i++)
        {
            for(j=0;j<noOfClasses;j++)
            {
                if(doutput[i][j]==1)
                {
                    for(l=0;l<noOfDimensions;l++)
                    {
                        dcluster[j][dclass_count[j]][l]=dinput[i][l];
                    }
                    dcluster[j][dclass_count[j]][noOfDimensions]=-1;
                    dclass_count[j]++;
                }
                
            }
            
        }
        /* for(i=0;i<noOfClasses;i++)
         {
         for(j=0;j<dclass_count[i];j++)
         {
         for(l=0;l<noOfDimension;l++)
         {
         cout<<dcluster[i][j][l]<<" ";
         }
         count++;
         cout<<endl;
         }
         cout<<"count***"<<count<<endl;
         }*/
        centroids=kmean(dcluster, dclass_count, k, noOfDimensions, noOfClasses);
        
        for(i=0;i<k*noOfClasses;i++)
        {
            for(j=0;j<=noOfDimensions;j++)
            {
                cout<<centroids[i][j]<<" ";
            }
            cout<<endl;
        }
        
        
        
        hidden=(double *)malloc(sizeof(double)*noOfNodesHidden);
        poutput=(double *)malloc(sizeof(double)*noOfClasses);
        error=(double *)malloc(sizeof(double)*noOfClasses);
        w1=(double **)malloc(sizeof(double *)*noOfNodesHidden);
        w1delta=(double **)malloc(sizeof(double *)*noOfNodesHidden);
        for(i=0;i<noOfNodesHidden;i++)
        {
            w1[i]=(double *)malloc(sizeof(double)*noOfClasses);
            w1delta[i]=(double *)malloc(sizeof(double)*noOfClasses);
            for(j=0;j<noOfClasses;j++)
            {
                w1[i][j]=(double)(rand()%10)/10;
                w1delta[i][j]=0;
            }
        }
        
        
        for(i=0;i<noOfIterations;i++)
        {
            for(l=0;l<lines;l++)
            {
                for(j=0;j<noOfNodesHidden;j++)
                {
                    hidden[j]=rbf(centroids[j],dinput[l],noOfDimensions);
                }
                
                for(j=0;j<noOfClasses;j++)
                {
                    poutput[j]=sigmoid(hidden,w1,noOfNodesHidden,j);
                    error[j]=(doutput[l][j]-poutput[j]);
                    //cout<<" d: " <<doutput[l][j]<<" p: "<<poutput[j];
                    
                }
                //cout<<endl;
                for(j=0;j<noOfNodesHidden;j++)
                {
                    for(m=0;m<noOfClasses;m++)
                    {
                        w1delta[j][m]=eta*error[m]*poutput[m]*(1-poutput[m])* hidden[j];
                    }
                }
                
                for(j=0;j<noOfNodesHidden;j++)
                {
                    for(m=0;m<noOfClasses;m++)
                    {
                        w1[j][m]+=w1delta[j][m];
                    }
                }
                
                
            }
            
        }
        
        for(i=0;i<noOfNodesHidden;i++)
        {
            for(j=0;j<noOfClasses;j++)
            {
                cout<<w1[i][j]<<" ";
            }
            cout<<endl;
        }
        
        for(i=0;i<noOfClasses;i++)
            for(j=0;j<noOfClasses;j++)
                confusion[i][j]=0;
        
        //strcpy(temp,PATH);
        
        fpt=test;
        //fopen(strcat(temp,"iris.test.txt"),"r");
        testinput=(double *)malloc(sizeof(double)*noOfDimensions);
        testoutput=(double *)malloc(sizeof(double)*noOfClasses);
        //cout<<"here"<<endl;
        while(!feof(fpt))
        {
            if(!feof(fpt))
            {
                for(i=0;i<noOfDimensions;i++)
                {
                    fscanf(fpt,"%lf",&testinput[i]);
                    //cout<<testinput[i]<<" ";
                }
                fscanf(fpt, "%d",&ylabel);
                //cout<<ylabel<<" "<<endl;
                
                for(i=0;i<noOfClasses;i++)
                {
                    if(i==(ylabel-1))
                    {
                        testoutput[i]=1;
                    }
                    else
                        testoutput[i]=0;
                }
                
                for(j=0;j<noOfNodesHidden;j++)
                {
                    hidden[j]=rbf(centroids[j],testinput,noOfDimensions);
                }
                
                for(j=0;j<noOfClasses;j++)
                {
                    testoutput[j]=sigmoid(hidden,w1,noOfNodesHidden,j);
                }
                
                int max_index=-1;
                double max=DBL_MIN;
                for(i=0;i<noOfClasses;i++)
                {
                    if(max < testoutput[i])
                    {
                        max=testoutput[i];
                        max_index=i;
                    }
                }
                
                confusion[ylabel-1][max_index]++;
                
            }
            
            
            
        }
        
        cout<<"Confusion Matrix of test data:"<<endl;
        for(i=0;i<noOfClasses;i++)
        {
            for(j=0;j<noOfClasses;j++)
            {
                cout<<confusion[i][j]<<" ";
            }
            cout<<endl;
        }
        
        
        
        
        
    }
    else
    {
        cout<<"Problem with File opening."<<endl;
    }
    
    
    
}

int main(int argc, const char * argv[]) {
    
    int choice;
    
    cout<<"Enter the choice for Classification problem with RBF"<<endl;
    
    cout<<"1. Wine Data (Dimension 13 and Classes 3)"<<endl;
    cout<<"2. Iris Data(Dimension 4 and Classes 3)"<<endl;
    
    cout<<"Enter the choice:";
    cin>>choice;
    switch(choice)
    {
        case 1:
            wine();
            break;
        case 2:
            iris();
            break;
    }
    
    
    return 0;
}
