#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 500
#define BUF 4096
#define COLS 8

typedef struct {char code[128],title[512],teacher[256],days[80],start[32],end[32],room[128],source[256];} Course;
typedef struct {char name[256],dept[128],office[128],days[80],start[32],end[32],source[256];} Faculty;
static Course courses[MAX];static Faculty faculty[MAX];
static int nc=0,nf=0;
static void cp(char *dest,size_t n,const char *src){if(n){size_t len=strlen(src?src:"");if(len>=n)len=n-1;memcpy(dest,src?src:"",len);dest[len]=0;}}
/* CSV parser supports commas and escaped quotation marks. */
static int csvparse(const char *src,char out[][1024],int max){int k=0,pos=0,quoted=0;memset(out,0,(size_t)max*1024);for(const char *p=src;*p && *p!='\n' && *p!='\r';++p){char c=*p;if(c=='"'){if(quoted && p[1]=='"'){if(pos<1023)out[k][pos++]=c;++p;}else quoted=!quoted;}else if(c==',' && !quoted){if(++k>=max)return -1;pos=0;}else if(pos<1023)out[k][pos++]=c;}return quoted?-1:k+1;}
static int read_courses(const char *path){FILE *fp=fopen(path,"r");if(!fp){perror(path);return 0;}char line[BUF],a[COLS][1024];int row=0;while(fgets(line,sizeof line,fp)){row++;if(row==1)continue;int fields=csvparse(line,a,COLS);if(fields<8){fprintf(stderr,"Invalid course CSV row %d\n",row);fclose(fp);return 0;}if(nc>=MAX){fclose(fp);return 0;}Course *c=&courses[nc++];cp(c->code,sizeof c->code,a[0]);cp(c->title,sizeof c->title,a[1]);cp(c->teacher,sizeof c->teacher,a[2]);cp(c->days,sizeof c->days,a[3]);cp(c->start,sizeof c->start,a[4]);cp(c->end,sizeof c->end,a[5]);cp(c->room,sizeof c->room,a[6]);cp(c->source,sizeof c->source,a[7]);}fclose(fp);return 1;}
static int read_faculty(const char *path){FILE *fp=fopen(path,"r");if(!fp){perror(path);return 0;}char line[BUF],a[COLS][1024];int row=0;while(fgets(line,sizeof line,fp)){row++;if(row==1)continue;int fields=csvparse(line,a,COLS);if(fields<7){fprintf(stderr,"Invalid faculty CSV row %d\n",row);fclose(fp);return 0;}if(nf>=MAX){fclose(fp);return 0;}Faculty *f=&faculty[nf++];cp(f->name,sizeof f->name,a[0]);cp(f->dept,sizeof f->dept,a[1]);cp(f->office,sizeof f->office,a[2]);cp(f->days,sizeof f->days,a[3]);cp(f->start,sizeof f->start,a[4]);cp(f->end,sizeof f->end,a[5]);cp(f->source,sizeof f->source,a[6]);}fclose(fp);return 1;}
static int daymask(const char *d){int m=0;for(const char *p=d;*p;++p){switch(*p){case 'M':m|=1;break;case 'T':m|=2;break;case 'W':m|=4;break;case 'R':m|=8;break;case 'F':m|=16;break;case 'S':m|=32;break;case 'U':m|=64;break;}}return m;}
static int minutes(const char *s){int h=-1,m=-1;if(sscanf(s,"%d:%d",&h,&m)!=2||h<0||h>23||m<0||m>59)return -1;return h*60+m;}
static void csvval(FILE *fp,const char *s){fputc('"',fp);for(;*s;s++){if(*s=='"')fputc('"',fp);fputc(*s,fp);}fputc('"',fp);}
static void field(FILE *fp,const char *s){csvval(fp,s);fputc(',',fp);}
static int conflict(const Faculty *f){int st=minutes(f->start),en=minutes(f->end);if(st<0||en<=st)return -1;for(int i=0;i<nc;i++){Course *c=&courses[i];if(strcmp(c->teacher,f->name))continue;if(!(daymask(c->days)&daymask(f->days)))continue;int a=minutes(c->start),b=minutes(c->end);if(a<0||b<=a)continue;if(st<b && a<en)return 1;}return 0;}
int create_reports(const char *cpth,const char *fpth,const char *outdir){
 if(!read_courses(cpth)||!read_faculty(fpth))return 0;
 char path[512];snprintf(path,sizeof path,"%s/teaching_schedule.csv",outdir);FILE *o=fopen(path,"w");if(!o){perror(path);return 0;}
 fputs("course,title,instructor,days,start,end,room,source,validation\n",o);
 int invalid=0;for(int i=0;i<nc;i++){Course *c=&courses[i];field(o,c->code);field(o,c->title);field(o,c->teacher);field(o,c->days);field(o,c->start);field(o,c->end);field(o,c->room);field(o,c->source);int a=minutes(c->start),b=minutes(c->end);const char *status=(!*c->days||!*c->start||!*c->end)?"asynchronous / no fixed meeting":(a<0||b<=a)?"review source time":"ok";if(strcmp(status,"ok"))invalid++;csvval(o,status);fputc('\n',o);}fclose(o);
 snprintf(path,sizeof path,"%s/office_hours.csv",outdir);o=fopen(path,"w");if(!o){perror(path);return 0;}
 fputs("instructor,department,office,days,start,end,status,source\n",o);int count=0,overlaps=0;
 for(int i=0;i<nf;i++){Faculty *f=&faculty[i];field(o,f->name);field(o,f->dept);field(o,f->office);field(o,f->days);field(o,f->start);field(o,f->end);const char *status="unknown - not supplied";if(*f->days&&*f->start&&*f->end){int c=conflict(f);status=c==1?"CONFLICT with teaching":c<0?"INVALID time":"provided - no teaching conflict";count++;if(c==1)overlaps++;}field(o,status);csvval(o,f->source);fputc('\n',o);}fclose(o);
 printf("Loaded %d courses and %d faculty records.\n",nc,nf);printf("Provided office-hour records: %d; teaching overlaps: %d; nonstandard course-time records: %d.\n",count,overlaps,invalid);
 printf("Wrote %s/teaching_schedule.csv and %s/office_hours.csv\n",outdir,outdir);return 1;
}
