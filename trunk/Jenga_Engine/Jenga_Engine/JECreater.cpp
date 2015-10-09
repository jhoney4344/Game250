#include "JECreater.h"

namespace JE
{

WingCreater::WingCreater(int _ID, PARTS _type)
{
	ID = _ID;
	type = _type;
}
WingCreater::~WingCreater(void)
{
}
//BodyPart* WingCreater::Create(void) const
//{	
//	return new Wing(ID,type);
//	//return new Wing;
//}
int WingCreater::GetID()
{
	return ID;
}
PARTS WingCreater::GetPart()
{
	return type;
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
ArmCreater::ArmCreater(int _ID, PARTS _type)
{
	ID = _ID;
	type = _type;
}
ArmCreater::~ArmCreater(void)
{
}
BodyPart* ArmCreater::Create(void) const
{
	return new Arm(ID,type);
	//return new Arm;
}
int ArmCreater::GetID()
{
	return ID;
}
PARTS ArmCreater::GetPart()
{
	return type;
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
LegCreater::LegCreater(int _ID, PARTS _type)
{
	ID = _ID;
	type = _type;
}
LegCreater::~LegCreater(void)
{
}
BodyPart* LegCreater::Create(void) const
{
	return new Leg(ID,type);
}
int LegCreater::GetID()
{
	return ID;
}
PARTS LegCreater::GetPart()
{
	return type;
}

}