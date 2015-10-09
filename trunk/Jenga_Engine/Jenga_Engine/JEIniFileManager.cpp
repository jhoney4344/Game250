/*******************************************************************************
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

filename    JEIniFileManager.cpp
author      Nahye Park
email       skqla12@gmail.com
course      GAM200
due date    12/12/2014

Brief Description:
This file contains the function definition for InifileManager.

*******************************************************************************/
#include "JEIniFileManager.h"
#include "JELoadTextureNew.h"

#include <stdlib.h>

using namespace INI;


/*******************************************************************************
Function: IntToString

Description: Inline function that change the int to string.


Inputs: number - integer number which will be changed


Outputs: string - changed string
*******************************************************************************/
inline std::string IntToString(int number)
{
	std::ostringstream convertStream;

	convertStream << number;
	return convertStream.str();
}



////////READ



/*******************************************************************************
Function: ReadCharacter

Description: Read Character ini file and store the information to member value.
(to SectionList)

Inputs: fileName - filname to read


Outputs: None.
*******************************************************************************/
void JE_IniManager::ReadCharacter(const char* fileName)
{
	std::ifstream infile(fileName);

	if (!infile.is_open())
	{
		std::cout << "Ini file cannot be open" << std::endl;
		return;
	}

	std::string line;
	SectionListItor it;

	while (!infile.eof())
	{
		getline(infile, line);

		if (line.size() == 0)
			continue;

		size_t begin = 0;
		size_t end = line.size();

		//section name
		if (line[begin] == '[')
			DoSectionNameCharacter(line, begin, end, it);

		//key value
		else
			DoKeyValue(line, begin, end, it);

	}

	infile.close();
}


void JE_IniManager::IniAddParts(JE_ShopManager& shop, JE::Bone* pRoot, std::string partName, JE::PARTS type, std::string depth)
{
	numberOfParts = 0;

	std::string partNameToFind;

	for (size_t j = 0; j < partName.size(); ++j)
		partNameToFind.push_back(partName[j]);

	partNameToFind.push_back('-');

	

	Part add;
	while (GetAddedPartInfo(shop, partNameToFind + IntToString(numberOfParts), add))
	{
		depth += '#';

		pRoot = JE::BoneAddChild(pRoot, add.x, add.y, add.a, add.l, 0, const_cast<char*>(depth.c_str()), 
										const_cast<char*>((partNameToFind + IntToString(numberOfParts)).c_str()), type);
		

		pRoot->textureID = add.textureID;
		if (add.numberOfActions) //add animation
		{

			for (unsigned i = 0; i < add.numberOfActions; ++i)
			{
				pRoot->keyframeCount[i] = add.frame[i];
				for (unsigned j = 0; j < add.frame[i]; ++j)
				{
					pRoot->keyframe[i][j].angle = add.keyframe[i][j].angle;
					pRoot->keyframe[i][j].length = add.keyframe[i][j].length;
					pRoot->keyframe[i][j].time = add.keyframe[i][j].time;
					
				}
				
			}

		}

		++numberOfParts;

	}


}


