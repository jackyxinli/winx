/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: winx/gdiplus/ImageCodec.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-23 12:07:42
// 
// $Id: ImageCodec.h,v 1.2 2006/09/02 01:49:08 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_GDIPLUS_IMAGECODEC_H
#define WINX_GDIPLUS_IMAGECODEC_H

#pragma pack(1)

// -------------------------------------------------------------------------
// EncoderParameterValueType

struct LONGRANGE
{
	LONG minValue;
	LONG maxValue;
};

struct RATIONAL
{
	LONG nNumerator;
	LONG nDenomintor;
};

struct RATIONALRANGE
{
	RATIONAL minValue;
	RATIONAL maxValue;
};

// -------------------------------------------------------------------------
// GetEncoderClsid

inline INT winx_call GetEncoderClsid(LPCWSTR format, CLSID* pClsid)
{
	UINT  num = 0;  // number of image encoders
	UINT  size = 0; // size of the image encoder array in bytes
	
	ImageCodecInfo* pImageCodecInfo = NULL;
	
	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure
	
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure
	
	GetImageEncoders(num, size, pImageCodecInfo);
	
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}
	
	free(pImageCodecInfo);
	return -1;  // Failure
}

// -------------------------------------------------------------------------
// EnhMetaFile2WinMetaFile

inline VOID winx_call EnhMetaFile2WinMetaFile(LPCWSTR szSrcFile, LPCWSTR szDestFile)
{
	// Construct a Metafile object from an existing EMF disk file.
	Metafile myMetafile(szSrcFile);
	
	// Get a handle to the EMF metafile.
	HENHMETAFILE hEmf = myMetafile.GetHENHMETAFILE();
	
	// Determine the size of the buffer that will receive the converted records.
	UINT size = Metafile::EmfToWmfBits(
		hEmf, 
		0, 
		NULL,
		MM_ANISOTROPIC,
		EmfToWmfBitsFlagsEmbedEmf);
	
	// Allocate a buffer to receive the converted records.
	BYTE* buffer = new BYTE[size];
	
	// Convert the EMF records to WMF records.
	INT convertedSize = Metafile::EmfToWmfBits(
		hEmf, 
		size,
		buffer, 
		MM_ANISOTROPIC,
		EmfToWmfBitsFlagsEmbedEmf);
	
	// Get a handle to the converted metafile.
	HMETAFILE hmf = SetMetaFileBitsEx(size, buffer);
	
	// Write the WMF metafile to a disk file.
	USES_CONVERSION;
	::CopyMetaFile(hmf, W2CT(szDestFile));
	
	DeleteMetaFile(hmf);
	DeleteEnhMetaFile(hEmf);
	delete[] buffer;
}

// -------------------------------------------------------------------------
// TransformJpegWithoutLossOfInformation
//	- category: Platform SDK: GDI+
//	- topic: Transforming a JPEG Image Without Loss of Information

inline Status winx_call TransformJpegWithoutLossOfInformation(LPCWSTR szJpegFile)
{
	CLSID             encoderClsid;
	EncoderParameters encoderParameters;
	ULONG             transformation;
	Status            status;

	// Get a JPEG image from the disk.
	Image image(szJpegFile);

#if defined(_DEBUG)
	// Determine whether the width and height of the image 
	// are multiples of 16.
	UINT width = image.GetWidth();
	UINT height = image.GetHeight();

	printf("The width of the image is %u", width);
	if(width / 16.0 - width / 16 == 0)
	  printf(", which is a multiple of 16.\n");
	else
	  printf(", which is not a multiple of 16.\n");

	printf("The height of the image is %u", height);
	if(height / 16.0 - height / 16 == 0)
	  printf(", which is a multiple of 16.\n");
	else
	  printf(", which is not a multiple of 16.\n");
#endif

	// Get the CLSID of the JPEG encoder.
	GetEncoderClsid(L"image/jpeg", &encoderClsid);

	// Before we call Image::Save, we must initialize an
	// EncoderParameters object. The EncoderParameters object
	// has an array of EncoderParameter objects. In this
	// case, there is only one EncoderParameter object in the array.
	// The one EncoderParameter object has an array of values.
	// In this case, there is only one value (of type ULONG)
	// in the array. We will set that value to EncoderValueTransformRotate90.

	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderTransformation;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;

	// Rotate and save the image.
	transformation = EncoderValueTransformRotate90;
	encoderParameters.Parameter[0].Value = &transformation;
	status = image.Save(L"ShapesR90.jpg", &encoderClsid, &encoderParameters);

#if defined(_DEBUG)
	if(status == Ok)
	  wprintf(L"%s saved successfully.\n", L"ShapesR90.jpg");
	else
	  wprintf(L"%d  Attempt to save %s failed.\n", status, L"ShapesR90.jpg");
#endif

	return status;
}

