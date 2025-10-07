#include <stdlib.h>
#include <memory.h>
#include <qstring.h>
#include "fmt.h"
#include "Refglobal.h"
//===============================================================================
// Shapefile (CPP)
//===============================================================================


//====================================
//!ReadInteger function definition - the return reading of bytes for the integer which is written down in 'big endian' (Sun or Motorola) a byte order
int ReadReverseInteger(FILE* stream)
//====================================
{	
	union {
		char str[4];
		int n;
	};	

	for(int i = 0; i < 4; i++)
		fread(&str[3-i], 1,1,stream);
	
	return(n);
}

//====================================
//!ReadInteger function definition - the return reading of bytes for the integer which is written down in 'big endian' (Sun or Motorola) a byte order
int ReadReverseInteger(MFile* stream)
//====================================
{	
	union {
		char str[4];
		int n;
	};	

	for(int i = 0; i < 4; i++)
		stream->GetData(&str[3-i], 1, 1);
	
	return(n);
}

//====================================
//!Constructor
MShapefile :: MShapefile()
//====================================
{
	IsFileOpen = false;
}


//====================================
//!definition of number of entries in the shape-file
int MShapefile :: GetNumberOfRecords()
//====================================
{
	if(IsFileOpen != true)
		return(-100);
	
	return(N);
}

//====================================
//!function of reading file type
int MShapefile :: GetFileType()
//====================================
{
	if(IsFileOpen != true)
		return(-100);

	int type=0;

	fseek(stream, 32, SEEK_SET);
	fread(&type, sizeof(int), 1, stream);
 	if (reverseReadFlag) reverseValue((char*)&type, sizeof(int) );

	return(type);
}


//====================================
//!function of reading n-th entry type
int MShapefile :: GetRecType(int n)
//====================================
{
	if(IsFileOpen != true)
		return(-100);

	int type=0;

	if(n > N-1)
        return(-1);                   //n > N-1 - number of all entries

	fseek(stream, p[n]*2 + 8, SEEK_SET);
	fread(&type, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&type, sizeof(int) );
		
	return(type);
}



//====================================
//!function of reading of quantity of parts for n-th entry
int MShapefile :: GetNumberOfRecParts(int n)
//====================================
{
	if(IsFileOpen != true)
		return(-100);

	int type=0, numparts=0;

	if(n > N-1)
        return(-1);                   //n > N-1 - number of all entries

	fseek(stream, 32, SEEK_SET);
	fread(&type, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&type, sizeof(type) );
	if((type==0)||(type==1)||(type==8))
        return(-2);                      //entries from the chosen file don't contain parts

	fseek(stream, p[n]*2 + 44, SEEK_SET);
	fread(&numparts, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&numparts, sizeof(numparts) );
		
    return(numparts);                    // numparts - number of parts for n-th entry
}



//====================================
//!function of reading of number of points for n-th entry
int MShapefile :: GetNumberOfRecPoints(int n)
//====================================
{
	if(IsFileOpen != true)
		return(-100);

	int type=0, numpoints=0;

	if(n > N-1)
        return(-1);                   //n > N-1 - number of all entries

	fseek(stream, 32, SEEK_SET);
	fread(&type, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&type, sizeof(type) );
	if((type==0)||(type==1))
        return(-2);                          //entries from the chosen file don't contain a set of points
	
	if(type==8)
	{
		fseek(stream, p[n]*2 + 44, SEEK_SET);
		fread(&numpoints, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&numpoints, sizeof(numpoints) );
		
		return(numpoints);
	}
	else
	{
		fseek(stream, p[n]*2 + 48, SEEK_SET);
		fread(&numpoints, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&numpoints, sizeof(numpoints) );
		
        return(numpoints);                       // numpoints - number of points for n-th entry
	}

	
}

//====================================
//!function of reading of the bordering rectangle for the shape-file
int MShapefile :: GetMapRect(MRect* prect)
// prect - the pointer on object of the class MRect
//====================================
{
	if(IsFileOpen != true)
		return(-100);

	fseek(stream, 36, SEEK_SET);
	fread(prect, sizeof(MRect), 1, stream);  //vipa?????
	return 1;
}