bool JE_IniManager::GetAddedPartInfo(JE_ShopManager& shop, std::string partsName, Part& addPartFrame)
{

	bool result = false;
	SectionList* pShopPart = shop.GetShopPartInfo();
	SectionList* pShopAction = shop.GetShopActionInfo();

	SectionListItor it_character;


	for (SectionListItor it = pShopPart->begin(); it != pShopPart->end(); ++it)
	{

		//for (it_character = m_sections.begin(); it_character != m_sections.end(); ++it_character)
		//{
		// if (it_character->first == "Part" + IntToString(partsNumber + 1))
		//  break;
		//}

		///////////FIND BY pROOT NAME!!!!!!!!!!!!!!!!!!//////////////////


		if (it->first == partsName)
		{

			result = true;

			   //StringMapPair newPart;
			   //newPart.first = "Part" + part;
			   //newPart.second = it->second;
			   //StringPair nameOfNewPart;
			   //nameOfNewPart.first = "PartName";
			   //nameOfNewPart.second = it->first;

			//////////////////INSERT!!!!!!!!!!!!/////////////////
			//m_sections.insert(it_character, newPart);

			for (KeyValMapItor it_key = it->second.begin(); it_key != it->second.end(); ++it_key)
			{
				if (it_key->first == "X")
					addPartFrame.x = std::stof(it_key->second);
				if (it_key->first == "Y")
					addPartFrame.y = std::stof(it_key->second);
				if (it_key->first == "Length")
					addPartFrame.l = std::stof(it_key->second)/LENGTH_DECREASE_RATE;
				if (it_key->first == "Angle")
					addPartFrame.a = std::stof(it_key->second);
				if (it_key->first == "NumberOfActions")
					addPartFrame.numberOfActions = (unsigned)std::stoi(it_key->second);
				if (it_key->first == "TextureID")
					addPartFrame.textureID = (unsigned)std::stoi(it_key->second);


			}

		}
	}


	if (result)
	{

		for (SectionListItor it = pShopAction->begin(); it != pShopAction->end(); ++it)
		{
			std::string partInAction;
			std::string actionNum;

			int actionNumber;

			size_t j;
			for (j = 0; it->first[j] != '-'; ++j);
			++j;
			actionNum.push_back(it->first[j]);
			actionNumber = std::stoi(actionNum);

			j +=2;
			for (; j < it->first.size(); ++j)
				partInAction.push_back(it->first[j]);

			if (partInAction == partsName)
			{
				StringMapPair action;
				action.first = it->first;
				action.second = it->second;

				m_actions.push_back(action);


				for (KeyValMapItor it_key = it->second.begin(); it_key != it->second.end(); ++it_key)
				{

					//Frame* temp = 0;

					///////////KEY FRAME
					if (it_key->first == "NumberOfFrames")
					{
						addPartFrame.frame[actionNumber] = std::stoi(it_key->second);
						continue;
					}
					

					std::string keyType;
					std::string frame;
					int frameNumber;
					for (size_t k = 0; k < it_key->first.size() - 1; ++k)
						keyType.push_back(it_key->first[k]);
					frame.push_back(it_key->first[it_key->first.size() - 1]);

					frameNumber = std::stoi(frame);

					if (keyType == "Time")
						addPartFrame.keyframe[actionNumber][frameNumber].time = std::stoi(it_key->second)*ANIMATION_SPEED;
					if (keyType == "Angle")
						addPartFrame.keyframe[actionNumber][frameNumber].angle = std::stof(it_key->second);
					if (keyType == "Length")
						addPartFrame.keyframe[actionNumber][frameNumber].length = std::stof(it_key->second) / LENGTH_DECREASE_RATE;



				}


			}

		}

	}




	return result;
}


/*******************************************************************************
Function: SelectParts

Description: Find all parts that user choose.
Read library file to add the parts to character.
Store all parts information to member value.

Inputs: libraryFileName - Library file to get information about parts.
partsNumber - parts number which is added new parts.
sectionName - parts name to find in library file.

Outputs: None.
*******************************************************************************/
void JE_IniManager::SelectParts(const char* libraryfile, int partsNumber, std::string partsName)
{
	std::ifstream infile(libraryfile);

	if (!infile.is_open())
	{
		std::cout << "Library ini file cannot be open" << std::endl;
		return;
	}

	std::string line;
	SectionListItor it;
	std::string partNameToFind;

	numberOfParts = 0;

	for (size_t j = 0; j < partsName.size(); ++j)
		partNameToFind.push_back(partsName[j]);

	partNameToFind.push_back('-');

	while (!infile.eof())
	{
		getline(infile, line);

		if (line.size() == 0)
			continue;

		size_t begin = 0;
		size_t end = line.size();



		if (line[begin] == '[')
		{
			if (FindSectionName(partNameToFind + IntToString(numberOfParts), line, begin, end)
				|| FindPartNameInAction(partNameToFind + IntToString(numberOfParts), line, begin, end))
				DoSectionNameLibrary(line, begin, end, it, partsNumber);
			//else
			//	continue;
			//else
			//	it =  m_garbage.end();

		}

		else
		{
			//if(it !=  m_garbage.end())
			if (//it == std::next(m_actions.end(),-1)|| 
				it->first == partNameToFind + IntToString(numberOfParts - 1)
				|| ActionNameIsAbout(partNameToFind + IntToString(numberOfParts - 1), it->first))
				//it->first == "Part"+IntToString(partsNumber+1))
				DoKeyValue(line, begin, end, it);
			//else
			//	continue;
		}


	}

	infile.close();
	Rearrange();
}


