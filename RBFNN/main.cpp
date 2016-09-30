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

void kmean(double **data,int n,int k, int classes)
{
    int i;
    srand((unsigned)time(NULL));
    for(i=0;i<k;i++)
    {
        
    }
    
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
    int ylabel;
    
    fp=fopen(strcat(temp,"wine.train.txt"),"r");
    lines=0;
    if(fp!=NULL)
    {
        while(!feof(fp))
        {
            fscanf(fp,"%*lf %*lf %*lf %*lf %*d");
            if(!feof(fp))
                lines++;
        }
        
        
        dinput=(double **)malloc(sizeof(double *)*lines);
        doutput=(double **)malloc(sizeof(double )*lines);
        
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
    FILE * fp=fopen(strcat(temp,"iris.train.txt"),"r");
    
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
