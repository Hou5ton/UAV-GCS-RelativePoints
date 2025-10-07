//PACKAGE : MAP.
//FILE: Layer.h   
//AUTHOR:
//DESCRIPTION:

#ifndef MLAYER_H
#define MLAYER_H
#include <list>
#include <algorithm>
#include <QMap>
#include <QVector>
#include "Array.h"
#include "Topobject.h"
#include "Nadp.h"
#include "MString.h"

// STring constants

// If English version
#if ENGVERSION == 1
	#define IDS_LAYER "Layer"
	#define IDS_ORDINARY "Ordinary"

// If Russian version
#else
	#define IDS_LAYER "слой"
	#define IDS_ORDINARY "обыкновенный"
#endif

// String of constant for saving non existable active layer
#define ID_WRITE_NO_NAME ""

class MKlassifikator; // pointer to MKlassifikator is used in this file
class MTopobject;     // pointer to MTopobject is used in this file

//PACKAGE : MAP.
//CLASS	: MLayer.
//DESCRIPTION : Class layer, which describes usual layer of topographic map.
class MLayer
{
public:

//PACKAGE : MAP.
//FUNCTION : MLayer::MLayer();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MLayer();

//PACKAGE : MAP.
//FUNCTION : MLayer::MLayer(MString key);
//DESCRIPTION : Destructor.
//INPUTS : MString.
//RETURNS : NONE.
	MLayer(MString key);

public:

	//  Saveable
	//  Name
	MString     m_name;			// < Name
	MString     m_sort;			// < Type
	MString     m_remark;		// < Comment
	MString		m_password;		// < Password
	double		m_UID;			// < Unique number of layer
	unsigned int m_editstat;	// < Status of layer edition
	int         m_trans_type;	// < Transparency type(gradation from 0 to 14) [priznak prozra4nosti]

	//  main
	MObjectList m_oblist;		// <  list of objects
	MRect       m_bounds;		// <  Bounding rectangle of layer
	MStArray    m_dbarray;		// <  Array of strings data

	// flags
	bool        x_visible;			// <  flag of representative
	bool        x_textvisible;		// <  flag of inscriptions represtative
	bool        x_common;			// <  flag of apply/disapply of characteristic of layer at drawing
	MColor		m_commoncolor_pen;	// <  line color for simplified drawing
	MColor		m_commoncolor_brush;// <  background color for simplified drawing
    int		    m_commoncolor_brushTransparent;// <  transparent for simplified drawing
    int		    m_commoncolor_penTransparent;// <  transparent for simplified drawing
	MColor		m_commoncolor_text;	// <  text color for simplified drawing
	int			m_commonpenwidth;	// <  line thickness (in logocal units) for simplified drawing
	int			m_commontextwidth;	// <  text size (in logical units) for simplified drawing
	bool        m_bDrawSmall;

	//  non saveable
	MKarta*       m_pKarta;			// < map, to which posesses a layer
	bool m_bOperLayer;				// < sign of active layer
	MString m_type;					// < layer type
	MString m_sheetname;			// < color name

