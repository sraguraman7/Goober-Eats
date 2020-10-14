#include "ExpandableHashMap.h"
#include "provided.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
	return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
	StreetMapImpl();
	~StreetMapImpl();
	bool load(string mapFile);
	bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
	int typeLine(string s);
	ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_map;
	StreetSegment m_seg;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
	m_map.reset();
	ifstream infile(mapFile);		//take in file
	if (! infile)
		return false;

	string s;
	string string1;
	string string2;
	string string3;
	string string4;
	while (getline(infile, s))		//while taking in lines
	{
		int type = typeLine(s);		//figure what line it is

		if (type == 0)		//if it is empty
			continue;

		if (type == 1)		//if it is a name
		{
			m_seg.name = s;
			continue;
		}
			
		if (type == 2)		//if it is number of segs
			continue;

		if (type == 3)		//if it is a coord line
		{
			int i = 0;
			for (i = 0; s[i] != ' '; i++)		//add each lat/long into a string, string 1 and string 2 are start, string 3 and 4 are end
				string1 += s[i];
			i++;
			for (; s[i] != ' '; i++)
				string2 += s[i];
			i++;
			for (; s[i] != ' '; i++)
				string3 += s[i];
			i++;
			for (; i != s.size(); i++)
				string4 += s[i];

			GeoCoord start(string1, string2);
			auto ptr = m_map.find(start);
			if (ptr == nullptr)		//if start isn't found, push it back and create association
			{
				vector<StreetSegment> vec;
				vec.push_back(m_seg);
				m_map.associate(start, vec);
			}
			else
				(*ptr).push_back(m_seg);		//otherwise, add it to the list
			GeoCoord end(string3, string4);
			GeoCoord temp = m_seg.start;		//reverse
			m_seg.start = m_seg.end;
			m_seg.end = temp;

			auto ptr1 = m_map.find(end);		//same with end for reverse segments
			if (ptr1 == nullptr)
			{
				vector<StreetSegment> vec;
				vec.push_back(m_seg);
				m_map.associate(end, vec);
			}
			else
				(*ptr1).push_back(m_seg);

			string1 = "";		//reset all strings
			string2 = "";
			string3 = "";
			string4 = "";
			s = "";
		}

		else
			continue;

	}
	return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	auto ptr = m_map.find(gc);		//find  pt
	if (ptr == nullptr)		//if not found, return false
		return false;
	
	segs.clear();		//otherwise, clear, and give the vector at the val
	segs = *ptr;
	return true;
}

int StreetMapImpl::typeLine(string s)		// 0 for empty string, 1 for name, 2 for Segment number, 3 for Coordinate line
{
	if (s.size() == 0)
		return 0;

	for (int i = 0; i != s.size(); i++)		//if there are alphabet chars
		if (isalpha(s[i]))
			return 1;

	for (int i = 0; i != s.size(); i++)		//if there is a space
		if (s[i] == ' ')
			return 3;

	return 2;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
	m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
	delete m_impl;
}

bool StreetMap::load(string mapFile)
{
	return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	return m_impl->getSegmentsThatStartWith(gc, segs);
}

