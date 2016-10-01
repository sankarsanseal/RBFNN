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

const char PATH[1024]="/Users/sankarsanseal/Documents/RBFNN/RBFNN";

double ** kmean(double **dinput, double **doutput,int n,int k,int dimensions, int classes)
{
    int i;
    double **centroids=NULL;
    centroids=(double **)malloc(sizeof(double *)*k*classes);
    for(i=0;i<k*classes;i++)
    {
        centroids[i]=(double *)malloc(sizeof(double)*dimensions);
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
        centroids=kmean(dinput, doutput, lines, k, noOfDimension, noOfClasses);
        
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
    int i,j,k;
    
    double **dinput;
    double **doutput;
    double **centroids;
    int ylabel;
    
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
        centroids=kmean(dinput, doutput, lines, k, noOfDimension, noOfClasses);
        
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
