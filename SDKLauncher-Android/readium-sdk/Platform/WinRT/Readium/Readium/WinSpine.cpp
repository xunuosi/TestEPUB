//
//  WinSpine.cpp
//  Readium
//
//  Created by Jim Dovey on 2013-10-10.
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

#include "WinSpine.h"
#include "WinManifest.h"
#include "WinPackage.h"

using namespace ::Windows::Foundation;

BEGIN_READIUM_API

_BRIDGE_API_IMPL_(::ePub3::SpineItemPtr, SpineItem)

SpineItem::SpineItem(::ePub3::SpineItemPtr native) : _native(native)
{
	_native->SetBridge(this);
}

Uri^ SpineItem::PageSpreadRightPropertyIRI::get()
{
	return IRIToURI(::ePub3::SpineItem::PageSpreadRightPropertyIRI);
}
Uri^ SpineItem::PageSpreadLeftPropertyIRI::get()
{
	return IRIToURI(::ePub3::SpineItem::PageSpreadLeftPropertyIRI);
}

Package^ SpineItem::ParentPackage::get()
{
	return Package::Wrapper(_native->GetPackage());
}

UINT SpineItem::ItemCount::get()
{
	return _native->Count();
}
UINT SpineItem::Index::get()
{
	return _native->Index();
}

String^ SpineItem::Identifier::get()
{
	return StringFromNative(_native->Identifier());
}
String^ SpineItem::Idref::get()
{
	return StringFromNative(_native->Idref());
}

ManifestItem^ SpineItem::ManifestItemRef::get()
{
	return ManifestItem::Wrapper(_native->ManifestItem());
}
bool SpineItem::Linear::get()
{
	return _native->Linear();
}

PageSpread SpineItem::Spread::get()
{
	return PageSpread(_native->Spread());
}

String^ SpineItem::Title::get()
{
	return StringFromNative(_native->Title());
}
void SpineItem::Title::set(String^ title)
{
	_native->SetTitle(StringToNative(title));
}

SpineItem^ SpineItem::Next::get()
{
	return Wrapper(_native->Next());
}
SpineItem^ SpineItem::Previous::get()
{
	return Wrapper(_native->Previous());
}
SpineItem^ SpineItem::NextStep::get()
{
	return Wrapper(_native->NextStep());
}
SpineItem^ SpineItem::PriorStep::get()
{
	return Wrapper(_native->PriorStep());
}

SpineItem^ SpineItem::SpineItemAt(UINT idx)
{
	return Wrapper(_native->at(idx));
}

#define PropertyHolder SpineItem
#include "PropertyHolderSubclassImpl.h"
#undef PropertyHolder

END_READIUM_API
