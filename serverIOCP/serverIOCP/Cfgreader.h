#pragma once
#include <boost/property_tree/xml_parser.hpp>

class Cfgreader
{
public:
	Cfgreader(void);
	~Cfgreader(void);

public:

private:
	boost::property_tree::basic_ptree<>	m_pt;
};

