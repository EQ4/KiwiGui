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

#ifndef __DEF_KIWI_GUI_SKETCHER__
#define __DEF_KIWI_GUI_SKETCHER__

#include "KiwiGuiEvent.h"

namespace Kiwi
{    
	// ================================================================================ //
	//                                      SKETCHER                                    //
	// ================================================================================ //
	
	//! The sketcher...
	/** The sketcher...
	 */
	class GuiSketcher : virtual public Attr::Manager
	{
	private:
        const wGuiContext           m_context;
		set<wGuiView,
		owner_less<wGuiView>>       m_views;
		mutex                       m_views_mutex;
        set<wGuiSketcher,
        owner_less<wGuiSketcher>>   m_childs;
        mutex                       m_childs_mutex;
        
	public:
		
		//! Constructor.
		/** The function does nothing.
         @param context The context.
		 */
		GuiSketcher(sGuiContext context) noexcept;
		
		//! Destructor.
		/** The function does nothing.
		 */
		virtual ~GuiSketcher() noexcept;
        
        //! Retrieves the context.
        /** The function retrieves the context of the sketcher.
         @return The context.
         */
        inline sGuiContext getContext() const noexcept
        {
            return m_context.lock();
        }
        
        //! Retrieves the device manager.
        /** The function retrieves the device manager of the sketcher.
         @return The device manager.
         */
        sGuiDeviceManager getDeviceManager() const noexcept;
		
		//! The paint method that should be override.
		/** The function shoulds draw some stuff in the sketch.
         @param ctrl    The controller that ask the draw.
		 @param sketch  A sketch to draw.
		 */
		virtual void draw(scGuiController ctrl, Sketch& sketch) const = 0;
        
        //! Creates a view.
        /** The function creates a view depending on the inheritance and the implemetation.
         @return The view.
         */
        sGuiView createView() noexcept;
        
        //! Removes a view.
        /** The function removes a view.
         @param view The view.
         */
        void removeView(sGuiView view) noexcept;
		
	protected:
		
		//! Send a notification to each listeners that the object needs to be redrawn.
		/** The function sends a notification to each listeners that the object should be redrawn.
		 */
		void redraw() noexcept;
        
        //! Adds a child sketcher to the sketcher.
        /** The function adds a child sketcher that will be displayed inside the sketcher.
         @param child The child.
         */
        void add(sGuiSketcher child) noexcept;
        
        //! Remove a child sketcher from the sketcher.
        /** The function removes a child sketcher and make it invisible.
         @param child The child.
         */
        void remove(sGuiSketcher child) noexcept;
        
    private:
        
        //! Create the controller.
        /** The function creates a controller depending on the inheritance.
         @return The controller.
         */
        virtual sGuiController createController() = 0;
	};
    
    // ================================================================================ //
    //                                      MOUSER                                      //
    // ================================================================================ //
    
    //! The mouser is the virutal class that receive mouse events.
    /**
     The mouser is a virtual class with a receive method that should be override to receveie a mouse event.
     */
    class GuiMouser
    {
    public:
        //! Constructor.
        /** The function does nothing.
         */
        GuiMouser() noexcept;
        
        //! Destructor.
        /** The function does nothing.
         */
        virtual ~GuiMouser() noexcept;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A mouser event.
         @param ctrl     The controller gives the event.
         @return true if the class has done something with the event otherwise false
         */
        virtual bool receive(scGuiController ctrl, MouseEvent const& event) = 0;
    };
    
    // ================================================================================ //
    //                                     KEYBOARDER                                   //
    // ================================================================================ //
    
    //! The mouser is the virutal class that receive keyboard events.
    /**
     The mouser is a virtual class with a receive method that should be override to receveie a keyboard events.
     */
    class GuiKeyboarder
    {
    public:
        //! Constructor.
        /** The function does nothing.
         */
        GuiKeyboarder() noexcept;
        
        //! Destructor.
        /** The function does nothing.
         */
        virtual ~GuiKeyboarder() noexcept;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A keyboard event.
         @return true if the class has done something with the event otherwise false
         */
        virtual bool receive(KeyboardEvent const& event) = 0;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A focus event.
         @return true if the class has done something with the event otherwise false
         */
        virtual bool receive(KeyboardFocus const  event)
        {
            return false;
        }
    };
    
    // ================================================================================ //
    //                                     TEXT EDITOR                                  //
    // ================================================================================ //
    
    //! The text editor...
    /**
     The mtext editor...
     */
    class GuiTextEditor : public GuiSketcher, public GuiMouser, public GuiKeyboarder
    {
    private:
        Font    m_font;
        string  m_text;
    public:
        //! Constructor.
        /** The function does nothing.
         @param context The context.
         */
        GuiTextEditor(sGuiContext context) noexcept;
        
        //! Destructor.
        /** The function does nothing.
         */
        virtual ~GuiTextEditor() noexcept;
        
        //! The paint method that should be override.
        /** The function shoulds draw some stuff in the sketch.
         @param ctrl    The controller that ask the draw.
         @param sketch  A sketch to draw.
         */
        virtual void draw(scGuiController ctrl, Sketch& sketch) const = 0;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A mouser event.
         @param ctrl     The controller gives the event.
         @return true if the class has done something with the event otherwise false
         */
        virtual bool receive(scGuiController ctrl, MouseEvent const& event) = 0;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A keyboard event.
         @return true if the class has done something with the event otherwise false
         */
        virtual bool receive(KeyboardEvent const& event) = 0;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A focus event.
         @return true if the class has done something with the event otherwise false
         */
        virtual bool receive(KeyboardFocus const  event)
        {
            return false;
        }
        
        //! Create the controller.
        /** The function creates a controller depending on the inheritance.
         @return The controller.
         */
        virtual sGuiController createController() = 0;
        
        //! Retrieves the text of the editor.
        /** The function retrieves the text of the editor.
         @return The text.
         */
        inline string getText() const noexcept
        {
            return m_text;
        }
        
        //! Sets the text of the editor.
        /** The function sets the text of the editor.
         @param text The text.
         */
        void setText(string const& text) noexcept;
        
        //! Clears the text of the editor.
        /** The function clears the text of the editor.
         */
        void clearText() noexcept;
        
        //! Retrieves the current position of the caret.
        /** The function retrieves the current position of the caret.
         @return The position of the caret.
         */
        ulong getCaretPosition() const noexcept;
        
        //! Sets the current position of the caret.
        /** The function sets the current position of the caret.
         @param pos The position of the caret.
         */
        void setCaretPosition(ulong pos) noexcept;
    };
}

#endif