void JE_IniManager::Rearrange(void)
{
	int partNumber = 0;
	std::string part;
	for (SectionListItor i = ++m_sections.begin(); i != m_sections.end(); ++i)
	{
		for (size_t j = 0; j < 4; ++j)
			part.push_back(i->first[j]);


		if (part != "Part")
		{
			StringPair key_value;
			key_value.first = "PartName";
			key_value.second = i->first;
			i->second.insert(key_value);
		}

		i->first = "Part" + IntToString(partNumber);
		++partNumber;
	}


	//	std::string checkPart;
	for (SectionListItor i = m_actions.begin(); i != m_actions.end(); ++i)
	{
		std::string actionPart_1;
		std::string actionPart_2;
		std::string namePartNumber;

		size_t j;
		//int count = 0;

		for (j = 0; i->first[j] != '/'; ++j)
		{
			//	if (i->first[j] == '/')
			//	break;

			actionPart_1.push_back(i->first[j]);
		}
		++j;
		for (; j < i->first.size(); ++j)
		{
			//			if (count < 4)
			//				checkPart.push_back(i->first[j]);

			actionPart_2.push_back(i->first[j]);
			//	++count;
		}

		///////////////
		//if (checkPart == "Part")
		//	continue;
		//////////////

		for (SectionListItor it_section = m_sections.begin(); it_section != m_sections.end(); ++it_section)
		{
			for (KeyValMapItor it_key = it_section->second.begin(); it_key != it_section->second.end(); ++it_key)
			{
				if (it_key->first == "PartName")
				{
					if (it_key->second == actionPart_2)
					{
						namePartNumber = it_section->first;
						break;
					}
				}
			}
		}//for

		i->first = actionPart_1;
		i->first.push_back('/');

		for (size_t k = 0; k < namePartNumber.size(); ++k)
			i->first.push_back(namePartNumber[k]);
	}
}


/*******************************************************************************
Function: RewriteFile

Description: Write a new character ini file using the saved data.


Inputs: characterFileName - character ini file to write.


Outputs: None.
*******************************************************************************/
void JE_IniManager::RewriteFile(char* characterfileName)
{
	//Rearrange();

	std::ofstream outfile;
	outfile.open(characterfileName, std::ofstream::out | std::ofstream::trunc);

	if (!outfile.is_open())
	{
		std::cout << "Character ini file cannot be open" << std::endl;
		return;
	}

	for (SectionListCItor it_section = m_sections.begin(); it_section != m_sections.end(); ++it_section)
	{
		outfile << '[' << it_section->first << "]\n";
		for (KeyValMapCItor it_value = it_section->second.begin(); it_value != it_section->second.end(); ++it_value)
			outfile << it_value->first << '=' << it_value->second << std::endl;
	}

	for (SectionListCItor it_action = m_actions.begin(); it_action != m_actions.end(); ++it_action)
	{
		outfile << '[' << it_action->first << "]\n";
		for (KeyValMapCItor it_value = it_action->second.begin(); it_value != it_action->second.end(); ++it_value)
			outfile << it_value->first << '=' << it_value->second << std::endl;
	}

	for (SectionListCItor it_garbage = m_garbage.begin(); it_garbage != m_garbage.end(); ++it_garbage)
	{
		outfile << '[' << it_garbage->first << "]\n";
		for (KeyValMapCItor it_value = it_garbage->second.begin(); it_value != it_garbage->second.end(); ++it_value)
			outfile << it_value->first << '=' << it_value->second << std::endl;
	}

}



//////////////////////////////////////////private

void JE_IniManager::DoSectionNameCharacter(std::string& line, size_t& begin, size_t& end, SectionListItor& it)
{


	if (line[begin] != '[')
		return;

	std::string section;
	for (size_t i = begin + 1; line[i] != ']'; ++i)
		section.push_back(line[i]);

	StringMapPair mapPair(section, KeyValMap());

	//action
	if (SectionTypeIs("Action", line, begin, end))
	{
		m_actions.push_back(mapPair);
		it = std::next(m_actions.end(), -1);
	}
	//garbage
	else if (SectionTypeIs("Spot", line, begin, end) || SectionTypeIs("Line", line, begin, end))
	{
		m_garbage.push_back(mapPair);
		it = std::next(m_garbage.end(), -1);
	}
	//parts
	else
	{
		m_sections.push_back(mapPair);
		it = std::next(m_sections.end(), -1);
	}


}
void JE_IniManager::DoSectionNameLibrary(std::string& line, size_t& begin, size_t& end, SectionListItor& it, int partsNumber)
{
	if (line[begin] != '[')
		return;

	std::string section;
	for (size_t i = begin + 1; line[i] != ']'; ++i)
		section.push_back(line[i]);

	StringMapPair mapPair(section, KeyValMap());

	if (SectionTypeIs("Action", line, begin, end))
	{
		m_actions.push_back(mapPair);
		it = std::next(m_actions.end(), -1);
	}
	else
	{
		for (it = m_sections.begin(); it != m_sections.end(); ++it)
		{
			if (it->first == "Part" + IntToString(partsNumber + 1))
				break;
		}

		m_sections.insert(it, mapPair);

		for (it = m_sections.begin(); it != m_sections.end(); ++it)
		{
			if (it->first == section)
				break;
		}

		++numberOfParts;
	}




}

