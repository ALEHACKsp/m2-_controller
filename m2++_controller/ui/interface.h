#pragma once
#include <inc.h>

namespace ui
{
	class c_interface : public s<c_interface>
	{
	public:
		HINSTANCE hInst;                                
		WCHAR szTitle[MAX_PATH];                 
		WCHAR szWindowClass[MAX_PATH];
		
	public:
		void	setup();
	};
}