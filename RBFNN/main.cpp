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

using namespace std;

const char PATH[1024]="/Users/sankarsanseal/Documents/RBFNN/RBFNN/";

double ** kmean(double ***dcluster, int *dclass_count,int k,int dimensions, int classes)
{
    int i;
    double **centroids=NULL;
    centroids=(double **)malloc(sizeof(double *)*k*classes);
    for(i=0;i<k*classes;i++)
    {
        centroids[i]=(double *)malloc(sizeof(double)*(dimensions+1));
    }
    
    srand((unsigned)time(NULL));
    if(centroids!=NULL)
    {
        for(i=0;i<k;i++)
        {
            
        }
    }
    
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
    
    double **dinput;
    double **doutput;
    double **centroids;
    double ***dcluster;
    int *dclass_count;
    int ylabel;
    int count=0;
    
    fp=fopen(strcat(temp,"iris.train.txt"),"r");
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
