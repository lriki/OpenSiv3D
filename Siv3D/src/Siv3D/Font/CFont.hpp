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
# include <Siv3D/Common.hpp>
# include <Siv3D/Font.hpp>
# include "IFont.hpp"

namespace s3d
{
	class CFont final : public ISiv3DFont
	{
	private:

	public:

		CFont();

		~CFont();

		void init() override;

	};
}