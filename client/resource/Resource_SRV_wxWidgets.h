#pragma once
#include <wx/wx.h>
#include <fstream>

namespace Resource_SRV
{
    namespace wxWidgets {
        void wxButtonDeleter(wxButton* obj);
        void wxTextCtrlDeleter(wxTextCtrl* obj);

        /// <summary>
        /// wxWidgets 3.0.5 doesn't support `new wxLogStream(&logFile, wxConvUTF8)`
        /// These code should do same thing
        /// </summary>
        class wxLogStream_UTF8 : public wxLog {
        public:
            explicit wxLogStream_UTF8(std::ofstream* ostr) : m_ostr(ostr) {}
        protected:
            void DoLogText(const wxString& msg) override {
                if (m_ostr && m_ostr->is_open()) {
                    wxScopedCharBuffer utf8Buf = (msg + wxS('\n')).ToUTF8();
                    m_ostr->write(utf8Buf.data(), utf8Buf.length());
                    m_ostr->flush();
                }
            }
        private:
            std::ofstream* m_ostr;
        };
    }
}
