#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <utility>
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <time.h>
#define InD -8
#define Mis -4
#define MATCH 5
#define Inf -2147483647
using namespace std;

int cnt(char* doc)//input data from a file
{
    char data;
    FILE *fp=fopen(doc,"r");
    if(!fp)
    {
        printf("can't open file\n");
        return -1;
    }
    int i=0;
    while(!feof(fp))
    {
        fscanf(fp,"%c",&data);
        if(data!='\n'&&(data=='A'||data=='C'||data=='G'||data=='T'))
        {
  	        i++;
        }
    }
    fclose(fp);
    return i;
}

int input(char* doc,char*a)//input data from a file
{
    char data;
    FILE *fp=fopen(doc,"r");
    if(!fp)
    {
        printf("can't open file\n");
        return -1;
    }
    int i=0;
    while(!feof(fp))
    {
        fscanf(fp,"%c",&data);
        if(data!='\n'&&(data=='A'||data=='C'||data=='G'||data=='T'))
        {
  	        a[i]=data;
  	        i++;
        }
    }
    fclose(fp);
    return i;
}

int score(char a, char b )
{
    char cha[]={'_','A','T','C','G'};
    if(a=='_' && b=='_')
	{
		return 0;
	}
	else if(a=='_'||b=='_')
	{
		return InD;
	}
	else if(a==b)
	{
        return MATCH;
    }
    else
        return Mis;
}

inline int score3(char a, char b,char c )
{
    return score(a,b)+score(b,c)+score(a,c);
}

int pair_ali(char* arr1, char* arr2, int* line, int length1,int length2)
{
	int s_mx[2][length2+1];
    s_mx[0][0]=0;
    s_mx[1][0]=InD;;
    for(int i=1;i<length2+1;i++)
    {
        s_mx[0][i]=s_mx[0][i-1]+InD;
    }
    line[0]=s_mx[0][length2];
    int sc_row=0;
    int sc_col=0;
    int sc_diag=0;
    for(int i=1;i<length1+1;i++)
    {
        for(int j=1;j<length2+1;j++)
        {
            sc_diag=s_mx[0][j-1]+score(arr1[i-1],arr2[j-1]);
            sc_row=s_mx[1][j-1]+InD;
            sc_col=s_mx[0][j]+InD;
            if(sc_diag>=sc_row && sc_diag>=sc_col)
            {
                s_mx[1][j]=sc_diag;
            }
            else
            {
                if(sc_row>=sc_col)
                {
                    s_mx[1][j]=sc_row;
                }
                else
                {
                    s_mx[1][j]=sc_col;
                }
            }
            if(i==length1)
            {
				line[j]=s_mx[1][j];
            }
            s_mx[0][j-1]=s_mx[1][j-1];
        }
        s_mx[0][length2]=s_mx[1][length2];
    }
    return s_mx[0][length2];
}

