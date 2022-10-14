///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2008-2010, Sony Pictures Imageworks Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
// Neither the name of the organization Sony Pictures Imageworks nor the
// names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_PYWSTRING_H
#define INCLUDED_PYWSTRING_H

#include <string>
#include <vector>

class pywstring
{
public:
	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @mainpage pystring
	///
	/// This is a set of functions matching the interface and behaviors of python string methods
	/// (as of python 2.3) using std::wstring.
	///
	/// Overlapping functionality ( such as index and slice/substr ) of std::wstring is included
	/// to match python interfaces.
	///

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @defgroup functions pystring
	/// @{


#define MAX_32BIT_INT 2147483647

//////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Return a copy of the string with only its first character capitalized.
///
	static std::wstring capitalize(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return centered in a string of length width. Padding is done using spaces.
	///
	static std::wstring center(const std::wstring & str, int width);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return the number of occurrences of substring sub in string S[start:end]. Optional
	/// arguments start and end are interpreted as in slice notation.
	///
	static int count(const std::wstring & str, const std::wstring & substr, int start = 0, int end = MAX_32BIT_INT);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return True if the string ends with the specified suffix, otherwise return False. With
	/// optional start, test beginning at that position. With optional end, stop comparing at that position.
	///
	static bool endswith(const std::wstring & str, const std::wstring & suffix, int start = 0, int end = MAX_32BIT_INT);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string where all tab characters are expanded using spaces. If tabsize
	/// is not given, a tab size of 8 characters is assumed.
	///
	static std::wstring expandtabs(const std::wstring & str, int tabsize = 8);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return the lowest index in the string where substring sub is found, such that sub is
	/// contained in the range [start, end). Optional arguments start and end are interpreted as
	/// in slice notation. Return -1 if sub is not found.
	///
	static int find(const std::wstring & str, const std::wstring & sub, int start = 0, int end = MAX_32BIT_INT);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Synonym of find right now. Python version throws exceptions. This one currently doesn't
	///
	static int index(const std::wstring & str, const std::wstring & sub, int start = 0, int end = MAX_32BIT_INT);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return true if all characters in the string are alphanumeric and there is at least one
	/// character, false otherwise.
	///
	static bool isalnum(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return true if all characters in the string are alphabetic and there is at least one
	/// character, false otherwise
	///
	static bool isalpha(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return true if all characters in the string are digits and there is at least one
	/// character, false otherwise.
	///
	static bool isdigit(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return true if all cased characters in the string are lowercase and there is at least one
	/// cased character, false otherwise.
	///
	static bool islower(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return true if there are only whitespace characters in the string and there is at least
	/// one character, false otherwise.
	///
	static bool isspace(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return true if the string is a titlecased string and there is at least one character,
	/// i.e. uppercase characters may only follow uncased characters and lowercase characters only
	/// cased ones. Return false otherwise.
	///
	static bool istitle(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return true if all cased characters in the string are uppercase and there is at least one
	/// cased character, false otherwise.
	///
	static bool isupper(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a string which is the concatenation of the strings in the sequence seq.
	/// The separator between elements is the str argument
	///
	static std::wstring join(const std::wstring & str, const std::vector< std::wstring > & seq);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return the string left justified in a string of length width. Padding is done using
	/// spaces. The original string is returned if width is less than str.size().
	///
	static std::wstring ljust(const std::wstring & str, int width);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string converted to lowercase.
	///
	static std::wstring lower(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string with leading characters removed. If chars is omitted or None,
	/// whitespace characters are removed. If given and not "", chars must be a string; the
	/// characters in the string will be stripped from the beginning of the string this method
	/// is called on (argument "str" ).
	///
	static std::wstring lstrip(const std::wstring & str, const std::wstring & chars = L"");

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string, concatenated N times, together.
	/// Corresponds to the __mul__ operator.
	/// 
	static std::wstring mul(const std::wstring & str, int n);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Split the string around first occurance of sep.
	/// Three strings will always placed into result. If sep is found, the strings will
	/// be the text before sep, sep itself, and the remaining text. If sep is
	/// not found, the original string will be returned with two empty strings.
	///
	static void partition(const std::wstring & str, const std::wstring & sep, std::vector< std::wstring > & result);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string with all occurrences of substring old replaced by new. If
	/// the optional argument count is given, only the first count occurrences are replaced.
	///
	static std::wstring replace(const std::wstring & str, const std::wstring & oldstr, const std::wstring & newstr, int count = -1);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return the highest index in the string where substring sub is found, such that sub is
	/// contained within s[start,end]. Optional arguments start and end are interpreted as in
	/// slice notation. Return -1 on failure.
	///
	static int rfind(const std::wstring & str, const std::wstring & sub, int start = 0, int end = MAX_32BIT_INT);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Currently a synonym of rfind. The python version raises exceptions. This one currently
	/// does not
	///
	static int rindex(const std::wstring & str, const std::wstring & sub, int start = 0, int end = MAX_32BIT_INT);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return the string right justified in a string of length width. Padding is done using
	/// spaces. The original string is returned if width is less than str.size().
	///
	static std::wstring rjust(const std::wstring & str, int width);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Split the string around last occurance of sep.
	/// Three strings will always placed into result. If sep is found, the strings will
	/// be the text before sep, sep itself, and the remaining text. If sep is
	/// not found, the original string will be returned with two empty strings.
	///
	static void rpartition(const std::wstring & str, const std::wstring & sep, std::vector< std::wstring > & result);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string with trailing characters removed. If chars is "", whitespace
	/// characters are removed. If not "", the characters in the string will be stripped from the
	/// end of the string this method is called on.
	///
	static std::wstring rstrip(const std::wstring & str, const std::wstring & chars = L"");

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Fills the "result" list with the words in the string, using sep as the delimiter string.
	/// If maxsplit is > -1, at most maxsplit splits are done. If sep is "",
	/// any whitespace string is a separator.
	///
	static void split(const std::wstring & str, std::vector< std::wstring > & result, const std::wstring & sep = L"", int maxsplit = -1);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Fills the "result" list with the words in the string, using sep as the delimiter string.
	/// Does a number of splits starting at the end of the string, the result still has the
	/// split strings in their original order.
	/// If maxsplit is > -1, at most maxsplit splits are done. If sep is "",
	/// any whitespace string is a separator.
	///
	static void rsplit(const std::wstring & str, std::vector< std::wstring > & result, const std::wstring & sep = L"", int maxsplit = -1);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a list of the lines in the string, breaking at line boundaries. Line breaks
	/// are not included in the resulting list unless keepends is given and true.
	///
	static void splitlines(const std::wstring & str, std::vector< std::wstring > & result, bool keepends = false);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return True if string starts with the prefix, otherwise return False. With optional start,
	/// test string beginning at that position. With optional end, stop comparing string at that
	/// position
	///
	static bool startswith(const std::wstring & str, const std::wstring & prefix, int start = 0, int end = MAX_32BIT_INT);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string with leading and trailing characters removed. If chars is "",
	/// whitespace characters are removed. If given not "",  the characters in the string will be
	/// stripped from the both ends of the string this method is called on.
	///
	static std::wstring strip(const std::wstring & str, const std::wstring & chars = L"");

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string with uppercase characters converted to lowercase and vice versa.
	///
	static std::wstring swapcase(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a titlecased version of the string: words start with uppercase characters,
	/// all remaining cased characters are lowercase.
	///
	static std::wstring title(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string where all characters occurring in the optional argument
	/// deletechars are removed, and the remaining characters have been mapped through the given
	/// translation table, which must be a string of length 256.
	///
	static std::wstring translate(const std::wstring & str, const std::wstring & table, const std::wstring & deletechars = L"");

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return a copy of the string converted to uppercase.
	///
	static std::wstring upper(const std::wstring & str);

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Return the numeric string left filled with zeros in a string of length width. The original
	/// string is returned if width is less than str.size().
	///
	static std::wstring zfill(const std::wstring & str, int width, wchar_t fillChar = '0');

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief function matching python's slice functionality.
	///
	static std::wstring slice(const std::wstring & str, int start = 0, int end = MAX_32BIT_INT);


	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief function to make a alignment string.
	/// align : 0-left 1-center 2-right
	///
	static std::wstring alignment(const std::wstring& str, int maxLen, int align = 0, wchar_t fillChar = ' ');

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief function to return a string c style length.
	/// don't use std::wstring::length bacause it may return not equal strlen(str.c_str())
	///
	static inline int length(const std::wstring& str)
	{
		return wcslen(str.c_str());
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief function to return is a string is a empty C style string.
	///
	static bool iscempty(const std::wstring& str);

	///
	/// @ }
	///
};


class os_pathw
{
public:
    // All of the function below have three versions.
    // Example:
    //   join(...)
    //   join_nt(...)
    //   join_posix(...)
    //
    // The regular function dispatches to the other versions - based on the OS
    // at compile time - to match the result you'd get from the python
    // interepreter on the same operating system
    // 
    // Should you want to 'lock off' to a particular version of the string
    // manipulation across *all* operating systems, use the version with the
    // _OS you are interested in.  I.e., you can use posix style path joining,
    // even on Windows, with join_posix.
    //
    // The naming, (nt, posix) matches the cpython source implementation.
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @defgroup functions pystring::os::path
    /// @{
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the base name of pathname path. This is the second half of the pair returned
    /// by split(path). Note that the result of this function is different from the Unix basename
    /// program; where basename for '/foo/bar/' returns 'bar', the basename() function returns an
    /// empty string ('').
    
    static std::wstring basename(const std::wstring & path);
    static std::wstring basename_nt(const std::wstring & path);
    static std::wstring basename_posix(const std::wstring & path);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the directory name of pathname path. This is the first half of the pair
    /// returned by split(path).
    
    static std::wstring dirname(const std::wstring & path);
    static std::wstring dirname_nt(const std::wstring & path);
    static std::wstring dirname_posix(const std::wstring & path);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return True if path is an absolute pathname. On Unix, that means it begins with a
    /// slash, on Windows that it begins with a (back)slash after chopping off a potential drive
    /// letter.

    static bool isabs(const std::wstring & path);
    static bool isabs_nt(const std::wstring & path);
    static bool isabs_posix(const std::wstring & s);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a normalized absolutized version of the pathname path.
    /// 
    /// NOTE: This differs from the interface of the python equivalent in that it requires you
    /// to pass in the current working directory as an argument.
    
    static std::wstring abspath(const std::wstring & path, const std::wstring & cwd);
    static std::wstring abspath_nt(const std::wstring & path, const std::wstring & cwd);
    static std::wstring abspath_posix(const std::wstring & path, const std::wstring & cwd);
    

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Join one or more path components intelligently. If any component is an absolute
    /// path, all previous components (on Windows, including the previous drive letter, if there
    /// was one) are thrown away, and joining continues. The return value is the concatenation of
    /// path1, and optionally path2, etc., with exactly one directory separator (os.sep) inserted
    /// between components, unless path2 is empty. Note that on Windows, since there is a current
    /// directory for each drive, os.path.join("c:", "foo") represents a path relative to the
    /// current directory on drive C: (c:foo), not c:\foo.
    
    /// This dispatches based on the compilation OS
    static std::wstring join(const std::wstring & path1, const std::wstring & path2);
    static std::wstring join_nt(const std::wstring & path1, const std::wstring & path2);
    static std::wstring join_posix(const std::wstring & path1, const std::wstring & path2);
    
    static std::wstring join(const std::vector< std::wstring > & paths);
    static std::wstring join_nt(const std::vector< std::wstring > & paths);
    static std::wstring join_posix(const std::vector< std::wstring > & paths);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Normalize a pathname. This collapses redundant separators and up-level references
    /// so that A//B, A/B/, A/./B and A/foo/../B all become A/B. It does not normalize the case
    /// (use normcase() for that). On Windows, it converts forward slashes to backward slashes.
    /// It should be understood that this may change the meaning of the path if it contains
    /// symbolic links!

    static std::wstring normpath(const std::wstring & path);
    static std::wstring normpath_nt(const std::wstring & path);
    static std::wstring normpath_posix(const std::wstring & path);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Split the pathname path into a pair, (head, tail) where tail is the last pathname
    /// component and head is everything leading up to that. The tail part will never contain a
    /// slash; if path ends in a slash, tail will be empty. If there is no slash in path, head
    /// will be empty. If path is empty, both head and tail are empty. Trailing slashes are
    /// stripped from head unless it is the root (one or more slashes only). In all cases,
    /// join(head, tail) returns a path to the same location as path (but the strings may
    /// differ).

    static void split(std::wstring & head, std::wstring & tail, const std::wstring & path);
    static void split_nt(std::wstring & head, std::wstring & tail, const std::wstring & path);
    static void split_posix(std::wstring & head, std::wstring & tail, const std::wstring & path);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Split the pathname path into a pair (drive, tail) where drive is either a drive
    /// specification or the empty string. On systems which do not use drive specifications,
    /// drive will always be the empty string. In all cases, drive + tail will be the same as
    /// path.
    
    static void splitdrive(std::wstring & drivespec, std::wstring & pathspec, const std::wstring & path);
    static void splitdrive_nt(std::wstring & drivespec, std::wstring & pathspec, const std::wstring & p);
    static void splitdrive_posix(std::wstring & drivespec, std::wstring & pathspec, const std::wstring & path);
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Split the pathname path into a pair (root, ext) such that root + ext == path, and
    /// ext is empty or begins with a period and contains at most one period. Leading periods on
    /// the basename are ignored; splitext('.cshrc') returns ('.cshrc', '').

    static void splitext(std::wstring & root, std::wstring & ext, const std::wstring & path);
    static void splitext_nt(std::wstring & root, std::wstring & ext, const std::wstring & path);
    static void splitext_posix(std::wstring & root, std::wstring & ext, const std::wstring & path);
    
    ///
    /// @ }
    ///
};

#endif
