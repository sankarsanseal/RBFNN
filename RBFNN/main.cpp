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

double ** kmean(double ***dcluster, int *dclass_count,int k,int noOfDimensions, int noOfClasses)
{
    int i,j,l,m;
    double **centroids=NULL;
    double **tempcentroids=NULL;
    centroids=(double **)malloc(sizeof(double *)*k*noOfClasses);
    tempcentroids=(double **)malloc(sizeof(double*)*k*noOfClasses);
    bool changed=true;
    
    for(i=0;i<k*noOfClasses;i++)
    {
        centroids[i]=(double *)malloc(sizeof(double)*(noOfDimensions+1));
        tempcentroids[i]=(double *)malloc(sizeof(double)*(noOfDimensions+1));
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
                index=rand()%dclass_count[i];
                cout<<index<<endl;
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
                            for(m=0;m<noOfDimensions;m++)
                                tempcentroids[l][m]+=dcluster[i][j][m];
                            tempcentroids[l][noOfDimensions]+=euclidean(centroids[ l], dcluster[i][j], noOfDimensions);
                            for(m=0;m<=noOfDimensions;m++)
                                cout<<" l:"<<l<<" m:"<<m<<" "<<tempcentroids[l][m]<<" ";
                        }
                        cout<<endl;
                    }
                    
                }
                
                int start_index=i*k;
                int end_index=(i*k)+k;
                for(j=start_index;j<end_index;j++)
                {
                    for(l=0;l<=noOfDimensions;l++)
                    {   //cout<<"j:"<<j<<" l:"<<l<<" temp:"<<tempcentroids[j][l]<<" ";
                        tempcentroids[j][l]/=dclass_count[i];
                       // cout<<"temp avg"<<tempcentroids[j][l]<<" ";
                    }
                   // cout<<endl;
                }
            }
            
            
            for(i=0;i<k*noOfClasses;i++)
            {
                bool all_zero=true;
                for(j=0;j<noOfDimensions;j++)
                {
                    if(tempcentroids[i][j]!=0)
                    {
                        all_zero=false;
                        break;
                    }
                    else
                        continue;
                        
                }
                if(!all_zero)
                {
                    for(j=0;j<=noOfDimensions;j++)
                    {
                        
                        centroids[i][j] = tempcentroids[i][j];
                    }
                }
            }
            
            
        }
    }
    for(i=0;i<k*noOfClasses;i++)
        centroids[i][noOfDimensions]=1/(2*pow(centroids[i][noOfDimensions],2));
    
    return centroids;
}

void wine()
{
    char temp[1024];
    strcpy(temp,PATH);
    int lines=0;
    FILE * fp;
    int noOfDimension=4;
    int noOfClasses=3;
    int i,j,k;
    
    double **dinput;
    double **doutput;
    double **centroids;
    int ylabel;
    
    fp=fopen(strcat(temp,"wine.train.txt"),"r");
    lines=0;
    if(fp!=NULL)
    {
        cout<<"Enter the number of cluster for each label:";
        cin>>k;
        
        while(!feof(fp))
        {
            fscanf(fp,"%*lf %*lf %*lf %*lf %*d");
            if(!feof(fp))
                lines++;
        }
        
        
        dinput=(double **)malloc(sizeof(double *)*lines);
        doutput=(double **)malloc(sizeof(double *)*lines);
        
        
        fseek(fp,0,SEEK_SET);
        
        for(i=0;i<lines;i++)
        {
            if((dinput[i]=(double *)malloc(sizeof(double)*noOfDimension))
               &&
               (doutput[i]=(double *)malloc(sizeof(double)*noOfClasses))
               )
            {
                
                for(j=0;j<noOfDimension;j++)
                {
                    fscanf(fp,"%lf",&dinput[i][j]);
                }
                fscanf(fp,"%d", &ylabel);
                
                for(j=0;j<noOfClasses;j++)
                {
                    if(j==(ylabel-1))
                        doutput[i][j]=1;
                    else
                        doutput[i][j]=0;
                }
            }
            
        }
       // centroids=kmean(, lines, k, noOfDimension, noOfClasses);
        
    }
    else
    {
        cout<<"Problem with File opening."<<endl;
    }
    
    
}

void iris()
{
    char temp[1024];
    strcpy(temp,PATH);
    int lines=0;
    FILE * fp;
    int noOfDimension=4;
    int noOfClasses=3;
    int i,j,k,l;
    int noOfNodesHidden=0;
    
    double **dinput;
    double **doutput;
    double **centroids;
    double ***dcluster;
    double *hidden;
    double **w1;
    double *poutput;
    int *dclass_count;
    int ylabel;
    //int count=0;
    
    fp=fopen(strcat(temp,"iris.train.txt"),"r");
    lines=0;
    if(fp!=NULL)
    {
        cout<<"Enter the number of cluster for each label:";
        cin>>k;
        noOfNodesHidden=k*noOfClasses;
        
        while(!feof(fp))
        {
            fscanf(fp,"%*lf %*lf %*lf %*lf %*d");
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
            if((dinput[i]=(double *)malloc(sizeof(double)*noOfDimension))
               &&
               (doutput[i]=(double *)malloc(sizeof(double)*noOfClasses))
               )
            {
                
                for(j=0;j<noOfDimension;j++)
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
                dcluster[i][j]=(double *)malloc(sizeof(double)*(noOfDimension+1));
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
                    for(l=0;l<noOfDimension;l++)
                    {
                        dcluster[j][dclass_count[j]][l]=dinput[i][l];
                    }
                    dcluster[j][dclass_count[j]][noOfDimension]=-1;
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
        centroids=kmean(dcluster, dclass_count, k, noOfDimension, noOfClasses);
        
        for(i=0;i<k*noOfClasses;i++)
        {
            for(j=0;j<=noOfDimension;j++)
            {
                cout<<centroids[i][j]<<" ";
            }
            cout<<endl;
        }
        
        
        
        hidden=(double *)malloc(sizeof(double)*noOfNodesHidden);
        poutput=(double *)malloc(sizeof(double)*noOfClasses);
        w1=(double **)malloc(sizeof(double *)*noOfNodesHidden);
        for(i=0;i<noOfNodesHidden;i++)
        {
            w1[i]=(double *)malloc(sizeof(double)*noOfClasses);
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