// -------------------------------------------------------------------------
// ValueTypeFromULONG

inline VOID winx_call ValueTypeFromULONG(ULONG index, WCHAR* strValueType)
{
	WCHAR* valueTypes[] = {
		L"Nothing",         // Nothing
		L"Byte",            // EncoderParameterValueTypeByte
		L"ASCII",           // EncoderParameterValueTypeASCII
		L"Short",           // EncoderParameterValueTypeShort
		L"Long",            // EncoderParameterValueTypeLong
		L"Rational",        // EncoderParameterValueTypeRational
		L"LongRange",       // EncoderParameterValueTypeLongRange
		L"Undefined",       // EncoderParameterValueTypeUndefined
		L"RationalRange"	// EncoderParameterValueTypeRationalRange
	};
	wcscpy(strValueType, valueTypes[index]);		
}

// -------------------------------------------------------------------------
// EncoderParameterCategoryFromGUID

inline VOID winx_call EncoderParameterCategoryFromGUID(GUID guid, WCHAR* category)
{
	if(guid == EncoderCompression)
		wcscpy(category, L"Compression");
	else if(guid == EncoderColorDepth)
		wcscpy(category, L"ColorDepth");
	else if(guid == EncoderScanMethod)
		wcscpy(category, L"ScanMethod");
	else if(guid == EncoderVersion)
		wcscpy(category, L"Version");
	else if(guid == EncoderRenderMethod)
		wcscpy(category, L"RenderMethod");
	else if(guid == EncoderQuality)
		wcscpy(category, L"Quality");
	else if(guid == EncoderTransformation)
		wcscpy(category, L"Transformation");
	else if(guid == EncoderLuminanceTable)
		wcscpy(category, L"LuminanceTable");
	else if(guid == EncoderChrominanceTable)
		wcscpy(category, L"ChrominanceTable");
	else if(guid == EncoderSaveFlag)
		wcscpy(category, L"SaveFlag");
	else
		wcscpy(category, L"Unknown category");
}

// -------------------------------------------------------------------------
// ListImageEncoderParameters

template <class LogT>
inline VOID winx_call ListImageEncoderParameters(LogT& log, ImageCodecInfo* pImageCodecInfo)
{
	log.trace(
		L"---> %s [%s(%s)] %s\n",
		pImageCodecInfo->MimeType,
		pImageCodecInfo->FormatDescription,
		pImageCodecInfo->FilenameExtension,
		pImageCodecInfo->CodecName);

	WCHAR strParameterCategory[50] = L"";
	WCHAR strValueType[50] = L"";
	
	// Create a Bitmap (inherited from Image) object so that we can call
	// GetParameterListSize and GetParameterList.
	Bitmap bitmap(1, 1);
	
	// How big (in bytes) is the encoder's parameter list?
	UINT listSize = 0; 
	listSize = bitmap.GetEncoderParameterListSize(&pImageCodecInfo->Clsid);	
	if(listSize == 0)
		return;

	// Allocate a buffer large enough to hold the parameter list.
	EncoderParameters* pEncoderParameters = NULL;
	pEncoderParameters = (EncoderParameters*)malloc(listSize);
	
	// Get the parameter list for the encoder.
	bitmap.GetEncoderParameterList(
		&pImageCodecInfo->Clsid, listSize, pEncoderParameters);
	
	// pEncoderParameters points to an EncoderParameters object, which
	// has a Count member and an array of EncoderParameter objects.
	// How many EncoderParameter objects are in the array?
	log.trace(L"  There are %d EncoderParameter objects in the array.\n",
		pEncoderParameters->Count);
	
	// For each EncoderParameter object in the array, list the
	// parameter category, data type, and number of values.
	for(UINT k = 0; k < pEncoderParameters->Count; ++k)
	{
		EncoderParameterCategoryFromGUID(
			pEncoderParameters->Parameter[k].Guid, strParameterCategory);
		
		ValueTypeFromULONG(
			pEncoderParameters->Parameter[k].Type, strValueType);
		
		log.trace(L"    Parameter[%d] - %s\n", k, strParameterCategory);
		log.trace(L"      The data type is %s.\n", strValueType);
		switch (pEncoderParameters->Parameter[k].Type)
		{
		case EncoderParameterValueTypeLong:
			{
				LONG* pLong = (LONG*)pEncoderParameters->Parameter[k].Value;
				for (UINT t = 0; t < pEncoderParameters->Parameter[k].NumberOfValues; ++t)
					log.trace(L"      The allowable value[%d]: %d\n", t, pLong[t]);
			}
			break;
		case EncoderParameterValueTypeLongRange:
			{
				LONGRANGE* pLongRange = (LONGRANGE*)pEncoderParameters->Parameter[k].Value;
				for (UINT t = 0; t < pEncoderParameters->Parameter[k].NumberOfValues; ++t)
					log.trace(L"      The allowable value[%d]: %d to %d\n",
						t, pLongRange[t].minValue, pLongRange[t].maxValue);
			}
			break;
		case EncoderParameterValueTypeShort:
			{
				SHORT* pShort = (SHORT*)pEncoderParameters->Parameter[k].Value;
				for (UINT t = 0; t < pEncoderParameters->Parameter[k].NumberOfValues; ++t)
					log.trace(L"      The allowable value[%d]: %d\n", t, pShort[t]);
			}
			break;
		}
	}
	
	free(pEncoderParameters);
}