void muti_ali(char* arr1, char* arr2,char *arr3, int length1,int length2,int length3,int** s_mx)
{
	int ***s_mx32;	
	s_mx32=(int***)malloc(2*sizeof(int**));
	for(int i=0;i<2;i++)
	{
		s_mx32[i]=(int**)malloc((length2+1)*sizeof(int*));
	}
	
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<length2+1;j++)
		{
			s_mx32[i][j]=(int*)malloc((length3+1)*sizeof(int));
		}
	}
    int choice2[7];
	int mx2=Inf;
	s_mx32[0][0][0]=0;
	s_mx32[1][0][0]=2*InD;
	for(int i=1;i<length3+1;i++)
	{
		s_mx32[0][0][i]=s_mx32[0][0][i-1]+2*InD;
	}
	for(int i=1;i<length2+1;i++)
	{
		s_mx32[0][i][0]=s_mx32[0][i-1][0]+2*InD;
	}
	for(int l=0;l<3;l++)
	{
		choice2[l]=Inf;
	}
	for(int j=1;j<length2+1;j++)
    {
	    for(int k=1;k<length3+1;k++)
		{
    		choice2[0]=s_mx32[0][j-1][k]+2*InD;//sc_row
    		choice2[1]=s_mx32[0][j-1][k-1]+2*InD+score(arr2[j-1],arr3[k-1]);//sc_fd_ch
        	choice2[2]=s_mx32[0][j][k-1]+2*InD;//sc_hei		        	
            mx2=Inf;
	        for(int l=0;l<3;l++)
	        {
	        	if(choice2[l]>mx2)
	        	{
	        		mx2=choice2[l];
	        	}
	        }
	        s_mx32[0][j][k]=mx2;	        
	    }
	}
	int tmp=0;
	for(int i=1;i<length1+1;i++)
	{
		for(int j=1;j<length3+1;j++)
		{	        
	        mx2=Inf;
	        choice2[0]=s_mx32[1][0][j-1]+2*InD;//sc_row
    		choice2[1]=s_mx32[0][0][j-1]+2*InD+score(arr1[i-1],arr3[j-1]);//sc_fd_ch
        	choice2[2]=s_mx32[0][0][j]+2*InD;//sc_hei
        	for(int l=0;l<3;l++)
	        {
	        	if(choice2[l]>mx2)
	        	{
	        		mx2=choice2[l];
	        	}
	        }
	        s_mx32[1][0][j]=mx2;
		}
		for(int j=1;j<length2+1;j++)
		{
			choice2[0]=s_mx32[1][j-1][0]+2*InD;
    		choice2[1]=s_mx32[0][j-1][0]+2*InD+score(arr1[i-1],arr2[j-1]);
        	choice2[2]=s_mx32[0][j][0]+2*InD;		        	
            mx2=Inf;
	        for(int l=0;l<3;l++)
	        {
	        	if(choice2[l]>mx2)
	        	{
	        		mx2=choice2[l];
	        	}
	        }
	        s_mx32[1][j][0]=mx2;
		}
	    for(int j=1;j<length2+1;j++)
	    {
		    for(int k=1;k<length3+1;k++)
			{
		    	for(int l=0;l<7;l++)
		    	{
		    		choice2[l]=Inf;
		    	}
				choice2[5]=s_mx32[0][j][k]+2*InD;//sc_col
				choice2[1]=s_mx32[0][j-1][k]+2*InD+score(arr1[i-1],arr2[j-1]);//sc_fd_cr
				choice2[0]=s_mx32[0][j-1][k-1]+score3(arr1[i-1],arr2[j-1],arr3[k-1]);//sc_sd
				choice2[3]=s_mx32[0][j][k-1]+2*InD+score(arr1[i-1],arr3[k-1]);//sc_fd_rh
				choice2[4]=s_mx32[1][j-1][k]+2*InD;//sc_row
				choice2[2]=s_mx32[1][j-1][k-1]+2*InD+score(arr2[j-1],arr3[k-1]);//sc_fd_ch
				choice2[6]=s_mx32[1][j][k-1]+2*InD;//sc_hei		        	
                mx2=Inf;
		        for(int l=0;l<7;l++)
		        {
		        	if(choice2[l]>mx2)
		        	{
		        		mx2=choice2[l];
		        	}
		        }
		        s_mx32[1][j][k]=mx2;	        
		    }
		}
        for(int j=0;j<length2+1;j++)
        {
            for(int k=0;k<length3+1;k++)
            {
		        s_mx32[0][j][k]=s_mx32[1][j][k];
            }
        }
	}
    for(int j=0;j<length2+1;j++)
    {
        for(int k=0;k<length3+1;k++)
        {
            s_mx[j][k]=s_mx32[1][j][k];
        }
    }
    for(int i=0;i<2;i++)
	{
		for(int j=0;j<length2+1;j++)
		{
			free(s_mx32[i][j]);
		}
	}
	
	for(int i=0;i<2;i++)
	{
		free(s_mx32[i]);
	}
	free(s_mx32);
}

inline void reverse_arr(char *a,int n, char *b)
{
    for(int i=0;i<n;i++)
    {
        b[i]=a[n-i-1];
    }
}

inline int total_len(int length_a,int length_b,int *pred)
{
    int total_length=length_b;//length for a,b alignment
	for(int i=0;i<length_a;i++)
	{
	    if(pred[i]==-1)
	    {
            total_length++;
	    }
	}
	return total_length;
}

inline void output_len(char* a,char* b,int length_a, int length_b, char *a_ali,char*b_ali,int *pred)
{
	int a_c=0;
	int b_c=0;
	int curr=0;
	for(;a_c<length_a;)
	{
	    if(pred[a_c]==-1)
	    {
            a_ali[curr]=a[a_c];
            b_ali[curr]='_';
            a_c++;
            curr++;
	    }
	    else
	    {
            while(pred[a_c]>b_c)
            {
                a_ali[curr]='_';
                b_ali[curr]=b[b_c];
                b_c++;
                curr++;
            }
            if(pred[a_c]==b_c)
            {
                a_ali[curr]=a[a_c];
                b_ali[curr]=b[b_c];
                curr++;
                a_c++;
                b_c++;
            }
	    }
	}
}

