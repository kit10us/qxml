// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

namespace qxml
{
	template< typename ReturnType, typename ParamType >
	ReturnType AttributeCast( const qxml::Element & element, std::string paramA )
	{
		return ReturnType{ element.GetAttribute< ParamType >( paramA ) };
	}

	template< typename ReturnType, typename ParamType >
	ReturnType AttributeCast( const qxml::Element & element, std::string paramA, std::string paramB )
	{
		return ReturnType{ element.GetAttribute< ParamType >( paramA ), element.GetAttribute< ParamType >( paramB ) };
	}

	template< typename ReturnType, typename ParamType >
	ReturnType AttributeCast( const qxml::Element & element, std::string paramA, std::string paramB, std::string paramC )
	{
		return ReturnType{ element.GetAttribute< ParamType >( paramA ), element.GetAttribute< ParamType >( paramB ), element.GetAttribute< ParamType >( paramC ) };
	}

	template< typename ReturnType, typename ParamType >
	ReturnType AttributeCast( const qxml::Element & element, std::string paramA, std::string paramB, std::string paramC, std::string paramD )
	{
		return ReturnType{ element.GetAttribute< ParamType >( paramA ), element.GetAttribute< ParamType >( paramB ), element.GetAttribute< ParamType >( paramC ), element.GetAttribute< ParamType >( paramD ) };
	}
}