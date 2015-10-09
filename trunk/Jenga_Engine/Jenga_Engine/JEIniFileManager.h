/*******************************************************************************
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.

filename    JEIniFileManager.h
author      Nahye Park
email       skqla12@gmail.com
course      GAM200
due date    12/12/2014

Brief Description:
  This file contains the function declaration for InifileManager.

*******************************************************************************/

#ifndef INIFILE_MANAGER
#define INIFILE_MANAGER

#define MAX_ANIMATION 20

#include "JEGraphicsNew.h"
#include "JEObjectTypes.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>


#include "JECharacter.h"
#include "JEParts.h"

struct Frame
{
	int time;
	float angle;
	float length;
};


struct Part
{
  float x;
  float y;
  float l;
  float a;
  unsigned int numberOfActions;
  unsigned int frame[MAX_ANIMATION];

  unsigned int textureID;

  Frame keyframe[MAX_ANIMATION][MAX_ANIMATION];
};


namespace INI{

	//typedef std::pair<Part, Frame**>             AddedPartFrame;
	//typedef std::list<AddedPartFrame>            AddedList;



	typedef std::pair<std::string, std::string>  StringPair;
	typedef std::map<std::string, std::string>   KeyValMap;
	typedef KeyValMap::iterator                  KeyValMapItor;
	typedef KeyValMap::const_iterator            KeyValMapCItor;
	typedef std::pair<std::string, KeyValMap>    StringMapPair;
	typedef std::list<StringMapPair>             SectionList;
	typedef SectionList::iterator                SectionListItor;
	typedef SectionList::const_iterator          SectionListCItor;
}

using namespace INI;


class JE_ShopManager
{
public:
  void LoadTextureID(void);
  void ReadShopData(const char* fileName);
  void RewriteShopfile(char* shopfileName);

  SectionList* GetShopPartInfo(void);
  SectionList* GetShopActionInfo(void);

private:
  void DoSectionNameInShop(std::string& line, size_t& begin, size_t& end, SectionListItor& it);
  void DoKeyValueInShop(std::string& line, size_t& begin, size_t& end, SectionListItor& it);
  bool SectionTypeIs(std::string toMatch, std::string& line, size_t& begin, size_t& end);
  void SeperateKeyValue(std::string& line, size_t& begin, size_t& end, StringPair& sPair) const;

  SectionList m_shop_parts;
  SectionList m_shop_actions;


};


class JE_IniManager
{
public:
	JE_IniManager(void)
	{
		part='A';
	}


	//Read character inifile
	void ReadCharacter(const char* fileName);

	void IniAddParts(JE_ShopManager& shop, JE::Bone* pRoot, std::string partName, JE::BODY_PART type, std::string depth);
	bool GetAddedPartInfo(JE_ShopManager& shop, std::string partsName, Part& addPartFrame);
    void SelectParts(const char* libraryfile, int partsNumber, std::string partsName);
	void Rearrange(void);
	void RewriteFile(char* characterfileName);

	//Get
	SectionList* GetPartInfo(void);
	SectionList* GetActionInfo(void);


private:

	void DoSectionNameCharacter(std::string& line, size_t& begin, size_t& end, SectionListItor& it);
	void DoSectionNameLibrary(std::string& line, size_t& begin, size_t& end, SectionListItor& it, int partsNumber);
	void DoKeyValue(std::string& line, size_t& begin, size_t& end, SectionListItor& it);

	void SeperateKeyValue(std::string& line, size_t& begin, size_t& end, StringPair& sPair) const;

	bool SectionTypeIs(std::string toMatch, std::string& line, size_t& begin, size_t& end);
	bool FindSectionName(std::string string, std::string& line, size_t& begin, size_t& end);
	bool FindPartNameInAction(std::string string, std::string& line, size_t& begin, size_t& end);
	bool ActionNameIsAbout(std::string partName, std::string name);
	

	char part;
	int  numberOfParts;


	SectionList m_sections; //character parts list
	SectionList m_actions;  //action list
	SectionList m_garbage;  //other information

};






#endif