//====================================
//!function of reading of the bordering rectangle for n-th entry
int MShapefile :: GetRecRect(MRect* prect, int n)
// prect - the pointer on object of the class MRect
//====================================
{
	if(IsFileOpen != true)
		return(-100);

	int type;
	double rect[4];
	
	

	if(n > N-1)
        return(-1);                                    //n > N-1 - number of all entries

	fseek(stream, 32, SEEK_SET);
	fread(&type, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&type, sizeof(type) );
	if((type==0)||(type==1))
        return(-2);                                        //entries from the chosen file don't contain parts

	fseek(stream, p[n]*2 + 12, SEEK_SET);
	fread(&rect, sizeof(MRect), 1, stream); //vipa?????
	prect->left = rect[0];
	prect->top = rect[1];
	prect->right = rect[2];
	prect->bottom = rect[3];
	
	return(1);
}



//====================================
//!function of reading of an index of the first point 's'th part for n-th entry
int MShapefile :: GetPartPointIndex(int n, int s)
//====================================
{
	if(IsFileOpen != true)
		return(-100);

    int type, i, numparts, index = 0;

	if(n > N-1)
        return(-1);                   //n > N-1 - number of all entries

	fseek(stream, 32, SEEK_SET);
	fread(&type, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&type, sizeof(int) );
	if((type==0)||(type==1)||(type==8))
        return(-2);                      //entries from the chosen file don't contain parts

	fseek(stream, p[n]*2 + 44, SEEK_SET);
	fread(&numparts, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&numparts, sizeof(int) );

	if(s+1 > numparts)
            return(-3);                     //'s' can't exceed number of parts for this entry
	fseek(stream, 4, SEEK_CUR);
	for(i=0; i<s+1; i++)
		fread(&index, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&index, sizeof(int) );
		
	return(index);
}


//====================================
//!function of definition of number of points in 's'th a knee for n-th entry
int MShapefile :: GetPartNumberOfPoints(int n, int s)
//====================================
{
	if(IsFileOpen != true)
		return(-100);

    int type, i, numparts, index = 0, numpoints, index1, number;

	if(n > N-1)
        return(-1);                   //n > N-1 - number of all entries

	fseek(stream, 32, SEEK_SET);
	fread(&type, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&type, sizeof(int) );
	if((type==0)||(type==1)||(type==8))
        return(-2);                      //entries from the chosen file don't contain parts

	fseek(stream, p[n]*2 + 44, SEEK_SET);
	
	fread(&numparts, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&numparts, sizeof(int) );
	fread(&numpoints, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&numpoints, sizeof(int) );
        
	if(s+1 > numparts)
        return(-3);                     //'p' can't exceed number of parts for this entry
	
	for(i=0; i<s+1; i++)
		fread(&index, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&index, sizeof(int) );
	if(s+1==numparts)
		number = numpoints - index;
	else
	{
		fread(&index1, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&index1, sizeof(int) );
		number = index1 - index;
	}

	return(number);
}


