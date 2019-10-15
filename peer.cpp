#include "myheader.h"
#define BUFF_SIZE 524288
#define BUFF 10240
#define BACK 2500
using namespace std;
string TR1ip;
string TR2ip;
string TR1port;
string TR2port;
string severip;
string serverport;
string user_id;
string group;
vector<thread> threadVector;
int threadCount;
sem_t m;

// unordered_map<string,set<string> >GroupMap;
// unordered_map<string,string>UserMap;
unordered_map<string,set<int> >AvailableChunkInfoPerFileBasis;
unordered_map<string,string>FileIdandFilepathMap;
unordered_map<string,int>download_status;
bool islogedin=false;
vector<string>ArrayOfString(string s,char del)
{
  stringstream ss(s);
  vector<string>a;
  string temp;
  while(getline(ss,temp,del))
  {
  	//cout<<"hi"<<endl;
    a.push_back(temp);
  }
  return a;
}
void create_user(string user_id ,string pass)
{
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
   	perror("Failed to obtain socket descriptor");
   	exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string token="create_user";
   token+=";";
   token+=user_id;
   token+=";";
   token+=pass;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   goto l2;
   l2:
     cout<<"";

}  
void share_file_details(string group_id,string FileId)
{
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
      perror("Failed to obtain socket descriptor");
      exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
      perror("Connect failed");
      exit(1);
   }
   string token="share_file_details";
   token+=";";
   token+=group_id;
   token+=";";
   token+=FileId;
   token+=";";
   token+=severip;
   token+=":";
   token+=serverport;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   goto l2;
   l2:
     cout<<"";
}
void upload_file(string group_id,string FileId)
{
   if(group==group_id){
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
      perror("Failed to obtain socket descriptor");
      exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
      perror("Connect failed");
      exit(1);
   }
   FileIdandFilepathMap[FileId]=FileId;
   string token="upload_file";
   token+=";";
   token+=group_id;
   token+=";";
   token+=FileId;
   token+=";";
   token+=severip;
   token+=":";
   token+=serverport;
   cout<<token<<endl;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   bzero(buffer,sizeof(buffer));
   recv(socketdes,buffer,sizeof(buffer),0);
   // cout<<"send succesfully"<<endl;
   // cout<<"recived successfull "<<buffer<<endl;
   string jk=buffer;
   int size=stoi(jk);
   // cout<<"in line 132"<<size<<endl;
   int no_of_chunnk=((size+(BUFF_SIZE-1))/BUFF_SIZE);
   cout<<"no_of_chunnk="<<no_of_chunnk<<endl;
   for(int i=0;i<no_of_chunnk;i++)
   {
      AvailableChunkInfoPerFileBasis[FileId].insert(i);
   }
   // cout<<"in line 138 AvailableChunkInfoPerFileBasis updadated"<<endl;
 }
 else{
   cout<<"Error you have to join "<<group_id<<"to upload the file"<<endl;
 }
 goto l2;
   l2:
     cout<<"";
}
void login(string user ,string pass)
{
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
   	perror("Failed to obtain socket descriptor");
   	exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string token="login";
   token+=";";
   token+=user;
   token+=";";
   token+=pass;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   char ss[1];
   // cout<<"send successfull"<<endl;

   recv(socketdes,ss,sizeof(ss),0);
   // cout<<ss<<endl;
   int status=ss[0]-'0';
   // cout<<status<<endl;
   if(status==1)
   {
   	islogedin=true;
   	user_id=user;
      // cout<<user_id<<endl;
   	cout<<"succesfully logged in"<<endl;
      // cout<<user_id;
   }
   else
   {
   	cout<<"wrong cred"<<endl;
   	cout<<"enter the cred again"<<endl;
   	//goto l2;

   }
  goto l2;
   l2:
     cout<<"";
}


