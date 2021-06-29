#include "..\..\Common.h"
#include "UICheckboxControl.h"

#pragma warning(disable: 4312)

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			VOID CUICheckbox::CreateWnd(const CUIBaseWindow& parent, const CUIBaseControl& control, const UINT menu_id)
			{
				Assert(!m_hWnd);
				Assert(menu_id);
				Assert(parent.Is());
				m_Created = FALSE;

				m_hWnd = control.Handle;

				Assert(m_hWnd);
				m_MenuId = menu_id;

				m_Checked = IsDlgButtonChecked(Parent(), m_MenuId);
			}

			VOID CUICheckbox::CreateWnd(const CUIBaseWindow &parent, const std::string &caption, const LONG l, const LONG t, const LONG w, const LONG h, const UINT menu_id)
			{
				Assert(!m_hWnd);
				Assert(menu_id);
				Assert(parent.Is());
				m_Created = TRUE;

				m_hWnd = CreateWindowExA(NULL, "button", caption.c_str(), 
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 
					l, t, w, h, parent.Handle, 
					(HMENU)menu_id,
					GetModuleHandleA(NULL), NULL);

				Assert(m_hWnd);

				m_Checked = FALSE;
				CheckDlgButton(m_hWnd, menu_id, BST_UNCHECKED);
				m_MenuId = menu_id;
			}

			VOID CUICheckbox::SetChecked(const BOOL value)
			{
				if (m_Checked == value)
					return;
				
				m_Checked = value;

				if (m_Checked)
					CheckDlgButton(Parent(), m_MenuId, BST_CHECKED);
				else
					CheckDlgButton(Parent(), m_MenuId, BST_UNCHECKED);
			}

			BOOL CUICheckbox::GetChecked(VOID) const
			{
				return m_Checked;
			}

			VOID CUICheckbox::Release(VOID)
			{
				if (m_Created && m_hWnd)
					DestroyWindow(m_hWnd);

				m_hWnd = NULL;
			}
		}
	}
}