//====================================
//!reading function 'k'th points n-th entry in a knee with number 's'
int MShapefile :: GetPoint(MPoint* geopoint,int n, int s, int k)
// geopoint - the pointer on object of the class MPoint
//====================================
{
	if(IsFileOpen != true)
		return(-100);

    int type, i,numparts,numpoints, index = 0, index1, number;
	double point[2];

	if(n > N-1)
        return(-1);                   //n > N-1 - number of all entries

	fseek(stream, 32, SEEK_SET);
	fread(&type, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&type, sizeof(int) );
	if(type==0)
        return(-2);                      //empty entry

	
	switch(type)
	{
	case 1:
	{
        fseek(stream, p[n]*2 + 12, SEEK_SET);                  //entries from the chosen file don't contain points
	
		fread(&point, sizeof(MPoint), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&point[0], sizeof(double) );
 		if (reverseReadFlag) reverseValue((char*)&point[1], sizeof(double) );
		geopoint->x = point[0];
		geopoint->y = point[1];

		return 1;
	}
	break;

	case 8:
	{
        fseek(stream, p[n]*2 + 44, SEEK_SET);                  //entries from the chosen file don't contain points
	
		fread(&numpoints, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&numpoints, sizeof(int) );
	
		
		if(k+1 > numpoints)
            return(-4);                           //k > numpoints - total number of points in the file
	
		for(i=0; i<k+1; i++) {
			fread(&point, sizeof(MPoint), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&point[0], sizeof(double) );
 		if (reverseReadFlag) reverseValue((char*)&point[1], sizeof(double) );
		}
		geopoint->x = point[0];
		geopoint->y = point[1];

		return 1;
	}
	break;

	default:
	{
		fseek(stream, p[n]*2 + 44, SEEK_SET);
		fread(&numparts, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&numparts, sizeof(int) );
		fread(&numpoints, sizeof(int), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&numpoints, sizeof(int) );

		if(s+1 > numparts)
            return(-3);                     //'s' can't exceed number of parts for this record
	

		for(i=0; i<s+1; i++)
			fread(&index, sizeof(int), 1, stream);
   		if (reverseReadFlag) reverseValue((char*)&index, sizeof(int) );
		if(s+1==numparts)
		{
			number = numpoints - index;
			if(k+1 > number)
                return(-4);                   //k > number - number of points in p-th part
			for(i=0; i<k+1; i++) {
				fread(&point, sizeof(MPoint), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&point[0], sizeof(double) );
 		if (reverseReadFlag) reverseValue((char*)&point[1], sizeof(double) );
                        }
			geopoint->x = point[0];
			geopoint->y = point[1];

		return 1;
		}

		else
		{
			fread(&index1, sizeof(int), 1, stream);
   		if (reverseReadFlag) reverseValue((char*)&index1, sizeof(int) );
			number = index1 - index;
			if(k+1 > number)
                return(-4);                   //k > number - number of points in p-th part
			fseek(stream, sizeof(int)*(numparts - (s + 2)), SEEK_CUR);
			for(i=0; i<k+1; i++) {
				fread(&point, sizeof(MPoint), 1, stream);
 		if (reverseReadFlag) reverseValue((char*)&point[0], sizeof(double) );
 		if (reverseReadFlag) reverseValue((char*)&point[1], sizeof(double) );
                        }
			geopoint->x = point[0];
			geopoint->y = point[1];
			
			return 1;
		}

		
	}
	break;
	}
}

//====================================
//!reading function n-th records
/** prect - the pointer on object of the class MRect
//index - the pointer on the array of indexes of the first points of parts
// geopoints -  the pointer on the array of objects of the class MPoint
// n - number of entry
*/
int MShapefile :: GetRecord(MRect* prect, int* index, MPoint* geopoints, int n)
//====================================
{
	if(IsFileOpen != true)
		return(-100);

	int type;
	
	if(n > N-1)
        return(-1);                                    //n > N-1 - number of all entries

	fseek(stream, 32, SEEK_SET);
	fread(&type, sizeof(int), 1, stream);
   		if (reverseReadFlag) reverseValue((char*)&type, sizeof(int) );
		                                       
	switch(type)
	{
	case 0:
        return(-2);                            //empty entry
	case 11:
	case 1:
		{
            fseek(stream, p[n]*2 + 12, SEEK_SET);                  //entries from the chosen file don't contain points
			fread(geopoints, sizeof(MPoint), 1, stream);
			return 1;
		}
	case 8:
		{
			int numpoints;

            fseek(stream, p[n]*2 + 12, SEEK_SET);           //entries from the chosen file don't contain points
			fread(prect, sizeof(MRect), 1, stream);
			fread(&numpoints, sizeof(int), 1, stream);
			fread(geopoints,numpoints * sizeof(MPoint), 1, stream);
			return 1;
		}
	default:

		{
			int numparts, numpoints;

            fseek(stream, p[n]*2 + 12, SEEK_SET);           //entries from the chosen file don't contain points
			fread(prect, sizeof(MRect), 1, stream);
			fread(&numparts, sizeof(int), 1, stream);
			fread(&numpoints, sizeof(int), 1, stream);
			fread(index, sizeof(int) * numparts, 1, stream);
			fread(geopoints, numpoints * sizeof(MPoint), 1, stream);
			return 1;
		}
		break;
	}
}

