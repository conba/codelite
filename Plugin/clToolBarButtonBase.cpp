#include "clToolBarButtonBase.h"
#include <wx/settings.h>
#include "drawingutils.h"

static wxBitmap MakeDisabledBitmap(const wxBitmap& bmp)
{
#ifdef __WXOSX__
    return DrawingUtils::CreateGrayBitmap(bmp);
#else
    return DrawingUtils::CreateDisabledBitmap(bmp);
#endif
}

clToolBarButtonBase::clToolBarButtonBase(clToolBar* parent, wxWindowID id, const wxBitmap& bmp, const wxString& label,
                                         size_t flags)
    : m_toolbar(parent)
    , m_id(id)
    , m_bmp(bmp)
    , m_label(label)
    , m_flags(flags)
    , m_renderFlags(0)
{
}

clToolBarButtonBase::~clToolBarButtonBase() {}

void clToolBarButtonBase::Render(wxDC& dc, const wxRect& rect)
{
    m_dropDownArrowRect = wxRect();
    m_buttonRect = rect;

    wxColour penColour;
    wxColour bgColour;
    wxColour textColour = wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT);

#if defined(__WXMSW__) || defined(__WXOSX__)
    wxColour bgHighlightColour("rgb(153, 209, 255)");
    penColour = bgHighlightColour;
#else
    wxColour bgHighlightColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
#endif
    DrawingUtils::FillMenuBarBgColour(dc, rect);

    if(IsEnabled() && (IsHover() || IsPressed() || IsChecked())) {
        penColour = bgHighlightColour;
        if(IsHover() || IsChecked()) {
            bgColour = bgHighlightColour;
        } else {
            bgColour = penColour;
        }
        dc.SetBrush(bgColour);
        dc.SetPen(penColour);
        dc.DrawRectangle(rect);
        textColour = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
    } else if(!IsEnabled()) {
        // A disabled button
        textColour = wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);
    }
    wxCoord xx = rect.GetX();
    wxCoord yy = 0;
    xx += CL_TOOL_BAR_X_MARGIN;

    if(m_bmp.IsOk()) {
        wxBitmap bmp(m_bmp);
        if(!IsEnabled()) {
            bmp = MakeDisabledBitmap(m_bmp);
        }
        yy = (rect.GetHeight() - bmp.GetScaledHeight()) / 2 + rect.GetY();
        dc.DrawBitmap(bmp, wxPoint(xx, yy));
        xx += bmp.GetScaledWidth();
        xx += CL_TOOL_BAR_X_MARGIN;
    }

    if(!m_label.IsEmpty() && m_toolbar->IsShowLabels()) {
        dc.SetTextForeground(textColour);
        wxSize sz = dc.GetTextExtent(m_label);
        yy = (rect.GetHeight() - sz.GetHeight()) / 2 + rect.GetY();
        dc.DrawText(m_label, wxPoint(xx, yy));
        xx += sz.GetWidth();
        xx += CL_TOOL_BAR_X_MARGIN;
    }

    // Do we need to draw a drop down arrow?
    if(HasMenu()) {
        // draw a drop down menu
        m_dropDownArrowRect =
            wxRect(xx, rect.GetY(), (2 * CL_TOOL_BAR_X_MARGIN) + CL_TOOL_BAR_DROPDOWN_ARROW_SIZE, rect.GetHeight());
        if(IsPressed() || IsHover()) {
            dc.DrawLine(wxPoint(xx, rect.GetY()), wxPoint(xx, rect.GetY() + rect.GetHeight()));
        }
        xx += CL_TOOL_BAR_X_MARGIN;

        wxPoint points[3];
        points[0].x = xx;
        points[0].y = (rect.GetHeight() - CL_TOOL_BAR_DROPDOWN_ARROW_SIZE) / 2 + rect.GetY();

        points[1].x = xx + CL_TOOL_BAR_DROPDOWN_ARROW_SIZE;
        points[1].y = points[0].y;

        points[2].x = xx + (CL_TOOL_BAR_DROPDOWN_ARROW_SIZE / 2);
        points[2].y = points[0].y + CL_TOOL_BAR_DROPDOWN_ARROW_SIZE;
        dc.SetPen(textColour);
        dc.SetBrush(textColour);
        dc.DrawPolygon(3, points);

        xx += CL_TOOL_BAR_DROPDOWN_ARROW_SIZE;
        xx += CL_TOOL_BAR_X_MARGIN;
    }
}