inline int a_cmp_b1(char* a,char b,int length_a)//assume Mismatch > InD
{
    int tmp=Inf;
    int pred;
    for(int i=0;i<length_a;i++)
    {
        if(score(a[i],b)>tmp)
        {
            tmp=score(a[i],b);
            pred=i;
        }
    }
    return pred;
}

void find_pair_aligment(char *a,char *b,int length_a,int length_b,int *pred,int a_offset,int b_offset)
{
    if(length_a>1&&length_b>1)
    {
        int breakpoint=(length_a+1)/2;
	    char * a_first=a;
	    int length_a1=breakpoint;
	    int sup_line[length_b];
	    pair_ali(a_first,b, sup_line,length_a1,length_b);
	    char * a_second=&a[breakpoint];
	    int length_a2=length_a-breakpoint;
	    int sub_line[length_b];
	    char b_r[length_b];
	    reverse_arr(b,length_b,b_r);
	    char a2_r[length_a2];
	    reverse_arr(a_second,length_a2,a2_r);
	    pair_ali(a2_r ,b_r , sub_line,length_a2,length_b);
	    int mx=Inf;
	    int tmp_sm=0;
	    int mark=0;
	    for(int i=0;i<length_b+1;i++)
	    {
            tmp_sm=sup_line[i]+sub_line[length_b-i];
            if(mx<tmp_sm)
            {
                    mx=tmp_sm;
                    mark=i;
            }
	    }
	    pred[breakpoint+a_offset]=mark+b_offset;//breakpoint+a_offset element and postion should -1
	    find_pair_aligment(a_first,b, length_a1, mark,pred, a_offset,b_offset);
	    find_pair_aligment(a_second,&b[mark], length_a2, length_b-mark,pred, a_offset+breakpoint,b_offset+mark);
    }
    else if (length_b==1||length_b==0)
    {
		for(int i=a_offset;i<a_offset+length_a;i++)
        {
            pred[i]=-1;
        }
        if(length_b==1)
        {
            pred[a_offset+ a_cmp_b1( a, b[0], length_a)]=b_offset;
        }
    }
    else if(length_a==1||length_a==0)
    {
        if(length_a==1)
        {
            pred[a_offset]= a_cmp_b1( b, a[0],  length_b)+b_offset;

        }
    }
}

void multi_align0(char*a,char*b,char *c,int length_a,int length_b,int length_c,string& str1,string &str2, string& str3)
{
	int pred[length_a];
	int a_offset=0;
	int b_offset=0;
	find_pair_aligment(a,b, length_a, length_b,pred, a_offset,b_offset);
	int total_length=total_len(length_a, length_b,pred);
	char a_ali[total_length];
	char b_ali[total_length];
	output_len( a, b, length_a, length_b, a_ali,b_ali,pred);
	char c_ali[total_length];
	for(int i=0;i<total_length+1;i++)
		c_ali[i]='_';
    int mark=0;
    if(length_c==1)
    {
        int mx=Inf;
        for(int i=0;i<total_length;i++)
        {
            if(mx<score3(a_ali[i],b_ali[i],c[0]))
            {
                mx=score3(a_ali[i],b_ali[i],c[0]);
                mark=i;
            }
        }
        c_ali[mark]=c[0];
    }
    str1.append(a_ali,total_length);
    str2.append(b_ali,total_length);
    str3.append(c_ali,total_length);
}