//====================================
//!function of opening of the file
//! string - a line - a full way to the necessary file
bool MShapefile :: OpenFile( MString string)
//====================================
{
	int i, lengthfile;
	MString stringShx;
	FILE* streamShx;
		

	if(IsFileOpen == true)
		return false;

	if( (stream = fopen(string, "rb")) == NULL )
      return false;

    // Instead of the simple
    // stringShx = string;
    // for copying of a line the operator Format, on a case is inserted
    // if the line looks like "хххххх ххх" (with a space)
	stringShx.Format("%s",(char *)string);
    i = stringShx.GetLength();
    stringShx.SetAt(i-1, 'x');         // stringShx - full way to the index file corresponding to the chosen shape-file

    if( (streamShx = fopen(stringShx, "rb")) != NULL )  // check of existence of the index file
	{
		fseek(streamShx, 24, SEEK_SET);
		lengthfile = ReadReverseInteger(streamShx);
		
        N = (lengthfile * 2 - 100)/8;   //N - number of entries
	                            
        p = (int*)malloc(N*4);         //we create the index array

		q = (int*)malloc(N*4); 

        fseek(streamShx, 100, SEEK_SET);   //we install the cursor for the beginning of the first entry

		for(i = 0; i < N; i++) {
			p[i] = ReadReverseInteger(streamShx);
			q[i] = ReadReverseInteger(streamShx);
		}

		fclose(streamShx);
	}

	else
	{
		int offset, nomer=0, reclength, ibuf;
		lengthfile = 0;

		while(!feof( stream )) {
			fread(&ibuf, sizeof(int), 1, stream);
			lengthfile = lengthfile + 4;
		}

		lengthfile = lengthfile - 4;
		fseek(stream, 100, SEEK_SET);
		offset = 0;

		while(offset !=(lengthfile - 100))
		{
			nomer = ReadReverseInteger(stream);
			reclength = ReadReverseInteger(stream);
			fseek(stream,reclength*2, SEEK_CUR); 
			offset += (reclength * 2 + 8);
		}

		N = nomer;

        p = (int*)malloc(N*4);         //we create the index array
		q = (int*)malloc(N*4);

		fseek(stream, 100, SEEK_SET);
		offset = 50;
		reclength = 0;

		for(i=0; i<N; i++)
		{
			fseek(stream, 4, SEEK_CUR);
			p[i] =offset;
			reclength = ReadReverseInteger(stream);
			q[i] = reclength;
			fseek(stream, reclength*2, SEEK_CUR); 
			offset += (reclength + 4);
		}


	}
	IsFileOpen = true;
	return true;
}

//====================================
//!function of closing of the file
bool MShapefile :: CloseFile()
//====================================
{
	if(IsFileOpen != true)
		return false;

	free(p);
	free(q);
	fclose(stream);
	IsFileOpen = false;
	return true;
}


//===============================================================================
// DBF (CPP)
//===============================================================================

//====================================
//!Constructor
MDbffile :: MDbffile()
//====================================
{
	IsFileOpen = false;
}

//====================================
//! definition of function of reading of number of fields
int MDbffile :: GetFieldsNumber()
//====================================
{
	if(IsFileOpen != true)
		return(-100);
	return(N);
}

//====================================
//! definition of function of reading of number of entries
int MDbffile :: GetRecordsNumber()
//====================================
{
	if(IsFileOpen != true)
		return(-100);
	return(Z);
}

//====================================
//! definition of function of reading of the name of a field according to number
MString MDbffile :: GetFieldName(int n)
//====================================
{
	if(IsFileOpen != true)
		return ("откройте файл");

	if(n > N-1)
		return("n превышает число полей N");

	int i, rec_offset;
	MString str;
	char name[11]; 
      
    rec_offset = 0;
	  
    for(i=1; i<=n; i++)
		rec_offset += 32; 

    fseek(stream, 32 + rec_offset, SEEK_SET);          //we get up for the beginning of the description the n-th field
	  
    fread(&name, 11, 1, stream);   //name - name of the n-th field
	str =name;
          

	return(str);
}

