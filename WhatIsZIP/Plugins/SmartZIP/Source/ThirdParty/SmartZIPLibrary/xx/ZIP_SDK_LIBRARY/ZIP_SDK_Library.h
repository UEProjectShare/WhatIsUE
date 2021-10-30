#ifndef ZIP_SDK_LIBRARY_H
#define ZIP_SDK_LIBRARY_H
#ifdef GS_EXPORTS
#define SMARTZIPLIBRARY_API __declspec(dllexport)
#else
#define SMARTZIPLIBRARY_API __declspec(dllimport)
#endif // EEE

enum e_zip_typ
{
	e_zip,
	e_unzip,
};
//1. 总数
//2. 剩余
//3. 名字
//#ifdef __cplusplus
//extern "C"
//{
//#endif

SMARTZIPLIBRARY_API unsigned long __stdcall zip_file_by_loacl(const char* src_path, const char* dest_filename, const char* password, bool b_overwrite);
SMARTZIPLIBRARY_API unsigned long __stdcall unzip_file_by_loacl(const char* src_path, const char* dest_filename, const char* password, bool b_overwrite);

//异步考虑
SMARTZIPLIBRARY_API void __stdcall zip_file_Callback_function(void(*zip_bind_fun)(e_zip_typ, int, int, const wchar_t*));
SMARTZIPLIBRARY_API void __stdcall clear_Callback_function();
//#ifdef __cplusplus
//}
//#endif
#endif // ZIP_SDK_Library