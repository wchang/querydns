/**
 * This file belongs to CERN.
 *
 * This is a function that resolves the hostname by specific nameserver.
 * We get the cname and then resolve the cname to IP address.
 *
 */

#include "querydns.h"

using namespace std; // NOLINT

bool QueryDns(const char *name,
              int ecord_type,
              const char *dns_server,
              const uint16_t port,
              string *result)
{
  int len;
  int ans_len = 1024;
  bool check_cname;
  u_char answer[ans_len];
  ns_msg ns;
  ns_rr rr;
  char *ans_cname;
  string string_buff;

  len = res_init();
  if (len != 0)
  {
    cout << "Initial Failed." << endl;
    return false;
  }
  
  // Query the canonical name.
  len = res_query(name,ns_c_in, T_CNAME, answer, ans_len);
  if (len >= 0) 
  {
    check_cname = true;
    len = ns_initparse(answer,len, &ns);
    ns_parserr(&ns, ns_s_an, len, &rr);
  
    // Uncompress the domain name.
    ns_name_uncompress(ns_msg_base(ns),ns_msg_end(ns),ns_rr_rdata(rr),
                       ans_cname,MAXDNAME);
  }

  // Set the nameserver and the port.
  if (dns_server != "DEFAULT")
  {
    _res.nsaddr_list[0].sin_addr.s_addr = inet_addr(dns_server);
    _res.nsaddr_list[0].sin_port = port;
  }

  // Resolve the cname to the IP address by specific nameserver.  
  if (check_cname == true)
     len = res_query(ans_cname,ns_c_in,ecord_type,answer,ans_len);
  else
     len = res_query(name,ns_c_in,ecord_type,answer,ans_len);

  if (len < 0)
  {
    cout << "Query IP Assress Failed." << endl;
    herror("res_query");
    return false;
  }

  len = ns_initparse(answer, len, &ns);
  ns_parserr(&ns, ns_s_an, len, &rr);

  // Get the IP address and change the output type. 
  string_buff = string_buff.assign(inet_ntoa(*((struct in_addr *)rr.rdata))); 
  *result = string_buff;

  return true;
}

int main(int argc,char* argv[])
{
  string my_string;
  QueryDns("www.cern.ch", T_A, "137.138.16.5", 53, &my_string);
  cout << "The IP address is :"<< my_string << endl;
  return 0;
}