//====================================
//!  Definition of function of reading field type according to number
char MDbffile :: GetFieldType(int n)
//====================================	
{
	if(IsFileOpen != true)
		return('F');

	if(n > N-1)
        return(' ');                 // 'n' exceeds number of fields 'N'

	/*
	//It was like this before MAKS: 
	int i, rec_offset;
	char type;
	rec_offset = 11;
  
	for(i=1; i<=n; i++)
		rec_offset += 32; 
    fseek(stream, 32 + rec_offset, SEEK_SET);          //we get up for the beginning of the description the n-th field
    fread(&type, 1, 1, stream);   //type - type of the n-th field
	           
	return(type);
	*/
	return(fieldTyp[n]);
}

//====================================
//!definition of function of reading of length of record for the set field
int MDbffile :: GetFieldLength(int n)
{
	if(IsFileOpen != true)
		return(-100);

	if(n > N-1)
        return(-1);               // 'n' exceeds number of fields 'N'
	
	/*
	//It was like this before MAKS: 

	int i, rec_offset;
	char type;

	union
	{
		unsigned ilen;
		struct
		{
			char len;
			char dec;
		}num_size;
	}len_info;

	len_info.ilen = 0;
	rec_offset = 11;
	  
    for(i=1; i<=n; i++)
		rec_offset += 32; 

    fseek(stream, 32 + rec_offset, SEEK_SET);  //we get up for the beginning of the description the n-th field
	fread(&type, 1, 1, stream);
	if(type == 'N')
	{		
		fseek(stream, 4, SEEK_CUR);
        fread(&len_info.num_size.len, 1, 1, stream);   //num_size.len - entry length of the n-th field
		len_info.ilen = (unsigned)len_info.num_size.len;
	}
	else {
		fseek(stream, 4, SEEK_CUR);
		fread(&len_info.ilen, 2, 1, stream);
	}

    
	return len_info.ilen;
	*/
	
	return fieldLen[n];
}


//==================================== 
//!function of a conclusion of value of a symbolical field 'n' for the z-th entry
MString MDbffile :: GetString(const char* pch, int n, int z)
//====================================
{
	if(IsFileOpen != true)
		return ("откройте файл");
		
	if(n > N-1)
		return("n превышает число полей N");
		
	if(z > Z-1)
		return("z превышает число записей Z");
	
	/*	it was like this before MAKS
	char type;
	int i, j, rec_offset, rec_inoffset;
	unsigned short data_offset, rec_size;
	union
	{
		unsigned ilen;
		struct
		{
			char len;
			char dec;
		}num_size;
	}len_info;

	len_info.ilen = 0;
	
	j=0;
	rec_inoffset = 0;
	rec_offset = 11;
	for(i=0; i<n; i++)
	{
		
       fseek(stream, 32 + rec_offset, SEEK_SET);  //we get up for the beginning of the description the n-th field

		fread(&type, 1, 1, stream);

		if(type == 'N')
		{	
			fseek(stream, 4, SEEK_CUR);
            fread(&len_info.num_size.len, 1, 1, stream);   //num_size.len - entry length of the n-th field
			rec_inoffset += (unsigned)len_info.num_size.len;
			rec_offset += 32;
		}
		else
		{
			fseek(stream, 4, SEEK_CUR);
			fread(&len_info.ilen, 2, 1, stream);
			rec_inoffset += len_info.ilen;
			rec_offset += 32;
		}
	}
		
	fseek(stream, 8, SEEK_SET);
    fread(&data_offset, 2, 1, stream);        //data_offset - shift with which information entries begin
    fread(&rec_size, 2, 1, stream);           //rec_size - size of information entry
	fseek(stream, 20, SEEK_CUR);
	
	rec_offset = 11;
	  
    for(i=0; i<n; i++)
		rec_offset += 32; 

    fseek(stream, 32 + rec_offset, SEEK_SET);  //we get up for the beginning of the description the n-th field
    fread(&type, 1, 1, stream);

	if(type != 'C')
		return("Выбранное поле не является символъным");
		
		
    fseek(stream, 4, SEEK_CUR);
	fread(&len_info.ilen, 2, 1, stream);
	
    fseek(stream, data_offset + 1 + rec_size*z + rec_inoffset, SEEK_SET);  //we get up for the beginning of entry for the specified field in information entry

	fread((void*)pch, len_info.ilen, 1,stream);
	
	return (pch);
	*/
	int i, rec_inoffset;

	rec_inoffset = 0;
	for(i=0; i<n; i++)	rec_inoffset += fieldLen[i];
	
    fseek(stream, frstRec + 1 + recLen*z + rec_inoffset, SEEK_SET);  //we get up for the beginning of entry for the specified field in information entry
	fread((void*)pch, fieldLen[n], 1,stream);
	QString st = pch;
	st.simplified();
    return (st.toLatin1().data());
	
}