	bool m_bSearchIn;				// < sign of layer to Search In (for city find function)

public:

//PACKAGE : MAP.
//FUNCTION : MLayer::MLayer(const MRect& position);
//DESCRIPTION : Constructor.
//INPUTS : const MRect& .
//RETURNS : NONE.
	MLayer(const MRect& position);

//PACKAGE : MAP.
//FUNCTION : MLayer::~MLayer();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MLayer();

//PACKAGE : MAP.
//FUNCTION : MLayer::Copy(MLayer* pLay);
//DESCRIPTION : Function of copying.
//INPUTS : MLayer pointer.
//RETURNS : NONE.
	void Copy(MLayer* pLay);

//PACKAGE : MAP.
//FUNCTION : MLayer::GetKarta();
//DESCRIPTION : Gets pointer to map of owner.
//INPUTS : NONE.
//RETURNS : MKarta pointer.
	MKarta* GetKarta() {return m_pKarta;};

//PACKAGE : MAP.
//FUNCTION : MLayer::GetPassportBytes();
//DESCRIPTION : Layer passport size in UTP.
//INPUTS : NONE.
//RETURNS : unsigned short.
	virtual unsigned short GetPassportBytes();

//PACKAGE : MAP.
//FUNCTION : MLayer::MasterRead(FILE* h_nuxfile,  int version);
//DESCRIPTION : Function of reading.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	bool		 MasterRead(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MLayer::MasterRead(MFile* file,  int version);
//DESCRIPTION : Function of reading.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	bool		 MasterRead(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MLayer::MasterRead(FILE* h_nuxfile,  int version);
//DESCRIPTION : Function of reading.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MLayer::MasterRead(MFile* file,  int version);
//DESCRIPTION : Function of reading.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MLayer::MasterWrite(FILE* h_nuxfile,  int version);
//DESCRIPTION : Function of writing.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	bool		 MasterWrite(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MLayer::MasterWrite(FILE* h_nuxfile,  int version);
//DESCRIPTION : Function of writing.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,  int version); 


//  Operations

//PACKAGE : MAP.
//FUNCTION : MLayer::ConnectKlassifikator(MKlassifikator* pKls);
//DESCRIPTION : Attachs to another classifier.
//INPUTS : MKlassifikator pointer.
//RETURNS : NONE.
	virtual void ConnectKlassifikator(MKlassifikator* pKls);

//PACKAGE : MAP.
//FUNCTION : MLayer::AddObjectsInObjectListByQuadrakod(MObjectList* oblist,MString quadrakod);
//DESCRIPTION : Adds to 'oblist' objects with defined quadrant.
//INPUTS : MObjectList pointer,MString .
//RETURNS : NONE.
	virtual void AddObjectsInObjectListByQuadrakod(MObjectList* oblist,MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : MLayer::DeleteFormulaPrivs(int formnomer, MString quadrakod);
//DESCRIPTION : Deletes vectors and splines of formula snaps with number 'formnomer' from objects of layer with defined quadrocode.
//INPUTS : int , MString .
//RETURNS : True.
	bool		 DeleteFormulaPrivs(int formnomer, MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : MLayer:: ChangeFormulaNomer(int oldformnom, int newformnom, MString quadrakod);
//DESCRIPTION : Changes number of formula 'oldformnom' in arrays of vectors and splines of formulas snaps of number 'newformnom'
//		 for objects of layer with defined quadrocode.
//INPUTS : int , int , MString .
//RETURNS : True.
	bool		 ChangeFormulaNomer(int oldformnom, int newformnom, MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : MLayer::IsObject(MObject* pObj);
//DESCRIPTION : Searchs for such object in layer.
//INPUTS : MObject pointer.
//RETURNS : True.
	virtual bool IsObject(MObject* pObj);

//PACKAGE : MAP.
//FUNCTION : MLayer::ReplaceObject(MObject* pOldObj, MObject* pNewObj);
//DESCRIPTION : Changes all pointers 'pOldObj' to 'pNewObj' in list.
//INPUTS : MObject pointer, MObject pointer.
//RETURNS : NONE.
	virtual void ReplaceObject(MObject* pOldObj, MObject* pNewObj);

//PACKAGE : MAP.
//FUNCTION : MLayer::GetObjectList(MObjectList* oblist);
//DESCRIPTION : Creates list of objects which are belong to relief layer
//				Result moves to 'oblist'.
//INPUTS : MObjectList pointer.
//RETURNS : NONE.
	virtual void GetObjectList(MObjectList* oblist);

//PACKAGE : MAP.
//FUNCTION : MLayer::CreateObjectList();
//DESCRIPTION : Returns list of objects - redefined for inheritors - collects
//				objects from all lists of 'm_oblist'.
//INPUTS : NONE.
//RETURNS : MObjectList pointer.
	virtual MObjectList* CreateObjectList(){return &m_oblist;};

//PACKAGE : MAP.
//FUNCTION : MLayer::CreateObjectListByObjType(short int iObjType);
//DESCRIPTION : Creates list of objects of layer, depended on choosed objects type.
//INPUTS : short int.
//RETURNS : MObjectList pointer.
	virtual MObjectList* CreateObjectListByObjType(short int iObjType);

//PACKAGE : MAP.
//FUNCTION : MLayer::FindObjectsAtPoint(MPoint findpt,MObjectList* findlist);
//DESCRIPTION : Finds and gets list of objects which located at defined point. to operational active layer.
//INPUTS : MPoint ,MObjectList pointer.
//RETURNS : NONE.
	virtual void FindObjectsAtPoint(MPoint findpt,MObjectList* findlist);

//PACKAGE : MAP.
//FUNCTION : MLayer::FindObjectsAtRect(MRect& findrec,MObjectList* findlist);
//DESCRIPTION : Creates list of objects at defined point.
//INPUTS : MRect& ,MObjectList pointer.
//RETURNS : NONE.
	virtual void FindObjectsAtRect(MRect& findrec,MObjectList* findlist);

//PACKAGE : MAP.
//FUNCTION : MLayer::FindObjectsAtPointExact(MPoint findpt,MObjectList* findlist, double delta = 0);
//DESCRIPTION : Creates list of objects in the defined point. More exact, but long search.
//INPUTS : MPoint ,MObjectList pointer, double .
//RETURNS : NONE.
	virtual void FindObjectsAtPointExact(MPoint findpt,MObjectList* findlist, double delta = 0);

//PACKAGE : MAP.
//FUNCTION : MLayer::MObject* FindObject(double uid);
//DESCRIPTION : Searchs for object by identifier.
//INPUTS : double.
//RETURNS : MObject pointer.
	virtual MObject* FindObject(double uid);

//PACKAGE : MAP.
//FUNCTION : MLayer::FindOblistByAttrValue(unsigned short int m_parAttrNumber, MString m_parValue);
//DESCRIPTION : Searches for topographical object by defined value (m_parValue) 
// 				of defined attribute (m_parAttrNumber).
//INPUTS : unsigned short int , MString .
//RETURNS : MObjectList pointer.	
	virtual MObjectList* FindOblistByAttrValue(unsigned short int m_parAttrNumber, MString m_parValue);

//PACKAGE : MAP.
//FUNCTION : MLayer::CreateCopyForUndo(MLayer* pLay);
//DESCRIPTION : Copies it's own members to 'pLay' layer
// 				To the list of objects are copied only pointers, instead of objects.
//INPUTS : MLayer pointer.
//RETURNS : NONE.
	virtual void CreateCopyForUndo(MLayer* pLay);

//PACKAGE : MAP.
//FUNCTION : MLayer::ClearObjectList();
//DESCRIPTION :  Clears the list of objects, but does not delete objects.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void ClearObjectList();

//PACKAGE : MAP.
//FUNCTION : MLayer::GetObjectCount();
//DESCRIPTION :  Gets quantity of objects of the map.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetObjectCount();

//PACKAGE : MAP.
//FUNCTION : MLayer::DeleteObjects();
//DESCRIPTION : Deletes all objects of layer.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void DeleteObjects();

//PACKAGE : MAP.
//FUNCTION : MLayer::GetInfoForShpWrite(MStArray& names,MInArray* dlina);
//DESCRIPTION : Returns information for saving to SHP-file.
//INPUTS : MStArray& ,MInArray pointer.
//RETURNS : NONE.
	virtual void GetInfoForShpWrite(MStArray& names,MInArray* dlina);

//PACKAGE : MAP.
//FUNCTION : MLayer::Transliter();
//DESCRIPTION : Transliteration.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void Transliter();

//PACKAGE : MAP.
//FUNCTION : MLayer::DaiShades(MLayer* shadelayer, MString pole);
//DESCRIPTION : Decantion of "shadows" from layer.
//INPUTS : MLayer pointer, MString .
//RETURNS : NONE.
	virtual void DaiShades(MLayer* shadelayer, MString pole);

//PACKAGE : MAP.
//FUNCTION : MLayer::SetKvadrokodByDbarray(MString QQ, MStArray* dbarrfieldvalues);
//DESCRIPTION : Attachs quadrocode to objects which fields of
// 		'm_dbarray' have defined values.
//INPUTS : MString , MStArray pointer.
//RETURNS : NONE.
	virtual void SetKvadrokodByDbarray(MString QQ, MStArray* dbarrfieldvalues);

//PACKAGE : MAP.
//FUNCTION : MLayer::SetDataByDbarray(MInArray* dbindex, MShInArray* dtnoms);
//DESCRIPTION : Imports values from 'm_dbarray' to the field of data.
//INPUTS : MInArray pointer, MShInArray pointer.
//RETURNS : NONE.
	virtual void SetDataByDbarray(MInArray* dbindex, MShInArray* dtnoms);

//PACKAGE : MAP.
//FUNCTION : MLayer::FindNearestObject(MPoint findpt);
//DESCRIPTION :  Finds map object which is nearest to the defined point.
//INPUTS : MPoint.
//RETURNS : MObject pointer.
	virtual MObject* FindNearestObject(MPoint findpt);

//PACKAGE : MAP.
//FUNCTION : MLayer::FindNearestObjectIntersectWithLine(MPoint linept1, MPoint linept2, MPoint& resultpt, double& rassto);
//DESCRIPTION : Finds nearest topobject and it's point of intersection of Our
//				topobject with straight line, defined by two points linept1, linept2.
//				If our topobject is crossed by straight line in couple places,
//				the nearest to a point linept2 point gets out of all points of intersection.
//				Calculated point of intersection is placed in resultpt.
//				Calculated distance in rassto..
//INPUTS : MPoint , MPoint , MPoint& , double& .
//RETURNS : MObject pointer.
	virtual MObject* FindNearestObjectIntersectWithLine(MPoint linept1, MPoint linept2, MPoint& resultpt, double& rassto);

//PACKAGE : MAP.
//FUNCTION : MLayer::ConvertToMaket();
//DESCRIPTION : Converts to model.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void ConvertToMaket();

//PACKAGE : MAP.
//FUNCTION : MLayer::ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo);
//DESCRIPTION : Converts to another coordinate system.
//INPUTS : MCoo pointer, MCoo pointer.
//RETURNS : NONE.
	virtual void ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo);

//PACKAGE : MAP.
//FUNCTION : MLayer::GetBounds();
//DESCRIPTION : Gets bounding rectangle.
//INPUTS : NONE.
//RETURNS : MRect pointer.
	MRect*    GetBounds()                    { return &m_bounds; }

//PACKAGE : MAP.
//FUNCTION : MLayer::SetBounds(MRect* newbounds);
//DESCRIPTION : Sets bounding rectangle.
//INPUTS : MRect pointer.
//RETURNS : NONE.
	void      SetBounds(MRect* newbounds)    { m_bounds = newbounds; }

//PACKAGE : MAP.
//FUNCTION : MLayer::AddObject(MObject* pObj, bool bMinimize = true);
//DESCRIPTION : Adds object.
//INPUTS : MObject pointer, bool .
//RETURNS : NONE.	
	virtual void      AddObject(MObject* pObj, bool bMinimize = true);

//PACKAGE : MAP.
//FUNCTION : MLayer::RemoveObject(MObject* pObj);
//DESCRIPTION : Deletes object.
//INPUTS : MObject pointer.
//RETURNS : NONE.
	virtual void      RemoveObject(MObject* pObj);

//PACKAGE : MAP.
//FUNCTION : MLayer::SetKey(MString key);
//DESCRIPTION :  Sets layer name.
//INPUTS : MString.
//RETURNS : NONE.
	void      SetKey(MString key){m_name = key;}

//PACKAGE : MAP.
//FUNCTION : MLayer::SetVisible(bool visible);
//DESCRIPTION : Sets layer visibility.
//INPUTS : bool.
//RETURNS : NONE.
	void      SetVisible(bool visible){x_visible = visible;}

//PACKAGE : MAP.
//FUNCTION : MLayer::SetCommon(bool common);
//DESCRIPTION : Sets flag of attachment/non attachment of characteristics of layer at drawing.
//INPUTS : bool.
//RETURNS : NONE.
	void      SetCommon(bool common){x_common = common;}

//PACKAGE : MAP.
//FUNCTION : MLayer::SetTextStr(MString str);
//DESCRIPTION : Sets text to string of DB-array.
//INPUTS : MString.
//RETURNS : NONE.
	void	  SetTextStr(MString str) {*m_dbarray[ID_TXT] = str;}

//PACKAGE : MAP.
//FUNCTION : MLayer::SetTransType(int type);
//DESCRIPTION : Gets sign of transparency.
//INPUTS : int.
//RETURNS : NONE.
	void	SetTransType(int type);

//PACKAGE : MAP.
//FUNCTION : MLayer::Transliter();
//DESCRIPTION : Transliteration.
//INPUTS : NONE.
//RETURNS : NONE.
	int		GetTransType() {return m_trans_type;}

//PACKAGE : MAP.
//FUNCTION : MLayer::MinimizeBounds();
//DESCRIPTION : Minimizes bounding layer of rectangle.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void	  MinimizeBounds();

//PACKAGE : MAP.
//FUNCTION : MLayer::Draw(MPlotDevice* pDevice,MRect geodrawrect);
//DESCRIPTION : Draws layer objects.
//INPUTS : MPlotDevice pointer,MRect .
//RETURNS : NONE.
	virtual void      Draw(MPlotDevice* pDevice,MRect geodrawrect);

//PACKAGE : MAP.
//FUNCTION : MLayer::Draw1(MPlotDevice* pDevice,MRect geodrawrect);
//DESCRIPTION : Simplified drawing of layer objects.
//INPUTS : MPlotDevice pointer,MRect .
//RETURNS : NONE.
	virtual void	  Draw1(MPlotDevice* pDevice,MRect geodrawrect);

//PACKAGE : MAP.
//FUNCTION : MLayer::DrawText(MPlotDevice* pDevice,MRect geodrawrect);
//DESCRIPTION : Text drawing at layer objects.
//INPUTS : MPlotDevice pointer,MRect .
//RETURNS : NONE.
	virtual void      DrawText(MPlotDevice* pDevice,MRect geodrawrect);

//PACKAGE : MAP.
//FUNCTION : MLayer::WhatTheLayer();
//DESCRIPTION : Returns layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual   MString WhatTheLayer() { return ID_SIMPLE_LAYER;}

//PACKAGE : MAP.
//FUNCTION : MLayer::FindCrosslinesWithRoads(MPtArray* plankontur, MPtArray* profar, MStArray* kodar, MVoidArray* pRoadObjects, MPtArray* Roadcoord, MPtArray* Roadheight);
//DESCRIPTION : Function finds all intersection of gas pipeline with roads.
//INPUTS : MPtArray pointer, MPtArray pointer, MStArray pointer, MVoidArray pointer, MPtArray pointer, MPtArray pointer.
//RETURNS : NONE.
	bool	  FindCrosslinesWithRoads(MPtArray* plankontur, MPtArray* profar, MStArray* kodar, MVoidArray* pRoadObjects,
									 MPtArray* Roadcoord, MPtArray* Roadheight);
	
#ifdef _DEBUG
	void AssertValid();
#endif
};

//PACKAGE : MAP.
//CLASS : MLayerDrawFilter.
//DESCRIPTION : 
class MLayerDrawFilter
{
public:

//PACKAGE : MAP.
//FUNCTION : MLayerDrawFilter::MLayerDrawFilter();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MLayerDrawFilter(){};
	
    QMap<QString,QVector<int> > layerMap;

    bool SetFilterScale(QString &layerName, int minScale, int maxScale)
	{
        QVector<int> tmp(0);
        tmp.append(minScale);
        tmp.append(maxScale);

        if(!layerName.isEmpty ())
		{
            layerMap[layerName] = tmp;
			return true;
		}
		else
			return false;
	}
   bool SetLayerMap(QMap<QString, QVector<int> > &layerMap)
   {
	   if(!layerMap.isEmpty () )
	   {
	   this->layerMap = layerMap;
	   return true;
	   }
	   return false;
   }

};

//PACKAGE : MAP.
//CLASS : MLayerList.
//DESCRIPTION : List of layers (STD- library)
class MLayerList:public std::list<MLayer*>{};


// Direct iterator through list of layers
typedef std::list<MLayer*>::iterator LAYITER;

// Direct const iterator through list of layers
typedef std::list<MLayer*>::const_iterator ConstLAYITER;

// Reverse iterator through list of layers
typedef std::list<MLayer*>::reverse_iterator REVLAYITER;

#endif// MLAYER_H