// -------------------------------------------------------------------------
// ListImageEncoders

template <class LogT>
inline VOID winx_call ListImageEncoders(LogT& log)
{
	UINT  num;        // number of image encoders
	UINT  size;       // size, in bytes, of the image encoder array
	
	ImageCodecInfo* pImageCodecInfo;
	
	// How many encoders are there?
	// How big (in bytes) is the array of all ImageCodecInfo objects?
	GetImageEncodersSize(&num, &size);
	
	// Create a buffer large enough to hold the array of ImageCodecInfo
	// objects that will be returned by GetImageEncoders.
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	
	// GetImageEncoders creates an array of ImageCodecInfo objects
	// and copies that array into a previously allocated buffer. 
	// The third argument, imageCodecInfo, is a pointer to that buffer. 
	GetImageEncoders(num, size, pImageCodecInfo);
	
	// Display the graphics file format (MimeType)
	// for each ImageCodecInfo object.
	for(UINT j = 0; j < num; ++j)
	{ 
		ListImageEncoderParameters(log, &pImageCodecInfo[j]);
		log.newline();
	}
	
	free(pImageCodecInfo);
}

// -------------------------------------------------------------------------
// ListImageDecoders

template <class LogT>
inline VOID winx_call ListImageDecoders(LogT& log)
{
	UINT  num;        // number of image decoders
	UINT  size;       // size, in bytes, of the image decoder array
	
	ImageCodecInfo* pImageCodecInfo;
	
	// How many decoders are there?
	// How big (in bytes) is the array of all ImageCodecInfo objects?
	GetImageDecodersSize(&num, &size);
	
	// Create a buffer large enough to hold the array of ImageCodecInfo
	// objects that will be returned by GetImageDecoders.
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	
	// GetImageDecoders creates an array of ImageCodecInfo objects
	// and copies that array into a previously allocated buffer. 
	// The third argument, imageCodecInfo, is a pointer to that buffer. 
	GetImageDecoders(num, size, pImageCodecInfo);
	
	// Display the graphics file format (MimeType)
	// for each ImageCodecInfo object.
	for(UINT j = 0; j < num; ++j)
	{ 
		log.trace(L"%s\n", pImageCodecInfo[j].MimeType);
	}
	
	free(pImageCodecInfo);
}

// -------------------------------------------------------------------------
// SaveImage

template <class Ouput>
inline Status winx_call SaveImage(
	Image& image, Ouput output, LPCWSTR format, IN const EncoderParameters* encoderParams = NULL)
{
	CLSID clsid;
	INT result = GetEncoderClsid(format, &clsid);
	WINX_ASSERT(result != -1);
	if (result == -1)
		return UnknownImageFormat;
	
	Status status = image.Save(output, &clsid, encoderParams);
	WINX_ASSERT(status == Ok);
	return status;
}

// -------------------------------------------------------------------------
// SetEncoderParameter

inline VOID winx_call SetEncoderParameter(EncoderParameter& param, const GUID& guid, LONG* value)
{
	param.Guid = guid;
	param.NumberOfValues = 1;
	param.Type = PropertyTagTypeLong;
	param.Value = value;
}