//==================================== 
//!function of a conclusion of value of the whole numerical field 'n' for the 'z'th entry
int MDbffile :: GetIntNumber(int n, int z)
//====================================
{
	if(IsFileOpen != true)
		return(-100);

	if(n > N-1)
        return(-1);                // 'n' exceeds number of fields 'N'

	if(z > Z-1)
        return(-2);                // 'z' exceeds number of fields 'Z'

	/*	it was like this before MAKS

	char type, name[25], num;
	int i, rec_offset, number, rec_inoffset;
	unsigned short data_offset, rec_size;
	union
	{
		unsigned ilen;
		struct
		{
			char len;
			char dec;
		}num_size;
	}len_info;

	len_info.ilen = 0;
	
	rec_inoffset = 0;
	rec_offset = 11;
	for(i=0; i<n; i++)
	{
		
       fseek(stream, 32 + rec_offset, SEEK_SET);  //we get up for the beginning of the description the n-th field
		fread(&type, 1, 1, stream);

		if(type == 'N')
		{	
			fseek(stream, 4, SEEK_CUR);
            fread(&len_info.num_size.len, 1, 1, stream);   //num_size.len - entry length of the n-th field
			rec_inoffset += (unsigned)len_info.num_size.len;
			rec_offset += 32;
		}
		else
		{
			fseek(stream, 4, SEEK_CUR);
			fread(&len_info.ilen, 2, 1, stream);
			rec_inoffset += len_info.ilen;
			rec_offset += 32;
		}
	}
		
	fseek(stream, 8, SEEK_SET);
    fread(&data_offset, 2, 1, stream);        //data_offset - shift with which information entries begin
    fread(&rec_size, 2, 1, stream);           //rec_size - size of information entry
	fseek(stream, 20, SEEK_CUR);

	rec_offset = 11;
	  
    for(i=0; i<n; i++)
		rec_offset += 32; 

    fseek(stream, 32 + rec_offset, SEEK_SET);  //we get up for the beginning of the description the n-th field
    fread(&type, 1, 1, stream);
	if(type != 'N')
			return(-3);

    fseek(stream, 4, SEEK_CUR);
    fread(&len_info.num_size.len, 1, 1, stream);
	fread(&num, 1, 1, stream);
    if(num!=0)
		return(-4);
		
    fseek(stream, data_offset + 1 + rec_size*z + rec_inoffset, SEEK_SET);  //we get up for the beginning of entry for the specified field in information entry
	fread(&name, (unsigned)len_info.num_size.len, 1,stream);
	number = atoi(name);
		
	return(number);
	*/

	char  name[25];
	int i, rec_inoffset, number;

	rec_inoffset = 0;
	for(i=0; i<n; i++)	rec_inoffset += fieldLen[i];
		
    fseek(stream, frstRec + 1 + recLen*z + rec_inoffset, SEEK_SET);  //we get up for the beginning of entry for the specified field in information entry
	fread(&name, fieldLen[n], 1,stream);
	number = atoi(name);
		
	return(number);
	
}

