#include<map>
#include<stdio.h>
#include<stdlib.h>
#include<time.h> 
#include<string>
#include<iostream>
#include <hiredis/hiredis.h>
#include<unistd.h>
//#include<syswait.h>
#include <sys/time.h>
using namespace std;
typedef map<string,string>::iterator IT;
typedef pair<string,string> pairmp;
#define EQUIPMENT 150
#define SIMULATE 300
#define STATUS 200
#define AREA 6
#define REDIS_HOST        "127.0.0.1"
#define REDIS_PORT        6379
#include<map>
// typedef std::map<std::string, std::string> TStrStrMap;
// typedef std::pair<std::string, std::string> TStrStrPair;
// #define REDIS_HOST "127.0.0.1"
// #define REDIS_PORT 6379
class Redis
{
public:

    Redis(){}

    ~Redis()
 {
    this->_connect = NULL;
    this->_reply = NULL;        
 }

 bool connect(string host, int port)
 {
    this->_connect = redisConnect(host.c_str(), port);
    if(this->_connect != NULL && this->_connect->err)
    {
        printf("connect error: %s\n", this->_connect->errstr);
        return 0;
    }
    return 1;
 }

 string get(string key)
 {
    this->_reply = (redisReply*)redisCommand(this->_connect, "mget %s", key.c_str());
    std::string str = this->_reply->str;
    freeReplyObject(this->_reply);
    return str;
 }

 void set(string key,string value)
 {
	//printf("mset %s %s\n",key.c_str(),value.c_str());
    redisCommand(this->_connect, "mset %s %s", key.c_str(), value.c_str());
 }

private:

    redisContext* _connect;
    redisReply* _reply;
    
};
//多进程创建
// void createsubprocess(int num,int max)   
// {   
    // if(num>=max)return;   
    // pid=fork();   
    // if(pid<0)   
    // {   
        // perror("fork error!\n");   
        // exit(1);   
    // }   
    // //子进程   
    // else if(pid==0)   
    // {   
        // //sleep(3);   
        // printf("子进程id=%d,父进程id=%d\n",getpid(),getppid());   
    // }   
    // //父进程   
    // else  
    // {   
        // num++;   
        // if(num==1)
		// {
			// printf("父进程id=%d\n",getpid());  
		// }			
        // if(num<max)createsubprocess(num,max);   
        // //此处加sleep是为了防止父进程先退出,从而产生异常   
        // //sleep(5);   
    // }   
// }   
int Random(int m, int n)
{
	int pos, dis;
	if (m == n)
	{
		return m;
	}
	else if (m > n)
	{
		pos = n;
		dis = m - n + 1;
		return rand() % dis + pos;
	}
	else
	{
		pos = m;
		dis = n - m + 1;
		return rand() % dis + pos;
	}
}


