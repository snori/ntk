/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: translationutility.cpp,v 1.6 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/translation/translationutility.h"


#include <vector>
#include <boost/scoped_array.hpp>
#include <ntk/windows/windows.h>
#include <ntk/storage/file.h>
#include <ntk/support/debug.h>
#include <ntk/interface/bitmap.h>


namespace ntk {


//########################################################

Bitmap*
TranslationUtils::get_bitmap(const String& file_name)
{
	File file;
	if(file.set_to(file_name, File::READ_ONLY) == false)
		return NULL;

	// FILEHEADER �̓ǂݍ���
	BITMAPFILEHEADER file_header;
	file >> file_header;
	if(file_header.bfType != *reinterpret_cast<word*>("BM"))
		return NULL;

	// INFOHEADER �̓ǂݍ���
	Bitmap::BitmapInfoHeader info_header;
	file >> info_header;

	if(info_header.bit_count < 8)
	{
		status_t(st::ERR, format_string("%d", info_header.bit_count)
			+ "�r�b�g�̃r�b�g�}�b�v�ɂ͑Ή����Ă��܂��� : " + file_name).show_error();
		return NULL;
	}

	// �J���[�p���b�g��
	int num_palettes = info_header.bit_count == 8 ? 256 : 0;

	// �t�@�C���|�C���^�� RGB ���̐擪��
//	size_t offset = sizeof(BITMAPFILEHEADER) + sizeof(Bitmap::BitmapInfoHeader) + sizeof(RGBQUAD) * num_palettes;
	File::Size new_position = file.seek(file_header.bfOffBits);// offset);
	if(new_position != file_header.bfOffBits)
	{
		status_t(st::FAILED, "�t�@�C���̓ǂݍ��݂����s���܂��� : " + file_name).show_error();
		return NULL;
	}

	Bitmap* bitmap = new Bitmap(info_header.width, info_header.height, (Bitmap::ColorSpace)info_header.bit_count);
	int y1 = 0, y2 = bitmap->height(), dy = 1;

	// �����t�@�C�����{�g���A�b�v��������o�b�t�@�̍Ōォ��t���ɓǂݍ���
	if(info_header.height >= 0)
	{
		y1 = y2-1;
		y2 = -1;
		dy = -1;
	}

	for(int y=y1; y!=y2; y += dy)
	{
		File::Size read_size = file.read(bitmap->at(0, y), bitmap->pitch());
		if(read_size != bitmap->pitch())
		{
			delete bitmap;
			return NULL;
		}
	}

	return bitmap;
}

status_t
TranslationUtils::store_bitmap(const String& file_name, const Bitmap& bitmap)
{
	FILE file;
	status_t sts = file.set_to(file_name, "wb");
	if(sts.is_valid_without_eval() == false)
		return sts;

	// FileHeader �̏�������
	BITMAPFILEHEADER file_header;
	memset(&file_header, 0, sizeof(file_header));
	file_header.bfType = *reinterpret_cast<word*>("BM");
	file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(Bitmap::BitmapInfoHeader);// + sizeof(RGBQUAD) * num_palettes
	file_header.bfSize = file_header.bfOffBits + bitmap.size();
	file << file_header;

	// InfoHeader �̏�������
	Bitmap::BitmapInfoHeader& info_header = const_cast<Bitmap::BitmapInfoHeader&>(bitmap.m_bitmap_info.info_header);

	if(info_header.height < 0)
	{
		info_header.height *= -1;// �ʏ�Ƃ͋t�Ȃ̂Ŕ��]
		file << info_header;
		info_header.height *= -1;

		for(int y=bitmap.height() -1; y >= 0; --y)
			file.write(bitmap.at(0, y), bitmap.pitch());
	} else {
		file << info_header;
		file.write(bitmap.bits(), bitmap.size());
	}

	return st::OK;
}


//########################################################
}// namespace ntk