void JE_IniManager::DoKeyValue(std::string& line, size_t& begin, size_t& end, SectionListItor& it)
{
	StringPair key_value;
	SeperateKeyValue(line, begin, end, key_value);


	if (!key_value.first.empty() && !key_value.second.empty())
		(*it).second.insert(key_value);

}

void JE_IniManager::SeperateKeyValue(std::string& line, size_t& begin, size_t& end, StringPair& sPair) const
{
	size_t i;
	for (i = begin; line[i] != '='; i++)
		sPair.first.push_back(line[i]);

	++i;
	for (; i< end; i++)
		sPair.second.push_back(line[i]);
}

bool JE_IniManager::SectionTypeIs(std::string toMatch, std::string& line, size_t& begin, size_t& end)
{
	if (line[begin] != '[')
		return false;

	std::string newString;

	for (size_t i = begin + 1; line[i] != ']'; ++i)
	{
		if (line[i] == '-')
			break;

		newString.push_back(line[i]);
	}
	end;
	return (toMatch == newString);
}

bool JE_IniManager::FindSectionName(std::string string, std::string& line, size_t& begin, size_t& end)
{
	if (line[begin] != '[')
		return false;

	std::string newString;

	for (size_t i = begin + 1; line[i] != ']'; ++i)
		newString.push_back(line[i]);

	end;
	return (string == newString);
}

bool JE_IniManager::FindPartNameInAction(std::string string, std::string& line, size_t& begin, size_t& end)
{
	if (line[begin] != '[' || !SectionTypeIs("Action", line, begin, end))
		return false;

	std::string newString;
	size_t j;
	for (j = begin + 1; line[j] != '/'; j++);

	j++;
	for (; line[j] != ']'; j++)
		newString.push_back(line[j]);

	return (newString == string);
}

bool JE_IniManager::ActionNameIsAbout(std::string partName, std::string name)
{
	std::string toCompare;
	size_t i;
	for (i = 0; i < name.size(); ++i)
	{
		if (name[i] == '/')
			break;
	}

	if (i == name.size())
		return 0;

	++i;
	for (; i < name.size(); ++i)
		toCompare.push_back(name[i]);

	return (toCompare == partName);
}

SectionList* JE_IniManager::GetPartInfo(void)
{
	return &m_sections;
}
SectionList* JE_IniManager::GetActionInfo(void)
{
	return &m_actions;
}







///*******************************************************************************
//   Function: DoKeyValueInLibrary
//
//Description: Seperate key value in library file and save that data into section
//             list. Add the depth and modify the angle for adding parts.
//
//     Inputs: line - line that has key and value.
//             partsNumber - parts number which will be added new parts.
//			 sectionName - section name to find and add the key and value.
//			 begin - line begin.
//			 end - line end.
//
//    Outputs: None.
//*******************************************************************************/
//void JE_IniManager::DoKeyValueInLibrary(std::string line,int partsNumber,std::string sectionName, size_t& begin, size_t& end)
//{
//	SectionListItor i;
//	FindSectionInList(i, sectionName);
//
//	if(i == m_sections.end())
//		return;
//
//	StringPair sPair;
//
//	SeperateKeyValueFromLine(sPair, line, begin, end);
//
//	
//
//	Add #
//	if(sPair.first == "Depth")
//	{
//		std::string prevString = "Part"+ IntToString(partsNumber);
//		SectionListItor it_prev;
//		FindSectionInList(it_prev, prevString);
//
//		std::string hash;
//
//		if(it_prev->second.find("Depth") != it_prev->second.end() )
//		{
//			KeyValMapItor it_depth = (*it_prev).second.find("Depth");
//			std::string depth = (*it_depth).second;
//			size_t end_hash = depth.size();
//
//			for(size_t j = 0 ; j < end_hash ; j++)
//				hash.push_back( (*it_depth).second[j] );
//		}
//		hash.push_back('#');
//
//		sPair.second.clear();
//		sPair.second.assign(hash);
//	}
//	
//	if(sPair.first == "Angle")
//	{
//		std::string prevString = "Part"+ IntToString(partsNumber);
//		SectionListItor it_prev;
//		FindSectionInList(it_prev, prevString);
//
//		std::string angle;
//		
//		if(it_prev->second.find("NormAngle") != it_prev->second.end() )
//		{
//			KeyValMapItor it_angle = (*it_prev).second.find("NormAngle");
//			std::string normAngle = (*it_angle).second;
//			size_t end_hash = normAngle.size();
//
//			for(size_t j = 0 ; j < end_hash ; j++)
//				angle.push_back( (*it_angle).second[j] );
//
//			double norm_angle = atof(angle.c_str());
//			double _angle = atof(sPair.second.c_str());
//
//			if(_angle > 1000)
//			{
//				_angle -= 1000;
//				_angle *= -1;
//			}
//
//			_angle += norm_angle;
//
//			if(_angle < 0)
//			{
//				_angle *= -1;
//				_angle += 1000;
//			}
//			
//
//			angle.clear();
//			angle.assign(DoubleToString(_angle));
//		}
//		sPair.second.clear();
//		sPair.second.assign(angle);
//
//	}
//
//
//	(*i).second.insert(sPair);
//
//}