int  main()
{

	map<string,string> my_map;
	//map<string,string> TStrStrMap;
	int i, m, n;
	char value[100] = {0};
	char key[50] = {0};
	string key1,key2;
	string value1,value2;
	key1 = "";
	value1 = "";
	struct  timeval  start;
	struct  timeval  end;
	unsigned long timer;
	//TStrStrMap::iterator p;
	int szTime = (int)time(NULL);
	//srand(szTime);
	m = 200;
	n = 240;
	Redis* redis = new Redis();
	if (!redis->connect(REDIS_HOST,REDIS_PORT)){
    	 	printf("redis connect error");
    	 	return 0;
    }
	//gettimeofday(&start,NULL);
	//printf("\tchild pid\n"); 	
     pid_t child;  
    if((child = fork()) == -1)  
    {  
        perror("fork");  
        exit(EXIT_FAILURE);  
    }  
    else if(child == 0)                 //子进程中  
    {  
        puts("in child");  
        printf("\tchild pid = %d\n",getpid());  
        printf("\tchild ppid = %d\n",getppid());
		while(true)
		{
			gettimeofday(&start,NULL);
			key1 = "";
			value1 = "";
			szTime = (int)time(NULL);
			srand(szTime);
			float ac = (Random(m, n) / 10.0);
			int ad=1;
			for(int i=71;i<72;i++)
			{
				for(int j=1;j<=100;j++)
				{
					//sprintf(key, "7d%_S0EjA%d",i,j,k);
					for(int k=1;k<=300;k++)
					{
						sprintf(key, "%d_S0E%dA%d",i,j,k);
						sprintf(value, "{\"real_value\": \"%0.1f\", \"status\": 1, \"save_time\": \"%d\", \"display_value\": \"%0.1f\"}", ac, szTime,ac);
						key1 = key;
						value1 = value;
						printf("key1:<%s> value1:<%s>\n",key1.c_str(),value1.c_str());
						my_map.insert(pair<string,string>(key1,value1));
					}
				}
			}
			for (IT p = my_map.begin(); p != my_map.end(); p++)
			{
				redis->set(p->first,p->second);
				
			}
			printf("\n");
			gettimeofday(&end,NULL);
			timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
			printf("timer = %ld us\n",timer);
		}
        exit(EXIT_SUCCESS);  
    }  
    else      
    {  
        puts("in parent");  
        printf("\tparent pid = %d\n",getpid());  
        printf("\tparent ppid = %d\n",getppid()); 
		while(true)
		{
			gettimeofday(&start,NULL);
			key1 = "";
			value1 = "";
			szTime = (int)time(NULL);
			srand(szTime);
			float ac = (Random(m, n) / 10.0);
			int ad=1;
			for(int i=71;i<72;i++)
			{
				for(int j=1;j<=100;j++)
				{
					//sprintf(key, "7d%_S0E%dA%d",i,j,k);
					for(int k=1;k<=200;k++)
					{
						sprintf(key, "%d_S0E%dD%d",i,j,k);
						// sprintf(value, "{ \"real_value\": \"%0.1f\", \"status\": 1, \"save_time\": \"%d\", \"display_value\": \"%0.1f\"}", ac, szTime,ac);
						sprintf(value,"{\"real_value\": \"%d\", \"status\": 1, \"save_time\": \"1489481955.37\", \"display_value\": \"%d\"}",ad,szTime,ad);
						key1 = key;
						value1 = value;
						//printf("key1:<%s> value1:<%s>\n",key1.c_str(),value1.c_str());
						my_map.insert(pair<string,string>(key1,value1));
					}
				}
			}
			for (IT p = my_map.begin(); p != my_map.end(); p++)
			{
				redis->set(p->first,p->second);
			}
			printf("\n");
			gettimeofday(&end,NULL);
			timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
			printf("timer = %ld us\n",timer);
		}
    }  
    exit(EXIT_SUCCESS);
	delete redis;
	//cout << szTime << endl;
	return 0;	
}  
	///////////////////////////////////////////////////////////////////////////////////
	// sprintf(key, "0_26637_1_1_0");
	// float ac = (Random(m, n) / 10.0);
	// sprintf(value, "{\"real_value\": \"%0.2f\", \"status\": 1, \"save_time\": \"%d\", \"display_value\": \"%0.2f\"}", ac, szTime,ac);
	// key1=key;
	// value1=value;
	// printf("key:<%s> value:<%s>\n",key1.c_str(),value1.c_str());
	// my_map.insert(pair<string,string>(key1,value1));
	
	///////////////////////////////////////////////////////////////////////////////////
	 // if (!redis->connect("127.0.0.1",6379)){
    	 	// printf("redis connect error");
    	 	// return 0;
    // }
	// for (int i = 0; i < 10; i++)
	// {
		// sprintf(key, "0_26637_1_1_0");
		// float ac = (Random(m, n) / 10.0);
		// sprintf(value, "{ \"real_value\": \"%0.1f\", \"status\": 1, \"save_time\": \"%d\", \"display_value\": \"%0.1f\"}", ac, szTime,ac);
		// key1 = key;
		// value1 = value;
		// tMap.insert(map<string,string>::(key1,value1));
	// }

	// for (IT p = my_map.begin(); p != my_map.end(); p++)
	// {
		// //s = client->set(p->first, p->second);
		
		// // if(s.ok()){
       		 // // printf("k = hello ssdb!\n");
   	   	 // // }else{
       		 // // printf("error!\n");
    	 // // }
		// redis->set(p->first,p->second);
		// sleep(5);	
	// }

	//delete client;
