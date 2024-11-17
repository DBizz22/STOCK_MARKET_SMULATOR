#include "stockMarketFrame.hpp"

class StockMarketAPP : public wxApp
{
    virtual bool OnInit();
};

wxIMPLEMENT_APP(StockMarketAPP);

bool StockMarketAPP::OnInit()
{
    StockMarketFrame *frame = new StockMarketFrame();
    frame->Show(true);
    return true;
}