///*******************************************************************************
//   Function: DoKeyValueInLibrary
//
//Description: Seperate key value in library file and save that data into section
//             list. Add the depth and modify the angle for adding parts.
//
//     Inputs: line - line that has key and value.
//             partsNumber - parts number which will be added new parts.
//			 partName - parts name to paste.
//			 sectionName - section name to find and add the key and value.
//			 begin - line begin.
//			 end - line end.
//
//    Outputs: None.
//*******************************************************************************/
//void JE_IniManager::DoKeyValueInLibrary(std::string line, int partsNumber,std::string partName ,std::string sectionName,size_t& begin, size_t& end)
//{
//	SectionListItor i;
//	FindSectionInList(i, sectionName);
//
//	if(i == m_sections.end())
//		return;
//
//	StringPair sPair;
//
//	SeperateKeyValueFromLine(sPair, line, begin, end);
//
//	//Add #
//	if(sPair.first == "Depth")
//	{
//		//std::string prevString = "Part"+ IntToString(partsNumber);
//		SectionListItor it_prev;
//		KeyValMapItor it_prev_key;
//		//FindSectionInList(it_prev, prevString);
//		 
//
//		std::string hash;
//
//		if(FindPartsName(it_prev, it_prev_key, partName) && it_prev->second.find("Depth") != it_prev->second.end() )
//		{
//			KeyValMapItor it_depth = (*it_prev).second.find("Depth");
//			std::string depth = (*it_depth).second;
//			size_t end_hash = depth.size();
//
//			for(size_t j = 0 ; j < end_hash ; j++)
//				hash.push_back( (*it_depth).second[j] );
//		}
//		hash.push_back('#');
//
//		sPair.second.clear();
//		sPair.second.assign(hash);
//	}
//
//	if(sPair.first == "Angle")
//	{
//		std::string prevString = "Part"+ IntToString(partsNumber);
//		SectionListItor it_prev;
//		FindSectionInList(it_prev, prevString);
//
//		std::string angle;
//		
//		if(it_prev->second.find("NormAngle") != it_prev->second.end() )
//		{
//			KeyValMapItor it_angle = (*it_prev).second.find("Depth");
//			std::string normAngle = (*it_angle).second;
//			size_t end_hash = normAngle.size();
//
//			for(size_t j = 0 ; j < end_hash ; j++)
//				angle.push_back( (*it_angle).second[j] );
//
//			double norm_angle = atof(angle.c_str());
//			double _angle = atof(sPair.second.c_str());
//
//						if(_angle > 1000)
//			{
//				_angle -= 1000;
//				_angle *= -1;
//			}
//
//			_angle += norm_angle;
//
//			if(_angle < 0)
//			{
//				_angle *= -1;
//				_angle += 1000;
//			}
//			angle.clear();
//			angle.assign(DoubleToString(_angle));
//		}
//		sPair.second.clear();
//		sPair.second.assign(angle);
//
//	}
//
//
//
//
//	(*i).second.insert(sPair);
//
//}