void create_group(string group_id)
{
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
   	perror("Failed to obtain socket descriptor");
   	exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   //trakeraddr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string token="create_group";
   token+=";";
   token+=group_id;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   char ss[1];

   recv(socketdes,ss,sizeof(ss),0);
   int status=ss[0]-'0';
   if(status==1)
   {
   	cout<<"Group created succesfully"<<endl;
   }
   else
   {
   	cout<<"Group already exits"<<endl;
   	cout<<"enter valid group again"<<endl;
   	//goto l2;
   }
  goto l2;
   l2:
     cout<<"";
}
void join_group(string group_id)
{
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
   	perror("Failed to obtain socket descriptor");
   	exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   //trakeraddr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string token="join_group";
   token+=";";
   token+=group_id;
   token+=";";
   token+=user_id;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   // cout<<"token="<<token<<endl;
   char ss[1];

   recv(socketdes,ss,sizeof(ss),0);
   int status=ss[0]-'0';
   if(status==1)
   {
      group=group_id;
   	cout<<"succesfully joined to group"<<endl;
   }
   else
   {
   	cout<<"Group donot exits"<<endl;
   	cout<<"enter valid group again"<<endl;
   	//goto l2;
   }
  goto l2;
   l2:
     cout<<"";
}
void list_files(string group_id)
{
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
      perror("Failed to obtain socket descriptor");
      exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   //trakeraddr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
      perror("Connect failed");
      exit(1);
   }
   string token="list_files";
   token+=";";
   token+=group_id;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   // cout<<"token="<<token<<endl;
   
   recv(socketdes,buffer,sizeof(buffer),0);
   string ans=buffer;
   vector<string>arr=ArrayOfString(ans,';');
   for(int i=0;i<arr.size();i++)
   {
      cout<<arr[i]<<" ";
   }
   cout<<endl;
   goto l2;
   l2:
     cout<<"";
}

void leave_group(string group_id)
{
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
      perror("Failed to obtain socket descriptor");
      exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   //trakeraddr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
      perror("Connect failed");
      exit(1);
   }
   string token="leave_group";
   token+=";";
   token+=group_id;
   token+=";";
   token+=user_id;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   cout<<"token="<<token<<endl;
   char ss[1];

   recv(socketdes,ss,sizeof(ss),0);
   int status=ss[0]-'0';
   if(status==1)
   {
      group="";
      cout<<"succesfully exit from group"<<endl;
   }
   else
   {
      cout<<"Group donot exits or user does not belong to the group"<<endl;
      cout<<"enter valid group again"<<endl;
      //goto l2;
   }
  goto l2;
   l2:
     cout<<"";
}
void list_groups()
{
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
   	perror("Failed to obtain socket descriptor");
   	exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   //trakeraddr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string token="list_groups";
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   //char buffer[BUFF];

   recv(socketdes,buffer,sizeof(buffer),0);
   string buff=(buffer);
   vector<string>grouplist=ArrayOfString(buff,';');
   for(auto i : grouplist)
   {
   	cout<<i<<endl;
   }
   goto l2;
   l2:
     cout<<"";
}
void send_the_packet_vector(int newsocketdes,string FileId)
{
   // cout<<"in line 326 send the packet vector"<<endl;
   string chunkdetails="";
      set<int>:: iterator it;
   for(it=AvailableChunkInfoPerFileBasis[FileId].begin();it!=AvailableChunkInfoPerFileBasis[FileId].end();it++)
   {
     if(it==AvailableChunkInfoPerFileBasis[FileId].begin())
     {
       chunkdetails+=to_string(*it);
     }
     else
     {
      chunkdetails+=";";
      chunkdetails+=to_string(*it);
     }
   }
   // cout<<"in line 341 chunkdetails= "<<chunkdetails<<endl;
   send(newsocketdes,chunkdetails.c_str(),strlen(chunkdetails.c_str()),0);
   close(newsocketdes);
  goto l2;
   l2:
     cout<<"";
}


