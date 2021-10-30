#include "ZIP_SDK_Library.h"
#include "Windows.h"
#include "zip.h"
#include "unzip.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <Strsafe.h> 

struct zip_bind_fun_instance_struct
{
	zip_bind_fun_instance_struct()
		:bfun(false)
	{}

	void(*zip_bind_fun_instance)(e_zip_typ, int, int, const wchar_t*);
	bool bfun;
};
zip_bind_fun_instance_struct fun_instance_struct;

#define ZIP_ERROR -1

#define ZIP_LEN_1024 1024

void recursive_directory(wchar_t dir[MAX_PATH], std::map<wchar_t*, bool>& f_map)
{
	WIN32_FIND_DATA w32_find_data;
	HANDLE h_Find = INVALID_HANDLE_VALUE;
	wchar_t dir_spec[MAX_PATH] = { 0 };

	StringCchCopy(dir_spec, MAX_PATH, dir);
	StringCchCat(dir_spec, MAX_PATH, L"\\*"); // FString A, A + TEXT("\\*");
	bool b_empty_folder = true;
	h_Find = FindFirstFile(dir_spec, &w32_find_data);
	if (h_Find == INVALID_HANDLE_VALUE)
	{
		FindClose(h_Find);

		wchar_t* path = new wchar_t[MAX_PATH];
		memset(path, 0x00, sizeof(path));
		wsprintf(path, L"%s", dir);
		f_map.insert(std::make_pair(path, true));

		return;
	}
	else
	{
		while (FindNextFile(h_Find, &w32_find_data) != 0)
		{
			if ((w32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 &&
				wcscmp(w32_find_data.cFileName, L".") == 0 ||
				wcscmp(w32_find_data.cFileName, L"..") == 0)
			{
				continue;
			}

			wchar_t dir_add[MAX_PATH] = { 0 };
			StringCchCopy(dir_add, MAX_PATH, dir);
			StringCchCat(dir_add, MAX_PATH, L"\\");
			StringCchCat(dir_add, MAX_PATH, w32_find_data.cFileName);
			if ((w32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				b_empty_folder = false;
				recursive_directory(dir_add, f_map);
			}
			else
			{
				b_empty_folder = false;
				wchar_t* path = new wchar_t[MAX_PATH];
				memset(path, 0x00, sizeof(path));
				wsprintf(path, L"%s", dir_add);
				f_map.insert(std::make_pair(path, false));
			}
		}

		if (b_empty_folder)
		{
			wchar_t* path = new wchar_t[MAX_PATH];
			memset(path, 0x00, sizeof(path));
			wsprintf(path, L"%s", dir);
			f_map.insert(std::make_pair(path, true));
		}

		FindClose(h_Find);
	}
}

int get_clean_filename(wchar_t* path, wchar_t* file_name)
{
	//c://user//dir//fname.ext
	wchar_t drive[_MAX_DRIVE];// c:
	wchar_t dir[_MAX_DIR];// //user//dir//
	wchar_t fname[_MAX_FNAME];//fname
	wchar_t ext[_MAX_EXT];//.ext
	int ret = _wsplitpath_s(path, drive, dir, fname, ext);
	wsprintf(file_name, L"%s%s", fname, ext);

	return ret;
}

//为异步考虑
void __stdcall zip_file_Callback_function(void(*zip_bind_fun)(e_zip_typ, int, int, const wchar_t*))
{
	fun_instance_struct.zip_bind_fun_instance = zip_bind_fun;
	fun_instance_struct.bfun = true;
}

void __stdcall clear_Callback_function()
{
	fun_instance_struct.bfun = false;
}

unsigned long __stdcall zip_file_by_loacl(const char* src_path, const char* dest_filename, const char* password, bool b_overwrite)
{
	ZRESULT zr;
	wchar_t ssrc_path[ZIP_LEN_1024 * 2] = {};
	wchar_t sdest_filename[ZIP_LEN_1024 * 2] = {};
	char spassword[ZIP_LEN_1024 * 2] = {};

	memset(ssrc_path, 0x00, sizeof(ssrc_path));
	memset(sdest_filename, 0x00, sizeof(sdest_filename));
	int iii = strlen(src_path);
	MultiByteToWideChar(CP_ACP, 0, src_path, strlen(src_path) + 1, ssrc_path, sizeof(ssrc_path) / sizeof(ssrc_path[0]));
	MultiByteToWideChar(CP_ACP, 0, dest_filename, strlen(dest_filename) + 1, sdest_filename, sizeof(sdest_filename) / sizeof(sdest_filename[0]));
	sprintf_s(spassword, "%s", password);

	char* ppassword = ((strlen(spassword) == 0 || strcmp(spassword, "") == 0) ? NULL : spassword);

	HZIP hz = CreateZip(sdest_filename, ppassword);

	DWORD wd = GetFileAttributes(ssrc_path);

	if (wd == INVALID_FILE_ATTRIBUTES)
	{

	}
	else if (wd == FILE_ATTRIBUTE_DIRECTORY)
	{
		std::map<wchar_t*, bool> f_map_path;
		recursive_directory(ssrc_path, f_map_path);

		int i = 0;
		for (std::map<wchar_t*, bool>::const_iterator m_iterator = f_map_path.cbegin(); m_iterator != f_map_path.cend(); m_iterator++)
		{
			i++;
			if (fun_instance_struct.bfun)
			{
				fun_instance_struct.zip_bind_fun_instance(e_zip_typ::e_zip, f_map_path.size(), i, m_iterator->first);
			}

			if (!wcscmp(m_iterator->first, ssrc_path))
			{
				continue;
			}

			wchar_t wchar_t_add_path[MAX_PATH] = {};
			memset(wchar_t_add_path, 0x00, sizeof(wchar_t_add_path));
			wsprintf(wchar_t_add_path, L"%s", m_iterator->first + wcslen(ssrc_path) + 1);

			if (!m_iterator->second)
			{
				zr = ZipAdd(hz, wchar_t_add_path, m_iterator->first);
			}
			else
			{
				zr = ZipAddFolder(hz, wchar_t_add_path);
			}

			if (zr != ZR_OK)
			{
				break;
			}
		}

		//完成后执行该接口
		if (fun_instance_struct.bfun)
		{
			fun_instance_struct.zip_bind_fun_instance(e_zip_typ::e_zip, f_map_path.size(), f_map_path.size(), NULL);
		}

		f_map_path.erase(f_map_path.begin(), f_map_path.end());
	}
	else
	{
		wchar_t file_name[MAX_PATH] = {};
		get_clean_filename(ssrc_path, file_name);
		ZRESULT zr = ZipAdd(hz, file_name, ssrc_path);
		if (zr != ZR_OK)
		{

		}
	}

	CloseZip(hz);

	return zr;
}

unsigned long __stdcall unzip_file_by_loacl(const char* src_path, const char* dest_filename, const char* password, bool b_overwrite)
{
	ZRESULT zt;
	wchar_t ssrc_path[ZIP_LEN_1024 * 2] = {};
	wchar_t sdest_filename[ZIP_LEN_1024 * 2] = {};
	char spassword[ZIP_LEN_1024 * 2] = {};

	memset(ssrc_path, 0x00, sizeof(ssrc_path));
	memset(sdest_filename, 0x00, sizeof(sdest_filename));

	MultiByteToWideChar(CP_ACP, 0, src_path, strlen(src_path) + 1, ssrc_path, sizeof(ssrc_path) / sizeof(ssrc_path[0]));
	MultiByteToWideChar(CP_ACP, 0, dest_filename, strlen(dest_filename) + 1, sdest_filename, sizeof(sdest_filename) / sizeof(sdest_filename[0]));
	sprintf_s(spassword, "%s", password);

	char* ppassword = ((strlen(spassword) == 0 || strcmp(spassword, "") == 0) ? NULL : spassword);

	if (!b_overwrite)
	{
		WIN32_FIND_DATA w32_find_data;
		HANDLE h_Find = FindFirstFile(sdest_filename, &w32_find_data);
		if (h_Find != INVALID_HANDLE_VALUE)
		{
			CloseHandle(h_Find);
			return false;
		}
	}

	HZIP hz = OpenZip(ssrc_path, ppassword);
	SetUnzipBaseDir(hz, sdest_filename);

	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int zip_number = ze.index;

	for (int i = 0; i < zip_number; i++)
	{
		GetZipItem(hz, i, &ze);
		zt = UnzipItem(hz, i, ze.name);

		//通知接口
		if (fun_instance_struct.bfun)
		{
			fun_instance_struct.zip_bind_fun_instance(e_zip_typ::e_unzip, zip_number, i, ze.name);
		}
	}

	//完成后执行该接口
	if (fun_instance_struct.bfun)
	{
		fun_instance_struct.zip_bind_fun_instance(e_zip_typ::e_unzip, zip_number, zip_number, NULL);
	}

	CloseZip(hz);
	return zt;
}
