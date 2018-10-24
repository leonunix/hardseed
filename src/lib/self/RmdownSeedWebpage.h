// last modified 

#pragma once

#include <string>
#include "SeedWebpage.h"

using std::string;


class RmdownSeedWebpage : public SeedWebpage
{
    public:
        RmdownSeedWebpage (const string& url, const string& proxy_addr);
        virtual ~RmdownSeedWebpage ();
		bool downloadSeed (const string& path, const string& base_name);
		
	private:
        string post_url_; 
};
