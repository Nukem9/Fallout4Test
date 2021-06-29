#pragma once

#include "UIBaseWindow.h"

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			class CUICheckbox : public CUIBaseControl
			{
			private:
				BOOL m_Created;
				BOOL m_Checked;
				UINT m_MenuId;
			public:
				VOID CreateWnd(const CUIBaseWindow &parent, const CUIBaseControl& control, const UINT menu_id);
				VOID CreateWnd(const CUIBaseWindow &parent, const std::string &caption, const LONG l, const LONG t, const LONG w, const LONG h, const UINT menu_id);
				VOID SetChecked(const BOOL value);
				BOOL GetChecked(VOID) const;
				VOID Release(VOID);
			public:
				PROPERTY(GetChecked, SetChecked) BOOL Checked;
			};
		}
	}
}