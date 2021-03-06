#include "wordlist.h"



struct WL_Word WL_GetWord(int serial,struct WL_Files filetmp)
{

}

void WL_FreeWord(struct WL_Word wordname)
{
	free(wordname.en);
	free(wordname.zh);
}

//The serial starts from 0 not 1
struct WL_Word WL_GetWordFF(int serial,int index)//Get word from file
{
	char* filename = WL_GetName(index);
	if(WL_GetLen(filename)<serial+1)
	{
		printf("Out of range");
		struct WL_Word error;
		error.en = NULL;
		error.zh = NULL;
		error.correct = -1;
		return error;
	}
	int openFlags = O_RDONLY;
	int fd=open(filename,openFlags);
	int sertmp = serial;
	char* ftmp=(char*)malloc(sizeof(char));
	char wordbuf[100];
	int wordlen=0;
	while(sertmp>0)
	{
		*ftmp=1;
		while(*ftmp!='\n')
		{
			//printf("%c",*ftmp);//for debug
			if(read(fd,ftmp,1)==-1)
			{
				printf("ERROR while reading %s",filename);
				free(filename);
				struct WL_Word error;
				error.en = NULL;
				error.zh = NULL;
				error.correct = -1;
				return error;
			}
		}
		sertmp--;
	}
	*ftmp=1;
	while(*ftmp!='\n')
	{
		if(read(fd,ftmp,1)==-1)
		{
			printf("ERROR while reading %s",filename);
			free(filename);
			struct WL_Word error;
			error.en = NULL;
			error.zh = NULL;
			error.correct = -1;
			return error;
		}
		wordbuf[wordlen]=*ftmp;
		wordlen++;
	}
	struct WL_Word wtmp;
	char* sp=(char*)malloc(wordlen);
	for(wordlen--;wordlen>=0;wordlen--)
	{
		*(sp+wordlen) = wordbuf[wordlen];
	}
	/*Work space*/
	wtmp.en = sp;
	wtmp.zh = NULL;
	free(filename);
	free(ftmp);
	return wtmp;
}

//starts from 1
char *WL_GetName(int index)//Remember to free it \ Full path
{
	char *buf;
	int i=0;
	buf=WL_GetItems();
	int t=index;
	t--;
	while(t>0)
	{
		while(*(buf+i++)!='\n');
		t--;
	}
	//printf("%s",(buf+i));//For debug	
	char fn[100];
	int n=0;
	while(*(buf+i+n)!='\0'&&*(buf+i+n)!='\n')
	{
		fn[n]=*(buf+i+n);
		n++;
	}
	fn[n]='\0';
	char ft[100]="../wordlist/";
	int j=0;
	while(j<=n)
	{
		ft[12+j]=fn[j];
		j++;
	}
	char* sp=(char*)malloc(n+12);
	for(n+=12;n>=0;n--)
	{
		*(sp+n)=ft[n];
	}
	free(buf);
	return sp;
}

int WL_ListItems()
{
	char *buf;
	buf=WL_GetItems();
	int i=0,j=1;
	int ff=0;
	while(*(buf+i)!='\0')
	{
		if((*(buf+i)=='\n'||ff==0)&&*(buf+i+1)!='\0')
		{
			if(ff==0)
				ff=1;
			else
				i++;
			printf("\n%d.",j++);
		}
		printf("%c",*(buf+i));
		i++;
	}
	j--;
	free(buf);
	return j;
}

char *WL_GetItems()//remember ro free it
{
	system("ls ../wordlist | grep .txt > ../tmp/Items");//warining , here has been changed for debug
	FILE *ItFP;
	ItFP=fopen("../tmp/Items","r");
	char ch = fgetc(ItFP);
	while(ch != EOF)
		ch=fgetc(ItFP);
	unsigned int n=ftell(ItFP);
	char *sp = (char*)malloc(n);
	rewind(ItFP);
	fread(sp,n,1,ItFP);
	fclose(ItFP);
	return sp;
}

unsigned int WL_GetLen(char* filename)//Full path
{
	FILE *LnFP;
	LnFP=fopen(filename,"r");
	if(LnFP==NULL)
	{
		printf("No such a file\n");
		return 0;
	}
	char ch = fgetc(LnFP);
	unsigned int len=0;
	while(ch != EOF)
	{
		if(ch=='\n')
			len++;
		ch = fgetc(LnFP);
	}
	len++;
	fclose(LnFP);
	return len;
}
