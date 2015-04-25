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

#ifndef __DEF_KIWI_GUI_TEXT_EDITOR__
#define __DEF_KIWI_GUI_TEXT_EDITOR__

#include "KiwiGuiModel.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                     TEXT EDITOR                                  //
    // ================================================================================ //
    
    //! The text editor...
    /**
     The text editor...
     */
    class GuiTextEditor : public GuiSketcher, public GuiMouser, public GuiKeyboarder
    {
    public:
        class Listener
        {
        public:
            inline virtual ~Listener()  {}
            
            virtual void textChanged(sGuiTextEditor editor) {}
            
            virtual void returnKeyPressed(sGuiTextEditor editor) {}
            
            virtual void tabKeyPressed(sGuiTextEditor editor) {}
            
            virtual void escapeKeyPressed(sGuiTextEditor editor) {}
            
            virtual void focusLost(sGuiTextEditor editor) {}
        };
        
        typedef shared_ptr<Listener>    sListener;
        typedef weak_ptr<Listener>      wListener;
        
        enum DisplayMode
        {
            Trunc   = 0,
            Trail   = 1,
            Wrap    = 2
        };
        
        enum BehaviorMode //: bool
        {
            UsedAsCharacter = 0,
            Notify          = 1
        };
        
    private:
        class Caret : public GuiSketcher, public Clock
        {
        private:
            atomic_bool m_status;
            atomic_bool m_active;
            Color       m_color;
        public:
            
            //! Constructor.
            /** The function does nothing.
             @param context The context.
             */
            inline Caret(sGuiContext context) noexcept : GuiSketcher(context), m_color(Colors::black) {}
            
            //! Destructor.
            /** The function does nothing.
             */
            inline  ~Caret() noexcept {}
            
            //! Sets the color of the caret.
            /** The function sets the color of the caret.
             @param color   The new color.
             */
            inline void setColor(Color const& color) noexcept {m_color = color;}
            
            //! Retrievs the color of the caret.
            /** The function retrieves the color of the caret.
             @return The color.
             */
            inline Color getColor() const noexcept {return m_color;}
            
            //! The draw method that should be override.
            /** The function shoulds draw some stuff in the sketch.
             @param ctrl    The controller that ask the draw.
             @param sketch  A sketch to draw.
             */
            void draw(scGuiView view, Sketch& sketch) const override
            {
                if(m_status)
                {
                    sketch.setColor(m_color);
                    sketch.drawLine(0., 0., 0., sketch.getSize().height(), 2.);
                }
            }
            
            //! The tick function that must be override.
            /** The tick function is called by a clock after a delay.
             */
            void tick() override
            {
                if(m_active)
                {
                    m_status = !m_status;
                    redraw();
                    delay(500.);
                }
            }
            
            //! Starts the blinking.
            /** The function starts the blinking.
             */
            void start()
            {
                if(!m_active)
                {
                    m_active = true;
                    tick();
                }
            }
            
            //! Stops the blinking.
            /** The function stops the blinking.
             */
            inline void stop()
            {
                m_active = false;
            }
            
            //! Retrieves the blinking.
            /** The function stops the blinking.
             */
            inline bool state()
            {
                return m_active;
            }
        };
        
        typedef shared_ptr<Caret> sCaret;
        
        const sCaret            m_caret;
        Font                    m_font;
        Font::Justification     m_justification;
        double                  m_line_space;
        DisplayMode             m_mode;
        Color                   m_color;
        
        wstring                 m_text;
        vector<wstring>         m_lines;
        vector<double>          m_widths;
        
        bool                    m_notify_return;
        bool                    m_notify_tab;
        bool                    m_formated;

        set<wListener,
        owner_less<wListener>> m_lists;
        mutex                  m_lists_mutex;
        
        //@internal
        void addCharacter(wchar_t character) noexcept;
        void getLineWidths() noexcept;
        bool format() noexcept;
        
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
        
        //! Sets the font of the editor.
        /** The function sets the font of the editor. The size of the text can changed so you should retrieve the it and resize the text editor if needed.
         @param font The new font.
         */
        void setFont(Font const& font) noexcept;
        
        //! Sets the justification of the editor.
        /** The function sets the justification of the editor. The text will always be top justified, only the horizontal position can be changed.
         @param justification The new justification.
         */
        void setJustification(const Font::Justification justification) noexcept;
        
        //! Sets the line spacing of the editor.
        /** The function sets the line spacing of the editor. The line spacing is a factor that multiplies the font height and defines the space between lines. The size of the text can changed so you should retrieve the it and resize the text editor if needed.
         @param factor The line space.
         */
        void setLineSpacing(const double factor) noexcept;
        
        //! Sets the display mode.
        /** The function sets the display mode of the editor. The display mode defines if when the text width exceeds the width of the editor the text should be truncated, the end of the lines should be replaced with trailling points or if the text should be wrapped to a new line. The size of the text can changed so you should retrieve the it and resize the text editor if needed.
         @param mode The display mode.
         */
        void setDisplayMode(const DisplayMode mode) noexcept;
        
        //! Sets the behavior of the return and tab keys of the editor.
        /** The function sets if the return key or the tab key notify the listeners or if the keys should be consider as new characters.
         @param returnNotifies  True if the return key should notify the listeners.
         @param tabNotifies     True if the return tab should notify the listeners.
         */
        inline void setKeyBehavior(const bool returnNotifies, const bool tabNotifies) noexcept
        {
            setReturnKeyBehavior(returnNotifies);
            setTabKeyBehavior(tabNotifies);
        }
        
        //! Sets the behavior of the return key of the editor.
        /** The function sets if the return key notifies the listeners or if the key should be consider as new characters.
         @param notify  True if the return key should notify the listeners.
         */
        void setReturnKeyBehavior(const bool notify) noexcept;
        
        //! Sets the behavior of the tab key of the editor.
        /** The function sets if the tab key notifies the listeners or if the key should be consider as new characters.
         @param notify  True if the tab key should notify the listeners.
         */
        void setTabKeyBehavior(const bool notify) noexcept;
        
        //! Sets the color of the text.
        /** The function sets the color of the text.
         @param color The new color.
         */
        void setColor(Color const& color) noexcept;
        
        //! Sets the text of the editor.
        /** The function sets the text of the editor.
         @param text The text.
         */
        void setText(wstring const& text) noexcept;
        
        //! Clears the text of the editor.
        /** The function clears the text of the editor.
         */
        void clearText() noexcept;
        
        //! Retrieves the font of the editor.
        /** The function retrieves the font of the editor. 
         @return The font.
         */
        inline Font getFont() const noexcept
        {
            return m_font;
        }
        
        //! Retrieves the justification of the editor.
        /** The function retrieves the justification of the editor.
         @return The justification.
         */
        inline Font::Justification getJustification() const noexcept
        {
            return m_justification;
        }
        
        //! Retrieves the line spacing of the editor.
        /** The function retrieves the line spacing of the editor.
         @return The line spacing.
         */
        inline double getLineSpacing() const noexcept
        {
            return m_line_space;
        }
        
        //! Retrieves the display mode of the editor.
        /** The function retrieves the ldisplay mode of the editor.
         @return The display mode.
         */
        inline DisplayMode getDisplayMode() const noexcept
        {
            return m_mode;
        }
        
        //! Retrieves if the return key is used as a character or if it notifies the listeners.
        /** The function retrieves if the return key is used as a character or if it notifies the listeners.
         @return true if the return key is used as a character, otherwise false if it notifies the listeners.
         */
        inline bool getReturnKeyBehavior() const noexcept
        {
            return !m_notify_return;
        }
        
        //! Retrieves if the tab key is used as a character or if it notifies the listeners.
        /** The function retrieves if the tab key is used as a character or if it notifies the listeners.
         @return true if the tab key is used as a character, otherwise false if it notifies the listeners.
         */
        inline bool getTabKeyBehavior() const noexcept
        {
            return !m_notify_tab;
        }
        
        //! Retrieves the text of the editor.
        /** The function retrieves the text of the editor.
         @return The text.
         */
        inline wstring getText() const noexcept
        {
            return m_text;
        }
        
        //! Retrieves the size of the text.
        /** The function retrieves the size of the text.
         */
        Size getTextSize() const noexcept;

        //! The draw method that should be override.
        /** The function shoulds draw some stuff in the sketch.
         @param ctrl    The controller that ask the draw.
         @param sketch  A sketch to draw.
         */
        void draw(scGuiView view, Sketch& sketch) const override;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A mouser event.
         @param ctrl     The controller gives the event.
         @return true if the class has done something with the event otherwise false
         */
        bool receive(scGuiView view, MouseEvent const& event) override;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A keyboard event.
         @return true if the class has done something with the event otherwise false
         */
        bool receive(scGuiView view, KeyboardEvent const& event) override;
        
        //! The receive method that should be override.
        /** The function shoulds perform some stuff.
         @param event    A focus event.
         @return true if the class has done something with the event otherwise false
         */
        bool receive(scGuiView view, KeyboardFocus const event) override;
        
        //! Notify the text editor that it should grab the keayboard focus.
        /** The function notifies the text editor that it should grab the keayboard focus.
         */
        inline void grabFocus() {GuiSketcher::grabFocus();}
        
        //! Notify the manager that the values of an attribute has changed.
        /** The function notifies the manager that the values of an attribute has changed.
         @param attr An attribute.
         @return pass true to notify changes to listeners, false if you don't want them to be notified
         */
        bool notify(sAttr attr) override;
        
        //! Add an instance listener in the binding list of the text editor.
        /** The function adds an instance listener in the binding list of the text editor.
         @param listener  The listener.
         */
        void addListener(sListener listener);
        
        //! Remove an instance listener from the binding list of the text editor.
        /** The function removes an instance listener from the binding list of the text editor.
         @param listener  The listener.
         */
        void removeListener(sListener listener);
    };
}

#endif