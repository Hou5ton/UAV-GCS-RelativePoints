#include "MImage.h"

MImage::MImage()
{
	w = h = d = bitordr = dpmx = dpmy = alpha = 0;
	data = new MByArray();
	ctbl = new MInArray();
}

MImage::MImage ( int w, int h, int depth, int bitOrder) 
{
	setWidth(w);
	setHeight(h);
	setDepth(depth);
	setBitOrder(bitOrder);

	dpmx = dpmy = alpha = 0;
	data = new MByArray();
	ctbl = new MInArray();
}

MImage::MImage ( int w, int h, int format ) 
{
	setWidth(w);
	setHeight(h);
	setFormat(format);

	dpmx = dpmy = 0;
	data = new MByArray();
	ctbl = new MInArray();
}

MImage::MImage(MImage* image)
{
	Copy(image);
}

MImage::~MImage()
{
	delete data;
	delete ctbl;
}

bool MImage::IsEqualTo(MImage *image)
{
	if (w != image->w) return false;
	if (h != image->h) return false;
	if (d != image->d) return false;
	if (bitordr != image->bitordr) return false;
	if (dpmx != image->dpmx) return false;
	if (dpmy != image->dpmy) return false;
	if (alpha != image->alpha) return false;
	if (data->GetSize() != image->data->GetSize()) return false;
	if (ctbl->GetSize() != image->ctbl->GetSize()) return false;
	for (int i = 0; i < data->GetSize(); i++)
		if (*data->GetAt(i) != *image->data->GetAt(i)) return false;
	for (int i = 0; i < ctbl->GetSize(); i++)
		if (*ctbl->GetAt(i) != *image->ctbl->GetAt(i)) return false;	
	return true;

}

bool MImage::WriteBin(FILE* h_nuxfile, int version) //writing in the UTP-file
{
	fwrite(&w, sizeof(w), 1, h_nuxfile);
	fwrite(&h, sizeof(h), 1, h_nuxfile);
	fwrite(&d, sizeof(d), 1, h_nuxfile);
	fwrite(&bitordr, sizeof(bitordr), 1, h_nuxfile);
	fwrite(&dpmx, sizeof(dpmx), 1, h_nuxfile);
	fwrite(&dpmy, sizeof(dpmy), 1, h_nuxfile);
	fwrite(&alpha, sizeof(alpha), 1, h_nuxfile);
	data->WriteBin(h_nuxfile, version);
	ctbl->WriteBin(h_nuxfile, version);
	return true;
}
	
bool MImage::ReadBin(FILE* h_nuxfile, int version)  //reading from the UTP-file
{
	fread(&w, sizeof(w), 1, h_nuxfile);
	fread(&h, sizeof(h), 1, h_nuxfile);
	fread(&d, sizeof(d), 1, h_nuxfile);
	fread(&bitordr, sizeof(bitordr), 1, h_nuxfile);
	fread(&dpmx, sizeof(dpmx), 1, h_nuxfile);
	fread(&dpmy, sizeof(dpmy), 1, h_nuxfile);
	fread(&alpha, sizeof(alpha), 1, h_nuxfile);
	data->ReadBin(h_nuxfile, version);
	ctbl->ReadBin(h_nuxfile, version);
	return true;
}

bool MImage::ReadBin(MFile* file, int version)  //reading from the UTP-file
{
	file->GetData(&w, sizeof(w), 1);
	file->GetData(&h, sizeof(h), 1);
	file->GetData(&d, sizeof(d), 1);
	file->GetData(&bitordr, sizeof(bitordr), 1);
	file->GetData(&dpmx, sizeof(dpmx), 1);
	file->GetData(&dpmy, sizeof(dpmy), 1);
	file->GetData(&alpha, sizeof(alpha), 1);
	data->ReadBin(file, version);
	ctbl->ReadBin(file, version);
	return true;
}

void MImage::Copy(MImage* image)
{
    //if is what to copy
	if(image != NULL){
		
		w = image->w;
		h = image->h;
		d = image->d;
		bitordr = image->bitordr;
		dpmx = image->dpmx;
		dpmy = image->dpmy;
		alpha = image->alpha;
		data = (MByArray *) image->data->Clone();
		ctbl = (MInArray *) image->ctbl->Clone();				
	}
	else
	{
		w = h = d = bitordr = dpmx = dpmy = alpha = 0;
		data = new MByArray();
		ctbl = new MInArray();
	}
}

void MImage::getBits (uchar* bits)
{
	for (int i = 0; i < numBytes(); i++)
		bits[i] = *data->GetAt(i);
}

void MImage::getColorTable (uint* colors)
{
	for (int i = 0; i < numColors(); i++)
		colors[i] = *ctbl->GetAt(i);
}

void MImage::setBits (uchar* bits, int numBytes)
{
	data->ClearAll();
	for (int i = 0; i < numBytes; i++)
	{
		uchar value = bits[i];
		data->AddTail(&value);
	}
}

void MImage::setColorTable (uint* colors, int numColors)
{
	ctbl->ClearAll();
	for (int i = 0; i < numColors; i++)
	{
		int color = colors[i];
		ctbl->AddTail(&color);
	}
}

int MImage::format()
{
	if (d == 1)
	{
		if (bitordr == 1)
			return 1; // QImage::Format_Mono
		if (bitordr == 2)
			return 2; // QImage::Format_MonoLSB
	}
	if (d == 8)
		return 3; // QImage::Format_Indexed8
	if (d == 32)
	{
		if (alpha == 0)
			return 4; // QImage::Format_RGB32
		if (alpha == 1)
			return 5; // QImage::Format_ARGB32

	}
	return 0;	
}

void MImage::setFormat(int format)
{
	d = bitordr = alpha = 0;
	switch (format)
	{
		case 1: // QImage::Format_Mono
			d = 1;
			bitordr = 1;
			break;
		case 2: // QImage::Format_MonoLSB
			d = 1;
			bitordr = 2;
			break;
		case 3: // QImage::Format_Indexed8
			d = 8;
			break;
		case 4: // QImage::Format_RGB32
			d = 32;
			break;
		case 5: // QImage::Format_ARGB32
			d = 32;
			alpha = 1;
			break;
	}
}
