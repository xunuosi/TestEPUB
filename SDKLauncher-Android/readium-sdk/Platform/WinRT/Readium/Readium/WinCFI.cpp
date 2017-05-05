//
//  WinCFI.cpp
//  Readium
//
//  Created by Jim Dovey on 2013-10-14.
//  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
//  
//  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//  
//  Licensed under Gnu Affero General Public License Version 3 (provided, notwithstanding this notice, 
//  Readium Foundation reserves the right to license this material under a different separate license, 
//  and if you have done so, the terms of that separate license control and the following references 
//  to GPL do not apply).
//  
//  This program is free software: you can redistribute it and/or modify it under the terms of the GNU 
//  Affero General Public License as published by the Free Software Foundation, either version 3 of 
//  the License, or (at your option) any later version. You should have received a copy of the GNU 
//  Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "WinCFI.h"

BEGIN_READIUM_API

using namespace ::Platform;

CFI^ CFI::Wrapper(::ePub3::CFI& native)
{
	CFI^ result = native.GetBridge<CFI>();
	if (result == nullptr)
		result = ref new CFI(native);
	return result;
}
CFI^ CFI::Wrapper(::ePub3::CFI&& native)
{
	CFI^ result = native.GetBridge<CFI>();
	if (result == nullptr)
		result = ref new CFI(std::move(native));
	return result;
}

CFI::CFI(const ::ePub3::CFI& native) : _native(native)
{
	_native.SetBridge(this);
}
CFI::CFI(::ePub3::CFI&& native) : _native(std::move(native))
{
	_native.SetBridge(this);
}

CFI::CFI() : CFI(std::move(::ePub3::CFI()))
{
}
CFI::CFI(CFI^ base, CFI^ start, CFI^ end) : CFI(std::move(::ePub3::CFI(base->NativeObject, start->NativeObject, end->NativeObject)))
{
}
CFI::CFI(String^ stringRepresentation) : CFI(std::move(::ePub3::CFI(StringToNative(stringRepresentation))))
{
}
CFI::CFI(CFI^ otherCFI, UINT fromIndex) : CFI(std::move(::ePub3::CFI(otherCFI->NativeObject, fromIndex)))
{
}

String^ CFI::ToString()
{
	return StringFromNative(_native.String());
}

bool CFI::IsRangeTriplet::get()
{
	return _native.IsRangeTriplet();
}
bool CFI::IsEmpty::get()
{
	return _native.Empty();
}

void CFI::Clear()
{
	_native.Clear();
}

bool CFI::Equals(Object^ obj)
{
	CFI^ cfi = dynamic_cast<CFI^>(obj);
	if (cfi != nullptr)
		return Equals(cfi);

	String^ str = dynamic_cast<String^>(obj);
	if (str != nullptr)
		return Equals(str);

	return false;
}
bool CFI::Equals(CFI^ otherCFI)
{
	return _native == otherCFI->NativeObject;
}
bool CFI::Equals(String^ stringRepresentation)
{
	return _native == StringToNative(stringRepresentation);
}

CFI^ CFI::Assign(CFI^ newValue)
{
	_native = newValue->NativeObject;
	return this;
}
CFI^ CFI::Assign(String^ stringRepresentation)
{
	_native = StringToNative(stringRepresentation);
	return this;
}
CFI^ CFI::Assign(CFI^ base, UINT fromIndex)
{
	_native.Assign(base->NativeObject, fromIndex);
	return this;
}

CFI^ CFI::Append(CFI^ otherCFI)
{
	_native += otherCFI->NativeObject;
	return this;
}
CFI^ CFI::Append(String^ stringRepresentation)
{
	_native += StringToNative(stringRepresentation);
	return this;
}

CFISideBias CFI::CharacterSideBias::get()
{
	return CFISideBias(_native.CharacterSideBias());
}

END_READIUM_API