//==================================== 
//!function of a conclusion of value of the whole numerical field 'n' for the 'z'th entry
double MDbffile :: GetDoubleNumber(int n, int z)
//====================================
{
	if(IsFileOpen != true)
		return(-100);

		if(n > N-1)
        return(-1);                // 'n' exceeds number of fields 'N'

	if(z > Z-1)
        return(-2);                // 'z' exceeds number of fields 'Z'

	/*	it was like this before MAKS
	char type, name[25], num;
	int i, rec_offset, rec_inoffset;
	unsigned short data_offset, rec_size;
	double number;
	union
	{
		unsigned ilen;
		struct
		{
			char len;
			char dec;
		}num_size;
	}len_info;

	len_info.ilen = 0;
	
	rec_inoffset = 0;
	rec_offset = 11;
	for(i=0; i<n; i++)
	{
		
       fseek(stream, 32 + rec_offset, SEEK_SET);  //we get up for the beginning of the description the n-th field

		fread(&type, 1, 1, stream);

		if(type == 'N')
		{	
			fseek(stream, 4, SEEK_CUR);
            fread(&len_info.num_size.len, 1, 1, stream);   //num_size.len - entry length of the n-th field
			rec_inoffset += (unsigned)len_info.num_size.len;
			rec_offset += 32;
		}
		else
		{
			fseek(stream, 4, SEEK_CUR);
			fread(&len_info.ilen, 2, 1, stream);
			rec_inoffset += len_info.ilen;
			rec_offset += 32;
		}
	}
		
	fseek(stream, 8, SEEK_SET);
    fread(&data_offset, 2, 1, stream);        //data_offset - shift with which information entries begin
    fread(&rec_size, 2, 1, stream);           //rec_size - size of information entry
	fseek(stream, 20, SEEK_CUR);

	rec_offset = 11;
	  
    for(i=0; i<n; i++)
		rec_offset += 32; 

    fseek(stream, 32 + rec_offset, SEEK_SET);  //we get up for the beginning of the description the n-th field
    fread(&type, 1, 1, stream);
	if(type != 'N')
			return(-1);

    fseek(stream, 4, SEEK_CUR);
    fread(&len_info.num_size.len, 1, 1, stream);
	fread(&num, 1, 1, stream);
    if(num==0)
		return(-2);
		
    fseek(stream, data_offset + 1 + rec_size*z + rec_inoffset, SEEK_SET);  //we get up for the beginning of entry for the specified field in information entry
	fread(&name, (unsigned)len_info.num_size.len, 1,stream);
	number = atof(name);
		
	return(number);
	*/
	char  name[25];
	int i, rec_inoffset;
	double number;

	rec_inoffset = 0;
	for(i=0; i<n; i++)	rec_inoffset += fieldLen[i];
		
    fseek(stream, frstRec + 1 + recLen*z + rec_inoffset, SEEK_SET);  //we get up for the beginning of entry for the specified field in information entry
	fread(&name, fieldLen[n], 1,stream);
	number = atof(name);
		
	return(number);
	
}

//====================================
//!function of opening of the file. string full way to the necessary file
bool MDbffile :: OpenFile( MString string)
{
	if(IsFileOpen == true)
		return false;

	if( (stream = fopen(string, "rb")) == NULL )
      return false;

	int n;
	char tmp;

	fseek(stream, 4, SEEK_SET);
	fread(&Z, 4, 1, stream);
	fseek(stream, 24, SEEK_CUR);
	n = 0;
	
    while(!(fgetc(stream) == 13)) {       //we define N - number of fields
		fseek(stream, 10, SEEK_CUR);
		fread(&fieldTyp[n], 1, 1, stream);
        fseek(stream, 4, SEEK_CUR);  //we get up for the beginning of the description of the size of a field
		
        //depending on type of a field we read out the size
		if((fieldTyp[n] == 'F')|(fieldTyp[n] == 'N'))
		{
			fread(&tmp, 1, 1, stream);   
			fieldLen[n] = (short)tmp;
			fseek(stream, 15, SEEK_CUR);
		}
		else
		{
			 fread(&fieldLen[n], 2, 1, stream);
			 fseek(stream, 14, SEEK_CUR);
		}

		n++;
	}

	N = n;

    //data of DBF header
	fseek(stream, 8, SEEK_SET);
    fread(&frstRec, 2, 1, stream);    //data_offset - shift with which information entries begin
    fread(&recLen, 2, 1, stream);     //rec_size - size of information entry
	
	IsFileOpen = true;
	return true;
}

//====================================
//!function of closing of the file
bool MDbffile :: CloseFile()
{
	if(IsFileOpen != true)
		return false;

	fclose(stream);
	IsFileOpen = false;
	return true;
}

