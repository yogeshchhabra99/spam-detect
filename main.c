#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define kreps 50
void filter(char*);
int distance(char*,char*);
int returndistance(char*,int,char*,int);
int min(int ,int , int );
int ldis(char *s1, char *s2);

typedef struct mystruct
{
	char* word;
	int groupdis;
	
}point;
typedef struct mystruct2
{
	point** cluster;
	int size;

}list;
typedef struct mystruct3
{
	int plot[kreps];
	char label;
}histogram;

void computehistogram(char*,histogram*,list*);
void main()
{	
	point *ptr=(point *)malloc(2*sizeof(point));
	int j=0;
	int i;
	int k;
	int l;
	int t;
	int spam=0;
	int ham=0;
	char* message=(char*)malloc(2*sizeof(char));
	FILE *fp=fopen("english.txt","r");
	if(fp==NULL)
	{
		printf("No file Found!!");
		return;
	}
	FILE *fp2=fopen("traindata.txt","w");
	if(fp2==NULL)
	{
		printf("No file Found!!");
		return;
	}
	FILE *fp3=fopen("testdata.txt","w");
	if(fp3==NULL)
	{
		printf("No file Found!!");
		return;
	}		
	char inp='c';
	while(1)
	{	i=0;
		message=(char*)realloc(message,(i+2)*sizeof(char));
		while(inp!='\n')
		{
			inp=tolower(fgetc(fp));
			if(inp==EOF) break;
			*(message+i)=inp;
			i++;
			message=(char*)realloc(message,(i+1)*sizeof(char));		
		}
		if(inp==EOF) break;
		i--;
		*(message+i)='\0';
		i--;i--;
		if(*(message+i)=='m' && *(message+i-1)=='a' && *(message+i-2)=='p' && *(message+i-3)=='s')
		{
			spam++;
			if(spam<43)
				{
				fputs(message,fp2);fputc('\n',fp2);
				}
			else
				{fputs(message,fp3);fputc('\n',fp3);}
		}
		if(*(message+i)=='m' && *(message+i-1)=='a' && *(message+i-2)=='h')
		{
			ham++;
			if(ham<450)
				{
				fputs(message,fp2);fputc('\n',fp2);
				}
			else
				{fputs(message,fp3);fputc('\n',fp3);}
		}
		inp='c';		
	}
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fp2=fopen("traindata.txt","r");
	if(fp2==NULL)
	{
		printf("No file Found!!");
		return;
	}
	inp='c';
	while(100)
	{
		
		((ptr+j)->word)=(char*)malloc(2*sizeof(char));
		i=0;
		while(inp != ' '&&inp != '\n' )
		{
			
			inp=fgetc(fp2);
			if(inp==EOF) break;
			if(inp=='.' || inp==';' || inp==',' || inp=='!' || inp=='+' || inp=='&' || inp=='/')
				continue;				
			else
			{
				*(((ptr+j)->word)+i)=inp;
				i++;
				((ptr+j)->word)=(char*)realloc(((ptr+j)->word),(i+2)*sizeof(char));
			}
		}
			
		if (inp==EOF) break;
		i--;
		*(((ptr+j)->word)+i)='\0';
		filter(((ptr+j)->word));		
		inp='c';
		if(i>2)
		{			  // removes words with only 1 or 2 letter				
			//printf("word:%s\n",((ptr+j)->word));
			j++;						//j gives total number of words.
			ptr=(point*)realloc(ptr,(j+2)*sizeof(point));
			(ptr+j)->groupdis=0;
		}	
	}
	int dis;
	int mindis=999;
	point* tempptr;
	list* group=(list*)malloc(kreps*sizeof(list));
	for(i=0;i<kreps;i++)
	{
		(group+i)->size=0;
		(group+i)->cluster=(point**)malloc((((group+i)->size)+1)*sizeof(point*));
		if(i<(kreps/2))k=rand()%300;
		else								//picking first 5 reps from first 300 words to get sufficient words from spams   
			k=(rand()%(j-300))+300;
		*(((group+i)->cluster)+((group+i)->size))=(ptr+k);
		((group+i)->size)++;
		(group+i)->cluster=(point**)realloc((group+i)->cluster,(((group+i)->size)+1)*sizeof(point*));
		//printf("rep-word: %s and size:%d \n",(*((group+i)->cluster))->word,((group+i)->size));
	
		
	}
	int temp;
	for(t=0;t<9;t++)
	{	printf("-----------------------\n");
		for(i=0;i<j;i++)
		{
			(ptr+i)->groupdis=0;
			mindis=999;
			temp=0;
			for(l=0;l<kreps;l++)
			{
				dis=ldis((ptr+i)->word,(*((group+l)->cluster))->word);
				if(dis<=mindis)
				{
					temp=l;					
					mindis=dis;			
				}
			}		
			*(((group+temp)->cluster)+((group+temp)->size))=(ptr+i);
			((group+temp)->size)++;
			(group+temp)->cluster=(point**)realloc((group+temp)->cluster,(((group+temp)->size)+1)*sizeof(point*));
		}
		for(l=0;l<kreps;l++)
		{
			mindis=999;
			for(i=0;i<((group+l)->size);i++)
			{
				for(k=0;k<((group+l)->size);k++)
				{
					(*(((group+l)->cluster)+i))->groupdis=(*(((group+l)->cluster)+i))->groupdis+ldis((*(((group+l)->cluster)+i))->word,(*(((group+l)->cluster)+k))->word);
				}				
				if( (*(((group+l)->cluster)+i))->groupdis<mindis)
				{
					temp=i;
				}
			}
			*(((group+l)->cluster))=*(((group+l)->cluster)+temp);
			((group+l)->size)=1;
		//	printf("new rep:%s\n",(*((group+l)->cluster))->word);
		}
	}
	printf("starting training-------------\n");
	histogram * hist=(histogram*)malloc(2*sizeof(histogram));	
	i=0;	
	k=0;
	fclose(fp2);
	fp2=fopen("traindata.txt","r");
	if(fp2==NULL)
	{
		printf("No file Found!!");
		return;
	}
	inp='c';
	while(1)
	{
		i=0;
		message=(char*)realloc(message,(i+2)*sizeof(char));
		while(inp!='\n')
		{
			inp=tolower(fgetc(fp2));
			if(inp==EOF) break;
			if(inp=='.' || inp==';' || inp==',' || inp=='!' || inp=='+' || inp=='&' || inp=='/')
				continue;
			else
			{			

			*(message+i)=inp;
			i++;
			message=(char*)realloc(message,(i+2)*sizeof(char));		
			}
		}
		if(inp==EOF) break;
		i--;
		*(message+i)='\0';
		i--;i--;
		if(*(message+i)=='m' && *(message+i-1)=='a' && *(message+i-2)=='p' && *(message+i-3)=='s')
		{
			(hist+k)->label='s';
			*(message+i-3)='\0';
		}
		if(*(message+i)=='m' && *(message+i-1)=='a' && *(message+i-2)=='h')
		{
			(hist+k)->label='h';
			*(message+i-2)='\0';
		}
		hist=(histogram*)realloc(hist,(k+2)*sizeof(histogram));
		computehistogram(message,hist+k,group);
		inp='c';
		k++;				//now i have k train histograms
	}
	fclose(fp2);
	fp3=fopen("testdata.txt","r");
	if(fp3==NULL)
	{
		printf("No file Found!!");
		return;
	}
	printf("starting testing-------------\n");
	histogram * hist2=(histogram*)malloc(2*sizeof(histogram));	
	i=0;
	t=0;
	int m;
	int dif=0;
	int mindif=1000000;
	int correctans=0;
	int wrongans=0;
	char ans;
	int g;
	while(70)
	{	//printf("error\n");
		dif=0;
		mindif=1000000;
		i=0;
		message=(char*)realloc(message,(i+2)*sizeof(char));
		while(inp!='\n')
		{
			inp=tolower(fgetc(fp3));
			if(inp==EOF) break;
			if(inp=='.' || inp==';' || inp==',' || inp=='!' || inp=='+' || inp=='&' || inp=='/')
				continue;
			else
			{			

			*(message+i)=inp;
			i++;
			message=(char*)realloc(message,(i+2)*sizeof(char));		
			}
		}
		if(inp==EOF) break;
		i--;
		*(message+i)='\0';
		i--;i--;
		if(*(message+i)=='m' && *(message+i-1)=='a' && *(message+i-2)=='p' && *(message+i-3)=='s')
		{
			(hist2+t)->label='s';
			*(message+i-3)='\0';
		}
		if(*(message+i)=='m' && *(message+i-1)=='a' && *(message+i-2)=='h')
		{
			(hist2+t)->label='h';
			*(message+i-2)='\0';
		}
		hist2=(histogram*)realloc(hist2,(t+2)*sizeof(histogram));
		computehistogram(message,hist2+t,group);
		inp='c';
		
		for(m=0;m<k;m++)
		{
			dif=0;
			for(l=0;l<kreps;l++)			//now i have t test histograms
			{
				if(*(((hist+m)->plot)+l)>*(((hist2+t)->plot)+l))
					dif=dif+*(((hist+m)->plot)+l)-*(((hist2+t)->plot)+l);
				else
					dif=dif+*(((hist2+t)->plot)+l)-*(((hist+m)->plot)+l);
			}
			//printf("%d\n",dif);
			if(dif<mindif)
			{
				mindif=dif;
				ans=((hist+m)->label);
				g=m;	
			}
		}
		if(ans==((hist2+t)->label))
			{correctans++;printf("%d---%c---c\n",t,ans);
			}
		else
			{wrongans++;printf("%d---%c----w\n",t,ans);
			}
		t++;	
		//printf("match with message no. %d\n",g);
		//printf("-------------------");
	}
	printf("correctans:%d\n",correctans);
	printf("wrongans:%d\n",wrongans);
	//printf("%d\n",t);
	fclose(fp3);
}
void filter(char* ptr)
{
	int i=0;
	while(*(ptr + i+2)!='\0')
	{
		if(*(ptr+i)=='h'&& *(ptr + i+1)=='a' && *(ptr + i+2)=='m')
		{
			*(ptr+i)='\0';
			return;		
		}
		i++;
		
	}
	i=0;
	while(*(ptr + i+3)!='\0')
	{
		if(*(ptr+i)=='s'&& *(ptr + i+1)=='p' && *(ptr + i+2)=='a' && *(ptr + i+3)=='m')
		{
			*(ptr+i)='\0';
			return;		
		}
		i++;
	}
	
}