inline VOID winx_call SetEncoderColorDepth(EncoderParameter& param, LONG* value)
{
	SetEncoderParameter(param, EncoderColorDepth, value);
}

inline VOID winx_call SetEncoderTransformation(EncoderParameter& param, LONG* value)
{
	SetEncoderParameter(param, EncoderTransformation, value);
}

inline VOID winx_call SetEncoderSaveFlag(EncoderParameter& param, LONG* value)
{
	SetEncoderParameter(param, EncoderSaveFlag, value);
}

inline VOID winx_call SetEncoderCompression(EncoderParameter& param, LONG* value)
{
	SetEncoderParameter(param, EncoderCompression, value);
}

inline VOID winx_call SetEncoderQuality(EncoderParameter& param, LONG* value)
{
	SetEncoderParameter(param, EncoderQuality, value);
}

// -------------------------------------------------------------------------

class ColorDepthParameter : public EncoderParameters
{
public:
	LONG LongValue;
	ColorDepthParameter(LONG value) {
		Count = 1;
		LongValue = value;
		SetEncoderColorDepth(Parameter[0], &LongValue);
	}
};

class TransformationParameter : public EncoderParameters
{
public:
	LONG LongValue;
	TransformationParameter(LONG value) {
		Count = 1;
		LongValue = value;
		SetEncoderTransformation(Parameter[0], &LongValue);
	}
};

class SaveFlagParameter : public EncoderParameters
{
public:
	LONG LongValue;
	SaveFlagParameter(LONG value) {
		Count = 1;
		LongValue = value;
		SetEncoderSaveFlag(Parameter[0], &LongValue);
	}
};

class CompressionParameter : public EncoderParameters
{
public:
	LONG LongValue;
	CompressionParameter(LONG value) {
		Count = 1;
		LongValue = value;
		SetEncoderCompression(Parameter[0], &LongValue);
	}
};

class QualityParameter : public EncoderParameters
{
public:
	LONG LongValue;
	QualityParameter(LONG value) {
		Count = 1;
		LongValue = value;
		SetEncoderQuality(Parameter[0], &LongValue);
	}
};

// -------------------------------------------------------------------------
// class MultiFrameImageCreator

template <class OutputT>
class MultiFrameImageCreatorT
{
private:
	SaveFlagParameter m_saveFlag;
	const CLSID& m_encoderClsid;
	OutputT m_output;
	Image* m_multi;
	
public:
	MultiFrameImageCreatorT(OutputT output, const CLSID& encoderClsid)
		: m_saveFlag(EncoderValueMultiFrame),
		  m_output(output),
		  m_encoderClsid(encoderClsid),
		  m_multi(NULL)
	{
	}
	~MultiFrameImageCreatorT()
	{
		if (m_multi)
			Done();
	}

	Status winx_call Add(Image& frame)
	{
		if (m_multi)
		{
			Status status = m_multi->SaveAdd(&frame, &m_saveFlag);
			WINX_ASSERT(status == Ok);
			return status;
		}
		else
		{
			m_multi = frame.Clone();
			Status status = m_multi->Save(m_output, &m_encoderClsid, &m_saveFlag);
			WINX_ASSERT(status == Ok);
			m_saveFlag.LongValue = EncoderValueFrameDimensionPage;
			return status;			
		}
	}

	Status winx_call Done()
	{
		m_saveFlag.LongValue = EncoderValueFlush;
	
		Status status = m_multi->SaveAdd(&m_saveFlag);
		WINX_ASSERT(status == Ok);
		
		delete m_multi;
		m_multi = NULL;
		
		return status;
	}
};

typedef MultiFrameImageCreatorT<LPCWSTR> MultiFrameImageCreator;

// -------------------------------------------------------------------------
// $Log: ImageCodec.h,v $
// Revision 1.2  2006/09/02 01:49:08  xushiwei
// WINX-Basic:
//   Log: TraceWindow, Storage: TraceStorage
// WINX-Extension-Gdiplus:
//   EncoderParameters: ColorDepth, Transformation, SaveFlag, Compression, Quality
//
// Revision 1.1  2006/08/27 19:25:46  xushiwei
// WINX-Extension-Gdiplus:
//   image-encoders / image-decoders / MultiFrameImageCreator
//

#pragma pack()

#endif /* WINX_GDIPLUS_IMAGECODEC_H */