void get_the_particular_packet(int newsocketdes,string FileId,string packetNos)
{
   // cout<<"in line 361 get_the_particular_packet"<<endl;
   string Filepath=FileIdandFilepathMap[FileId];
   // cout<<"in line 363 Filepath="<<Filepath<<endl;
   FILE *fp=fopen(Filepath.c_str(),"rb");
   // cout<<"in line 365"<<endl;
   int val;
   char buffer[BUFF];
   bzero(buffer,BUFF);
   vector<string>ArrayofPacket=ArrayOfString(packetNos,';');
   vector<int>ArrayofPacket_int;
   for(int i=0;i<ArrayofPacket.size();i++)
   {
      ArrayofPacket_int.push_back(stoi(ArrayofPacket[i]));
   }
   for(int i=0;i<ArrayofPacket_int.size();i++)
   {
      fseek(fp,ArrayofPacket_int[i]*BUFF_SIZE,SEEK_SET);
      int size=BUFF_SIZE;
      while((val=fread(buffer,sizeof(char),BUFF,fp))>0&&size>0)
      {
         // cout<<"in line 442 in get get_the_particular_packet val= size= "<<val<<" "<<size<<endl;
         send(newsocketdes,buffer,val,0);
         //char bu[1]='\0';
         //recv(newsocketdes,bu,1,0);
         memset ( buffer , '\0', sizeof(buffer));
         size=size-val;
      }
      bzero(buffer,BUFF);
      
   }
   fclose(fp);
   close(newsocketdes);
   goto l2;
   l2:
     cout<<"";
}
vector<int> query_for_chunk(string IPport,string FileId)
{
   // cout<<"in line 374 query_for_chunk"<<endl;
   vector<string>IPcred=ArrayOfString(IPport,':');
   int socketdes;
   int newsocketdes;
   // cout<<"in line 391 "<<IPcred[0]<<" "<<IPcred[1]<<endl;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
      perror("Failed to obtain socket descriptor");
      exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(IPcred[1]));
   //trakeraddr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,IPcred[0].c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
      perror("Connect failed");
      exit(1);
   }
   string token="send_the_packet_vector";
   token=token+";";
   token+=FileId;
   // token+=";";
   // token+=FileId;
    // cout<<token<<endl;
    send(socketdes,token.c_str(),strlen(token.c_str()),0);
   //char buffer[BUFF];

   recv(socketdes,buffer,sizeof(buffer),0);
   // cout<<"in line 405 succesfully recived "<<buffer<<endl;
   vector<string>PacketVector=ArrayOfString(buffer,';');
   vector<int>Packet;
   for(int i=0;i<PacketVector.size();i++)
   {
     Packet.push_back(stoi(PacketVector[i]));
   }
   return Packet;
}
void downloadPiece(vector<int>packetList,string IPport,string FileId,string downloadFilepath)
{
   // cout<<"in line 428 in downloadPiece"<<endl;
   // cout<<"printing the packetList"<<endl;
   cout<<"Downloading the following packetList from IPport="<<IPport<<endl;
   cout<<"printing the packetlist"<<endl;
   for(int i=0;i<packetList.size();i++)
   {
      cout<<packetList[i]<<" ";
   }
   cout<<endl;
    vector<string>IPcred=ArrayOfString(IPport,':');
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   for(int i=0;i<packetList.size();i++)
   {
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
      perror("Failed to obtain socket descriptor");
      exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(IPcred[1]));
   //trakeraddr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,IPcred[0].c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
      perror("Connect failed");
      exit(1);
   }
   string token="get_the_particular_packet";
   token+=";";
   token+=FileId;
   token+=";";
   token+=to_string(packetList[i]);
   // cout<<"in line 457 token= "<<token<<endl;
   // cout<<"in line 534 ipport== 0"<<IPport<<endl;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   bzero(buffer,sizeof(buffer));
   int val;
   int size=BUFF_SIZE;
  
   sem_wait(&m);
   // fstream in;
   //    in.open(downloadFilepath.c_str(),ios::out|ios::in|ios::binary);
   //    cout<<"in line 547 seeking to a "<<packetList[i]<<" "<<packetList[i]*BUFF<<endl;
   //    in.seekp(packetList[i]*BUFF,ios::beg);
    char ans[BUFF_SIZE];
    int count=0;
    int sum=0;
    fstream in;
      in.open(downloadFilepath.c_str(),ios::out|ios::in|ios::binary);
      // cout<<"in line 547 seeking to a "<<packetList[i]<<" "<<packetList[i]*BUFF_SIZE<<endl;
      in.seekp(packetList[i]*BUFF_SIZE,ios::beg);
      cout<<"download file in piece in progress "<<"for chunk "<<packetList[i]<<endl;

   while((val=recv(socketdes,buffer,BUFF,0))>0&&size>0)
   {
      // cout<<"in line 534 Val= "<<val<<endl;
      
     //  if(count==0)
     //  {
     //     strcpy(ans,buffer);
     //  }
     //  else 
     //  {
     //     strcat(ans,buffer);
     //  }
     //  count++;
      
     // // cout<<"buffer="<<buffer<<endl;

     //  // ans+=buffer;
     //  //char b[1]='h';

      // cout<<"buffer="<<buffer<<endl;
      in.write(buffer,val);
      
      bzero(buffer,sizeof(buffer));
      sum=sum+val;
      size=size-val;
      val=0;
      
     

   }
   // cout<<"final buffer="<<ans<<endl;
   // cout<<"in line 582 size of sum ="<<sum<<endl;
    // in.write(ans,sum);
      
      
      in.close();
     // FILE *fp=fopen(downloadFilepath.c_str(),"rb+");
     //  fseek(fp,packetList[i]*BUFF_SIZE,SEEK_SET);
     //  fwrite(ans,1,sum,fp);
     //  memset(ans,'\0',sizeof(ans));
     //  fclose(fp);
    // cout<<"il line 584"<<endl;
    AvailableChunkInfoPerFileBasis[FileId].insert(packetList[i]);
    // in.close();
    sem_post(&m);
    

   close(socketdes);
 }
 goto l2;
   l2:
     cout<<"";

}
void download_file(string group_id,string FileId,string downloadFilepath)
{
   if(group==group_id){
   
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
      perror("Failed to obtain socket descriptor");
      exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   //trakeraddr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
      perror("Connect failed");
      exit(1);
   }
   string token="seeder_list";
   token=token+";";
   token+=group_id;
   token+=";";
   token+=FileId;
    send(socketdes,token.c_str(),strlen(token.c_str()),0);
   //char buffer[BUFF];

   recv(socketdes,buffer,sizeof(buffer),0);
   // cout<<"Buffer recieved from form the server "<<buffer<<endl;

   vector<string>IPport=ArrayOfString(buffer,';');
   vector<thread>FileDownloadThread;
   int fileSize=stoi(IPport[0]);
   cout<<"printing the seeder list "<<endl;
   for(int i=0;i<IPport.size();i++)
   {
      cout<<IPport[i]<<" ";
   }
   cout<<endl;
   // cout<<fileSize<<endl;
   string ss="";
   for(int i=0;i<fileSize;i++)
   {
      ss+='\0';
   }
   fstream in(downloadFilepath,ios::out|ios::binary);
   in.write(ss.c_str(),strlen(ss.c_str()));  
   in.close();
   int NumberOfPacket=((fileSize+(BUFF_SIZE-1))/BUFF_SIZE);
   cout<<"no of packets is "<<NumberOfPacket<<endl;
   
   FileIdandFilepathMap[FileId]=downloadFilepath;
   vector<vector<int> > ListofSeederWithChunk;
   // cout<<"in line 536 "<<IPport.size()<<endl;
   vector<int>temp;
   for(int i=1;i<IPport.size();i++)
   {
      // cout<<"in line 539 i="<<i<<endl;
      temp=query_for_chunk(IPport[i],FileId);
      cout<<"printing chunk details for each seeder"<<endl;
      for(int i=0;i<temp.size();i++)
      {
         cout<<temp[i]<<" ";
      }
      cout<<endl;
      ListofSeederWithChunk.push_back(temp);

   }
   cout<<"printing the ListofSeederWithChunk"<<endl;
   for(int i=0;i<ListofSeederWithChunk.size();i++   )
   {
     for (int j=0;j<ListofSeederWithChunk[i].size();j++)
     {
      cout<<ListofSeederWithChunk[i][j]<<" ";
     }
     cout<<endl;
   }
   cout<<endl;
   int seederCount=IPport.size()-1;
   // cout<<"in line 541 "<<seederCount<<endl;

   vector<int>PacketArray;
   vector<vector<int> >seedersWithPacketMap(seederCount);


   for(int i=0;i<NumberOfPacket;i++)
   {
      PacketArray.clear();
      for(int j=0;j<ListofSeederWithChunk.size();j++)
      {
         if(ListofSeederWithChunk[j].begin(),ListofSeederWithChunk[j].end(),1)
         {
            PacketArray.push_back(j);
         }
      }
      int randomLocation=rand()%PacketArray.size();
      seedersWithPacketMap[PacketArray[randomLocation]].push_back(i);

   }
   cout<<"printing seeder with packet map"<<endl;
   for(int i=0;i<seedersWithPacketMap.size();i++   )
   {
     for (int j=0;j<seedersWithPacketMap[i].size();j++)
     {
      cout<<seedersWithPacketMap[i][j]<<" ";
     }
     cout<<endl;
   }
   thread upload(share_file_details,group_id,FileId);
   upload.detach();
   // cout<<"in line 572 share_file_details completed"<<endl;
   for(int i=0;i<seedersWithPacketMap.size();i++)
   {
      FileDownloadThread.push_back(thread(downloadPiece,seedersWithPacketMap[i],IPport[i+1],FileId,downloadFilepath));
   }
   vector<thread>:: iterator it;
  for(it=FileDownloadThread.begin();it!=FileDownloadThread.end();it++)

   {
      if(it->joinable()) 
         it->join();
   }

}
else
{
   cout<<"You have to join the group "<<group_id<<"  to download the file"<<endl;
  goto l2; 
   
}
l2:
    cout<<"";

}
void serverequest(int newsocketdes,string ip,int port)
{
   l2:
   // cout<<"in line 598"<<endl;

   char buffer[BUFF];
   bzero(buffer,BUFF);
   read(newsocketdes,buffer,sizeof(buffer));
   // cout<<"in line 603 buffer="<<buffer<<endl;
   string r=buffer;
   // cout<<"in line 605 r="<<r<<endl;

   vector<string> requestarray=ArrayOfString(r,';');
   string request=requestarray[0];
   if(request=="send_the_packet_vector")
   {
      string FileId=requestarray[1];
       send_the_packet_vector(newsocketdes,FileId);
   }
   else if(request=="get_the_particular_packet")
   {
      string FileId=requestarray[1];
      string packetNos=requestarray[2];
      get_the_particular_packet(newsocketdes,FileId,packetNos);
   }
   else
   {
      cout<<"Some randome data is coming"<<endl;
      cout<<"please send it again"<<endl;
      goto l2;
   }
}
void serverpart()
{
   int socketdes;
   int newsocketdes;
   int val;
   socklen_t size;
   struct sockaddr_in myaddr;
   struct sockaddr_in otheraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))<0)
   {
      perror("failed to obtained the socket descriptor");
       exit(1);
   }
  myaddr.sin_family=AF_INET;
  myaddr.sin_port=htons(stoi(serverport));
  inet_pton(AF_INET,severip.c_str() , &myaddr.sin_addr); 
  //myaddr.sin_addr.s_addr=TR1ip;

  bzero(&(myaddr.sin_zero),8);

  if(bind(socketdes,(struct sockaddr *)&myaddr,sizeof(struct sockaddr))==-1)
  {
   perror("failed to obtained the bind");
   exit(1);
  }
  if(listen(socketdes,BACK)==-1)
  {
     perror("error backlog overflow");
     exit(1);

  }
  size=sizeof(struct sockaddr);
  while((newsocketdes=accept(socketdes,(struct sockaddr *)&otheraddr,&size))!=-1)
  {
   // cout<<"Got a connection from another peer "<<endl;
   string ip=inet_ntoa(otheraddr.sin_addr);
   int port=(ntohs(otheraddr.sin_port));
   // cout<<"ip="<<ip<<"port"<<port<<endl;
   threadVector.push_back(thread(serverequest,newsocketdes,ip,port));
   size=sizeof(struct sockaddr);
  }
  vector<thread>:: iterator it;
  for(it=threadVector.begin();it!=threadVector.end();it++)

   {
      if(it->joinable()) 
         it->join();
   }
   cout<<"Returning from server "<<endl;
   goto l2;
   l2:
     cout<<"";

}
void logout()
{
   if(group.size()>1){
   int socketdes;
   int newsocketdes;
   char buffer[BUFF];
   struct sockaddr_in trakeraddr;
   if((socketdes=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
      perror("Failed to obtain socket descriptor");
      exit(1);
   }
   trakeraddr.sin_family=AF_INET;
   trakeraddr.sin_port=htons(stoi(TR1port));
   //trakeraddr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,TR1ip.c_str() , &trakeraddr.sin_addr); 
   bzero(&(trakeraddr.sin_zero),8);
   if(connect(socketdes,(struct sockaddr *)&trakeraddr,sizeof(struct sockaddr))==-1)
   {
      perror("Connect failed");
      exit(1);
   }
   string token="logout";
   token+=";";
   token+=severip;
   token+=":";
   token+=serverport;
   token+=";";
   token+=group;
   send(socketdes,token.c_str(),strlen(token.c_str()),0);
   //char buffer[BUFF];
   char ss[1];
   // cout<<"send successfull"<<endl;

   recv(socketdes,ss,sizeof(ss),0);
   // cout<<ss<<endl;
   int status=ss[0]-'0';
   // cout<<status<<endl;
   if(status==1)
   {
      islogedin=false;
      user_id="";
      group="";
      cout<<user_id<<endl;
      cout<<"succesfully logged out from tracker"<<endl;
      cout<<user_id;
   }
   else
   {
      cout<<"failed to logout"<<endl;
   }
 }
   else
   {
      cout<<"user does not belong to any group so loggout form system only "<<endl;
      islogedin=false;
       user_id="";
      group="";
      // cout<<"enter the cred again"<<endl;
      //goto l2;

   }
  goto l2;
   l2:
     cout<<"";
    
}

int main(int argc,char ** argv)
{
   sem_init(&m,0,1);
	if(argc!=3)
	{
		cout<<"Opps! Please give command line arguent in the format <IP>:<PORT> traker_info.txt"<<endl;
		perror("Error in command line argument list ");
		return -1;
	}
    string clientIPort=argv[1];
    string traker_info_path=argv[2];
    vector<string>IPort=ArrayOfString(clientIPort,':');
    severip=IPort[0];
    serverport=IPort[1];
    fstream serverfilestream(traker_info_path,ios::in);
    vector<string>IPortTrakers;
    string temp;
    while(getline(serverfilestream,temp,'\n'))
    {
    	IPortTrakers.push_back(temp);
    }
    IPort=ArrayOfString(IPortTrakers[0],':');
    TR1ip=IPort[0];
 	TR1port=IPort[1];
 	IPort=ArrayOfString(IPortTrakers[1],':');
 	TR2ip=IPort[0];
 	TR2port=IPort[1];
 	cout<<severip<<" "<<serverport<<" "<<TR1ip<<" "<<TR1port<<" "<<TR2ip<<" "<<TR2port<<endl;
 	thread serverthread(serverpart);
 	serverthread.detach();
 	string request;
 l2: for(;;)
 	 {

      getline(cin,request);
      cout<<request<<endl;
      vector<string>requestParse=ArrayOfString(request,' ');
      string command=requestParse[0];
      cout<<command<<endl;
      if(command=="create_user")
      {
      		cout<<"In line 286"<<endl;
      		if(!islogedin)
 		    {
 			cout<<"Please enter the login cred to enter into the system"<<endl;
 			goto l2;
 		    }
            if(requestParse.size()!=3)
            {
            	cout<<"Enter the valid argument"<<endl;
                goto l2;
            }
            else
            {
            	threadVector.push_back(thread(create_user,requestParse[1],requestParse[2]));
            }
      }
      else if(command=="login")
      {
      		cout<<"In line 304"<<endl;

            if(requestParse.size()!=3)
            {
            	cout<<"error in input"<<requestParse.size()<<endl;
                goto l2;
            }
            else
            {
            	// cout<<"In line 313"<<endl;
            	threadVector.push_back(thread(login,requestParse[1],requestParse[2]));

            }
      }
      else if(command=="create_group")
      {
      		// cout<<"In line 319"<<endl;

      	    if(!islogedin)
 		    {
 			cout<<"Please enter the login cred to enter into the system"<<endl;
 			goto l2;
 		    }
            if(requestParse.size()!=2)
            {
            	cout<<"Enter the valid argument"<<endl;
               goto l2;
            }
            else
            {
            	threadVector.push_back(thread(create_group,requestParse[1]));
            }
      }
      else if(command=="join_group")
      {
         // cout<<"In line 343"<<endl;

             if(!islogedin)
          {
         cout<<"Please enter the login cred to enter into the system"<<endl;
         goto l2;
          }
            if(requestParse.size()!=2)
            {
               cout<<"Enter the valid argument"<<endl;
               goto l2;
            }
            else
            {
               threadVector.push_back(thread(join_group,requestParse[1]));
            }
      }
      else if(command=="leave_group")
      {
        // cout<<"In line 877"<<endl;

             if(!islogedin)
          {
         cout<<"Please enter the login cred to enter into the system"<<endl;
         goto l2;
          }
            if(requestParse.size()!=2)
            {
               cout<<"Enter the valid argument"<<endl;
               goto l2;
            }
            else
            {
               threadVector.push_back(thread(leave_group,requestParse[1]));
            }
      }
      else if(command=="list_request")
      {
        cout<<"Not Inplemented "<<endl;
       goto l2;
      }
      else if(command=="accept_request")
      {
       cout<<"Not Inplemented "<<endl;
       goto l2;
      }
      else if(command=="list_groups")
      {
            cout<<"In line 358"<<endl;

             if(!islogedin)
          {
         cout<<"Please enter the login cred to enter into the system"<<endl;
         goto l2;
          }
          if(requestParse.size()!=1)
            {
               cout<<"Enter the valid argument"<<endl;
               goto l2;
            }
            else {
          threadVector.push_back(thread(list_groups));

            }
      }
      else if(command=="list_files")
      {
         // cout<<"In line 941 "<<endl;

             if(!islogedin)
          {
         cout<<"Please enter the login cred to enter into the system"<<endl;
         goto l2;
          }
            if(requestParse.size()!=2)
            {
               cout<<"Enter the valid argument"<<endl;
               goto l2;
            }
            else
            {
               threadVector.push_back(thread(list_files,requestParse[1]));
            }
      }
      else if(command=="upload_file")
      {
        cout<<"In line 697"<<endl;

             if(!islogedin)
          {
         cout<<"Please enter the login cred to enter into the system"<<endl;
         goto l2;
          }
          if(requestParse.size()!=3)
            {
               cout<<"Enter the valid argument"<<endl;
               goto l2;
            }
            else {
               string group_id=requestParse[1];
               string FileId=requestParse[2];
               // cout<<group_id<<" "<<FileId<<endl;
          threadVector.push_back(thread(upload_file,group_id,FileId));

            }
      }
      else if(command=="download_file")
      {
        
            // cout<<"In line 533"<<endl;

             if(!islogedin)
          {
         cout<<"Please enter the login cred to enter into the system"<<endl;
         goto l2;
          }
          if(requestParse.size()!=4)
            {
               cout<<"Enter the valid argument"<<endl;
               goto l2;
            }
            else {
               string group_id=requestParse[1];
               string FileId=requestParse[2];
               string Filepath=requestParse[3];
          threadVector.push_back(thread(download_file,group_id,FileId,Filepath));

            }
      }
      else if(command=="logout")
      {
         cout<<"In line 894"<<endl;

             if(!islogedin)
          {
         cout<<"Please enter the login cred to enter into the system"<<endl;
         goto l2;
          }
            if(requestParse.size()!=1)
            {
               cout<<"Enter the valid argument"<<endl;
               goto l2;
            }
            else
            {
               threadVector.push_back(thread(logout));
            }
        
      }
      else if(command=="Show_downloads")
      {
            cout<<"Not Inplemented "<<endl;
       goto l2;
      }
      else if(command=="stop_share")
      {
            cout<<"Not Inplemented "<<endl;
       goto l2;
      }
      else if(command=="exit")
      {
        cout<<"Good bye"<<endl;
        return 0;
      }
      else
      {
      	cout<<"In line 387"<<endl;

      	// cout<<"Please Enter a valid command "<<endl;
      }
    
 	}
 	return 0;
} 