#include "resolveparameters.h"

struct ConfigValue {
  string value;
  string source;
};

using namespace std;
bool resolveparameters()
{
  string flag;
  string cvmfs_conf_val;
  string res_hostname;
  string my_string;
  char *shell;
  flag="@";

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
    cvmfs_conf_val = iter->second.value;
    if( cvmfs_conf_val.find_first_of(flag) == 0 )
    {  
       res_hostname = res_hostname.assign(cvmfs_conf_val,1,cvmfs_conf_val.length()-1);
       QueryDns(res_hostname, AF_INET,"137.138.234.60", 53, &my_string);       
       iter->second.value = my_string;
       setenv(iter->first.c_str(),my_string.c_str(),1);
    }
  }
 
  shell = getenv("CVMFS_HTTP_PROXY");
  cout <<"ENV CVMFS_HTTP_PROXY :"<< shell << endl; 
  shell = getenv("CVMFS_HTTP_PROXY2");
  cout <<"ENV CVMFS_HTTP_PROXY2 :"<< shell << endl;

  return true;
}

int main(int argc,char* argv[])
{
  resolveparameters();
  return 0;
}
