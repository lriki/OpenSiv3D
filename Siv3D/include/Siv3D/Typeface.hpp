﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include "Common.hpp"

namespace s3d
{
	enum class Typeface : uint32
	{
		CJK_Regular_JP,
		CJK_Regular_KR,
		CJK_Regular_SC,
		CJK_Regular_TC,
		CJK_Regular_HK,

		MonochromeEmoji,
		ColorEmoji,

		Mplus_Thin,
		Mplus_Light,
		Mplus_Regular,
		Mplus_Medium,
		Mplus_Bold,
		Mplus_Heavy,
		Mplus_Black,

		Icon_Awesome_Solid,
		Icon_Awesome_Brand,
		Icon_MaterialDesign,

		Thin = Mplus_Thin,

		Light = Mplus_Light,

		Regular = Mplus_Regular,

		Medium = Mplus_Medium,

		Bold = Mplus_Bold,

		Heavy = Mplus_Heavy,

		Black = Mplus_Black,
	};
}