//===================================================
//!turns any record 'z' of a field 'n' at line
MString MDbffile::ReturnString(int n, int z)
//===================================================
{
	/* it was like this before MAKS
	char type = GetFieldType(n);
	int rec_offset, rec_offset1, rec_inoffset;
	unsigned short data_offset, rec_size;
	union
	{
		unsigned ilen;
		struct
		{
			char len;
			char dec;
		}num_size;
	}len_info;

	len_info.ilen = 0;
	rec_inoffset = 0;
    //shift from which we read field type
	rec_offset = 11;
    //shift from which we read the amount of entry of a field
	rec_offset1 = 16;
    //data_offset - shift with which information entries begin
	fseek(stream, 8, SEEK_SET);
	fread(&data_offset, 2, 1, stream);
    //rec_size - size of information entry
	fseek(stream, 10, SEEK_SET);
	fread(&rec_size, 2, 1, stream);        
	char n_type;
	
    //we will find shift with which our data in entry 'z' begin
	for(int i=0; i<n; i++)
	{		
        //we get up for the beginning of the description of the i-th field
		fseek(stream, 32 + rec_offset, SEEK_SET);  
		fread(&n_type, 1, 1, stream);
		fseek(stream, 32 + rec_offset1, SEEK_SET);
        //we copy the amount of entry of a field
		if(n_type == 'N')
		{	
            fread(&len_info.num_size.len, 1, 1, stream);   //num_size.len - entry length of the n-th field
			rec_inoffset += (unsigned)len_info.num_size.len;			
		}

		else
		{
			fread(&len_info.ilen, 2, 1, stream);
			rec_inoffset += len_info.ilen;	
		}
		rec_offset += 32;
		rec_offset1 += 32;
	}		
    //we get up on reading the amount of entry
	fseek(stream, 32 + rec_offset1, SEEK_SET);
    //the returned size
	MString stroka = "";
	char *buffer; 
    //we turn into a line, previously having marked out memory
	switch(type){
	case 'N':
		{
			fread(&len_info.num_size.len, 1, 1, stream);
            //1 - removal flag
            fseek(stream, data_offset + 1 + rec_size*z + rec_inoffset, SEEK_SET);  //we get up for the beginning of entry for the specified field in information entry
			buffer = (char *) new (char[len_info.num_size.len+1]); 
			buffer[len_info.num_size.len] = '\0';
			//buffer.GetBuffer(len_info.num_size.len)
			fread(buffer, (unsigned)len_info.num_size.len, 1,stream);
			stroka = buffer;
			delete [] buffer;
		}
		break;
	case 'C':
		{			
			fread(&len_info.ilen, 2, 1, stream);
            //1 - removal flag
            fseek(stream, data_offset + 1 + rec_size*z + rec_inoffset, SEEK_SET);  //we get up for the beginning of entry for the specified field in information entry
			buffer = (char *) new (char[len_info.ilen+1]);
			buffer[len_info.ilen] = '\0';
			//buffer.GetBuffer(len_info.ilen)
			fread(buffer, (unsigned)len_info.ilen, 1, stream);
			stroka = buffer;
			delete [] buffer;
		}
		break;
	}

	stroka.TrimLeft();//" ");
	stroka.TrimRight();//" ");
	
	return stroka;
	*/
	int  rec_inoffset = 0;
	int i;
	MString stroka = "";
	char *buffer; 


	for(i=0; i<n; i++)	rec_inoffset += fieldLen[i];


    fseek(stream, frstRec + 1 + recLen*z + rec_inoffset, SEEK_SET);  //we get up for the beginning of entry for the specified field in information entry
    buffer = new char[fieldLen[n]+1];
	buffer[fieldLen[n]] = '\0';
	fread(buffer/*.GetBuffer(len_info.num_size.len)*/, fieldLen[n], 1,stream);
	stroka = buffer;
	delete [] buffer;

	stroka.TrimLeft();//" ");
	stroka.TrimRight();//" ");
	
	return stroka;

}

QString MDbffile::GetValueByfieldName(MString &fieldName, int z)
{  
    int n =0;
    for(int i=0;i<N;++i)
	{
		if (GetFieldName(i)==fieldName)
		{
			n = i;
			break;
		}
	}
    QString string =QString( ReturnString(n,z).c_str());
	return string;
}
