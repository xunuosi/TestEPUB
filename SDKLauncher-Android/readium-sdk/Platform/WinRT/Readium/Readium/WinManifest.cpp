//
//  WinManifest.cpp
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

#include "WinManifest.h"
#include "WinPackage.h"
#include "WinEncryptionInfo.h"
#include "Streams.h"
#include "CollectionBridges.h"
#include <ePub3/xml/document.h>

using namespace ::Platform;
using namespace ::Windows::Data::Xml::Dom;
using namespace ::Windows::Foundation;
using namespace ::Windows::Foundation::Collections;

BEGIN_READIUM_API

_BRIDGE_API_IMPL_(::ePub3::ManifestItemPtr, ManifestItem)

ManifestItem::ManifestItem(::ePub3::ManifestItemPtr native) : _native(native)
{
	_native->SetBridge(this);
}

Package^ ManifestItem::ParentPackage::get()
{
	return Package::Wrapper(_native->GetPackage());
}

String^ ManifestItem::AbsolutePath::get()
{
	return StringFromNative(_native->AbsolutePath());
}

String^ ManifestItem::Identifier::get()
{
	return StringFromNative(_native->Identifier());
}
String^ ManifestItem::Href::get()
{
	return StringFromNative(_native->Href());
}
String^ ManifestItem::MediaType::get()
{
	return StringFromNative(_native->MediaType());
}

String^ ManifestItem::MediaOverlayID::get()
{
	return StringFromNative(_native->MediaOverlayID());
}
ManifestItem^ ManifestItem::MediaOverlay::get()
{
	return Wrapper(_native->MediaOverlay());
}

String^ ManifestItem::FallbackID::get()
{
	return StringFromNative(_native->FallbackID());
}
ManifestItem^ ManifestItem::Fallback::get()
{
	return Wrapper(_native->Fallback());
}

String^ ManifestItem::BaseHref::get()
{
	return StringFromNative(_native->BaseHref());
}

bool ManifestItem::HasProperty(ItemProperties^ prop)
{
	return _native->HasProperty(prop->NativeObject);
}
bool ManifestItem::HasProperty(UINT prop)
{
	return _native->HasProperty(prop);
}
bool ManifestItem::HasProperty(IVectorView<Uri^>^ prop)
{
	std::vector<::ePub3::IRI> propertyIRIs;
	propertyIRIs.reserve(prop->Size);
	for (auto uri : prop)
	{
		propertyIRIs.emplace_back(URIToIRI(uri));
	}
	return _native->HasProperty(propertyIRIs);
}

EncryptionInfo^ ManifestItem::Encryption::get()
{
	return EncryptionInfo::Wrapper(_native->GetEncryptionInfo());
}

bool ManifestItem::CanLoadDocument::get()
{
	return _native->CanLoadDocument();
}

::Windows::Data::Xml::Dom::XmlDocument^ ManifestItem::LoadDocument()
{
	return _native->ReferencedDocument()->xml();
}

IClosableStream^ ManifestItem::ReadStream()
{
	return ref new Stream(_native->Reader());
}

#define PropertyHolder ManifestItem
#include "PropertyHolderSubclassImpl.h"
#undef PropertyHolder

END_READIUM_API
