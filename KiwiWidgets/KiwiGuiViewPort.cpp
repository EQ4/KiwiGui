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

#include "KiwiGuiViewport.h"
#include "KiwiGuiDevice.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  GUI VIEW PORT                                   //
    // ================================================================================ //
    
    GuiViewport::GuiViewport(sGuiContext context) noexcept : GuiModel(context),
    m_scrollbar_h(make_shared<GuiScrollBar>(context, GuiScrollBar::Horizontal)),
    m_scrollbar_v(make_shared<GuiScrollBar>(context, GuiScrollBar::Vertical))
    {
        addChild(m_scrollbar_h);
        addChild(m_scrollbar_v);
    }
    
    GuiViewport::~GuiViewport() noexcept
    {
        ;
    }
    
    void GuiViewport::setContent(sGuiModel model) noexcept
    {
        if(m_content)
        {
            removeChild(m_content);
        }
        addChild(m_content);
    }
    
    sGuiController GuiViewport::createController()
    {
        return make_shared<Controller>(static_pointer_cast<GuiViewport>(shared_from_this()));
    }
    
    // ================================================================================ //
    //                              GUI VIEW PORT CONTROLLER                            //
    // ================================================================================ //
    
    GuiViewport::Controller::Controller(sGuiViewport viewport) noexcept : GuiController(viewport),
    m_view_port(viewport)
    {
        ;
    }
    
    void GuiViewport::Controller::scrollBarMoved(GuiScrollBar::sController scrollbar)
    {
        
    }

}
















