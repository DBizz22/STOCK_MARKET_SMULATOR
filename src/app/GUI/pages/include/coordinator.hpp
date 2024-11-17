#ifndef COORDINATOR_HPP_
#define COORDINATOR_HPP_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define wxUSE_STATTEXT 1
#define wxUSE_LISTCTRL 1
#include "wx/sizer.h"
#include "wx/panel.h"
#include "wx/textctrl.h"
#include "wx/stattext.h"
#include "wx/event.h"
#include "wx/listctrl.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <memory>
#include "databaseClient.hpp"
#include "mySqlClient.hpp"
#include "httpClient.hpp"
#include "curlClient.hpp"
#include "alphavantage.hpp"

#include <iostream>
#include <memory>
#include <unordered_set>
#include <algorithm>
#include <wx/valnum.h>
#include "signInModel.hpp"
#include "signUpModel.hpp"
#include "equityModel.hpp"
#include "profileModel.hpp"
#include "searchModel.hpp"
#include "refreshModel.hpp"
#include <cctype>

#include <wx/icon.h>

enum class PAGES
{
    LOGIN,
    REGISTER,
    PROFILE,
    EQUITIES
};

class Page
{
public:
    virtual void create()
    {
    }
    virtual void close()
    {
    }
    virtual ~Page() = default;
};

#endif