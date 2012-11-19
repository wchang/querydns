/**
 *
 * This is a function that resolves the hostname which is given by users.
 * This function uses c-ares package
 *
 */

#include "querydns.h"

// Global variable for passing ip address
static char ip[INET6_ADDRSTRLEN];

using namespace std; // NOLINT

static void run_ares_resolve(ares_channel channel)
{ 
  int nfds, count;
  fd_set readers, writers;
  struct timeval tv, *tvp;
  while (1)
  {
    FD_ZERO(&readers);
    FD_ZERO(&writers);
    nfds = ares_fds(channel, &readers, &writers);
    if (nfds == 0)
       break;
    tvp = ares_timeout(channel, NULL, &tv);
    count = select(nfds, &readers, &writers, NULL, tvp);
    ares_process(channel, &readers, &writers);
  }
}

static void callback(void *arg, int status, int timeouts, struct hostent *host)
{
    if (!host || status != ARES_SUCCESS)
    {
        cout << "Lookup Fail :" <<endl;
        cout << ares_strerror(status)<<endl;
        return;
    }
  
//    for (int ii = 0; host->h_addr_list[ii]; ++ii) 
    inet_ntop(host->h_addrtype, host->h_addr_list[0], ip, sizeof(ip));    
}

bool QueryDns(const char *name, int type, const char *dns_server,
              const uint16_t port, string *result)
{
  ares_channel channel;
  int status;
  struct ares_options options;
  int optmask = 0;
  string string_buff;

  status = ares_library_init(ARES_LIB_INIT_ALL);
  if (status != ARES_SUCCESS)
  {
        cout << "Ares Library_Init Fail :" <<endl;
        cout << ares_strerror(status);
        return 1;
  }
  
  status = ares_init_options(&channel, &options, optmask); 
  if (status != ARES_SUCCESS)
  {
        cout << "Ares Init_Options Fail :" << endl;
        cout << ares_strerror(status);
        return 1;
  }

  ares_gethostbyname(channel, name, type, callback, NULL);

  run_ares_resolve(channel);

  // Get the IP address and change the output type. 
  string_buff = string_buff.assign(ip);
  *result = string_buff;

  ares_destroy(channel);
  ares_library_cleanup();

  return true;
}

int main(int argc,char* argv[])
{
  string my_string;
  QueryDns("www.cern.ch", AF_INET,"137.138.16.5", 53, &my_string);
  cout << "The IP address is :"<< my_string << endl;
  return 0;
}
