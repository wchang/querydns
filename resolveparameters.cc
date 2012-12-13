#include "resolveparameters.h"

struct ConfigValue 
{
  string value;
  string source;
};

using namespace std;
bool ResolveParameters()
{
  bool retval;
  string flag = "@";
  string res_result;
  string res_host;

  int type =  AF_INET;
  const uint16_t port = 53;
  const string ns_server="137.138.234.60";

  map<string,ConfigValue> *cvmfs_conf = NULL;
  cvmfs_conf = new map<string, ConfigValue>();

  ConfigValue value;

  value.source = "config_file";
  value.value = "cern.ch";
  (*cvmfs_conf)["CVMFS_DEFAULT_DOMAIN"]=value;
  value.value = "@httpproxy.geo.cdn.cernvm.org";
  (*cvmfs_conf)["CVMFS_HTTP_PROXY"]=value;
  (*cvmfs_conf)["CVMFS_HTTP_PROXY2"]=value; 

  for (map<string, ConfigValue>::iterator iter = cvmfs_conf->begin(),
       iEnd = cvmfs_conf->end(); iter != iEnd; ++iter)
  {
    string conf_val = iter->second.value;
    if( conf_val.find_first_of(flag) == 0 )
    {  
       res_host = res_host.assign(conf_val,1,conf_val.length()-1);
       retval = QueryDns(res_host, type, &ns_server, port, &res_result);       
       if (retval == 1)
          iter->second.value = res_result;
       else 
          iter->second.value = "";

       setenv(iter->first.c_str(),res_result.c_str(),1);
    }
  }
 
  for (map<string, ConfigValue>::iterator iter = cvmfs_conf->begin(),
       iEnd = cvmfs_conf->end(); iter != iEnd; ++iter)  
     cout << "Key:"<<iter->first<<" "<<"Value:"<<iter->second.value<<endl;  

  return true;
}

int main(int argc,char* argv[])
{
  ResolveParameters();
  return 0;
}