int min(int a,int b, int c)
{
	if(a>=b)
	{
		if(b>=c)
			return c;
		else 
			return a;
	}
	else
	{
		if(a>=c)
			return c;
		else 
			return b;
		
	}	
}
int ldis(char *s1, char *s2) {
    unsigned int x, y, s1len, s2len;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int matrix[s2len+1][s1len+1];
    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++)
        matrix[x][0] = matrix[x-1][0] + 1;
    for (y = 1; y <= s1len; y++)
        matrix[0][y] = matrix[0][y-1] + 1;
    for (x = 1; x <= s2len; x++)
        for (y = 1; y <= s1len; y++)
            matrix[x][y] = min(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));

    return(matrix[s2len][s1len]);
}

void computehistogram(char*message,histogram* hist,list* group)
{

	//printf("%s",message);
	//printf("------------");
	int i,k,l;
	for(i=0;i<kreps;i++)
	{
		*((hist->plot)+i)=0;	
	}
	char inp='c';
	char* ptr=malloc(1*sizeof(char));
	i=0;
	k=0;
	while(100)
	{
		i=0;
		ptr=(char*)realloc(ptr,(i+2)*sizeof(char));
		
		while(inp != ' '&&inp != '\n' )
		{
			
			inp=*(message+k);
			k++;
			if(inp=='\0') break;
			if(inp=='.' || inp==';' || inp==',' || inp=='!' || inp=='+' || inp=='&' || inp=='/')
				continue;				
			else
			{
				*(ptr+i)=inp;
				i++;
				ptr=(char*)realloc(ptr,(i+2)*sizeof(char));
			}
		}
			
		if (inp=='\0') break;
		i--;
		*(ptr+i)='\0';		
		inp='c';
		if(i>2)
		{	
			for(l=0;l<kreps;l++)
			{
				//*((hist->plot)+k)=*((hist->plot)+k)+(999-ldis(ptr,((*((group+k)->cluster))->word)));
				*((hist->plot)+l)=*((hist->plot)+l)+(100-ldis(ptr,((*((group+l)->cluster))->word)));		
			}
		}	
	}
	for(i=0;i<kreps;i++)
	{
	//	printf("%d--",*((hist->plot)+i));	
	}
	printf("%c\n",hist->label);
	//printf("\n");
}



















