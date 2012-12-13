bool ResolveParameters()
{
  bool retval;
  string flag = "@";
  string res_result;
  string res_host;

  int type =  AF_INET;
  const uint16_t port = 53;
  const string ns_server="137.138.234.60";

  for (map<string, ConfigValue>::iterator iter = conf_->begin(),
       iEnd = conf_->end(); iter != iEnd; ++iter)
  {   
    string  conf_val = iter->second.value;

    if( conf_val.find_first_of(flag) == 0 )
    {
       res_host = res_host.assign(conf_val, 1, conf_val.length()-1);
       retval = QueryDns(res_host, type, &ns_server, port, &res_result);
       if (retval == 1)
          iter->second.value = res_result;
       else
          iter->second.value = "";

       setenv(iter->first.c_str(), my_string.c_str(),1);
    }
  }

  return true;
}
