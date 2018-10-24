// last modified 

#include "RmdownSeedWebpage.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include "../helper/Misc.h"


using namespace std;

static bool
parsePostMultiSections ( const string& webpage_txt,
                         vector<pair<string, string>>& post_sections_list ) 
{
    // parse the ref section
	string post_url_("");
    static const string& keyword_ref_section_begin("<INPUT TYPE=\"hidden\" name=\"ref\" value=\"");
    static const string& keyword_ref_section_end("\"");
    const pair<string, size_t>& pair_tmp = fetchStringBetweenKeywords( webpage_txt,
                                                                       keyword_ref_section_begin,
                                                                       keyword_ref_section_end );
    const string& ref_content = pair_tmp.first;
    if (ref_content.empty()) {
        cerr << "WARNING! parsePostMultiSections() CANNOT find the keyword "
             << "\"" << keyword_ref_section_begin << "\"" << " and "
             << "\"" << keyword_ref_section_end << "\"" << endl;
        return(false);
    }
    post_sections_list.push_back(make_pair("ref", ref_content));
	post_url_ = post_url_ + "?ref=" + ref_content;
    // const auto keyword_ref_section_end_pos = pair_tmp.second;

    // parse the reff section
    static const string& keyword_reff_section_begin("INPUT TYPE=\"hidden\" NAME=\"reff\" value=\"");
    static const string& keyword_reff_section_end("\"");
    const pair<string, size_t>& pair_tmp2 = fetchStringBetweenKeywords( webpage_txt,
                                                                       keyword_reff_section_begin,
                                                                       keyword_reff_section_end);
    const string& reff_content = pair_tmp2.first;
    if (reff_content.empty()) {
        cerr << "WARNING! parsePostMultiSections() CANNOT find the keyword "
             << "\"" << keyword_reff_section_begin << "\"" << " and "
             << "\"" << keyword_reff_section_end << "\"" << endl;
        return(false);
    }
	post_url_ = post_url_ + "&reff=" + reff_content;
    post_sections_list.push_back(make_pair("reff", reff_content));
    post_sections_list.push_back(make_pair("url", post_url_));

    return(true);
}

// seed fetch URL. http://www.rmdown.com/ and http://www.xunfs.com/ are
// the same one website, on the other word, from http://www.rmdown.com/abcd
// download the seed file same as from http://www.xunfs.com/abcd, so, I need
// just ONE fetch URL 
RmdownSeedWebpage::RmdownSeedWebpage (const string& url, const string& proxy_addr)
    : SeedWebpage(url, proxy_addr, "http://www.rmdown.com/download.php", parsePostMultiSections)
{
    ;
}

RmdownSeedWebpage::~RmdownSeedWebpage ()
{
    ;
}

bool
RmdownSeedWebpage::downloadSeed (const string& path, const string& base_name)
{
    if (post_sections_list_.empty()) {
        return(false);
    }

    // make seed name
    static const string seed_postfix(".torrent");
    string seed_filename = path + "/" + base_name + seed_postfix;
	for (const auto& e : post_sections_list_) {
        const string& name = e.first;
        const string& content = e.second;
		if (name == "url"){
			post_url_ = "http://www.rmdown.com/download.php" + content;
		}
	}
	cout << post_url_ << endl;
    return(downloadFile(post_url_, seed_filename, ""));
}

