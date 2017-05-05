//
//  NavElementList.h
//  Readium
//
//  Created by Jim Dovey on 2013-10-11.
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

#pragma once

#include "WinNavTable.h"
#include "WinNavPoint.h"
#include "CollectionBridges.h"

BEGIN_READIUM_API

struct NavElementToNative : public std::unary_function<INavigationElement^, ::ePub3::NavigationElementPtr>
{
	::ePub3::NavigationElementPtr operator()(INavigationElement^ input) {
		NavigationPoint^ pt = dynamic_cast<NavigationPoint^>(input);
		if (pt != nullptr)
			return std::dynamic_pointer_cast<::ePub3::NavigationElement>(pt->NativeObject);
		NavigationTable^ tb = dynamic_cast<NavigationTable^>(input);
		if (tb != nullptr)
			return std::dynamic_pointer_cast<::ePub3::NavigationElement>(tb->NativeObject);
		return nullptr;
	}
};
struct NavElementFromNative : public std::unary_function<::ePub3::NavigationElementPtr, INavigationElement^>
{
	INavigationElement^ operator()(::ePub3::NavigationElementPtr native)
	{
		auto pt = std::dynamic_pointer_cast<::ePub3::NavigationPoint>(native);
		if (bool(pt))
			return NavigationPoint::Wrapper(pt);
		auto tb = std::dynamic_pointer_cast<::ePub3::NavigationTable>(native);
		if (bool(tb))
			return NavigationTable::Wrapper(tb);
		return nullptr;
	}
};

using NavElementList = BridgedVectorView<INavigationElement^, ::ePub3::NavigationElementPtr, NavElementToNative, NavElementFromNative>;

END_READIUM_API