//////////////////////SHOP
void JE_ShopManager::LoadTextureID(void)
{


	int partID;
	for (SectionListItor i = m_shop_parts.begin(); i != m_shop_parts.end(); ++i)
	{

		for (KeyValMapItor it_key = i->second.begin(); it_key != i->second.end(); ++it_key)
		{
			if (it_key->first == "TextureName")
			{
				std::string part = "Texture//";

				for (size_t j = 0; j < it_key->second.size(); ++j)
					part.push_back(it_key->second[j]);
				
				part.push_back('.');
				part.push_back('t');
				part.push_back('g');
				part.push_back('a');

				char* cstr = new char[part.length() + 1];
				strcpy_s(cstr, part.length() + 1, part.c_str());

				partID = JE::GraphicLoadTexture(cstr);

				StringPair textureID;
				textureID.first = "TextureID";
				textureID.second = IntToString(partID);

				i->second.insert(textureID);

				delete[] cstr;

				break;
			}

		}

	}

}

void JE_ShopManager::ReadShopData(const char* fileName)
{
	std::ifstream infile(fileName);

	if (!infile.is_open())
	{
		std::cout << "Ini file cannot be open" << std::endl;
		return;
	}

	std::string line;
	SectionListItor it;

	while (!infile.eof())
	{
		getline(infile, line);

		if (line.size() == 0)
			continue;

		size_t begin = 0;
		size_t end = line.size();

		//section name
		if (line[begin] == '[')
			DoSectionNameInShop(line, begin, end, it);

		//key value
		else
			DoKeyValueInShop(line, begin, end, it);

	}

	infile.close();
}


void JE_ShopManager::RewriteShopfile(char* shopfileName)
{
	std::ofstream outfile;
	outfile.open(shopfileName, std::ofstream::out | std::ofstream::trunc);

	if (!outfile.is_open())
	{
		std::cout << "Shop ini file cannot be open" << std::endl;
		return;
	}

	for (SectionListCItor it_section = m_shop_parts.begin(); it_section != m_shop_parts.end(); ++it_section)
	{
		outfile << '[' << it_section->first << "]\n";
		for (KeyValMapCItor it_value = it_section->second.begin(); it_value != it_section->second.end(); ++it_value)
			outfile << it_value->first << '=' << it_value->second << std::endl;
	}

	for (SectionListCItor it_action = m_shop_actions.begin(); it_action != m_shop_actions.end(); ++it_action)
	{
		outfile << '[' << it_action->first << "]\n";
		for (KeyValMapCItor it_value = it_action->second.begin(); it_value != it_action->second.end(); ++it_value)
			outfile << it_value->first << '=' << it_value->second << std::endl;
	}

}

void JE_ShopManager::DoSectionNameInShop(std::string& line, size_t& begin, size_t& end, SectionListItor& it)
{
	if (line[begin] != '[')
		return;

	std::string section;
	for (size_t i = begin + 1; line[i] != ']'; ++i)
		section.push_back(line[i]);

	StringMapPair mapPair(section, KeyValMap());

	//action
	if (SectionTypeIs("Action", line, begin, end))
	{
		m_shop_actions.push_back(mapPair);
		it = std::next(m_shop_actions.end(), -1);
	}
	//parts
	else
	{
		m_shop_parts.push_back(mapPair);
		it = std::next(m_shop_parts.end(), -1);
	}

}
void JE_ShopManager::DoKeyValueInShop(std::string& line, size_t& begin, size_t& end, SectionListItor& it)
{
	StringPair key_value;
	SeperateKeyValue(line, begin, end, key_value);


	if (!key_value.first.empty() && !key_value.second.empty())
		(*it).second.insert(key_value);
}

bool JE_ShopManager::SectionTypeIs(std::string toMatch, std::string& line, size_t& begin, size_t& end)
{
	if (line[begin] != '[')
		return false;

	std::string newString;

	for (size_t i = begin + 1; line[i] != ']'; ++i)
	{
		if (line[i] == '-')
			break;

		newString.push_back(line[i]);
	}
	end;
	return (toMatch == newString);
}

void JE_ShopManager::SeperateKeyValue(std::string& line, size_t& begin, size_t& end, StringPair& sPair) const
{
	size_t i;
	for (i = begin; line[i] != '='; i++)
		sPair.first.push_back(line[i]);

	++i;
	for (; i< end; i++)
		sPair.second.push_back(line[i]);
}

SectionList* JE_ShopManager::GetShopPartInfo(void)
{
	return &m_shop_parts;
}
SectionList* JE_ShopManager::GetShopActionInfo(void)
{
	return &m_shop_actions;
}