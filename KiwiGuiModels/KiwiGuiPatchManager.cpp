/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#include "KiwiGuiPatchManager.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                     GUI INSTANCE                                 //
    // ================================================================================ //
	
	GuiPatchManager::GuiPatchManager(sGuiDeviceManager device) noexcept :
	m_device(device)
	{
		;
	}
	
	GuiPatchManager::~GuiPatchManager()
	{
		m_patchers.clear();
	}
	
	void GuiPatchManager::add(sGuiPatcher patcher)
	{
		if(patcher)
		{
			lock_guard<mutex> guard(m_mutex);
			if(find(m_patchers.begin(), m_patchers.end(), patcher) == m_patchers.end())
			{
				m_patchers.push_back(patcher);
			}
		}
	}
	
	void GuiPatchManager::remove(sGuiPatcher patcher)
	{
		bool success = false;
		if(patcher)
		{
			lock_guard<mutex> guard(m_mutex);
			auto it = find(m_patchers.begin(), m_patchers.end(), patcher);
			if(it != m_patchers.end())
			{
				m_patchers.erase(it);
				success = true;
			}
		}
		if(success)
		{
			//remove window ?
		}
	}
}



