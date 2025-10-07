//PACKAGE : MAP.
//FILE: Id.h    
//AUTHOR:
//DESCRIPTION:


#ifndef ID_H
#define ID_H
#include <string>
#include "BaseClId.h"

// Array of unique number identifiers of map scales
const unsigned int MASSTAB_AR[] = {1000000,500000,200000,
									100000,50000,25000,10000,
									5000,2000,1000,500};

// Names array(lines identifiers) of maps scales
const std::string MASSTAB_AR_STRING[] = {"1000000","500000",
	"200000","100000","50000",
	"25000","10000","5000",
	"2000","1000","500"};

// Array dimension of scales of identifiers
#define MASSTAB_AR_SIZE 11

// List of unique scales indexes
enum{MASSTAB_1000000_INDEX = 0,
	MASSTAB_500000_INDEX = 1,
	MASSTAB_200000_INDEX = 2,
	MASSTAB_100000_INDEX = 3,
	MASSTAB_50000_INDEX = 4,
	MASSTAB_25000_INDEX = 5,
	MASSTAB_10000_INDEX = 6,
	MASSTAB_5000_INDEX = 7,
	MASSTAB_2000_INDEX = 8,
	MASSTAB_1000_INDEX = 9,
	MASSTAB_500_INDEX = 10};

#define ID_UTP_KARTA    "UTP"	// < Map identifier

// Identifiers of map objects
#define ID_TOPOBJECT	"TPO"	// < Identifier of topobject
#define	ID_NADPKRIV		"NPK"	// < Identifier in NUX-format
#define ID_NADGENERAL	"NPG"	// < Identifier of global inscription
#define ID_NADPIS		"NPS"	// < Identifier of inscription
#define ID_RAMKA		"RMK"	// < Identifier in NUX-format
#define ID_SCENERY		"SCN"	// < Identifier in NUX-format
#define ID_HEIGHTPOINT	"HPT"	// < Identifier of isoline
#define ID_IZOLINE		"IZO"	// < Identifier of isoline
#define ID_OBRYV		"OBR"	// < Identifier of steep
#define ID_OTKOS		"OTK"	// < Identifier of slope
#define ID_YAMA			"YAM"	// < Identifier of hole
#define ID_PODPSTENKA	"PST"	// < Identifier of supporting wall
#define ID_GIDRO		"GDR"	// < Identifier of area topobject


// Identifiers of classifier objects
#define ID_ZNAK "ZNK"			// < Identifier of sign
#define ID_RAZDEL "RDL"			// < Identifier of partition
#define ID_ZNAKSISTEMA "ZST"	// < Identifier of sign system
#define ID_KLASSIFIKATOR "KLS"	// < Identifier of classifier

// Identifiers of map objects Arrays and classifiers
#define ID_GDA        "GDA"		// < Array of pointers to MArray
#define ID_RTA        "RTA"		// < Array of pointers to MDiskBmp 
#define ID_ZNA        "ZNA"		// < Array of signs
#define ID_RLA        "RLA"		// < Array of partitions
#define ID_ZSA        "ZSA"		// < Array of sign systems
#define ID_ZBK        "ZBK"		// < Array of pointers to MZarubka
#define ID_RZV        "RZV"		// < Array of pointers to MRazryv
#define ID_SGT        "SGT"		// < Array of pointers to MSegment
#define ID_TRG        "TRG"		// < Array of pointers to MTriangle


// Identifiers of different layers
#define ID_SIMPLE_LAYER "LAY" // < Regular layer
#define ID_ROAD_LAYER   "ROD" // < Layer of roads
#define ID_GIDRO_LAYER  "GDR" // < Layer of hydrography
#define ID_RELIEF_LAYER "RLL" // < Layer of relief


// Identifier of object which attached to service classifier
#define SERV_OBJECT "SERV"

// Identifiers of language number in Array of sign languages
#define IDY_SECOND		1 // second language of description

// Identifiers, which attached to service classifier
#define SERV_KLS	"SERV"
#define SERV_ZS		"ZS"
#define SERV_RZ		"RZ"
#define SERV_DOT	"DOT"
#define SERV_LIN	"LIN"
#define SERV_PLO	"PLO"
#define SERV_HPT	"HPT"
#define SERV_IZO	"IZO"
#define SERV_OBR	"OBR"
#define SERV_OTK	"OTK"
#define SERV_YAM	"YAM"
#define SERV_PST	"PST"
#define SERV_GDR	"GDR"
#define SERV_OBJECT_DOT "SERV ZS RZ DOT"
#define SERV_OBJECT_LIN "SERV ZS RZ LIN"
#define SERV_OBJECT_PLO "SERV ZS RZ PLO"
#define SERV_OBJECT_HPT	"SERV ZS RZ HPT"
#define SERV_OBJECT_IZO	"SERV ZS RZ IZO"
#define SERV_OBJECT_OBR	"SERV ZS RZ OBR"
#define SERV_OBJECT_OTK	"SERV ZS RZ OTK"
#define SERV_OBJECT_YAM	"SERV ZS RZ YAM"
#define SERV_OBJECT_PST	"SERV ZS RZ PST"
#define SERV_OBJECT_GDR	"SERV ZS RZ GDR"

// Addition to base path for different types of files
#define TEMP_3D_BMP				"_3d.bmp"
#define TEMP_CLIPBOARD_TMP		"_clpb.tmp"
#define TEMP_PRINT_BMP			"_print.bmp"
#define TEMP_HEIGHT_MATRIX_BMH	"_heigh_matr.bmh"
#define TEMP_MODUS_BMP			"_modus.bmp"

#define BMH_NOVALUE      -9999
#define BMH_OUTOFBOUNDS  -20000

#endif
