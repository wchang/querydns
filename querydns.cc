/**
 * This file belongs to CERN.
 *
 * This is a function that resolves the hostname by specific nameserver.
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
  u_char answer[ans_len];
  ns_msg ns;
  ns_rr rr;
  string string_tmp;

  len = res_init();
  if (len < 0)
  {
    cout << "Initial Failed." << endl;
    return false;
  }
  
  // Set the nameserver and the port.
  _res.nsaddr_list[0].sin_addr.s_addr = inet_addr(dns_server);
  _res.nsaddr_list[0].sin_port = port;

  len = res_query(name, C_IN, ecord_type, answer, ans_len);
  if (len < 0)
  {
    herror("res_query");
    return false;
  }

  len = ns_initparse(answer, len, &ns);
  ns_parserr(&ns, ns_s_an, len, &rr);
  string_tmp = string_tmp.assign(inet_ntoa(*((struct in_addr *)rr.rdata)));
  *result = string_tmp;
  return true;
}

int main(int argc,char* argv[])
{
  string my_string;
  QueryDns("www.cern.ch", T_A, "137.138.16.5", 53, &my_string);
  cout << "The IP is :"<< my_string << endl;
  return 0;
}
