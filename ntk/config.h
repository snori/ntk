/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: config.h,v 1.9 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_CONFIG_H__
#define __NTK_CONFIG_H__


//--------------------------------------------------------
// global configurations


#define NTK_LANG_JP
/*
 * ����ʂ̃R�[�h����������ꍇ�A���{���I�����܂��B
 */


//#define NTK_STATIC_LINK
/*
 * ���̃}�N�����`����ƃG�N�X�|�[�g�p�}�N�� (NtkExport ��)
 * �𖳌������܂��B
 */


//#define NTK_UNICODE
/*
 * ���̃}�N������`����Ă���ƈȉ��_���ύX����܂�
 * �Estring : std::basic_string<char> --> std::basic_string<wchar_t>
 * �Echar_t :                    char --> wchar_t
 * �E���̑��������
 */


//#define NTK_DISABLE_WARNING_4018 // signed �� unsigned ���r�����Ƃ��ɏo��x��
#define NTK_DISABLE_WARNING_4786 // ���ʎq��256�����ȏ�ɂȂ������ɂł�x��
/*
 * �i�璷�ȁj�x�����A�����ɂ��܂��B
 */


//#define NTK_ENABLE_ALL_WARNINGS
/*
 * ���̃}�N�����`����ƁA��̌ʂ̌x���������𖳎������ׂėL���ɂ��܂��B
 */


//#define NTK_CHECK_MEMORY_LEAKS
/*
 * �����t�@�C���̐擪������ new �̑���� NTK_NEW ���g�p���邱�ƂŃ��������[�N��
 * ���o�@�\�𗘗p�ł���悤�ɂȂ�܂��B
 */


//#define NTK_USE_DEBUG_NEW
/*
 * ���̃}�N������`����Ă���ƁA
 *
 *	#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
 *
 * ����`����A�W���� new �ł���̃��������[�N���o�@�\�������悤�ɂȂ�܂��B
 */


#define NTK_USING_BASIC_TYPES
/*
 * int32 ���̊�{�^���O���[�o�����O��Ԃ� using ����B���̃��C�u�����Ɣ�铙�A
 * ��肪����ꍇ�̓R�����g�A�E�g���Ă��������Busing ���Ȃ��悤�ɂȂ�܂��B
 */


//#define NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
/*
 * ������2���ڂƑg�ݍ��킹�ANTK �̌��J����Ă���^�̎��ʎq�����O���[�o����
 * typedef ���܂��B
 */


#define NTK_TYPEDEF_GLOBAL_NCLASS
/*
 * ::ntk::Class -> ::NClass �� typedef (template class �Ɍ��� #define)
 * ��L���ɂ��܂��B
 * �i���j ���̍��ڂ͐�� NTK_TYPEDEF_TYPES_AS_GLOBAL ���L���Ȏ��̂ݗL���ɂȂ�܂�
 */


#define NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
/*
 * ::ntk::Class -> ::ntk_class �� typedef (template class �Ɍ��� #define)
 * ��L���ɂ��܂��B
 * �i���j ���̎w��͐�� NTK_TYPEDEF_TYPES_AS_GLOBAL ���L���Ȏ��̂ݗL���ɂȂ�܂�
 *
 * �܂��g�b�v���x���X�R�[�v�̃N���X���ɂ�����J����Ă���^��
 * ::ntk::DataIO::Stat -> ::ntk::DataIO::stat_t �̂悤�� (...)_t �Ƃ���
 * typedef ����܂��B
 * �i���j ����� NTK_TYPEDEF_TYPES_AS_GLOBAL �Ƃ͊֌W�Ȃ��L���ł��B
 */


#define NTK_DEF_MAIN
/*
 * ���̃}�N������`����Ă���ƁA
 *
 * 	#define main ntk_main
 *
 * ����`����܂��B
 */


//#define NTK_WINDOW_OWN_THREAD
/*
 * *OBSOLETE*
 * ���̐ݒ��L���ɂ���ƁAWindow �N���X�����ꂼ��X���b�h�����悤�ɂȂ�܂��B
 * ���݂́A���̎�@�ƃE�B���h�E�YOS�Ƃ̑������������Ƃ��킩�������߁A�C���v����
 * ���g����Ă��܂���B
 */


#define NTK_NUM_BITS_OF_FLAG 32
/*
 * �����^���t���O�Ƃ��ė��p���鎞�̃r�b�g��
 */


#define NTK_ENABLE_SCINTILLA
/*
 * ���̐ݒ肪�L���̏ꍇ ntk::Scintilla �R���g���[�����g�p�\�ɂȂ�܂��B
 * ���̏ꍇ�A�w�b�_�p�X�̐ݒ�ƁAScintilla.dll/SciLexer.dll �����p�\�ɂȂ���
 * ����K�v������܂��B
 */


//--------------------------------------------------------
// basic type configurations


#ifndef NTK_INT8_TYPE
#define NTK_INT8_TYPE char
//#define NTK_INT8_TYPE _int8
#endif

#ifndef NTK_INT16_TYPE
#define NTK_INT16_TYPE short
//#define NTK_INT16_TYPE _int16
#endif

#ifndef NTK_INT32_TYPE
#define NTK_INT32_TYPE long
//#define NTK_INT32_TYPE _int32
#endif

#ifndef NTK_INT64_TYPE
//#define NTK_INT64_TYPE long long
#define NTK_INT64_TYPE _int64
#endif

#ifndef NTK_BYTE_TYPE
#define NTK_BYTE_TYPE char
#endif

#ifndef NTK_WORD_TYPE
#define NTK_WORD_TYPE short
#endif

#ifndef NTK_DWORD_TYPE
#define NTK_DWORD_TYPE long
#endif

#ifndef NTK_QWORD_TYPE
#define NTK_QWORD_TYPE _int64
#endif


#endif//EOH