int find_aligment3(char *a,char *b,char* c,int length_a,int length_b,int length_c,int a_offset,int b_offset,int c_offset,	string &str_a,string&str_b,string&str_c)
{
    if(length_a>1 && length_b>1 && length_c>1)
    {
        int breakpoint=(length_a+1)/2;
	    char * a_first=a;
	    int length_a1=breakpoint;
	   	int **sp_mx;
	    sp_mx=(int**)malloc((length_b+1)*sizeof(int*));
	    for(int i=0;i<length_b+1;i++)
	    {
	    	sp_mx[i]=(int*)malloc((length_c+1)*sizeof(int));
	    }
	    muti_ali(a_first, b, c, length_a1, length_b, length_c, sp_mx);
	    char * a_second=&a[breakpoint];
	    int length_a2=length_a-breakpoint;
	    char b_r[length_b];
	    reverse_arr(b,length_b,b_r);
	    char a2_r[length_a2];
	    reverse_arr(a_second,length_a2,a2_r);
	    char c_r[length_c];
   	    reverse_arr(c,length_c,c_r);
	    int **sb_mx;
	    sb_mx=(int**)malloc((length_b+1)*sizeof(int*));
	    for(int i=0;i<length_b+1;i++)
	    {
	    	sb_mx[i]=(int*)malloc((length_c+1)*sizeof(int));
	    }
	    muti_ali(a2_r ,b_r , c_r,length_a2,length_b,length_c,sb_mx);
	    int mx=Inf;
	    int tmp_sm=0;
	    int mark_b=0;
	    int mark_c=0;
		for(int i=0;i<length_b+1;i++)
	    {
	    	for(int j=0;j<length_c+1;j++)
	    	{
		        tmp_sm=sp_mx[i][j]+sb_mx[length_b-i][length_c-j];
		        if(mx<tmp_sm)
		        {
		                mx=tmp_sm;
		                mark_b=i;
		                mark_c=j;
		        }
		    }
	    }
	    for(int i=0;i<length_b+1;i++)
	    {
	    	free(sp_mx[i]);
	    	free(sb_mx[i]);
	    }
	    free(sp_mx);
	    free(sb_mx);
        string str_a1,str_a2,str_b1,str_b2,str_c1,str_c2;
	    find_aligment3(a_first,b,c, length_a1, mark_b,mark_c, a_offset,b_offset,c_offset, str_a1, str_b1, str_c1);
	    find_aligment3(a_second,&b[mark_b],&c[mark_c], length_a2, length_b-mark_b,length_c-mark_c, a_offset+breakpoint,b_offset+mark_b,c_offset+mark_c,str_a2,str_b2,str_c2);
	    str_a+=str_a1;
	    str_a+=str_a2;
	    str_b+=str_b1;
	    str_b+=str_b2;
	    str_c+=str_c1;
	    str_c+=str_c2;
	    return mx;
    }
    else if (length_c==1||length_c==0)
    {
        multi_align0(a,b,c,length_a,length_b,length_c,str_a,str_b,str_c);
        return 0;
    }
    else if (length_a==1||length_a==0)
    {
        multi_align0(c,b,a,length_c,length_b,length_a,str_c,str_b,str_a);
        return 0;
    }
    else if (length_b==1||length_b==0)
    {
        multi_align0(a,c,b,length_a,length_c,length_b,str_a,str_c,str_b);
        return 0;
	}
}

int main()
{
	clock_t t;
	t=clock();
	char doc1[]="NM_000492.fasta";
	int length_a=cnt(doc1);
	char *a;
	a=(char*)malloc(length_a*sizeof(char));	
	input( doc1,a);
	printf("file1 open correctly! length is %d\n",length_a);
	char doc2[]="NM_021050.fasta";
	int length_b=cnt(doc2);	
	char *b;
	b=(char*)malloc(length_b*sizeof(char));
	input( doc2,b);
	printf("file2 open correctly! length is %d\n",length_b);
	char doc3[]="NM_031506.fasta";
	int length_c=cnt(doc3);	
	char *c;
	c=(char*)malloc(length_c*sizeof(char));
	input( doc3,c);
	printf("file3 open correctly! length is %d\n",length_c);
    int a_offset=0;
    int b_offset=0;
    int c_offset=0;
    string str_a,str_b,str_c;
    int total_score=find_aligment3(a,b,c,length_a, length_b,length_c,a_offset, b_offset, c_offset, str_a, str_b, str_c);
    printf("Total score is %d.\n",total_score);
    cout<<"Length of alignment is "<<str_a.size()<<". "<<endl;
    int perfect_match=0;
	for(int i=0;i<str_a.size();i++)
	{
		if(str_a[i]==str_b[i]&& str_b[i]==str_c[i])
		{
			perfect_match++;
		}
	}
	printf("Perfect match number is %d.\n",perfect_match);
	cout<<str_a<<endl<<str_b<<endl<<str_c<<endl;

	free(a);
	free(b);
	free(c);
	t=clock()-t;
	printf("It tooks me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);

    return 0;
}
